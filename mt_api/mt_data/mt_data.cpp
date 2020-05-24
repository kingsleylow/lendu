#include <string.h>
#include "mt_data.h"
#include "../log/mt_log_manager.h"
#include "../base/mt_error.h"


char IPCName[] = "Global\\MT4MappingObj";    // 共享内存名称
char mutex_name[] = "MT4Mutex";      // 互斥锁名称
char mutex_order[] = "MutexOrder";      // 订单互斥锁名
char mutex_account[] = "MutexAccount";
char mutex_symbol[] = "MutexSymbol";
char mutex_log[] = "MutexLog";
char mutex_dog[] = "MutexDog";
char mutex_order_count[] = "MutexOrderCount";
char mutex_connect[] = "MutexConnectFlag";
static int log_w_index = 0;
static int log_r_index = 0;
Mt_order_cal::Mt_order_cal()
{
	memset(&order, 0, sizeof(Tran_Var_Array));
	memset(&account_info, 0, sizeof(Account_Info));
	init_ipc_mutex();
	init_shm();
}

Mt_order_cal::~Mt_order_cal()
{
}

int Mt_order_cal::init_shm()
{
	int ret = 0;
	// 创建共享文件句柄  
	HANDLE hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // 物理文件句柄  
		NULL,                    // 默认安全级别  
		PAGE_READWRITE,          // 可读可写  
		0,                       // 高位文件大小  
		BUF_SIZE,                // 地位文件大小  
		(LPCSTR)IPCName         // 共享内存名称  
	);

	if (NULL == hMapFile)    
	{      
		Log_manager::getInstance()->warn_log("CreateFileMapping fail:%d",GetLastError());
		return ERR_SHARE_ERR1;
	}

	char *pBuf = (char *)MapViewOfFile(
		hMapFile,            // 共享内存的句柄  
		FILE_MAP_ALL_ACCESS, // 可读写许可  
		0,
		0,
		BUF_SIZE
	);
	shm_buf = pBuf;
	ret = GetLastError();
	if(shm_buf == NULL)
	{
		Log_manager::getInstance()->warn_log("MapViewOfFile err,GetLastError ret=%d",ret);
		return ERR_SHARE_ERR2;
	}

	if(ret == ERROR_ALREADY_EXISTS)
	{
		Log_manager::getInstance()->warn_log("ERROR_ALREADY_EXISTS");
		return 0;
	}
	WaitForSingleObject(ipcMutex, INFINITE);
	memset(shm_buf, 0, BUF_SIZE);
	ReleaseMutex(ipcMutex);
	return 0;
}

int Mt_order_cal::init_ipc_mutex()
{
	ipcMutex = CreateMutex(NULL, false, mutex_name);
	ipcMutex_order = CreateMutex(NULL, false, mutex_order);
	ipcMutex_account = CreateMutex(NULL, false, mutex_account);
	ipcMutex_symbol = CreateMutex(NULL, false, mutex_symbol);
	ipcMutex_log = CreateMutex(NULL, false, mutex_log);
	ipcMutex_dog = CreateMutex(NULL, false, mutex_dog);
	ipcMutex_order_count = CreateMutex(NULL, false, mutex_order_count);
	ipcMutex_connect = CreateMutex(NULL, false, mutex_connect);
	return 0;
}

int Mt_order_cal::uninstall_dll()
{
	WaitForSingleObject(ipcMutex, INFINITE);
	memset(shm_buf, 0, BUF_SIZE);
	ReleaseMutex(ipcMutex);
	return 0;
}

int Mt_order_cal::GetOrderInfo(Tran_Var_Array *info)
{
	//memcpy(info, &order_info, sizeof(Order_Info));
	//对共享内存临界区进行加锁处理，封装成原子操作，防止不同进程读写时造成数据错乱
	WaitForSingleObject(ipcMutex_order, INFINITE);
	memcpy(info, shm_buf + sizeof(Account_Info), sizeof(Tran_Var_Array)*TRADE_COUNT);
	//show_order_info(info);
	ReleaseMutex(ipcMutex_order);
	return 0;
}

