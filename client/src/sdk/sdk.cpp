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
#include "sdk.h"

#include "boost/bind.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "buffer.h"
#include "datetime.h"
#include "engine.h"
#include "iapplication.h"
#include "logger.h"
#include "logmessage.h"
#include "options.h"
#include "option.h"
#include "osirislink.h"

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

OS_STATIC_ASSERT(sizeof(os_int8) == sizeof(int8));
OS_STATIC_ASSERT(sizeof(os_int16) == sizeof(int16));
OS_STATIC_ASSERT(sizeof(os_int32) == sizeof(int32));
OS_STATIC_ASSERT(sizeof(os_int64) == sizeof(int64));
OS_STATIC_ASSERT(sizeof(os_uint8) == sizeof(uint8));
OS_STATIC_ASSERT(sizeof(os_uint16) == sizeof(uint16));
OS_STATIC_ASSERT(sizeof(os_uint32) == sizeof(uint32));
OS_STATIC_ASSERT(sizeof(os_uint64) == sizeof(uint64));

OS_STATIC_ASSERT(sizeof(os_time) >= sizeof(time_t));

OS_STATIC_ASSERT(OS_SDK_LOG_LEVEL_DEBUG == logLevelDebug);
OS_STATIC_ASSERT(OS_SDK_LOG_LEVEL_NOTICE == logLevelNotice);
OS_STATIC_ASSERT(OS_SDK_LOG_LEVEL_INFO == logLevelInfo);
OS_STATIC_ASSERT(OS_SDK_LOG_LEVEL_WARNING == logLevelWarning);
OS_STATIC_ASSERT(OS_SDK_LOG_LEVEL_ERROR == logLevelError);
OS_STATIC_ASSERT(OS_SDK_LOG_LEVEL_CRITICAL == logLevelCritical);

//////////////////////////////////////////////////////////////////////

class Application : public IApplication,
					public DynamicSingleton<Application>
{	
	typedef IApplication ApplicationBase;

// Construction
public:
	Application();
	virtual ~Application();

// Operations
public:
	static LogLevel convertLogLevel(uint8 level);

private:
	void log(shared_ptr<LogMessage> message);

// IApplication interface
protected:
	virtual void handleProcessCommands(const std::string &commands);

public:
	static os_log_handler g_logHandler;

private:
	shared_ptr<Logger> m_logger;
};

//////////////////////////////////////////////////////////////////////

os_log_handler Application::g_logHandler = nullptr;

//////////////////////////////////////////////////////////////////////

Application::Application() : ApplicationBase(PlatformManager::applicationConsole)
{
	m_logger = Logger::create(boost::bind(&Application::log, this, _1));
}

Application::~Application()
{
	m_logger.reset();
}

LogLevel Application::convertLogLevel(uint8 level)
{
	switch(level)
	{
	case OS_SDK_LOG_LEVEL_DEBUG:		return logLevelDebug;
	case OS_SDK_LOG_LEVEL_NOTICE:		return logLevelNotice;
	case OS_SDK_LOG_LEVEL_INFO:			return logLevelInfo;
	case OS_SDK_LOG_LEVEL_WARNING:		return logLevelWarning;
	case OS_SDK_LOG_LEVEL_ERROR:		return logLevelError;
	case OS_SDK_LOG_LEVEL_CRITICAL:		return logLevelCritical;

	default:							OS_ASSERTFALSE();
										break;
	}

	return logLevelDefault;
}

void Application::log(shared_ptr<LogMessage> message)
{
	if(g_logHandler != nullptr)
	{
		std::string str = message->getMessage().to_utf8();
		os_uint8 level = static_cast<os_uint8>(message->getLevel());
		os_time timestamp = static_cast<os_time>(DateTime(message->getTimestamp()).toTime_t());

		g_logHandler(str.c_str(), level, timestamp);
	}
}

void Application::handleProcessCommands(const std::string &commands)
{
	OS_LOG_DEBUG("Command(s) not supported: " + commands);
}

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(Application)

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API const char * osiris_version_name()
{
	static std::string version = Engine::getVersionName(false);
	return version.c_str();
}

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API os_uint16 osiris_version_number()
{
	return OS_MAKE_UINT16(OS_OSIRIS_VERSION_MINOR, OS_OSIRIS_VERSION_MAJOR);
}

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API os_result osiris_init(int argc, char *argv[], const char *root)
{
	if(Application::exists())
		return OS_SDK_ERROR_ALREADY_INITIALIZED;

	OS_INIT_LEAKS_DETECTION();

	bool done = false;

	if(argc == 0)
	{
		if(root == nullptr)
			return OS_SDK_ERROR_INVALID_PARAM;

		String path;
		path.from_utf8(root);
		done = Application::create()->init(path);
	}
	else
	{
		done = Application::create()->init(argc, argv);
	}	
	
	return done ? OS_SDK_OK : OS_SDK_ERROR_GENERIC;
}

