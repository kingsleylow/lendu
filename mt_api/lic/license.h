#pragma once
#include <string.h>
#include "../base/typedef.h"
#define PATH_SIZE (1024)
#define LINE_BUFF_LEN	(1024)
#define LICENSE_STRING_SIZE (64)
#define LICENSE_MARK_BASE (32)
#define LICENSE_FOLDER	"./lic/"

enum lic_enum
{
	lic_base = 0,
	lic_5g = 1,
	lic_end = 64
};

//时间类型
typedef struct 
{
	u16 sec;                   // Seconds.     [0-60] (1 leap second) 
	u16 min;                   // Minutes.     [0-59] 
	u16 hour;                  // Hours.       [0-23] 
	u16 mday;                  // Day.         [1-31] 
	u16 mon;                   // Month.       [0-11] 
	u16 year;                  // Year - 1900.  
}TIME_TYPE;

typedef struct {
	char SN[32];
	int license_type;
	int limit_flag;
	TIME_TYPE tm_start;
	TIME_TYPE tm_end;
} License_Info;

class License
{
private:
	License_Info lic_info;
	char dll_path[1024];
public:
	License(char *work_dir);
	~License();

	int license_initial();
	int CheckLicense(char *server_name, char *account_number);
	int CheckLicenseName(char *path, char *exd, char* buf);

private:
	/*******************************************************************************
	函数: parse_license()
	功能: license分析，如果license合法，则置位相应的license标志
	参数:
	buf: license内容
	返回:
	none
	*******************************************************************************/
	int parse_license(u8 *buf);

	void ascii_to_hex(u8 *dest, u8 *src, u32 len);

	int decrypt(int *text, int n, int *random);

	int tea_decrypt(int *v, int n, int *k);

	s32 tea_encrypt(s32 *v, u32 n, s32 *k);

	int mx(u32 z, u32 y, u32 sum, u32 e, int *k, int p);

private:
	void printdatetime(TIME_TYPE *tm_date);
	int get_current_time(TIME_TYPE *tm_type);
	s32 compare_date_time(TIME_TYPE *tm_now, TIME_TYPE *tm_start, TIME_TYPE *tm_end);
	s32 compare_date_time2(TIME_TYPE *tm_date1,TIME_TYPE *tm_date2);

	int check_device_time();
	void check_device_time_file();
};