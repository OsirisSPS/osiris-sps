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
#include "pythonconverters.h"

#include "boost/date_time/posix_time/ptime.hpp"
#include "boost/none.hpp"
#include "ordered_map.h"
#include "python/datetime.h"
#include "portalsportaloptions.h"
#include "thirdpartylibrariesreporter.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <>
struct PythonConverter<null_ptr>
{
	static void * convertible(PyObject *obj)
	{
		if(obj == Py_None)
			return obj;

		return null;
	}

	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
	{
		void *memory_chunk = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<null_ptr> *>(data)->storage.bytes;
		new (memory_chunk) null_ptr();
		data->convertible = memory_chunk; 
	}

	static PyObject * convert(const null_ptr &obj)
	{
		return boost::python::detail::none();
	}
};

//////////////////////////////////////////////////////////////////////

template <>
struct PythonConverter<String>
{
	static void * convertible(PyObject *obj)
	{
		if(PyString_Check(obj) || PyUnicode_Check(obj))
			return obj;
			
		return null;
	}

	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
	{
		void *memory_chunk = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<String> *>(data)->storage.bytes;
		String *str = new (memory_chunk) String();
		data->convertible = memory_chunk;        	

		boost::python::object py_str(boost::python::handle<>(boost::python::borrowed(obj)));
		if(PyString_Check(obj))
			str->from_ascii(boost::python::extract<std::string>(py_str));
		else // unicode
			str->from_wide(boost::python::extract<std::wstring>(py_str));					
	}

	static PyObject * convert(const String &obj)
	{
		return boost::python::incref(boost::python::object(obj.to_wide()).ptr());
	}
};

//////////////////////////////////////////////////////////////////////

template <>
struct PythonConverter<boost::none_t>
{
	static void * convertible(PyObject *obj)
	{
		if(obj == Py_None)
			return obj;

		return null;
	}

	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
	{
		void *memory_chunk = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<boost::none_t> *>(data)->storage.bytes;
		new (memory_chunk) boost::none_t();
		data->convertible = memory_chunk; 
	}

	static PyObject * convert(const boost::none_t &obj)
	{
		return boost::python::detail::none();
	}
};

//////////////////////////////////////////////////////////////////////

template <>
struct PythonConverter<boost::posix_time::ptime>
{
	static void * convertible(PyObject *obj)
	{
		PyDateTime_IMPORT;

		//	TODO: supportare anche PyDate_Check e PyTime_Check?
		if(PyDateTime_Check(obj))
			return obj;

		return null;
	}

	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
	{
		void *memory_chunk = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<boost::posix_time::ptime> *>(data)->storage.bytes;
		new (memory_chunk) boost::posix_time::ptime(boost::gregorian::date(PyDateTime_GET_YEAR(obj), PyDateTime_GET_MONTH(obj), PyDateTime_GET_DAY(obj)), boost::posix_time::time_duration(PyDateTime_DATE_GET_HOUR(obj), PyDateTime_DATE_GET_MINUTE(obj), PyDateTime_DATE_GET_SECOND(obj), PyDateTime_DATE_GET_MICROSECOND(obj)));
		data->convertible = memory_chunk;        	
	}

	static PyObject * convert(const boost::posix_time::ptime &obj)
	{
		if(utils::ptime_is_valid(obj) == false)
			return boost::python::detail::none();

		PyDateTime_IMPORT;

		boost::posix_time::ptime::date_type date = obj.date();
		boost::posix_time::ptime::time_duration_type time = obj.time_of_day();

		return PyDateTime_FromDateAndTime(date.year(), date.month(), date.day(), time.hours(), time.minutes(), time.seconds(), static_cast<uint32>(time.fractional_seconds()));
	}
};

//////////////////////////////////////////////////////////////////////

template <typename T>
struct PythonContainerConverter
{
	typedef typename T::value_type value_type;

	static void * convertible(PyObject *obj)
	{
		if(PyList_Check(obj))
			return obj;

		return null;
	}
	
	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
	{
		void *memory_chunk = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<T> *>(data)->storage.bytes;		
		T *container = new (memory_chunk) T();
		data->convertible = memory_chunk;

		boost::python::handle<> obj_handle(boost::python::borrowed(obj));
		boost::python::object obj_ref(obj_handle);
		boost::python::extract<boost::python::list> obj_proxy(obj_ref);
		boost::python::list list = obj_proxy();
		
		size_t len_values = boost::python::len(list);
		for(size_t i = 0; i < len_values; ++i) 
		{
			boost::python::object value_obj = list[i];
			boost::python::extract<value_type> value_proxy(value_obj);
			if(value_proxy.check() == false) 
			{
				PyErr_SetString(PyExc_ValueError, "Unsuitable type");
				boost::python::throw_error_already_set();
			}			

			std::inserter(*container, container->end()) = value_proxy();			
		}
	}
};

//////////////////////////////////////////////////////////////////////

#define OS_DECLARE_PYTHONCONTAINERCONVERTER(T)		template <> struct PythonConverter<T > : public PythonContainerConverter<T > {};

