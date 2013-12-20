// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#include "stdafx.h"
#include "platformwin.h"

#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/scoped_array.hpp"
#include <iphlpapi.h>
#include "log.h"
#include <shellapi.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <tchar.h>
#include "unicode.h"
#include "utils.h"
#include "dbghelp.h"
#include <wininet.h>
#include <Psapi.h>

#ifndef _WIN64
	#include "crashrpt.h"
#endif

//////////////////////////////////////////////////////////////////////

#define DBGHLP_BUFFERSIZE			1024
#define DBGHLP_SYMBOLS_PATH			_S("symbols")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class pimpl<PlatformManager>::RegKey
{
// Construction
public:
	RegKey(HKEY handle = nullptr);
	~RegKey();

// Operations
public:
	void attach(HKEY handle);
	HKEY detach();

	void close();

// Operators
public:
	inline operator HKEY() const;
	inline operator PHKEY();

protected:
	HKEY m_handle;
};

//////////////////////////////////////////////////////////////////////

inline pimpl<PlatformManager>::RegKey::operator HKEY() const { return m_handle; }
inline pimpl<PlatformManager>::RegKey::operator PHKEY() { return &m_handle; }

//////////////////////////////////////////////////////////////////////

static unsigned int ansi_from_ucs2_length(const wchar *s)
{
    if(s == nullptr)
        return 0;

    return ::WideCharToMultiByte(CP_ACP, 0, s, -1, NULL, 0, NULL, NULL);
}

static unsigned int ucs2_from_ansi_length(const char *s)
{
    if(s == nullptr)
        return 0;

    return ::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, -1, NULL, 0);
}

static unsigned int utf8_from_ucs2_length(const wchar *s)
{
    if(s == nullptr)
        return 0;

    return ::WideCharToMultiByte(CP_UTF8, 0, s, -1, NULL, 0, NULL, NULL);
}

static unsigned int ucs2_from_utf8_length(const char *s)
{
    if(s == nullptr)
        return 0;

	return ::MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0);
}

//////////////////////////////////////////////////////////////////////

pimpl<PlatformManager>::RegKey::RegKey(HKEY handle) : m_handle(handle)
{

}

pimpl<PlatformManager>::RegKey::~RegKey()
{
	close();
}

void pimpl<PlatformManager>::RegKey::attach(HKEY handle)
{
	close();
	m_handle = handle;
}

HKEY pimpl<PlatformManager>::RegKey::detach()
{
	HKEY handle = m_handle;
	m_handle = nullptr;
	return handle;
}

