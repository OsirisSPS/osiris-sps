#ifndef _CRASHRPT_H
#define _CRASHRPT_H

#ifdef CRASHRPT_LIB
	#define CRASHRPTAPI extern "C" __declspec(dllexport)
#else
	#define CRASHRPTAPI extern "C" __declspec(dllimport)
#endif

// Client crash callback
typedef BOOL (CALLBACK *LPCRASHRPTCALLBACK) (LPVOID lpvState);

#ifndef _DEBUG

#ifndef CRASHRPT_LIB
	#pragma comment(lib, "CrashRpt.lib")
#endif

//-----------------------------------------------------------------------------
// Install
//    Initializes the library and optionally set the client crash callback and
//    sets up the email details.
//
// Parameters
//    pfn         Client crash callback
//    lpTo        Email address to send crash report
//    lpSubject   Subject line to be used with email
//
// Return Values
//    If the function succeeds, the return value is a pointer to the underlying
//    crash object created.  This state information is required as the first
//    parameter to all other crash report functions.
//
// Remarks
//    Passing NULL for lpTo will disable the email feature and cause the crash 
//    report to be saved to disk.
//
CRASHRPTAPI LPVOID CrashRptInstall(LPCRASHRPTCALLBACK pfn, const char *lpMailTo, const char *lpMailSubject);

//-----------------------------------------------------------------------------
// Uninstall
//    Uninstalls the unhandled exception filter set up in Install().
//
// Parameters
//    lpState     State information returned from Install()
//
// Return Values
//    void
//
// Remarks
//    This call is optional.  The crash report library will automatically 
//    deinitialize when the library is unloaded.  Call this function to
//    unhook the exception filter manually.
//
CRASHRPTAPI void CrashRptUninstall(LPVOID lpState);

//-----------------------------------------------------------------------------
// AddFile
//    Adds a file to the crash report.
//
// Parameters
//    lpState     State information returned from Install()
//    lpFile      Fully qualified file name
//    lpDesc      Description of file, used by details dialog
//
// Return Values
//    void
//
// Remarks
//    This function can be called anytime after Install() to add one or more
//    files to the generated crash report.
//
CRASHRPTAPI void CrashRptAddFile(LPVOID lpState, const char *lpFile, const char *lpDesc);

#else // _DEBUG

#ifndef CRASHRPT_LIB
	#define CrashRptInstall(pfn, lpTo, lpSubject)		((LPVOID) 0)
	#define CrashRptUninstall(lpState)
	#define CrashRptAddFile(lpState, lpFile, lpDesc)
#endif

#endif // _DEBUG

#endif // _CRASHRPT_H