extern "C" OS_SDK_API os_result osiris_cleanup()
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	Application::instance()->cleanup();
	Application::destroy();

	return OS_SDK_OK;
}

extern "C" OS_SDK_API os_result osiris_start()
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	return Application::instance()->start() ? OS_SDK_OK : OS_SDK_ERROR_GENERIC;
}

extern "C" OS_SDK_API os_result osiris_stop()
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	Application::instance()->stop();
	
	return OS_SDK_OK;
}

extern "C" OS_SDK_API os_result osiris_options_get(const char *name, void *value, os_uint32 *size, os_uint32 *type)
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	if((name == NULL) || (size == NULL))
		return OS_SDK_ERROR_INVALID_PARAM;

	Locked<const XMLOptions::Options, boost::shared_mutex>::shared options = Options::instance()->getOptions();
	XMLOptions::Options::const_iterator i = options->find(name);
	if(i == options->end())
		return OS_SDK_ERROR_NOT_FOUND;

	const DataItem &option = i->second->getValue();

	Buffer out_value;
	os_uint32 out_type = OS_SDK_OPTION_TYPE_UNKNOWN;

	switch(option.getType())
	{
	case DataItem::vt_bool:		{			
									out_type = OS_SDK_OPTION_TYPE_UINT;
									byte tmp = option.get<bool>() ? 1 : 0;
									if(out_value.assign(&tmp, sizeof(byte)) != sizeof(byte))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_byte:		{
									out_type = OS_SDK_OPTION_TYPE_UINT;
									byte tmp = option.get<byte>();
									if(out_value.assign(&tmp, sizeof(byte)) != sizeof(byte))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_int32:	{
									out_type = OS_SDK_OPTION_TYPE_INT;
									int32 tmp = option.get<int32>();
									if(out_value.assign(&tmp, sizeof(int32)) != sizeof(int32))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_uint32:	{
									out_type = OS_SDK_OPTION_TYPE_UINT;
									uint32 tmp = option.get<uint32>();
									if(out_value.assign(&tmp, sizeof(uint32)) != sizeof(uint32))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_int64:	{
									out_type = OS_SDK_OPTION_TYPE_INT;
									int64 tmp = option.get<int64>();
									if(out_value.assign(&tmp, sizeof(int64)) != sizeof(int64))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_uint64:	{
									out_type = OS_SDK_OPTION_TYPE_UINT;
									uint64 tmp = option.get<uint64>();
									if(out_value.assign(&tmp, sizeof(uint64)) != sizeof(uint64))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_double:	{
									out_type = OS_SDK_OPTION_TYPE_DOUBLE;
									double tmp = option.get<double>();
									if(out_value.assign(&tmp, sizeof(double)) != sizeof(double))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_string:	{
									out_type = OS_SDK_OPTION_TYPE_STRING;
									std::string tmp = option.get<String>().to_utf8();
									if(out_value.assign(tmp.data(), static_cast<uint32>(tmp.size())) != static_cast<uint32>(tmp.size()))
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	case DataItem::vt_buffer:	{
									out_type = OS_SDK_OPTION_TYPE_BUFFER;
									Buffer tmp = option.get<Buffer>();
									if(out_value.assign(tmp.getData(), tmp.getSize()) != tmp.getSize())
										return OS_SDK_ERROR_GENERIC;
								}

								break;

	default:					out_type = OS_SDK_OPTION_TYPE_UNKNOWN;
								break;
	}

	os_result result = OS_SDK_OK;

	os_uint32 out_size = static_cast<os_uint32>(out_value.getSize());

	if(value != NULL)
	{
		if(*size < out_size)
		{
			result = OS_SDK_ERROR_BUFFER_TOO_SMALL;
		}
		else
		{
			OS_ASSERT(out_value.getPosition() == out_value.getData());
			if(out_value.peek(value, out_size) != out_size)
				return OS_SDK_ERROR_GENERIC;
		}
	}

	*size = out_size;

	if(type != NULL)
		*type = out_type;

	return result;
}

extern "C" OS_SDK_API os_result osiris_options_set(const char *name, const void *value, os_uint32 size)
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	if(name == NULL)
		return OS_SDK_ERROR_INVALID_PARAM;

	Locked<const XMLOptions::Options, boost::shared_mutex>::shared options = Options::instance()->getOptions();
	XMLOptions::Options::const_iterator i = options->find(name);
	if(i == options->end())
		return OS_SDK_ERROR_NOT_FOUND;

	Buffer value_buffer;
	if(value_buffer.assign(value, static_cast<uint32>(size)) != static_cast<uint32>(size))
		return OS_SDK_ERROR_GENERIC;

	DataItem in_value;	

	switch(i->second->getType())
	{
	case DataItem::vt_bool:		{			
									byte tmp = 0;
									
									if(value_buffer.getSize() != sizeof(byte))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(byte)) != sizeof(byte))
										return OS_SDK_ERROR_GENERIC;

									in_value = static_cast<bool>(tmp == 1);
								}

								break;

	case DataItem::vt_byte:		{
									byte tmp = 0;

									if(value_buffer.getSize() != sizeof(byte))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(byte)) != sizeof(byte))
										return OS_SDK_ERROR_GENERIC;

									in_value = tmp;
								}

								break;

	case DataItem::vt_int32:	{
									int32 tmp = 0;

									if(value_buffer.getSize() != sizeof(int32))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(int32)) != sizeof(int32))
										return OS_SDK_ERROR_GENERIC;

									in_value = tmp;
								}

								break;

	case DataItem::vt_uint32:	{
									uint32 tmp = 0;

									if(value_buffer.getSize() != sizeof(uint32))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(uint32)) != sizeof(uint32))
										return OS_SDK_ERROR_GENERIC;

									in_value = tmp;
								}

								break;

	case DataItem::vt_int64:	{
									int64 tmp = 0;

									if(value_buffer.getSize() != sizeof(int64))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(int64)) != sizeof(int64))
										return OS_SDK_ERROR_GENERIC;

									in_value = tmp;
								}

								break;

	case DataItem::vt_uint64:	{
									uint64 tmp = 0;

									if(value_buffer.getSize() != sizeof(uint64))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(uint64)) != sizeof(uint64))
										return OS_SDK_ERROR_GENERIC;

									in_value = tmp;
								}

								break;

	case DataItem::vt_double:	{
									double tmp = 0;

									if(value_buffer.getSize() != sizeof(double))
										return OS_SDK_ERROR_INVALID_PARAM;

									if(value_buffer.peek(&tmp, sizeof(double)) != sizeof(double))
										return OS_SDK_ERROR_GENERIC;

									in_value = tmp;
								}

								break;

	case DataItem::vt_string:	{
									String tmp;
									if(value_buffer.getSize() > 0)
										tmp.from_utf8(std::string(reinterpret_cast<std::string::const_pointer>(value_buffer.getData()), static_cast<std::string::size_type>(value_buffer.getSize())));

									in_value = tmp;
								}

								break;

	case DataItem::vt_buffer:	in_value = value_buffer; 
								break;

	default:					return OS_SDK_ERROR_GENERIC;
	}

	return i->second->setValue(in_value) ? OS_SDK_OK : OS_SDK_ERROR_ILLEGAL_VALUE;
}

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API os_result osiris_options_update()
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	Options::instance()->update();

	return OS_SDK_OK;
}

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API os_result osiris_log(const char *message, os_uint8 level)
{
	if(Application::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	if(message == NULL)
		return OS_SDK_ERROR_INVALID_PARAM;

	String str;
	str.from_utf8(message);
	OS_NAMESPACE_NAME::log(str, Application::convertLogLevel(level));

	return OS_SDK_OK;
}

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API os_result osiris_log_handler(os_log_handler handler)
{
	Application::g_logHandler = handler;

	return OS_SDK_OK;
}

//////////////////////////////////////////////////////////////////////

extern "C" OS_SDK_API os_result osiris_parse_link(const char *link, char *value, os_uint32 *size)
{
	if(Application::exists() == false || Engine::exists() == false)
		return OS_SDK_ERROR_NOT_INITIALIZED;

	if((link == NULL) || (size == NULL))
		return OS_SDK_ERROR_INVALID_PARAM;

	String tmp;	

	OsirisLink shellLink;
	if(shellLink.parse(tmp.from_utf8(link).to_ascii()) == false)
		return OS_SDK_ERROR_ILLEGAL_VALUE;

	tmp.clear();
	if(Engine::instance()->processLink(shellLink, tmp) == false)
		return OS_SDK_ERROR_GENERIC;

	std::string out_value = tmp.to_utf8();

	os_result result = OS_SDK_OK;

	os_uint32 out_size = static_cast<os_uint32>(out_value.size());

	if(value != NULL)
	{
		if(*size < out_size)
			result = OS_SDK_ERROR_BUFFER_TOO_SMALL;
		else
			memcpy(value, out_value.data(), out_size);			
	}

	*size = out_size;

	return result;
}

//////////////////////////////////////////////////////////////////////
