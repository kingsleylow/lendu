#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include <io.h>
#include "license.h"
#include "../log/mt_log_manager.h"
#include "../base/mt_error.h"

const int cryptogram[4] = { 0x714F3B9, 0x6276E82C, 0x6C4318A2, 0x5CC0A180 };
const int lic_passwd[4] = { 0x59997512, 0x4c96a252, 0x090b6754, 0x92541453 };

License::License(char *work_dir)
{
	memset(&dll_path, 0, sizeof(dll_path));
	memset(&lic_info, 0, sizeof(License_Info));
	strcpy_s(dll_path, work_dir);
	check_device_time_file();
}

License::~License()
{

}

int License::license_initial()
{
	int ret = ERR_RET_OK;
	FILE *fp = (FILE*)NULL;
	char path[PATH_SIZE] = {0};
	char linebuff[LINE_BUFF_LEN] = {0};
	//strcat_s(path, LICENSE_FOLDER);
	strcat_s(path, dll_path);
	char lic_name[260] ={0};
	CheckLicenseName("./", "lic", lic_name);
	strcat_s(path, lic_name);
	Log_manager::getInstance()->warn_log("lic_name=%s",lic_name);
	//strcat_s(path, "mt4_license.lic");
	fopen_s(&fp, path, "rb");
	if (fp == (FILE *)NULL)
	{
		return ERR_LIC_LOST;
	}
	fseek(fp, 0, SEEK_SET);
	while (fgets(linebuff, LINE_BUFF_LEN, fp) != NULL)
	{
		if ((strncmp(linebuff, "#", 1) == 0) ||
			(strncmp(linebuff, "//", 2) == 0))
		{
			memset(linebuff, 0, LINE_BUFF_LEN);
			continue;
		}
		if (strlen(linebuff) < LICENSE_STRING_SIZE)
		{
			memset(linebuff, 0, LINE_BUFF_LEN);
			continue;
		}
		parse_license((u8*)linebuff);
		break;
	}
	fclose(fp);
	return ret;
}

int License::CheckLicense(char *server_name, char *account_number)
{
	char mt4_name[1024] = {0};
	int ret = ERR_RET_OK;
	memset(&mt4_name, 0, sizeof(mt4_name));
	try
	{
		strcat_s(mt4_name, server_name);
		strcat_s(mt4_name, account_number);

		if ((strcmp(lic_info.SN, mt4_name) != 0))
		{
			Log_manager::getInstance()->warn_log("parse_license,mt4_name=%s,sn=%s",mt4_name,lic_info.SN);
			Log_manager::getInstance()->warn_log("license sn not match");
			return ERR_LIC_INVALID;
		}
		if(lic_info.limit_flag)
		{
			TIME_TYPE t_now;
			get_current_time(&t_now);
			Log_manager::getInstance()->warn_log("compare_date_time lic_info.limit_flag=%d",lic_info.limit_flag);
			if(compare_date_time(&t_now, &lic_info.tm_start, &lic_info.tm_end))
			{
				return ERR_LIC_OVERDUE;
			}
			ret = check_device_time();
			if(ret)
			{
				return ret;
			}
		}
	}
	catch (exception e)
	{
		return ERR_LIC_COMPARE_ERR;
	}
	return lic_info.license_type;
}

void License::check_device_time_file()
{
	FILE *fp1;
	time_t time_now = time(NULL);//这句返回的只是一个时间cuo
	if((fp1=fopen("dev_time","rb")) == NULL)
	{
		printf("read dev_time time file failed\n");
		if((fp1=fopen("dev_time","wb"))==NULL)
		{
			printf("write open time file failed\n");
			return;
		}
		fwrite(&time_now, sizeof(char), sizeof(time_t), fp1);
		fclose(fp1);
		SetFileAttributes("dev_time",  FILE_ATTRIBUTE_HIDDEN );       //设定为隐藏
	}
}

int License::check_device_time()
{
	FILE *fp1;
	time_t nowtime = time(NULL);
	time_t last_time;
	int i;

	if((fp1=fopen("dev_time","rb"))==NULL)
	{
		printf("read open time file failed\n");
		return ERR_DEV_TIME;
	}
	else
	{
		fread(&last_time,sizeof(char),sizeof(time_t),fp1);
		fclose(fp1);
	}

	//printf("current time=%u,last time=%llu\n",(u32)nowtime, last_time);
	if((nowtime > last_time))
	{
		//printf("time ok\n");
		SetFileAttributes("dev_time",  FILE_ATTRIBUTE_NORMAL );   
		if((fp1=fopen("dev_time","wb"))==NULL)
		{
			printf("write open time file failed\n");
			return 0;
		}
		fwrite(&nowtime,sizeof(char),sizeof(time_t),fp1);
		fclose(fp1);
		SetFileAttributes("dev_time",  FILE_ATTRIBUTE_HIDDEN );       //设定为隐藏
	}
	else
	{
		Log_manager::getInstance()->warn_log("current time=%u,last time=%llu",(u32)nowtime, last_time);
		return ERR_DEV_TIME;
	}
	return 0;
}

