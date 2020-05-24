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
	OP_CLOSE_ALL = 7, //һ��ƽ��
};

enum shock_type_enum
{
    manual_trading = 0,  //�ֶ�����
    shock_trading  = 1,  //�𵴽���
    trend_trading  = 2,  //���ƽ���
	Hedging_System1 = 3,  //�Գ�ϵͳ
	Expert_System  = 4, //ר��ϵͳ�������ƶ�ֹ��ҵ��
	Hedging_System2 = 5,  //�Գ�ϵͳ2
	Hedging_System3 = 6,  //�Գ�ϵͳ3
	trade_end,
};

//���װ涨����������ṹ��
typedef struct _tran_var_array
{
	int               Type;     // ����
	int               Var1;		//����ϵͳ������ʶ
	double            Var2;     // order_lots_system  ����ϵͳ��λ
	char              Var3[32]; //order_comment  ����ϵͳ����ע��
	double            Var4;     //����ϵͳ�Զ������
	double            Var5;     //����ϵͳ�Զ������
	double            Var6;     //����ϵͳ�Զ������
	double            Var7;     //����ϵͳ�Զ������
	double            Var8;     //����ϵͳ�Զ������
	double            Var9;     //����ϵͳ�Զ������
	double            Var10;    //����ϵͳ�Զ������ ....
	int               Var11;    //����ϵͳһ��ƽ�ֲ��� ...
    int               Var12;    //���ַ�ʽ
	char              Var13[32]; //�Գ�ϵͳ���׻��Ҷ�
	int               Var14;    //��ϵͳ������ϵͳ lenϵ��
	int               Var15;
}Tran_Var_Array;

//������Ϣ�ṹ��
//typedef struct _order_info
//{
//   int               ret_status;   //����ִ�н�����λ
//   int               order_type;   //��������(��OP_BUY������OP_SELL��һ��ƽ��OP_CLOSEALL��)
//   double            open_lots;    // order_lots_system  ��������
//   double            takeprofit;   //ֹӯ����
//   double            stoploss;     //ֹ�����
//   char              comment[32];  //order_comment  ����ϵͳ����ע��
//}Order_Info;

typedef struct _symbol_info
{
	char current_symbol_name[32];   //��ǰ���Ҷ�����
	double current_symbol_ask;      //�û��ҵ�ǰ���
	double current_symbol_bid;      //�û��ҵ�ǰ����
}Symbol_Info;

typedef struct _account_info
{
	int ea_unique_id;            //EA����Ψһ��ʶ
	int leverage;                //�˻��ܸ�
	int number;                  //�˻�����
	int reserver;                //����
	double balance;              //�˻����
	double credit;               //�˻����õ���
	double equity;               //�˻����ʲ�ֵ
	double free_margin;          //�˻���ѱ�֤��
	double free_margin_check;    //�˻���ǰ�۸����ɱ�֤��
	double free_margin_mode;     //�˻���ѱ�֤��ģʽ
	double margin;               //�˻���֤��
	double profit;               //�˻�����
	char company[32];            //�˻���˾��
	char currency[32];           //�˻���������
	char name[32];               //�˻�����
	char server[32];             //�˻�����������
	char server_time[32];        //������ʱ��
}Account_Info;

typedef struct _mt_log
{
	int log_type;                   //��־���
	int show_flag;                  //��ʾ��ʽ���Ƿ�������C# UI��ʾ
	char log_info[64];              //��־����
}MT_Log;

typedef struct _mt_log_model
{
	int flag[LOG_MAX];      //���б�־�����Ϊ1��ʾ�Ѿ�д������־����δ����������־�󣬽���־��Ϊ0����ʱ���ɼ���д
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