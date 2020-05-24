#ifndef _WIN_CRASH_DUMP_H_
#define _WIN_CRASH_DUMP_H_

#ifndef CRASH_DUMP_EXT
#define CRASH_EXTERN extern
#else
#define CRASH_EXTERN
#endif

CRASH_EXTERN void RunCrashHandler();
CRASH_EXTERN void RegesterProcDump();
CRASH_EXTERN int CreateDumpProcess();
CRASH_EXTERN int TerminateDumpProcess();
#endif