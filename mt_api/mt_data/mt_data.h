#pragma once
#include <Windows.h>
#define BUF_SIZE (16 * 1024 * 1024)
#define LOG_MAX (32)
#define TRADE_COUNT (7)

enum _op_order_enum
{
	OP_NONE = 0,
	OP_BUY = 1,
	OP_SELL,
	OP_SELLLIMIT,
	OP_BUYLIMIT,
	OP_BUYSTOP,
	OP_SELLSTOP,
	OP_CLOSE_ALL = 7, //一键平仓
};

enum shock_type_enum
{
    manual_trading = 0,  //手动交易
    shock_trading  = 1,  //震荡交易
    trend_trading  = 2,  //趋势交易
	Hedging_System1 = 3,  //对冲系统
	Expert_System  = 4, //专家系统，用于移动止损业务
	Hedging_System2 = 5,  //对冲系统2
	Hedging_System3 = 6,  //对冲系统3
	trade_end,
};

//简易版定义参数变量结构体
typedef struct _tran_var_array
{
	int               Type;     // 类型
	int               Var1;		//交易系统启动标识
	double            Var2;     // order_lots_system  交易系统仓位
	char              Var3[32]; //order_comment  交易系统开单注释
	double            Var4;     //交易系统自定义参数
	double            Var5;     //交易系统自定义参数
	double            Var6;     //交易系统自定义参数
	double            Var7;     //交易系统自定义参数
	double            Var8;     //交易系统自定义参数
	double            Var9;     //交易系统自定义参数
	double            Var10;    //交易系统自定义参数 ....
	int               Var11;    //交易系统一键平仓参数 ...
    int               Var12;    //开仓方式
	char              Var13[32]; //对冲系统交易货币对
	int               Var14;    //震荡系统、趋势系统 len系数
	int               Var15;
}Tran_Var_Array;

//订单信息结构体
//typedef struct _order_info
//{
//   int               ret_status;   //订单执行结果标记位
//   int               order_type;   //订单类型(买单OP_BUY、卖单OP_SELL、一键平仓OP_CLOSEALL等)
//   double            open_lots;    // order_lots_system  交易手数
//   double            takeprofit;   //止盈参数
//   double            stoploss;     //止损参数
//   char              comment[32];  //order_comment  交易系统开单注释
//}Order_Info;

typedef struct _symbol_info
{
	char current_symbol_name[32];   //当前货币对名称
	double current_symbol_ask;      //该货币当前买价
	double current_symbol_bid;      //该货币当前卖价
}Symbol_Info;

typedef struct _account_info
{
	int ea_unique_id;            //EA启动唯一标识
	int leverage;                //账户杠杆
	int number;                  //账户数字
	int reserver;                //保留
	double balance;              //账户余额
	double credit;               //账户信用点数
	double equity;               //账户净资产值
	double free_margin;          //账户免费保证金
	double free_margin_check;    //账户当前价格自由保证金
	double free_margin_mode;     //账户免费保证金模式
	double margin;               //账户保证金
	double profit;               //账户利润
	char company[32];            //账户公司名
	char currency[32];           //账户基本货币
	char name[32];               //账户名称
	char server[32];             //账户服务器名称
	char server_time[32];        //服务器时间
}Account_Info;

typedef struct _mt_log
{
	int log_type;                   //日志编号
	int show_flag;                  //显示方式，是否立刻在C# UI显示
	char log_info[64];              //日志内容
}MT_Log;

typedef struct _mt_log_model
{
	int flag[LOG_MAX];      //空闲标志，如果为1表示已经写入了日志且尚未读。读完日志后，将标志置为0，此时方可继续写
	MT_Log log[LOG_MAX];
}MT_Log_Model;

class Mt_order_cal
{
private:
	HANDLE ipcMutex;
	HANDLE ipcMutex_order;
	HANDLE ipcMutex_account;
	HANDLE ipcMutex_symbol ;
	HANDLE ipcMutex_log;
	HANDLE ipcMutex_dog;
	HANDLE ipcMutex_order_count;
	HANDLE ipcMutex_connect;
	Tran_Var_Array order;
	Account_Info account_info;
	char *shm_buf;
public:
	Mt_order_cal();
	~Mt_order_cal();

	int uninstall_dll();
	int GetOrderInfo(Tran_Var_Array *info);
	int SetOrderInfo(Tran_Var_Array *info);

	int GetAccountInfo(Account_Info *info);
	int SetAccountInfo(Account_Info *info);
	int SetOrderCommand(int trade_type, Tran_Var_Array *info);

	int GetSymbolInfo(Symbol_Info *info);
	int SetSymbolInfo(Symbol_Info *info, int symbol_flag);

	int GetLogInfo(MT_Log *info);
	int SetLogInfo(MT_Log *info);

	int set_watch_dog_value(int value);
	int get_watch_dog_value();

	int GetOrderStatus(int shock_type);
	int SetOrderStatus(int shock_type, int order_count);

	int SetConnectedFlag(int value);
	int GetConnectedFlag();

private:
	int init_shm();
	int init_ipc_mutex();
	int show_order_info(Tran_Var_Array *info);

};