// dll_test.cpp : 定义 DLL 应用程序的导出函数
#include <Windows.h>
#include <stdio.h>
#include "dll_mt4_interface.h"
#include "mt_data_manager.h"
#include "base/mt_error.h"
#include "Crash Dump/WinCrashDump.h"

Mt_data_manager* p_mt_data_manager = NULL;


inline BOOL wt_assert(Mt_data_manager* p_mt_data_manager)
{
	if (p_mt_data_manager == NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

int GetStructValue(WT_DAC_TYPE *dac,int size)
{
	int i=0;
	for(i=0; i<size; i++)
	{
		dac[i].gain = i + 0.24;
		dac[i].sampling_freq = i + 120;
		dac[i].source = i + 4;
		dac[i].wave_preset = i + 5000;
		memcpy(dac[i].name, "teng", sizeof("teng"));
		memcpy(dac[i].server, "192.168.1.1", sizeof("192.168.1.1"));
	}
	//dac->gain = 0.24;
	//dac->sampling_freq = 120;
	//dac->source = 4;
	//dac->wave_preset = 5000;
	return 0;
}

int SetArrayItemValue(double *arr,const int arraysize,const int nitem,const double value)
{
	if(arr == NULL)
	{
		printf("GetArrayItemValue: NULL array\n");
		return 0;
	}
	if(arraysize<=0)
	{
		printf("GetArrayItemValue: wrong arraysize (%d)\n", arraysize);
		return 0;
	}
	if(nitem<0 || nitem>=arraysize)
	{
		printf("GetArrayItemValue: wrong item number (%d)\n", nitem);
		return 0;
	}
	//arr[nitem]=value;
	int i;
	for(i=0;i<arraysize; i++)
	{
		arr[i] = i+ 0.12;
	}
	return 1;
}

int add_test(int a, int b)
{
	return a + b;
}

void testerr()
{
	char *str="hello,world";
	str[1] = 'a';
	char path[10] = {0};
	strcpy_s(path, "dssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssss");
}

int mt_module_initial()
{
	Log_manager::getInstance()->warn_log("start mt_module_initial");
	if(p_mt_data_manager == NULL)
	{ 
		p_mt_data_manager = new Mt_data_manager();
	}
	//RegesterProcDump();
	//testerr();
	return 0;
}

int uninstall_dll()
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->uninstall_dll();
}

int CheckLicense(char *server_name, char *account_number)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->CheckLicense(server_name, account_number);
}

int GetOrderInfo(Tran_Var_Array *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->GetOrderInfo(info);
}

int SetOrderInfo(Tran_Var_Array *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetOrderInfo(info);
}

int SetOrderCommand(int trade_type, Tran_Var_Array *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetOrderCommand(trade_type, info);
}

int SetAccountInfo(Account_Info *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetAccountInfo(info);
}

int GetAccountInfo(Account_Info *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->GetAccountInfo(info);
}

int GetSymbolInfo(Symbol_Info *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->GetSymbolInfo(info);
}
int SetSymbolInfo(Symbol_Info *info, int symbol_flag)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetSymbolInfo(info, symbol_flag);
}

int set_watch_dog_value(int value)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->set_watch_dog_value(value);
}

int get_watch_dog_value()
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->get_watch_dog_value();
}

int GetOrderStatus(int shock_type)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->GetOrderStatus(shock_type);
}

int SetOrderCount(int shock_type, int order_count)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetOrderStatus(shock_type, order_count);
}

int SetConnectedFlag(int value)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetConnectedFlag(value);
}
int GetConnectedFlag()
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->GetConnectedFlag();
}

int GetLogInfo(MT_Log *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->GetLogInfo(info);
}
int SetLogInfo(MT_Log *info)
{
	if (!wt_assert(p_mt_data_manager))
	{
		return ERR_MT_NOT_INIT;
	}
	return p_mt_data_manager->SetLogInfo(info);
}

void startup_exe()
{
	if (!wt_assert(p_mt_data_manager))
	{
		return;
	}
	Log_manager::getInstance()->warn_log("startup_exe");
	//WinExec("calc.exe",SW_SHOW);
	//WinExec("22.bat",SW_SHOW);
	p_mt_data_manager->startup_exe();
}