int License::parse_license(u8 *buf)
{
	int start_year = 0, start_month = 0, start_day = 0;
	int end_year = 0, end_month = 0, end_day = 0;
	char lic[64*10] = {0};
	char func[32*10] = {0};
	TIME_TYPE t_now;
	ascii_to_hex((u8*)lic, buf, LICENSE_STRING_SIZE * 2);
	decrypt((int*)lic, 16, (int*)lic_passwd);
	strcpy_s(lic_info.SN, lic);
	memcpy(func, &lic[LICENSE_MARK_BASE], LICENSE_MARK_BASE);
	char *p_func = &func[0];

	if(func[2] == 1)
	{
		memcpy(&start_year, p_func+3, 2);
		memcpy(&start_month, p_func+5, 1);
		memcpy(&start_day, p_func+6, 1);

		memcpy(&end_year, p_func+7, 2);
		memcpy(&end_month, p_func+9, 1);
		memcpy(&end_day, p_func+10, 1);
		if(func[1] == 1)
		{
			func[1] = 0;
		}
		get_current_time(&t_now);

		lic_info.tm_start.year = start_year;
		lic_info.tm_start.mon = start_month;
		lic_info.tm_start.mday = start_day;
		lic_info.tm_start.hour = 0;
		lic_info.tm_start.min = 0;
		lic_info.tm_start.sec = 0;

		lic_info.tm_end.year = end_year;
		lic_info.tm_end.mon = end_month;
		lic_info.tm_end.mday = end_day;
		lic_info.tm_end.hour = 23;
		lic_info.tm_end.min = 59;
		lic_info.tm_end.sec = 59;

		lic_info.limit_flag = 1;

		//printdatetime(&t_now);
		//printdatetime(&lic_info.tm_start);
		//printdatetime(&lic_info.tm_end);
		Log_manager::getInstance()->warn_log("current time  %d-%d-%d %d:%d:%d",t_now.year,t_now.mon,t_now.mday,t_now.hour,t_now.min,t_now.sec);
		Log_manager::getInstance()->warn_log("start time    %d-%d-%d %d:%d:%d",lic_info.tm_start.year,lic_info.tm_start.mon,lic_info.tm_start.mday,lic_info.tm_start.hour,lic_info.tm_start.min,lic_info.tm_start.sec);
		Log_manager::getInstance()->warn_log("end time      %d-%d-%d %d:%d:%d",lic_info.tm_end.year,lic_info.tm_end.mon,lic_info.tm_end.mday,lic_info.tm_end.hour,lic_info.tm_end.min,lic_info.tm_end.sec);
	}

	lic_info.license_type = atoi(func);
	//printf("license_type=%d\n", lic_info.license_type);
	Log_manager::getInstance()->warn_log("license_type=%d,lic_info.limit_flag=%d",lic_info.license_type,lic_info.limit_flag);
	return lic_info.license_type;
}
int License::CheckLicenseName(char *path, char *exd, char* buf)
{
	    //文件句柄
    long   hFile   =   0;
    //文件信息
    struct _finddata_t fileinfo;
    string pathName, exdName;
	string str_exd = exd;
    if (0 != strcmp(exd, ""))
    {
        exdName = "\\*." + str_exd;
    }
    else
    {
        exdName = "\\*";
    }

    if((hFile = _findfirst(pathName.assign(path).append(exdName).c_str(),&fileinfo)) !=  -1)
    {
        do
        {
            //如果是文件夹中仍有文件夹,迭代之
            {
                if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
                    //files.push_back(pathName.assign(path).append("\\").append(fileinfo.name)); // 要得到绝对目录使用该语句
                    //如果使用
					strcpy(buf, fileinfo.name);
                    //files.push_back(fileinfo.name); // 只要得到文件名字使用该语句
            }
        }while(_findnext(hFile, &fileinfo)  == 0);
        _findclose(hFile);
    }
	return 0;
}