OS_DECLARE_PYTHONCONTAINERCONVERTER(deque<std::string>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(deque<std::wstring>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(deque<String>::type)

OS_DECLARE_PYTHONCONTAINERCONVERTER(list<std::string>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(list<std::wstring>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(list<String>::type)

OS_DECLARE_PYTHONCONTAINERCONVERTER(set<std::string>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(set<std::wstring>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(set<String>::type)

OS_DECLARE_PYTHONCONTAINERCONVERTER(unordered_set<std::string>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(unordered_set<std::wstring>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(unordered_set<String>::type)

OS_DECLARE_PYTHONCONTAINERCONVERTER(vector<std::string>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(vector<std::wstring>::type)
OS_DECLARE_PYTHONCONTAINERCONVERTER(vector<String>::type)

//////////////////////////////////////////////////////////////////////

template <typename T>
struct PythonMapConverter
{
	typedef typename T::key_type key_type;
	typedef typename T::mapped_type mapped_type;

	static void * convertible(PyObject *obj)
	{
		if(PyDict_Check(obj))
			return obj;

		return null;
	}

	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data)
	{
		void *memory_chunk = reinterpret_cast<boost::python::converter::rvalue_from_python_storage<T> *>(data)->storage.bytes;		
		T *map = new (memory_chunk) T();
		data->convertible = memory_chunk;

		boost::python::handle<> obj_handle(boost::python::borrowed(obj));
		boost::python::object obj_ref(obj_handle);
		boost::python::extract<boost::python::dict> obj_proxy(obj_ref);
		boost::python::dict dictionary = obj_proxy();
		
		boost::python::list keys = dictionary.keys();
		size_t len_keys = boost::python::len(keys);
		for(size_t i = 0; i < len_keys; ++i) 
		{
			boost::python::object key_obj = keys[i];
			boost::python::extract<key_type> key_proxy(key_obj);
			if(key_proxy.check() == false) 
			{
				PyErr_SetString(PyExc_KeyError, "Unsuitable type");
				boost::python::throw_error_already_set();
			}

			boost::python::object mapped_obj = dictionary[key_obj];
			boost::python::extract<mapped_type> mapped_proxy(mapped_obj);
			if(mapped_proxy.check() == false) 
			{
				PyErr_SetString(PyExc_ValueError, "Unsuitable type");
				boost::python::throw_error_already_set();
			}

			(*map)[key_proxy()] = mapped_proxy();
		}
	}
};

//////////////////////////////////////////////////////////////////////

#define OS_DECLARE_PYTHONMAPCONVERTER(T)		template <> struct PythonConverter<T > : public PythonMapConverter<T > {};	// Lo spazio serve nel caso in cui il valore sia template per evitare la doppia >>
#define OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(T)	T, T

OS_DECLARE_PYTHONMAPCONVERTER(map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(std::string)>::type)
OS_DECLARE_PYTHONMAPCONVERTER(map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(std::wstring)>::type)
OS_DECLARE_PYTHONMAPCONVERTER(map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(String)>::type)

OS_DECLARE_PYTHONMAPCONVERTER(ordered_map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(std::string)>)
OS_DECLARE_PYTHONMAPCONVERTER(ordered_map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(std::wstring)>)
OS_DECLARE_PYTHONMAPCONVERTER(ordered_map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(String)>)

OS_DECLARE_PYTHONMAPCONVERTER(unordered_map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(std::string)>::type)
OS_DECLARE_PYTHONMAPCONVERTER(unordered_map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(std::wstring)>::type)
OS_DECLARE_PYTHONMAPCONVERTER(unordered_map<OS_DECLARE_PYTHONMAPCONVERTER_PARAMS(String)>::type)

//////////////////////////////////////////////////////////////////////

void PythonConverters::init()
{
	add<boost::none_t>();
	add<boost::posix_time::ptime>();
	add<null_ptr>();
	add<String>();
	
	// N.B.: la conversione da C++ a Python è già esposta tramite indexing suite

	add_from<deque<std::string>::type>();	
	add_from<deque<std::wstring>::type>();	
	add_from<deque<String>::type>();	

	add_from<list<std::string>::type>();	
	add_from<list<std::wstring>::type>();	
	add_from<list<String>::type>();	

	add_from<set<std::string>::type>();	
	add_from<set<std::wstring>::type>();	
	add_from<set<String>::type>();	

	add_from<unordered_set<std::string>::type>();	
	add_from<unordered_set<std::wstring>::type>();	
	add_from<unordered_set<String>::type>();	

	add_from<vector<std::string>::type>();	
	add_from<vector<std::wstring>::type>();	
	add_from<vector<String>::type>();	

	add_from<map<std::string, std::string>::type>();	
	add_from<map<std::wstring, std::wstring>::type>();	
	add_from<map<String, String>::type>();	

	add_from<ordered_map<std::string, std::string> >();	
	add_from<ordered_map<std::wstring, std::wstring> >();	
	add_from<ordered_map<String, String> >();		

	add_from<unordered_map<std::string, std::string>::type>();	
	add_from<unordered_map<std::wstring, std::wstring>::type>();	
	add_from<unordered_map<String, String>::type>();
	
	// wrappers Locked (il parametro template della Locked va comunque registrato a parte manualmente o tramite script di generazione)

	add_locked_to<Locked<const PortalOptions::IsisEndpoints>::unique>();
	add_locked_to<Locked<const ThirdPartyLibrariesReporter::Libraries>::unique>();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
