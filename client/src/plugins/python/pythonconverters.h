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

#ifndef _OS_PYTHON_PYTHONCONVERTERS_H
#define _OS_PYTHON_PYTHONCONVERTERS_H

#include "boost/type_traits/remove_const.hpp"
#include "boost/type_traits/remove_pointer.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
struct PythonConverter
{
	// Controlla se l'oggetto Python è convertibile in T: se convertibile restituisce obj, altrimenti null
	static void * convertible(PyObject *obj);
	// Implementazione della conversione da Python a T
	static void create(PyObject *obj, boost::python::converter::rvalue_from_python_stage1_data *data);
	// Effettua la conversione da T a Python
	static PyObject * convert(const T &obj);
};

//////////////////////////////////////////////////////////////////////

template <typename L>
class PythonLockPtrConverter
{
public:
	typedef typename boost::remove_const<typename L::value_type>::type value_type;

// Internal
private:	
	template <typename T>
	class lock_ptr_holder
	{
	public:
		lock_ptr_holder(shared_ptr<T> ptr) : m_ptr(ptr)
		{
			
		}

		lock_ptr_holder(const lock_ptr_holder &second) : m_ptr(second.m_ptr)
		{

		}
		
		~lock_ptr_holder()
		{

		}

		lock_ptr_holder<T> & operator =(const lock_ptr_holder &second)
		{
			m_ptr = second.m_ptr;
		}

		template <typename V>
		inline void operator()(V *p) const
		{
	
		}

	private:
		shared_ptr<T> m_ptr;
	};

// Operations
public:
	static PyObject * convert(const L &obj)
	{
		// N.B.: in teoria la Locked verrà rilasciata in seguito alla distruzione dell'oggetto python associato. Rimane da verificare la circostanza in cui l'esecuzione di uno script venga messa in pausa (dopo aver acquisito la Locked) per eseguire un nuovo contesto python
		// e che quest'ultimo tenti di acquisire la stessa Locked acquisita dal contesto precedente (in teoria va in deadlock...)

		shared_ptr<L> lock_ptr(OS_NEW_T(L)(obj), os_delete_t());
		shared_ptr<value_type> value_ptr(const_cast<value_type *>(obj.get()), lock_ptr_holder<L>(lock_ptr));
		return boost::python::incref(boost::python::object(value_ptr).ptr());
	}
};

//////////////////////////////////////////////////////////////////////

struct PythonConverters
{
public:
	static void init();
	
private:
	template <typename T>
	static void add_from()
	{
		boost::python::converter::registry::push_back(&PythonConverter<T>::convertible, &PythonConverter<T>::create, boost::python::type_id<T>());		
	}

	template <typename T>
	static void add_to()
	{
		boost::python::to_python_converter<T, PythonConverter<T> >();
	}

	template <typename L>
	static void add_locked_to()
	{
		boost::python::register_ptr_to_python<boost::shared_ptr<typename PythonLockPtrConverter<L>::value_type> >();	// Necessario per "lock_ptr_holder"
		boost::python::to_python_converter<L, PythonLockPtrConverter<L> >();
	}

	template <typename T>
	static void add()
	{
		add_from<T>();
		add_to<T>();		
	}
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYTHONCONVERTERS_H