//----------------------------------------------------------------
//功能:得到系统时间 
//参数:
//返回:
//----------------------------------------------------------------
int License::get_current_time(TIME_TYPE *tm_type)
{
	time_t tt = time(NULL);//这句返回的只是一个时间cuo
	tm* t_tm= localtime(&tt);

	tm_type->sec = t_tm->tm_sec;
	tm_type->min = t_tm->tm_min;
	tm_type->hour = t_tm->tm_hour;
	tm_type->mday = t_tm->tm_mday;
	tm_type->mon = t_tm->tm_mon + 1;
	tm_type->year = t_tm->tm_year + 1900;

	return 0;
}

void License::printdatetime(TIME_TYPE *tm_date)
{
	printf("%d-%d-%d %d:%d:%d\n",tm_date->year,tm_date->mon,tm_date->mday,tm_date->hour,tm_date->min,tm_date->sec);
}

s32 License::compare_date_time(TIME_TYPE *tm_now, TIME_TYPE *tm_start, TIME_TYPE *tm_end)
{
	printf("%d,%d\n",compare_date_time2(tm_now,tm_start),compare_date_time2(tm_now,tm_end));
	if(compare_date_time2(tm_now,tm_start) == 1 || compare_date_time2(tm_now,tm_end) == 2)
	{
		return 1;
	}
	return 0;
}

//0 =
//1 <
//2 >
s32 License::compare_date_time2(TIME_TYPE *tm_date1,TIME_TYPE *tm_date2)
{
	if(tm_date1->year != tm_date2->year)
	{
		if(tm_date1->year < tm_date2->year)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(tm_date1->mon != tm_date2->mon)
	{
		if(tm_date1->mon < tm_date2->mon)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(tm_date1->mday != tm_date2->mday)
	{
		if(tm_date1->mday < tm_date2->mday)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(tm_date1->hour != tm_date2->hour)
	{
		if(tm_date1->hour < tm_date2->hour)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(tm_date1->min != tm_date2->min)
	{
		if(tm_date1->min < tm_date2->min)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else if(tm_date1->sec != tm_date2->sec)
	{
		if(tm_date1->sec < tm_date2->sec)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	return 0;
}

s32 License::tea_encrypt(s32 *v, u32 n, s32 *k)
{
	const u32 DELTA = 0x9e3779b9;
	u32 p, z, y, sum, e;
	s32  q;
	if (n <= 0)
	{
		return 0;
	}
	z = v[n - 1], y = v[0], sum = 0;

	if (n > 1)
	{
		/* Coding Part */
		q = 6 + 52 / n;
		while (q-- > 0)
		{
			sum += DELTA;
			e = (sum >> 2) & 3;
			for (p = 0; p < n - 1; p++)
			{
				y = v[p + 1];
				z = v[p] += mx(z, y, sum, e, k, p);
			}
			y = v[0];
			z = v[n - 1] += mx(z, y, sum, e, k, p);
		}
		return 0;
	}
	return 1;
}

int License::tea_decrypt(int *v, int n, int *k)
{
	const u32 DELTA = 0x9e3779b9;
	u32 z, y, sum, e;
	s32 p, q;

	if (n <= 0)
	{
		return 0;
	}

	z = v[n - 1];
	y = v[0];
	sum = 0;
	q = 6 + 52 / n;
	sum = q * DELTA;
	while (sum != 0)
	{
		e = (sum >> 2) & 3;
		for (p = n - 1; p > 0; p--)
		{
			z = v[p - 1];
			y = v[p] -= mx(z, y, sum, e, k, p);
		}
		z = v[n - 1];
		y = v[0] -= mx(z, y, sum, e, k, p);
		sum -= DELTA;
	}
	return 0;
}

void License::ascii_to_hex(u8 *dest, u8 *src, u32 len)
{
	u32 count = len / 2;
	u32 i;

	for (i = 0; i<count; i++)
	{
		if (*src >= 'A')
			dest[i] = (*src - 'A' + 10) * 16;
		else
			dest[i] = (*src - '0') * 16;
		src++;
		if (*src >= 'A')
			dest[i] += (*src - 'A' + 10);
		else
			dest[i] += (*src - '0');
		src++;
	}
}

int License::decrypt(int *text, int n, int *random)
{
	int ci[4] = { 0 };
	memcpy((char *)ci, (char *)cryptogram, 16);
	tea_encrypt(ci, 4, random);
	tea_decrypt(text, n, ci);
	return 0;
}

int License::mx(u32 z, u32 y, u32 sum, u32 e, int *k, int p)
{
	return (int)((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4) ^ (sum ^ y) + ((u32)k[(u32)p & 3 ^ e] ^ z));
}