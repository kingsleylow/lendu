#pragma once
#include <Windows.h>
#include "log/mt_log_manager.h"
#include "lic/license.h"
#include "mt_data/mt_data.h"
class Mt_data_manager
{
private:
	int log_level;
	char work_dir[MAX_PATH];
	License *mt_license;
	Mt_order_cal* p_mt_order_cal;
public:
	Mt_data_manager();
	~Mt_data_manager();
	int uninstall_dll();

	int CheckLicense(char *server_name, char *account_number);
	int GetOrderInfo(Tran_Var_Array *info);
	int SetOrderInfo(Tran_Var_Array *info);
	int SetOrderCommand(int trade_type, Tran_Var_Array *info);

	int GetAccountInfo(Account_Info *info);
	int SetAccountInfo(Account_Info *info);

	int GetSymbolInfo(Symbol_Info *info);
	int SetSymbolInfo(Symbol_Info *info, int symbol_flag);

	int set_watch_dog_value(int value);
	int get_watch_dog_value();

	int GetOrderStatus(int shock_type);
	int SetOrderStatus(int shock_type, int order_count);

	int SetConnectedFlag(int value);
	int GetConnectedFlag();

	int GetLogInfo(MT_Log *info);
	int SetLogInfo(MT_Log *info);

	void startup_exe();

private:
	int get_dll_path(char* path);
	int mt_module_initial();
};