int Mt_order_cal::show_order_info(Tran_Var_Array *info)
{
	if(info[6].Var11)
	{
	Log_manager::getInstance()->warn_log("info[0].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[0].Type,info[0].Var1,info[0].Var2,info[0].Var4,info[0].Var5,info[0].Var6,info[0].Var7,info[0].Var8,info[0].Var9,info[0].Var10,info[0].Var11,info[0].Var12,info[0].Var13,info[0].Var14);
	Log_manager::getInstance()->warn_log("info[1].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[1].Type,info[1].Var1,info[1].Var2,info[1].Var4,info[1].Var5,info[1].Var6,info[1].Var7,info[1].Var8,info[1].Var9,info[1].Var10,info[1].Var11,info[1].Var12,info[1].Var13,info[1].Var14);
	Log_manager::getInstance()->warn_log("info[2].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[2].Type,info[2].Var1,info[2].Var2,info[2].Var4,info[2].Var5,info[2].Var6,info[2].Var7,info[2].Var8,info[2].Var9,info[2].Var10,info[2].Var11,info[2].Var12,info[2].Var13,info[2].Var14);
	Log_manager::getInstance()->warn_log("info[3].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[3].Type,info[3].Var1,info[3].Var2,info[3].Var4,info[3].Var5,info[3].Var6,info[3].Var7,info[3].Var8,info[3].Var9,info[3].Var10,info[3].Var11,info[3].Var12,info[3].Var13,info[3].Var14);
	Log_manager::getInstance()->warn_log("info[4].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[4].Type,info[4].Var1,info[4].Var2,info[4].Var4,info[4].Var5,info[4].Var6,info[4].Var7,info[4].Var8,info[4].Var9,info[4].Var10,info[4].Var11,info[4].Var12,info[4].Var13,info[4].Var14);
	Log_manager::getInstance()->warn_log("info[5].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[5].Type,info[5].Var1,info[5].Var2,info[5].Var4,info[5].Var5,info[5].Var6,info[5].Var7,info[5].Var8,info[5].Var9,info[5].Var10,info[5].Var11,info[5].Var12,info[5].Var13,info[5].Var14);
	Log_manager::getInstance()->warn_log("info[6].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[6].Type,info[6].Var1,info[6].Var2,info[6].Var4,info[6].Var5,info[6].Var6,info[6].Var7,info[6].Var8,info[6].Var9,info[6].Var10,info[6].Var11,info[6].Var12,info[6].Var13,info[6].Var14);
	
	}
	return 0;
}

int Mt_order_cal::SetOrderInfo(Tran_Var_Array *info)
{
	//memcpy(&order_info, info, sizeof(Order_Info));
	WaitForSingleObject(ipcMutex_order, INFINITE);
	memcpy(shm_buf + sizeof(Account_Info), info, sizeof(Tran_Var_Array)*TRADE_COUNT);
	ReleaseMutex(ipcMutex_order);
	return 0;
}

int Mt_order_cal::SetOrderCommand(int trade_type, Tran_Var_Array *info)
{
	if(trade_type >= trade_end)
	{
		Log_manager::getInstance()->warn_log("SetOrderCommand Err,trade_type=%d",trade_type);
		return ERR_TRADE_TYPE;
	}
	//Tran_Var_Array order[TRADE_COUNT]={0};
	//Log_manager::getInstance()->warn_log("info[0].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info[0].Type,info[0].Var1,info[0].Var2,info[0].Var4,info[0].Var5,info[0].Var6,info[0].Var7,info[0].Var8,info[0].Var9,info[0].Var10,info[0].Var11,info[0].Var12,info[0].Var13,info[0].Var14);
	//Log_manager::getInstance()->warn_log("info[0].Type=%d,info[0].Var1=%d,info[0].Var2=%lf,info[0].Var4=%lf,info[0].Var5=%lf,info[0].Var6=%lf,info[0].Var7=%lf,info[0].Var8=%lf,info[0].Var9=%lf,info[0].Var10=%lf,info[0].Var11=%d,info[0].Var12=%d,info[0].Var13=%s,info[0].Var14=%d",info->Type,info->Var1,info->Var2,info->Var4,info->Var5,info->Var6,info->Var7,info->Var8,info->Var9,info->Var10,info->Var11,info->Var12,info->Var13,info->Var14);
	//Log_manager::getInstance()->warn_log("SetOrderCommand ,trade_type=%d,turns=%lf,trade_start_flag=%d,var11=%d,13=%s,var14=%d",trade_type,info->Var4,info->Var1,info->Var11,info->Var13,info->Var14);
	WaitForSingleObject(ipcMutex_order, INFINITE);
	memcpy(shm_buf + sizeof(Account_Info) + sizeof(Tran_Var_Array) * trade_type, info, sizeof(Tran_Var_Array));
	//memcpy(order, shm_buf + sizeof(Account_Info), sizeof(Tran_Var_Array)*TRADE_COUNT);
	//show_order_info(order);
	ReleaseMutex(ipcMutex_order);
	return 0;
}

int Mt_order_cal::GetAccountInfo(Account_Info *info)
{
	//对共享内存临界区进行加锁处理，封装成原子操作，防止不同进程读写时造成数据错乱
	WaitForSingleObject(ipcMutex_account, INFINITE);
	memcpy(info, shm_buf, sizeof(Account_Info));
	ReleaseMutex(ipcMutex_account);
	return 0;
}

int Mt_order_cal::SetAccountInfo(Account_Info *info)
{
	WaitForSingleObject(ipcMutex_account, INFINITE);
	memcpy(shm_buf, info, sizeof(Account_Info));
	ReleaseMutex(ipcMutex_account);
	return 0;
}

int Mt_order_cal::GetSymbolInfo(Symbol_Info *info)
{
	//对共享内存临界区进行加锁处理，封装成原子操作，防止不同进程读写时造成数据错乱
	WaitForSingleObject(ipcMutex_symbol, INFINITE);
	memcpy(info, shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT, sizeof(Symbol_Info));
	ReleaseMutex(ipcMutex_symbol);
	return 0;
}

int Mt_order_cal::SetSymbolInfo(Symbol_Info *info, int symbol_flag)
{
	WaitForSingleObject(ipcMutex_symbol, INFINITE);
	if(symbol_flag)
	{
		memcpy(shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT, info, sizeof(Symbol_Info));
	}
	else
	{
		Symbol_Info *symbol = (Symbol_Info*)(shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT);
		symbol->current_symbol_ask = info->current_symbol_ask;
		symbol->current_symbol_bid = info->current_symbol_bid;
	}
	ReleaseMutex(ipcMutex_symbol);
	return 0;
}

int Mt_order_cal::set_watch_dog_value(int value)
{
	WaitForSingleObject(ipcMutex_dog, INFINITE);
	memcpy(shm_buf+sizeof(Account_Info) + sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info), &value, sizeof(int));
	ReleaseMutex(ipcMutex_dog);
	return 0;
}

int Mt_order_cal::get_watch_dog_value()
{
	int value = 0;
	WaitForSingleObject(ipcMutex_dog, INFINITE);
	memcpy(&value, shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info), sizeof(int));
	ReleaseMutex(ipcMutex_dog);
	return value;
}

int Mt_order_cal::GetOrderStatus(int shock_type)
{
	int value = 0;
	WaitForSingleObject(ipcMutex_order_count, INFINITE);
	memcpy(&value, shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info) + sizeof(int)*shock_type, sizeof(int));
	ReleaseMutex(ipcMutex_order_count);
	return value;
}

int Mt_order_cal::SetOrderStatus(int shock_type, int order_count)
{
	WaitForSingleObject(ipcMutex_order_count, INFINITE);
	memcpy(shm_buf+sizeof(Account_Info) + sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info) + sizeof(int)*shock_type, &order_count, sizeof(int));
	ReleaseMutex(ipcMutex_order_count);
	return 0;
}

int Mt_order_cal::SetConnectedFlag(int value)
{
	WaitForSingleObject(ipcMutex_connect, INFINITE);
	memcpy(shm_buf+sizeof(Account_Info) + sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info) + sizeof(int)*(TRADE_COUNT+1), &value, sizeof(int));
	ReleaseMutex(ipcMutex_connect);
	return 0;
}
int Mt_order_cal::GetConnectedFlag()
{
	int value = 0;
	WaitForSingleObject(ipcMutex_connect, INFINITE);
	memcpy(&value, shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info) + sizeof(int)*(TRADE_COUNT+1), sizeof(int));
	ReleaseMutex(ipcMutex_connect);
	return value;
}

//ret 1 表示有未显示日志， 0无日志
int Mt_order_cal::GetLogInfo(MT_Log *info)
{
	int ret = 0;
	WaitForSingleObject(ipcMutex_log, INFINITE);
	log_r_index++;
	if(log_r_index >= LOG_MAX)
	{
		log_r_index = 0;
	}
	MT_Log_Model *p_mt_log_model = (MT_Log_Model *)(shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info) + sizeof(int)*(TRADE_COUNT+2));
	for(; log_r_index<LOG_MAX; log_r_index++)
	{
		if(p_mt_log_model->flag[log_r_index])
		{
			p_mt_log_model->flag[log_r_index] = 0;
			memcpy(info, &(p_mt_log_model->log[log_r_index]), sizeof(MT_Log));
			ret = 1;
			break;
		}
	}
	if(!ret)
	{
		for(log_r_index = 0; log_r_index<LOG_MAX; log_r_index++)
		{
			if(p_mt_log_model->flag[log_r_index])
			{
				p_mt_log_model->flag[log_r_index] = 0;
				memcpy(info, &(p_mt_log_model->log[log_r_index]), sizeof(MT_Log));
				ret = 1;
				break;
			}
		}
	}
	ReleaseMutex(ipcMutex_log);
	return ret;
}

int Mt_order_cal::SetLogInfo(MT_Log *info)
{
	WaitForSingleObject(ipcMutex_log, INFINITE);
	log_w_index++;
	if(log_w_index >= LOG_MAX)
	{
		log_w_index = 0;
	}
	MT_Log_Model *p_mt_log_model = (MT_Log_Model *)(shm_buf+sizeof(Account_Info)+sizeof(Tran_Var_Array)*TRADE_COUNT + sizeof(Symbol_Info) + sizeof(int)*(TRADE_COUNT+2));
	p_mt_log_model->flag[log_w_index] = 1;
	p_mt_log_model->log[log_w_index].log_type = info->log_type;
	p_mt_log_model->log[log_w_index].show_flag = info->show_flag;
	memcpy((p_mt_log_model->log[log_w_index].log_info), info->log_info, sizeof(p_mt_log_model->log[0].log_info));  //len 64
	//Log_manager::getInstance()->warn_log("SetLogInfo,index=%d,type=%d,log=%s",log_w_index,info->log_type,info->log_info);
	ReleaseMutex(ipcMutex_log);
	return 0;
}