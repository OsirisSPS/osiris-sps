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
#include "osirisimpl.h"

#include "boost/bind.hpp"
#include "boost/filesystem/operations.hpp"
#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/weak_ptr.hpp"
#include <iostream>
#include <sstream>

//////////////////////////////////////////////////////////////////////

static nsresult convert_result(os_result result)
{
	switch(result)
	{
	// Results with a corrispective value
	case OS_SDK_OK:								return NS_OK;
	case OS_SDK_ERROR_NOT_INITIALIZED:			return NS_ERROR_NOT_INITIALIZED;
	case OS_SDK_ERROR_ALREADY_INITIALIZED:		return NS_ERROR_ALREADY_INITIALIZED;
	case OS_SDK_ERROR_INVALID_PARAM:			return NS_ERROR_INVALID_ARG;
	case OS_SDK_ERROR_ILLEGAL_VALUE:			return NS_ERROR_ILLEGAL_VALUE;
	case OS_SDK_ERROR_NOT_FOUND:				return NS_ERROR_NOT_AVAILABLE;

	// Results with not a corrispective value
	case OS_SDK_ERROR_GENERIC:
	case OS_SDK_ERROR_BUFFER_TOO_SMALL:
												return NS_ERROR_FAILURE;

	default:									NS_ASSERTION(false, "Unknown SDK result value");
												break;
	}

	return OS_SDK_SUCCEEDED(result) ? NS_OK : NS_ERROR_UNEXPECTED;
}

static std::string to_string(const nsAString &str, nsCStringEncoding encoding)
{
	std::string result;

	nsCString tmp;
	if(NS_SUCCEEDED(NS_UTF16ToCString(str, encoding, tmp)))
		result.assign(static_cast<std::string::const_pointer>(tmp.get()), static_cast<std::string::size_type>(tmp.Length()));

	return result;
}

static std::string to_ascii(const nsAString &str)
{
	return to_string(str, NS_CSTRING_ENCODING_ASCII);
}

static std::string to_utf8(const nsAString &str)
{
	return to_string(str, NS_CSTRING_ENCODING_UTF8);
}

static nsString from_string(const std::string &str, nsCStringEncoding encoding)
{
	nsCString tmp;
	tmp.Assign(static_cast<const nsCString::char_type *>(str.data()), static_cast<nsCString::size_type>(str.size()));

	nsString result;
	if(NS_SUCCEEDED(NS_CStringToUTF16(tmp, encoding, result)))
		return result;

	return nsString();
}

static nsString from_ascii(const std::string &str)
{
	return from_string(str, NS_CSTRING_ENCODING_ASCII);
}

static nsString from_utf8(const std::string &str)
{
	return from_string(str, NS_CSTRING_ENCODING_UTF8);
}

//////////////////////////////////////////////////////////////////////

#ifdef XP_WIN
    #define DYNLIB_FILE_EXTENSION       ".dll"
#elif defined(XP_MACOSX)
    #define DYNLIB_FILE_EXTENSION       ".dylib"
#elif defined(XP_UNIX)
    #define DYNLIB_FILE_EXTENSION       ".so"
#else
    #error Unsupported platform
#endif

//////////////////////////////////////////////////////////////////////

typedef void * handle;

//////////////////////////////////////////////////////////////////////

