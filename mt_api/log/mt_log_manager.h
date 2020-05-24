#pragma once
#include <string>
using namespace std;
static char m_log_path[1024];
static char log_buf[1024];
class Log_manager
{
private:
	int log_level;
private:
	Log_manager();
	~Log_manager();
public:
	static Log_manager* getInstance();
	static void SetLogName(const char* path);
	static void MT_Log_Save(int log_len, const char *log_str);
	static void warn_log(const char *format, ...);
};
