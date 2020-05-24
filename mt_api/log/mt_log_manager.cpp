#include <stdio.h>
#include <stdarg.h>
#include "mt_log_manager.h"
Log_manager::Log_manager()
{
	log_level = 0;
	memset(m_log_path, 0, sizeof(m_log_path));
	memset(log_buf, 0, sizeof(log_buf));
	strcat_s(m_log_path, "lendu_dll.log");
}

Log_manager::~Log_manager()
{
}

Log_manager* Log_manager::getInstance()
{
	static Log_manager instance;
	return &instance;
}

void Log_manager::SetLogName(const char* path)
{
	memset(m_log_path, 0, sizeof(m_log_path));
	strcat_s(m_log_path, path);
	strcat_s(m_log_path, "lendu_dll.log");
}

void Log_manager::MT_Log_Save(int log_len, const char *log_str)
{
	FILE *fp = NULL;	/* 用于打开指定的文件 */
	fopen_s(&fp, m_log_path, "a+");
	if (fp != NULL)
	{
		strcat_s(log_buf, log_str);
		strcat_s(log_buf, "\n");
		fwrite(log_buf, log_len + 1, 1, fp);
		fclose(fp);
		memset(log_buf, 0, sizeof(log_buf));
	}
}

void Log_manager::warn_log(const char *format, ...)
{
	time_t now;
	struct tm *timenow;
	va_list ap;
	FILE *fp = NULL;
	char buf[1024] = {0};

	va_start(ap, format);
	vsprintf(buf, format, ap);
	va_end(ap);
	fopen_s(&fp, m_log_path, "a+");
	if (fp != NULL)
	{
		fprintf(fp, "%s\n", buf);
		fclose(fp);
	}
}