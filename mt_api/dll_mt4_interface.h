// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� DLL_MT4_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// DLL_TEST_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#include "mt_data/mt_data.h"

struct WT_DAC_TYPE
{
	double  gain;
	double  sampling_freq;
	int     source;
	int     wave_preset;
	char    name[32]; 
	char    server[32];
};

#define DLL_MT4_EXPORTS
#ifdef DLL_MT4_EXPORTS
#define DLL_MT4_API __declspec(dllexport)
#else
#define DLL_MT4_API __declspec(dllimport)
#endif

EXTERN_C DLL_MT4_API int GetStructValue(WT_DAC_TYPE *dac,int size);
EXTERN_C DLL_MT4_API int SetArrayItemValue(double *arr,const int arraysize,const int nitem,const double value);
EXTERN_C DLL_MT4_API int add_test(int a, int b);
EXTERN_C DLL_MT4_API int mt_module_initial();
EXTERN_C DLL_MT4_API int uninstall_dll();
EXTERN_C DLL_MT4_API int CheckLicense(char *server_name, char *account_number);
EXTERN_C DLL_MT4_API int GetOrderInfo(Tran_Var_Array *info);
EXTERN_C DLL_MT4_API int SetOrderInfo(Tran_Var_Array *info);
EXTERN_C DLL_MT4_API int SetOrderCommand(int trade_type, Tran_Var_Array *info);
EXTERN_C DLL_MT4_API int GetAccountInfo(Account_Info *info);
EXTERN_C DLL_MT4_API int SetAccountInfo(Account_Info *info);
EXTERN_C DLL_MT4_API int GetSymbolInfo(Symbol_Info *info);
EXTERN_C DLL_MT4_API int SetSymbolInfo(Symbol_Info *info, int symbol_flag);

EXTERN_C DLL_MT4_API int set_watch_dog_value(int value);
EXTERN_C DLL_MT4_API int get_watch_dog_value();
EXTERN_C DLL_MT4_API int GetOrderStatus(int shock_type);
EXTERN_C DLL_MT4_API int SetOrderCount(int shock_type, int order_count);
EXTERN_C DLL_MT4_API int SetConnectedFlag(int value);
EXTERN_C DLL_MT4_API int GetConnectedFlag();

EXTERN_C DLL_MT4_API int GetLogInfo(MT_Log *info);
EXTERN_C DLL_MT4_API int SetLogInfo(MT_Log *info);
EXTERN_C DLL_MT4_API void startup_exe();