#ifdef XP_WIN
	static void * findLibraryProcedure(handle hLibrary, const std::string &procedure)
    {
		return GetProcAddress(static_cast<HMODULE>(hLibrary), procedure.c_str());
    }

    static handle loadLibrary(const nsString &libraryName)
    {
		// NS_StringCloneData to ensure a nullptr terminated buffer
		boost::shared_ptr<PRUnichar> data(NS_StringCloneData(libraryName), &nsMemory::Free);
		return static_cast<handle>(LoadLibraryEx(data.get(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH));
    }

    static void unloadLibrary(handle hLibrary)
    {
		FreeLibrary(static_cast<HMODULE>(hLibrary));
    }
#else
    static void * findLibraryProcedure(handle hLibrary, const std::string &procedure)
    {
		return dlsym(static_cast<void *>(hLibrary), procedure.c_str());
    }

    static handle loadLibrary(const nsString &libraryName)
    {
		return static_cast<handle>(dlopen(to_utf8(libraryName).c_str(), RTLD_LAZY));
    }

    static void unloadLibrary(handle hLibrary)
    {
		dlclose(static_cast<void *>(hLibrary));
    }
#endif

//////////////////////////////////////////////////////////////////////

static boost::filesystem::path createPath(const nsString &path)
{
	return boost::filesystem::path(to_utf8(path), boost::filesystem::native);
}

//////////////////////////////////////////////////////////////////////

class CurrentDirectory
{
public:
	CurrentDirectory(const nsString &dir)
	{
		try
		{
			m_prevPath = boost::filesystem::current_path();
			boost::filesystem::current_path(createPath(dir));
		}
		catch(std::exception &e)
		{
			std::cerr << "CurrentDirectory::CurrentDirectory error: " << e.what() << std::endl;
		}
	}

	~CurrentDirectory()
	{
		try
		{
			if(m_prevPath.empty() == false)
				boost::filesystem::current_path(m_prevPath);
		}
		catch(std::exception &e)
		{
			std::cerr << "CurrentDirectory::~CurrentDirectory error: " << e.what() << std::endl;
		}	
	}

private:
	boost::filesystem::path m_prevPath;
};

//////////////////////////////////////////////////////////////////////

class DynLib
{
// Construction
public:
	DynLib();
    ~DynLib();

// Attributes
public:
	inline bool loaded() const;

// Operations
public:
	void * findProcedure(const std::string &procedure) const;

	bool load(const nsString &filename);
	void unload();

public:
	handle m_handle;
};

//////////////////////////////////////////////////////////////////////

DynLib::DynLib() : m_handle(NULL)
{

}

DynLib::~DynLib()
{
	unload();
}

inline bool DynLib::loaded() const
{
    return m_handle != NULL;
}

void * DynLib::findProcedure(const std::string &procedure) const
{
	return findLibraryProcedure(m_handle, procedure);
}

bool DynLib::load(const nsString &filename)
{
	unload();

	nsString str;
	str.Append(filename);
	str.Append(from_ascii(DYNLIB_FILE_EXTENSION));
	m_handle = loadLibrary(str);
	return m_handle != NULL;
}

void DynLib::unload()
{
	if(m_handle != NULL)
	{
		unloadLibrary(m_handle);
		m_handle = NULL;
	}
}

//////////////////////////////////////////////////////////////////////

static void log_callback(const char *message, os_uint8 level, os_time timestamp)
{
	std::cout << "Osiris log: " << message << std::endl;
}

//////////////////////////////////////////////////////////////////////

NS_IMPL_ISUPPORTS1(OsirisImpl, IOsiris)

//////////////////////////////////////////////////////////////////////

class OsirisInterface
{
	typedef os_result (* os_sdk_init_callback)(int argc, char *argv[], const char *root_path);
	typedef os_result (* os_sdk_cleanup_callback)();
	typedef os_result (* os_sdk_start_callback)();
	typedef os_result (* os_sdk_stop_callback)();
	typedef os_result (* os_sdk_get_option_callback)(const char *, void *, os_uint32 *, os_uint32 *);
	typedef os_result (* os_sdk_set_option_callback)(const char *, const void *, os_uint32);
	typedef os_result (* os_sdk_log_handler_callback)(os_log_handler);
	typedef os_result (* os_sdk_parse_link_callback)(const char *, char *, os_uint32 *);

// Internal
private:
	struct OsirisLibrary
	{
		OsirisLibrary();

		DynLib library;
		bool initialized;
		bool started;
		boost::mutex dataMutex;
		boost::mutex libraryMutex;
		os_sdk_init_callback initCallback;
		os_sdk_cleanup_callback cleanupCallback;
		os_sdk_start_callback startCallback;
		os_sdk_stop_callback stopCallback;
		os_sdk_get_option_callback getOptionCallback;
		os_sdk_set_option_callback setOptionCallback;
		os_sdk_log_handler_callback logHandlerCallback;
		os_sdk_parse_link_callback parseLinkCallback;
	};

	class OsirisContext : public IOsirisContext
	{
	// Construction
	public:
		OsirisContext(OsirisLibrary &library);
		virtual ~OsirisContext();

	// IOsirisContext interface
	public:
		virtual NS_IMETHODIMP parse(const nsAString &link, nsAString &retval);
		virtual NS_IMETHODIMP getHomepage(nsAString &nsString);
		virtual NS_IMETHODIMP getStarted(PRBool *aStarted);

	private:
		OsirisLibrary &m_library;
	};

// Construction
private:
	OsirisInterface();
	~OsirisInterface();

// Attributes
public:
	static OsirisInterface * instance();

// Operations
public:
	NS_IMETHODIMP initContext(boost::shared_ptr<IOsirisContext> &context, const nsAString &root, PRBool *retval);

	static void reportError(const std::string &error);
	static void reportMessage(const std::string &message);

private:
	void finalizeContext(IOsirisContext *context);

	bool loadLibrary(const nsAString &rootPath);
	void unloadLibrary();

	NS_IMETHODIMP init(const nsAString &root);
	NS_IMETHODIMP cleanup();
	NS_IMETHODIMP start();
	NS_IMETHODIMP stop();

	void threadCallback();

private:
	OsirisLibrary m_library;
	boost::weak_ptr<IOsirisContext> m_context;
	boost::scoped_ptr<boost::thread> m_thread;
};

//////////////////////////////////////////////////////////////////////

OsirisInterface::OsirisLibrary::OsirisLibrary() : initialized(false),
												  started(false),
												  initCallback(NULL),
												  cleanupCallback(NULL),
												  startCallback(NULL),
												  stopCallback(NULL),
												  getOptionCallback(NULL),
												  setOptionCallback(NULL),
												  logHandlerCallback(NULL),
												  parseLinkCallback(NULL)
{

}

//////////////////////////////////////////////////////////////////////

OsirisInterface::OsirisContext::OsirisContext(OsirisLibrary &library) : m_library(library)
{
	OsirisInterface::instance()->reportMessage("OsirisContext::OsirisContext()");
}

OsirisInterface::OsirisContext::~OsirisContext()
{
	OsirisInterface::instance()->reportMessage("OsirisContext::~OsirisContext()");
}

NS_IMETHODIMP OsirisInterface::OsirisContext::parse(const nsAString &link, nsAString &retval)
{
	OsirisInterface::instance()->reportMessage("OsirisInterface::OsirisContext::parse");

	boost::mutex::scoped_lock dataLock(m_library.dataMutex);

	if(m_library.started == false)
		return NS_ERROR_NOT_AVAILABLE;

	boost::mutex::scoped_lock libraryLock(m_library.libraryMutex);

	if(m_library.parseLinkCallback == NULL)
		return NS_ERROR_NOT_AVAILABLE;

	std::string utf8Link = to_utf8(link);

	OsirisInterface::instance()->reportMessage("in = \"" + utf8Link + "\"");

	std::vector<char> tmp;
	tmp.resize(1024);
	os_uint32 size = 0;

	for(;;)
	{
		size = static_cast<os_uint32>(tmp.size());
		os_result result = m_library.parseLinkCallback(utf8Link.c_str(), &tmp[0], &size);
		if(result == OS_SDK_OK)
			break;

		if(result != OS_SDK_ERROR_BUFFER_TOO_SMALL)
		{
			OsirisInterface::instance()->reportMessage("errorello");
			return convert_result(result);
		}

		tmp.resize(tmp.size() * 2);
	}

	std::string out(&tmp[0], size);

	OsirisInterface::instance()->reportMessage("out = \"" + out + "\"");

	retval = from_utf8(out);
	return NS_OK;
}

NS_IMETHODIMP OsirisInterface::OsirisContext::getHomepage(nsAString &homepage)
{
	boost::mutex::scoped_lock dataLock(m_library.dataMutex);

	if(m_library.started == false)
		return NS_ERROR_NOT_AVAILABLE;

	boost::mutex::scoped_lock libraryLock(m_library.libraryMutex);

	if(m_library.getOptionCallback == NULL)
		return NS_ERROR_NOT_AVAILABLE;

	os_uint32 port = 0;
	os_uint32 size = sizeof(os_uint32);
	os_result result = m_library.getOptionCallback("web.server.port", &port, &size, NULL);
	if(OS_SDK_FAILED(result))
		return convert_result(result);

	std::stringstream stream;
	stream << "http://127.0.0.1:" << port;
	homepage = from_ascii(stream.str());

	return NS_OK;
}

NS_IMETHODIMP OsirisInterface::OsirisContext::getStarted(PRBool *started)
{
	boost::mutex::scoped_lock lock(m_library.dataMutex);

	*started = m_library.started ? PR_TRUE : PR_FALSE;

	return NS_OK;
}

//////////////////////////////////////////////////////////////////////

OsirisInterface::OsirisInterface()
{

}

OsirisInterface::~OsirisInterface()
{
	unloadLibrary();
}

OsirisInterface * OsirisInterface::instance()
{
	static OsirisInterface i;
	return &i;
}

NS_IMETHODIMP OsirisInterface::initContext(boost::shared_ptr<IOsirisContext> &context, const nsAString &root, PRBool *retval)
{
	reportMessage("OsirisInterface::initContext");

	*retval = PR_FALSE;

	boost::mutex::scoped_lock lock(m_library.libraryMutex);

	if(loadLibrary(root) == false)
		return NS_ERROR_NOT_AVAILABLE;

	nsresult result = NS_OK;

	if(m_library.initialized == false)
	{
		result = init(root);
		if(NS_FAILED(result))
			return result;

		m_library.initialized = true;
	}

	*retval = PR_TRUE;

	context = m_context.lock();
	if(!context)
	{
		context.reset(new OsirisContext(m_library), boost::bind(&OsirisInterface::finalizeContext, this, _1));
		m_context = context;

		m_thread.reset(new boost::thread(boost::bind(&OsirisInterface::threadCallback, this)));
	}

	return result;
}

void OsirisInterface::reportError(const std::string &error)
{
	// TODO: log error to Firefox
#ifdef XP_WIN
	MessageBoxA(0, error.c_str(), "Osiris", MB_ICONERROR | MB_OK);
#else
	std::cerr << error << std::endl;
#endif
}

void OsirisInterface::reportMessage(const std::string &message)
{
	/*
	// TODO: log message to Firefox
#ifdef XP_WIN
	MessageBoxA(0, message.c_str(), "Osiris", MB_ICONINFORMATION | MB_OK);
#else
	std::cout << message << std::endl;
#endif
	*/
}

void OsirisInterface::finalizeContext(IOsirisContext *context)
{
	reportMessage("OsirisInterface::finalizeContext");

	boost::scoped_ptr<IOsirisContext> deleter(context);

	boost::mutex::scoped_lock lock(m_library.libraryMutex);

	if(m_thread)
	{
		m_thread->join();
		m_thread.reset();
	}

	stop();
	cleanup();
}

bool OsirisInterface::loadLibrary(const nsAString &rootPath)
{
	if(m_library.library.loaded() == false)
	{
		reportMessage("OsirisInterface::loadLibrary");

		nsString sdkPath;
		sdkPath.Append(rootPath);

#ifdef XP_WIN
		sdkPath.Append(from_ascii("\\osiris\\sdk"));
#else
		nsString workingDir;
		workingDir.Append(rootPath);
		workingDir.Append(from_ascii("/osiris"));
		CurrentDirectory cd(workingDir);

		sdkPath.Append(from_ascii("/osiris/libsdk"));
#endif
		if(m_library.library.load(sdkPath) == false)
		{
			reportError("Error loading sdk library");
			return false;
		}

		m_library.initCallback = reinterpret_cast<os_sdk_init_callback>(m_library.library.findProcedure("osiris_init"));
		m_library.cleanupCallback = reinterpret_cast<os_sdk_cleanup_callback>(m_library.library.findProcedure("osiris_cleanup"));
		m_library.startCallback = reinterpret_cast<os_sdk_start_callback>(m_library.library.findProcedure("osiris_start"));
		m_library.stopCallback = reinterpret_cast<os_sdk_stop_callback>(m_library.library.findProcedure("osiris_stop"));
		m_library.getOptionCallback = reinterpret_cast<os_sdk_get_option_callback>(m_library.library.findProcedure("osiris_options_get"));
		m_library.setOptionCallback = reinterpret_cast<os_sdk_set_option_callback>(m_library.library.findProcedure("osiris_options_set"));
		m_library.logHandlerCallback = reinterpret_cast<os_sdk_log_handler_callback>(m_library.library.findProcedure("osiris_log_handler"));
		m_library.parseLinkCallback = reinterpret_cast<os_sdk_parse_link_callback>(m_library.library.findProcedure("osiris_parse_link"));
	}

	return true;
}

void OsirisInterface::unloadLibrary()
{
	reportMessage("OsirisInterface::unloadLibrary");

	m_library.initCallback = NULL;
	m_library.cleanupCallback = NULL;
	m_library.startCallback = NULL;
	m_library.stopCallback = NULL;
	m_library.getOptionCallback = NULL;
	m_library.setOptionCallback = NULL;
	m_library.logHandlerCallback = NULL;
	m_library.parseLinkCallback = NULL;

	m_library.library.unload();
}

NS_IMETHODIMP OsirisInterface::init(const nsAString &root)
{
	reportMessage("OsirisInterface::init");

	if(m_library.initCallback == NULL)
		return NS_ERROR_NOT_AVAILABLE;

	std::string path = to_utf8(root);
#ifdef XP_WIN
	path.append("\\osiris");
#else
	path.append("/osiris");
#endif

	reportMessage("OsirisInterface::init with \"" + path + "\"");

	m_library.logHandlerCallback(&log_callback);
	
	return convert_result(m_library.initCallback(0, NULL, path.c_str()));
}

NS_IMETHODIMP OsirisInterface::cleanup()
{
	reportMessage("OsirisInterface::cleanup");

	if(m_library.cleanupCallback == NULL)
		return NS_ERROR_NOT_AVAILABLE;

	return convert_result(m_library.cleanupCallback());
}

NS_IMETHODIMP OsirisInterface::start()
{
	reportMessage("OsirisInterface::start");

	if(m_library.startCallback == NULL)
		return NS_ERROR_NOT_AVAILABLE;

	os_result result = m_library.startCallback();
	if(OS_SDK_SUCCEEDED(result))
	{
		boost::mutex::scoped_lock lock(m_library.dataMutex);
		m_library.started = true;
	}

	if(OS_SDK_SUCCEEDED(result))
		reportMessage("Osiris started");
	else
		reportError("Osiris start error");

	return convert_result(result);
}

NS_IMETHODIMP OsirisInterface::stop()
{
	reportMessage("OsirisInterface::stop");

	if(m_library.stopCallback == NULL)
		return NS_ERROR_NOT_AVAILABLE;

	os_result result = m_library.stopCallback();

	{
		boost::mutex::scoped_lock lock(m_library.dataMutex);
		m_library.started = false;
	}

	return convert_result(result);
}

void OsirisInterface::threadCallback()
{
	reportMessage("OsirisInterface::threadCallback");

	boost::mutex::scoped_lock lock(m_library.libraryMutex);
	start();
}

//////////////////////////////////////////////////////////////////////

IOsirisContext::IOsirisContext()
{

}

IOsirisContext::~IOsirisContext()
{

}

//////////////////////////////////////////////////////////////////////

OsirisImpl::OsirisImpl()
{

}

OsirisImpl::~OsirisImpl()
{

}

NS_IMETHODIMP OsirisImpl::Init(const nsAString &root, PRBool *retval)
{
	return OsirisInterface::instance()->initContext(m_context, root, retval);
}

NS_IMETHODIMP OsirisImpl::Parse(const nsAString &link, nsAString &retval)
{
	if(!m_context)
		return NS_ERROR_NOT_AVAILABLE;

	return m_context->parse(link, retval);
}

NS_IMETHODIMP OsirisImpl::GetStarted(PRBool *aStarted)
{
	if(!m_context)
		return NS_ERROR_NOT_AVAILABLE;

	return m_context->getStarted(aStarted);
}

NS_IMETHODIMP OsirisImpl::GetHomepage(nsAString &nsString)
{
	if(!m_context)
		return NS_ERROR_NOT_AVAILABLE;

	return m_context->getHomepage(nsString);
}

//////////////////////////////////////////////////////////////////////
