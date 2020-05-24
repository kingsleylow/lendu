#include<atlbase.h>
#include <stdio.h>
#include "mt_data_manager.h" 
Mt_data_manager::Mt_data_manager()
{
	log_level = 0;
	memset(work_dir, 0, sizeof(work_dir));
	get_dll_path(work_dir);
	Log_manager::getInstance()->warn_log("work_dir=%s",work_dir);
	mt_module_initial();
}

Mt_data_manager::~Mt_data_manager()
{
	if (mt_license != NULL)
	{
		delete mt_license;
		mt_license = NULL;
	}
}

int Mt_data_manager::mt_module_initial()
{
	int ret = 0;
	Log_manager::getInstance()->SetLogName(work_dir);

	mt_license = new License(work_dir);
	ret = mt_license->license_initial();
	Log_manager::getInstance()->warn_log("mt_module_initial,ret=%d",ret);
	p_mt_order_cal = new Mt_order_cal();
	return ret;
}

int Mt_data_manager::CheckLicense(char *server_name, char *account_number)
{
	return mt_license->CheckLicense(server_name, account_number);
}

int Mt_data_manager::get_dll_path(char* path)
{
	char    szBuff[MAX_PATH] = { 0 };
	HMODULE hModuleInstance = _AtlBaseModule.GetModuleInstance();
	GetModuleFileNameA(hModuleInstance, szBuff, MAX_PATH);
	int len = strlen(szBuff);
	int i = len - 1;
	while (szBuff[i] != 92) /* 92µÄasciÂë ÊÇÐ±¸Ü */
	{
		i--;
	}
	memcpy(path, szBuff, i + 1);
	return 0;
}

int Mt_data_manager::uninstall_dll()
{
	return p_mt_order_cal->uninstall_dll();
}

int Mt_data_manager::GetOrderInfo(Tran_Var_Array *info)
{
	return p_mt_order_cal->GetOrderInfo(info);
}

int Mt_data_manager::SetOrderInfo(Tran_Var_Array *info)
{
	return p_mt_order_cal->SetOrderInfo(info);
}

int Mt_data_manager::SetOrderCommand(int trade_type, Tran_Var_Array *info)
{
	return p_mt_order_cal->SetOrderCommand(trade_type, info);
}

int Mt_data_manager::GetAccountInfo(Account_Info *info)
{
	return p_mt_order_cal->GetAccountInfo(info);
}

int Mt_data_manager::SetAccountInfo(Account_Info *info)
{
	return p_mt_order_cal->SetAccountInfo(info);;
}

int Mt_data_manager::GetSymbolInfo(Symbol_Info *info)
{
	return p_mt_order_cal->GetSymbolInfo(info);
}

int Mt_data_manager::SetSymbolInfo(Symbol_Info *info, int symbol_flag)
{
	return p_mt_order_cal->SetSymbolInfo(info, symbol_flag);
}

int Mt_data_manager::set_watch_dog_value(int value)
{
	return p_mt_order_cal->set_watch_dog_value(value);
}

int Mt_data_manager::get_watch_dog_value()
{
	return p_mt_order_cal->get_watch_dog_value();
}

int Mt_data_manager::GetOrderStatus(int shock_type)
{
	return p_mt_order_cal->GetOrderStatus(shock_type);
}

int Mt_data_manager::SetOrderStatus(int shock_type, int order_count)
{
	return p_mt_order_cal->SetOrderStatus(shock_type, order_count);
}

int Mt_data_manager::SetConnectedFlag(int value)
{
	return p_mt_order_cal->SetConnectedFlag(value);
}
int Mt_data_manager::GetConnectedFlag()
{
	return p_mt_order_cal->GetConnectedFlag();
}

int Mt_data_manager::GetLogInfo(MT_Log *info)
{
	return p_mt_order_cal->GetLogInfo(info);
}
int Mt_data_manager::SetLogInfo(MT_Log *info)
{
	return p_mt_order_cal->SetLogInfo(info);
}

void Mt_data_manager::startup_exe()
{
	try
	{
	char exe_path[1024] = {0};
	char *substr = "MQL4";
	char *str = strstr(work_dir, substr);
	Log_manager::getInstance()->warn_log("work_dir=%s,len=%d,str=%s",work_dir,strlen(work_dir),str);
	memcpy(exe_path, work_dir,strlen(work_dir)-strlen(str));
	strcat(exe_path, "MQL4\\Experts\\TRADE\\QuantitativeTradingMaster.exe");
	Log_manager::getInstance()->warn_log("len=%d,exe_path=%s",strlen(exe_path),exe_path);
	WinExec(exe_path,SW_SHOW);
	//WinExec("calc.exe",SW_SHOW);
	}
	catch(exception w)
	{
		return;
	}
}