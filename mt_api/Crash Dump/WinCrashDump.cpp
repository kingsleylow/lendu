#include <windows.h>
#include <DbgHelp.h>
#include <stdlib.h>
#include <stdio.h>
#include  <time.h>
#define CRASH_DUMP_EXT
#include "WinCrashDump.h"
#include "../log/mt_log_manager.h"
#pragma comment(lib, "dbghelp.lib")
#include <process.h>


BOOL IsDataSectionNeeded(const WCHAR *pModuleName)
{
	WCHAR szFileName[_MAX_FNAME] = L"";

	if(pModuleName == NULL)
	{
		return FALSE;
	}

	_wsplitpath(pModuleName, NULL, NULL, szFileName, NULL);

	if(wcsicmp(szFileName, L"ntdll") == 0)
	{ return TRUE; }

	return FALSE;
}

BOOL CALLBACK MiniDumpCallback(PVOID pParam,
							   const PMINIDUMP_CALLBACK_INPUT   pInput,
							   PMINIDUMP_CALLBACK_OUTPUT        pOutput)
{
	if(pInput == 0 || pOutput == 0)
	{ return FALSE; }

	switch(pInput->CallbackType)
	{
		case ModuleCallback:
			if(pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
			{
				if(!IsDataSectionNeeded(pInput->Module.FullPath))
				{
					pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
				}
			}
		case IncludeModuleCallback:
		case IncludeThreadCallback:
		case ThreadCallback:
		case ThreadExCallback:
			return TRUE;
		default:
			;
	}

	return FALSE;
}

void CreateDumpFile(EXCEPTION_POINTERS *pException, LPCTSTR lpstrDumpFilePathName)
{
	MINIDUMP_TYPE MiniDumpWithDataSegs = (MINIDUMP_TYPE)(MiniDumpNormal);
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	MINIDUMP_CALLBACK_INFORMATION mci;
	HANDLE hDumpFile = CreateFileA(lpstrDumpFilePathName,
								   GENERIC_READ | GENERIC_WRITE,
								   FILE_SHARE_WRITE,
								   NULL,
								   CREATE_ALWAYS,
								   FILE_ATTRIBUTE_NORMAL,
								   NULL);

	if((hDumpFile != NULL) && (hDumpFile != INVALID_HANDLE_VALUE))
	{
		dumpInfo.ExceptionPointers = pException;
		dumpInfo.ThreadId = GetCurrentThreadId();
		dumpInfo.ClientPointers = TRUE;

		MiniDumpWithDataSegs = (MINIDUMP_TYPE)(MiniDumpNormal
											   | MiniDumpWithHandleData
											   | MiniDumpWithUnloadedModules
											   | MiniDumpWithIndirectlyReferencedMemory
											   | MiniDumpScanMemory
											   | MiniDumpWithProcessThreadData
											   | MiniDumpWithThreadInfo);

		mci.CallbackRoutine     = (MINIDUMP_CALLBACK_ROUTINE)MiniDumpCallback;
		mci.CallbackParam       = 0;

		MiniDumpWriteDump(GetCurrentProcess(),
						  GetCurrentProcessId(),
						  hDumpFile,
						  MiniDumpWithDataSegs,
						  &dumpInfo,
						  NULL,
						  &mci);

		CloseHandle(hDumpFile);
	}
}

//----------------------------------------------------------------
//功能:处理异常的回调函数
//参数:
//  pException。异常信息
//返回:
//----------------------------------------------------------------
LONG CrashHandler(EXCEPTION_POINTERS *pException)
{
	char szFileName[MAX_PATH] = {0};

	//StoreAllCaptureDatas();
	//MoTerm();

	return EXCEPTION_EXECUTE_HANDLER;
}

//----------------------------------------------------------------
//功能:处理异常的UI显示
//返回:TRUE/FALSE
//----------------------------------------------------------------
LONG WINAPI UnhandledExceptionFilterEx(PEXCEPTION_POINTERS pExceptionInfo)
{
	CrashHandler(pExceptionInfo);
	return EXCEPTION_EXECUTE_HANDLER;
}

//----------------------------------------------------------------
//功能:处理异常的过滤回调
//参数:
//  pException。异常信息
//返回:
//----------------------------------------------------------------
LPTOP_LEVEL_EXCEPTION_FILTER WINAPI WT_DummySetUnhandledExceptionFilter(LPTOP_LEVEL_EXCEPTION_FILTER lpTopLevelExceptionFilter)
{
	return NULL;
}

// 此函数一旦成功调用，之后对 SetUnhandledExceptionFilter 的调用将无效
BOOL PreventSetUnhandledExceptionFilter()
{
	void *pOrgEntry = NULL;
	void *pNewFunc = NULL;
	unsigned char newJump[100] = {0};
	DWORD dwOrgEntryAddr = 0;
	DWORD dwNewEntryAddr = 0;
	DWORD dwRelativeAddr = 0;
	SIZE_T bytesWritten = 0;
	BOOL bRet = FALSE;

	HMODULE hKernel32 = LoadLibraryA("kernel32.dll");
	if(NULL == hKernel32)
	{ return FALSE; }


	pOrgEntry = GetProcAddress(hKernel32, "SetUnhandledExceptionFilter");
	if(NULL == pOrgEntry)
	{
		FreeLibrary(hKernel32);
		return FALSE;
	}


	dwOrgEntryAddr = (DWORD) pOrgEntry;
	dwOrgEntryAddr += 5; // add 5 for 5 op-codes for jmp far


	pNewFunc = &WT_DummySetUnhandledExceptionFilter;
	dwNewEntryAddr = (DWORD) pNewFunc;
	dwRelativeAddr = dwNewEntryAddr -  dwOrgEntryAddr;


	newJump[ 0 ] = 0xE9;  // JMP absolute
	memcpy(&newJump[1], &dwRelativeAddr, sizeof(pNewFunc));

	bRet = WriteProcessMemory(GetCurrentProcess(), pOrgEntry, newJump, sizeof(pNewFunc) + 1, &bytesWritten);
	return bRet;
}

void RunCrashHandler()
{
	//注册异常处理函数
	SetUnhandledExceptionFilter(UnhandledExceptionFilterEx);

	//使之后的SetUnhandledExceptionFilter无效
	PreventSetUnhandledExceptionFilter();
}

typedef struct
{
	BOOL IsRunning;
	PROCESS_INFORMATION ProcessInfo;
	DWORD ProcessExitCode;
}ProcessControl;

static ProcessControl sProcessControler = { 0 };

static HANDLE ProcDump_Handle = INVALID_HANDLE_VALUE;
static unsigned int __stdcall ThreadProcDumpFun(LPVOID pM)
{
	const char *proc_dump_exe = "procdump.exe";
	STARTUPINFO startInfo;
	PROCESS_INFORMATION process_info;
	BOOL result = FALSE;
	char cmdline[MAX_PATH] = {0};

	memset(&startInfo, 0, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(STARTUPINFO);
	startInfo.dwFlags = STARTF_USESHOWWINDOW;
	startInfo.wShowWindow = SW_HIDE;

	sprintf(cmdline, "%s -accepteula -e -f \"\" %d", proc_dump_exe, GetCurrentProcessId());
	result = CreateProcessA(NULL, cmdline, NULL, NULL, 0, CREATE_NO_WINDOW, NULL, NULL, &startInfo, &process_info);

	if (result == TRUE)
	{
		sProcessControler.ProcessInfo = process_info;
		GetExitCodeProcess(process_info.hProcess, &sProcessControler.ProcessExitCode);
		sProcessControler.IsRunning = TRUE;
		Log_manager::getInstance()->warn_log("CreateProcessA Sucess\n");
	}
	else
	{
		Log_manager::getInstance()->warn_log("CreateProcessA fail\n");
	}

	// Wait until child process exits.
	WaitForSingleObject(process_info.hProcess, INFINITE);

	// Close process and thread handles.
	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
	return 1;
}

void RegesterProcDump()
{
	RunCrashHandler();
}

int CreateDumpProcess()
{
	if (sProcessControler.IsRunning == FALSE)
	{
		int i = 0;
		ProcDump_Handle = (HANDLE)_beginthreadex(NULL, 0, ThreadProcDumpFun, NULL, 0, NULL);
		for (i = 0; i < 1000; i++)
		{
			if (sProcessControler.IsRunning == TRUE)
			{
				return 0;
			}
			Sleep(1);
		}
	}
	Log_manager::getInstance()->warn_log("CreateDumpProcess fail\n");
	return 1;
}

int TerminateDumpProcess()
{
	if (sProcessControler.IsRunning == TRUE)
	{
		BOOL result = FALSE;

		result = TerminateProcess(sProcessControler.ProcessInfo.hProcess, sProcessControler.ProcessExitCode);
		if (result == TRUE)
		{
			sProcessControler.IsRunning = FALSE;
			Log_manager::getInstance()->warn_log("TerminateDumpProcess Sucess\n");
			return 0;
		}
	}
	Log_manager::getInstance()->warn_log("TerminateDumpProcess fail\n");
	return 1;
}