void pimpl<PlatformManager>::RegKey::close()
{
	if(m_handle != nullptr)
	{
		RegCloseKey(m_handle);
		m_handle = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////

static int32 convertThreadPriority(TaskPriority priority)
{
	switch(priority)
	{
	case taskPriorityCritical:			return THREAD_PRIORITY_TIME_CRITICAL;
	case taskPriorityHigh:				return THREAD_PRIORITY_HIGHEST;
	case taskPriorityAboveNormal:		return THREAD_PRIORITY_ABOVE_NORMAL;
	case taskPriorityNormal:			return THREAD_PRIORITY_NORMAL;
	case taskPriorityBelowNormal:		return THREAD_PRIORITY_BELOW_NORMAL;
	case taskPriorityLow:				return THREAD_PRIORITY_LOWEST;
	case taskPriorityIdle:				return THREAD_PRIORITY_IDLE;

	default:							OS_ASSERTFALSE();
										break;
	}

	return THREAD_PRIORITY_NORMAL;
}

//////////////////////////////////////////////////////////////////////

pimpl<PlatformManager>::pimpl()
{
	// High-resolution performance counter
	static LARGE_INTEGER F;
	QueryPerformanceFrequency(&F);
	if(F.QuadPart == 0)
	{
		// installed hardware does not support a high-resolution performance counter
		hiResFrequency = 0;
	}
	else
	{
		hiResFrequency = double(F.QuadPart)/1000;
	}
}

pimpl<PlatformManager>::~pimpl()
{

}

void pimpl<PlatformManager>::init(PlatformManager::ApplicationType type)
{
#ifndef _WIN64
	// URGENT: gestire la configurazione dall'esterno
	CrashRptInstall(NULL, "bugreport@osiris-sps.org", "Crash Report");
#endif
}

void pimpl<PlatformManager>::cleanup()
{

}

String pimpl<PlatformManager>::getExpandedEnvironmentPath(const String &path) const
{
	wchar outpath[_MAX_PATH];
	ExpandEnvironmentStrings(path.c_str(), outpath, _MAX_PATH);
	return outpath;
}

String pimpl<PlatformManager>::getUserDataPath() const
{
	wchar path[_MAX_PATH];
	if(SUCCEEDED(SHGetFolderPath(nullptr, CSIDL_APPDATA, NULL, 0, path)))
		return path;

	return String::EMPTY;
}

String pimpl<PlatformManager>::getTempPath() const
{
	wchar path[MAX_PATH];
	if(::GetTempPath(MAX_PATH, path) == 0)
		return String::EMPTY;

	return path;
}

String pimpl<PlatformManager>::getTempFilePath() const
{
	wchar path[MAX_PATH];
	if(::GetTempFileName(getTempPath().c_str(), NULL, 0, path) == 0)
		return String::EMPTY;

	return path;
}

bool pimpl<PlatformManager>::isPrivilegedUser() const
{
	// 0.14 RC2
	return (IsUserAnAdmin() == TRUE);
	/*
	bool done = false;

	HKEY hKey;
	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE, NULL, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
	{
		done = true;
		RegCloseKey(hKey);
	}

	return done;
	*/
}

int pimpl<PlatformManager>::getTimeOffset() const
{
	//It is used to access DST and other timezone related information and sets all timezone global variables
    tzset(); 

	return -timezone; // CEST, not WEST
}

int pimpl<PlatformManager>::getTimeDST() const
{
	//It is used to access DST and other timezone related information and sets all timezone global variables
    tzset(); 

	// TODO: Not universally correct.
	if(daylight != 0)
		return 3600; // One hour is the default
	else
		return 0;
}

void * pimpl<PlatformManager>::findProcedure(handle hLibrary, const String &procedure) const
{
	OS_ASSERT(hLibrary != 0);
	return static_cast<void *>(GetProcAddress(reinterpret_cast<HMODULE>(hLibrary), procedure.to_ascii().c_str()));
}

handle pimpl<PlatformManager>::loadLibrary(const String &libraryName) const
{
	// LOAD_WITH_ALTERED_SEARCH_PATH serve per poter caricare delle plugin e poter mettere le dll da cui dipendono
	// nella stessa directory (altrimenti verrebbero cercate nella directory dell'applicazione ma non in quella in cui
	// si trova la plugin). L'unica eccezione è che il path della dll sia completo

	wchar path[MAX_PATH];
	OS_ZEROMEMORY(path, MAX_PATH);

	wchar *filename = nullptr;
	::GetFullPathName(libraryName.c_str(), MAX_PATH, path, &filename);
	HMODULE hModule = LoadLibraryEx(path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

	if(hModule == nullptr)
	{
		String error = _S("Error loading library '") + libraryName + _S("'");

		String lastError = getLastError();
		if(lastError.empty() == false)
			error += _S(", details: ") + lastError;

		OS_LOG_ERROR(error);
	}

	return reinterpret_cast<handle>(hModule);
}

void pimpl<PlatformManager>::unloadLibrary(handle hLibrary) const
{
	FreeLibrary(reinterpret_cast<HMODULE>(hLibrary));
}

boost::any pimpl<PlatformManager>::getCurrentThreadID()
{
	// N.B.: viene castato a uint32 per consentire dall'esterno la possibilità di tentare un cast con boost::any_cast<uint32>
	OS_STATIC_ASSERT(sizeof(DWORD) == sizeof(uint32));

	return static_cast<uint32>(GetCurrentThreadId());
}

bool pimpl<PlatformManager>::setThreadPriority(const boost::any &threadID, TaskPriority priority)
{
	OS_ASSERT(boost::any_cast<uint32>(&threadID) != nullptr);

	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, boost::any_cast<uint32>(threadID));
	if(hThread == nullptr)
		return false;

    bool done = SetThreadPriority(hThread, convertThreadPriority(priority)) == TRUE;
	CloseHandle(hThread);

	return done;
}

bool pimpl<PlatformManager>::compareThreadsID(const boost::any &first, const boost::any &second)
{
	OS_ASSERT(boost::any_cast<uint32>(&first) != nullptr);
	OS_ASSERT(boost::any_cast<uint32>(&second) != nullptr);

	return boost::any_cast<uint32>(first) == boost::any_cast<uint32>(second);
}

uint32 pimpl<PlatformManager>::getCurrentProcessID()
{
	return static_cast<uint32>(::GetCurrentProcessId());
}

shared_ptr<PlatformManager::ProcessDetails> pimpl<PlatformManager>::getProcessDetails(uint32 processID)
{
	HANDLE processHandle = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, static_cast<DWORD>(processID));
	if(processHandle == nullptr)
		return nullptr;

	DWORD exitCode = 0; 
	if(GetExitCodeProcess(processHandle, &exitCode) == FALSE)
		exitCode = STILL_ACTIVE;

	shared_ptr<PlatformManager::ProcessDetails> processDetails;
	
	if(exitCode == STILL_ACTIVE)
	{
		processDetails.reset(OS_NEW_T(PlatformManager::ProcessDetails)(processID), os_delete_t());
		
		TCHAR buffer[MAX_PATH];
		OS_ZEROMEMORY(buffer, MAX_PATH);
		if(GetModuleFileNameEx(processHandle, NULL, buffer, MAX_PATH) != 0)
			processDetails->executablePath = utils::standardisePath(buffer, false);
	}

	CloseHandle(processHandle);
	
	return processDetails;
}

double pimpl<PlatformManager>::tickCount()
{
	if(hiResFrequency != 0)
	{
		LARGE_INTEGER C;
		QueryPerformanceCounter(&C);
		return double(C.QuadPart)/hiResFrequency;
	}
	else
	{
		return static_cast<double>(GetTickCount());
	}
}

void pimpl<PlatformManager>::trace(const std::string &str, const char *file, size_t line, const char *function)
{
	// N.B.: non usare qui una String::format per evitare che in fase di terminazione vengano fatte allocazioni/deallocazioni custom

	if(file != nullptr && line != 0)
	{
		std::stringstream stream;
		stream << file << " (" << line << "): ";
		OutputDebugStringA(stream.str().c_str());
	}
	
	OutputDebugStringA(str.c_str());	
}

String pimpl<PlatformManager>::normalizePath(const String& path) const
{
	// Under Windows, if we use only "/" in path, protocol & file association don't work. (Tested on XP).

	String out = path;	
	out.replace_all(_S("/"),_S("\\"));
	return out;
}

bool pimpl<PlatformManager>::contentTypeFromExtension(const String &extension, String &content_type)
{
	String regExtension = extension;
	if(regExtension.starts_with(_S(".")) == false)
		regExtension = _S(".") + regExtension;

	bool done = false;

	HKEY hKey;
	if(RegOpenKeyEx(HKEY_CLASSES_ROOT, regExtension.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
	{
		wchar buffer[_MAX_PATH];
		DWORD size = _MAX_PATH;
		DWORD type = REG_SZ;
		if(RegQueryValueEx(hKey, _S("Content Type").c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &size) == ERROR_SUCCESS)
		{
			content_type = buffer;
			done = true;
		}

		RegCloseKey(hKey);
	}

	return done;
}

bool pimpl<PlatformManager>::extensionFromContentType(const String &content_type, String &extension)
{
	bool done = false;

	HKEY hMime;
	if(RegOpenKeyEx(HKEY_CLASSES_ROOT, _S("MIME").c_str(), 0, KEY_READ, &hMime) == ERROR_SUCCESS)
	{
		HKEY hDatabase;
		if(RegOpenKeyEx(hMime, _S("Database").c_str(), 0, KEY_READ, &hDatabase) == ERROR_SUCCESS)
		{
			HKEY hContentType;
			if(RegOpenKeyEx(hDatabase, _S("Content Type").c_str(), 0, KEY_READ, &hContentType) == ERROR_SUCCESS)
			{
				HKEY hKey;
				if(RegOpenKeyEx(hContentType, content_type.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS)
				{
					wchar buffer[_MAX_PATH];
					DWORD size = _MAX_PATH;
					DWORD type = REG_SZ;
					if(RegQueryValueEx(hKey, _S("Extension").c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer), &size) == ERROR_SUCCESS)
					{
						extension = buffer;
						done = true;
					}

					RegCloseKey(hKey);
				}

				RegCloseKey(hContentType);
			}

			RegCloseKey(hDatabase);
		}

		RegCloseKey(hMime);
	}

	return done;
}

bool pimpl<PlatformManager>::execute(const String &command,const String &parameters)
{
	LPCWSTR lpParameters = nullptr;
	if(parameters.empty() == false)
		lpParameters = parameters.c_str();
	ShellExecute(nullptr, _S("open").c_str(), command.c_str(), lpParameters, nullptr, SW_SHOWNORMAL);
	return true;
}

String pimpl<PlatformManager>::getSystemCulture()
{
	int32 langId = GetSystemDefaultLangID();

	langId = langId % 1024;

	String langCode;

	switch(langId)
	{
	case 0x09:	langCode = _S("en-US");
				break;

	case 0x10:	langCode = _S("it-IT");
				break;

	case 0x0c:	langCode = _S("fr-FR");
				break;

	case 0x07:	langCode = _S("de-DE");
				break;

	default:	langCode = _S("en-US");
				break;
	}

	return langCode;
}

String pimpl<PlatformManager>::getEnv(const String &name)
{
	return utils::safeString(_wgetenv(name.c_str()));
}

bool pimpl<PlatformManager>::setEnv(const String &name, const String &value)
{
	String str = name + _S("=") + value;
	return _wputenv(str.c_str()) == 0;
}

String pimpl<PlatformManager>::getLastError() const
{
	String error;

	LPVOID lpMsgBuf = nullptr;
	if(FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					 nullptr,
                     GetLastError(),
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					 reinterpret_cast<LPTSTR>(&lpMsgBuf),
					 0,
					 nullptr))
	{
		error = utils::safeString(reinterpret_cast<LPCTSTR>(lpMsgBuf));
		LocalFree(lpMsgBuf);
	}

	return error;
}

void pimpl<PlatformManager>::sleep(uint32 milliseconds)
{
	Sleep(milliseconds);
}

FILE * pimpl<PlatformManager>::fileOpen(const String &filename, const String &mode)
{
	return _wfopen(filename.c_str(), mode.c_str());
}

bool pimpl<PlatformManager>::fileClose(FILE *handle)
{
	return fclose(handle) == 0;
}

uint32 pimpl<PlatformManager>::fileRead(FILE *handle, void *v, uint32 size)
{
	return static_cast<uint32>(fread(reinterpret_cast<byte *>(v), 1, size, handle));
}

uint32 pimpl<PlatformManager>::fileWrite(FILE *handle, const void *v, uint32 size)
{
	return static_cast<uint32>(fwrite(reinterpret_cast<const byte *>(v), 1, size, handle));
}

bool pimpl<PlatformManager>::fileSeek(FILE *handle, uint64 offset, int32 origin)
{
	return _fseeki64(handle, offset, origin) == 0;
}

uint64 pimpl<PlatformManager>::filePosition(FILE *handle)
{
	return _ftelli64(handle);
}

bool pimpl<PlatformManager>::fileFlush(FILE *handle)
{
	return fflush(handle) == 0;
}

bool pimpl<PlatformManager>::fileEof(FILE *handle)
{
	return feof(handle) != 0;
}

bool pimpl<PlatformManager>::fileStats(FILE *handle, boost::posix_time::ptime *timeCreation, boost::posix_time::ptime *timeLastModify, boost::posix_time::ptime *timeLastAccess)
{
	if(handle == nullptr)
	{
		OS_ASSERTFALSE();
		return false;
	}

	struct stat stats;
	if(fstat(fileno(handle), &stats) != 0)
		return false;

	if(timeCreation != nullptr)
		*timeCreation = boost::posix_time::from_time_t(stats.st_ctime);

	if(timeLastAccess != nullptr)
		*timeLastAccess = boost::posix_time::from_time_t(stats.st_atime);

	if(timeLastModify != nullptr)
		*timeLastModify = boost::posix_time::from_time_t(stats.st_mtime);

	return true;
}

void pimpl<PlatformManager>::debugBreak()
{
	DebugBreak();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
