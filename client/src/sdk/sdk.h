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

#ifndef _OS_SDK_SDK_H
#define _OS_SDK_SDK_H

//////////////////////////////////////////////////////////////////////

#if defined _WINDOWS || defined __WIN32__ || defined _WIN32 || defined WIN32
	#define OS_SDK_LIBRARY_EXPORT	_declspec(dllexport)
	#define OS_SDK_LIBRARY_IMPORT	_declspec(dllimport)

	typedef __int8					os_int8;
	typedef __int16					os_int16;
	typedef __int32					os_int32;
	typedef __int64					os_int64;
	typedef unsigned __int8			os_uint8;
	typedef unsigned __int16		os_uint16;
	typedef unsigned __int32		os_uint32;
	typedef unsigned __int64		os_uint64;
#else
	#include <stdint.h>

	#define OS_SDK_LIBRARY_EXPORT
	#define OS_SDK_LIBRARY_IMPORT

	typedef int8_t		 			os_int8;
	typedef int16_t		 			os_int16;
	typedef int32_t 				os_int32;
	typedef int64_t			 		os_int64;
	typedef uint8_t		 			os_uint8;
	typedef uint16_t	 			os_uint16;
	typedef uint32_t	 			os_uint32;
	typedef uint64_t		 		os_uint64;
#endif

//////////////////////////////////////////////////////////////////////

#ifndef OS_SDK_API
	#ifdef OS_SDK_LIB
		#define OS_SDK_API			OS_SDK_LIBRARY_EXPORT
	#else
		#define OS_SDK_API			OS_SDK_LIBRARY_IMPORT
	#endif
#endif

//////////////////////////////////////////////////////////////////////

typedef os_int32 os_result;
typedef os_uint64 os_time;

//////////////////////////////////////////////////////////////////////

#define OS_SDK_OK							0		// No error
#define OS_SDK_ERROR_GENERIC				1		// Generic error
#define OS_SDK_ERROR_NOT_INITIALIZED		2		// Something must be initialized first
#define OS_SDK_ERROR_ALREADY_INITIALIZED    3		// Something is already initialized
#define OS_SDK_ERROR_NOT_FOUND			    4		// The requested item was not found
#define OS_SDK_ERROR_INVALID_PARAM			5		// One of the specified params is invalid
#define OS_SDK_ERROR_ILLEGAL_VALUE			6		// The specified value is not allowed
#define OS_SDK_ERROR_BUFFER_TOO_SMALL		7		// The specified buffer isn't big enough

//////////////////////////////////////////////////////////////////////

#define OS_SDK_FAILED(result)				(result > 0)
#define OS_SDK_SUCCEEDED(result)			(result == 0)

//////////////////////////////////////////////////////////////////////

#define OS_SDK_OPTION_TYPE_UNKNOWN			0		// Unknown value
#define OS_SDK_OPTION_TYPE_INT				1		// Signed integer value, variable size
#define OS_SDK_OPTION_TYPE_UINT				2		// Unsigned integer value, variable size
#define OS_SDK_OPTION_TYPE_DOUBLE			3		// Real value, fixed size (double)
#define OS_SDK_OPTION_TYPE_STRING			4		// String value (utf-8 encoded), variable size
#define OS_SDK_OPTION_TYPE_BUFFER			5		// Raw value, variable size

//////////////////////////////////////////////////////////////////////

// Return the SDK version as string format
extern "C" OS_SDK_API const char * osiris_version_name();

//////////////////////////////////////////////////////////////////////

// Return the SDK version as number (low part is minor version, hi part is major version)
extern "C" OS_SDK_API os_uint16 osiris_version_number();

//////////////////////////////////////////////////////////////////////

// Initialize the Osiris environment (must be called only once at startup)
// argc [in]: command line arguments (can be 0)
// argv [in, optional]: command line values
// root [in, optional]: if argc is 0, root is required and must be a null terminated string of the path (utf-8 encoded) where Osiris looks for "system.xml"
extern "C" OS_SDK_API os_result osiris_init(int argc, char *argv[], const char *root);

//////////////////////////////////////////////////////////////////////

// Terminate the Osiris environment (must be called only once at exit)
extern "C" OS_SDK_API os_result osiris_cleanup();

//////////////////////////////////////////////////////////////////////

// Start the Osiris engine
extern "C" OS_SDK_API os_result osiris_start();

//////////////////////////////////////////////////////////////////////

// Stop the Osiris engine
extern "C" OS_SDK_API os_result osiris_stop();

//////////////////////////////////////////////////////////////////////

// Get the value of an option
// name [in, required]: name of the option (ascii encoded)
// value [out, optional]: pointer to the buffer in which the value for the requested option is returned
// size [in/out, required]: pointer to the size, in bytes, of the "value" buffer (if "value" is NULL, it must be 0). On return, it will be set to the size returned if "value" is not NULL or the required size if "value" is NULL
// type [out, optional]: pointer to the type of the option (OS_SDK_OPTION_TYPE_*)
extern "C" OS_SDK_API os_result osiris_options_get(const char *name, void *value, os_uint32 *size, os_uint32 *type);

//////////////////////////////////////////////////////////////////////

// Set the value of an option
// name [in, required]: name of the option (ascii encoded)
// value [in, required]: pointer to the buffer in which the value for the requested option is specified
// size [in]: size, in bytes, of the buffer pointed to by "value"
extern "C" OS_SDK_API os_result osiris_options_set(const char *name, const void *value, os_uint32 size);

//////////////////////////////////////////////////////////////////////

// Apply the current options
extern "C" OS_SDK_API os_result osiris_options_update();

//////////////////////////////////////////////////////////////////////

#define OS_SDK_LOG_LEVEL_DEBUG				0
#define OS_SDK_LOG_LEVEL_NOTICE				1
#define OS_SDK_LOG_LEVEL_INFO				2
#define OS_SDK_LOG_LEVEL_WARNING			3
#define OS_SDK_LOG_LEVEL_ERROR				4
#define OS_SDK_LOG_LEVEL_CRITICAL			5

//////////////////////////////////////////////////////////////////////

// Log a message
// message [in, required]: null terminated string of the message (utf-8 encoded)
// level: the severity of the message (OS_SDK_LOG_LEVEL_*)
extern "C" OS_SDK_API os_result osiris_log(const char *message, os_uint8 level);

//////////////////////////////////////////////////////////////////////

// message: null terminated string of the log message (utf-8 encoded)
// level: the severity of the message (OS_SDK_LOG_LEVEL_*)
// timestamp: the timestamp of the message (localtime)
typedef void (* os_log_handler)(const char * /*message*/, os_uint8 /*level*/, os_time /*timestamp*/);

// Define a callback for the logs
extern "C" OS_SDK_API os_result osiris_log_handler(os_log_handler handler);

//////////////////////////////////////////////////////////////////////

// Process an osiris link and returns the parsed http url
// link [in, required]: null terminated string of the link (utf-8 encoded) in the form of "osiris://"
// value [out, optional]: pointer to the buffer in which the value for the parsed link is returned
// size [in/out, required]: pointer to the size, in bytes, of the "value" buffer (if "value" is NULL, it must be 0). On return, it will be set to the size returned if "value" is not NULL or the required size if "value" is NULL
extern "C" OS_SDK_API os_result osiris_parse_link(const char *link, char *value, os_uint32 *size);

//////////////////////////////////////////////////////////////////////

#endif // _OS_SDK_SDK_H
