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

#ifndef _OS_CORE_SINGLETON_H
#define _OS_CORE_SINGLETON_H

#include "boost/thread/once.hpp"
#include "boost/noncopyable.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_IMPLEMENT_SINGLETON(s)		template<> s * OS_NAMESPACE_NAME::DynamicSingleton<s>::s_instance = null;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class DynamicSingleton : public boost::noncopyable
{
// Construction
public:
    DynamicSingleton();
    virtual ~DynamicSingleton();

// Attributes
public:
	static T * instance();
	static bool exists();

// Operations
public:
	static T * create();
	template <typename I>
	static T * create();
	static void destroy();

protected:
    static T *s_instance;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
T * DynamicSingleton<T>::instance()
{
	OS_ASSERT(s_instance != null);
	return s_instance;
}

template <typename T>
bool DynamicSingleton<T>::exists()
{
    return s_instance != null;
}

//////////////////////////////////////////////////////////////////////

template <typename T>
DynamicSingleton<T>::DynamicSingleton()
{
	OS_ASSERT(s_instance == null);
	s_instance = static_cast<T *>(this);
}

template <typename T>
DynamicSingleton<T>::~DynamicSingleton()
{
	OS_ASSERT(s_instance != null);
	s_instance = null;
}

template <typename T>
T * DynamicSingleton<T>::create()
{
	return new T();
}

template <typename T>
template <typename I>
T * DynamicSingleton<T>::create()
{
	return static_cast<T *>(new I());
}

template <typename T>
void DynamicSingleton<T>::destroy()
{
	delete s_instance;
}

//////////////////////////////////////////////////////////////////////

template <typename T, bool lazy = true>	// Se lazy == true il singleton viene istanziato in fase di runtime alla prima "instance()", altrimenti viene precreato come una variabile statica globale
class StaticSingleton : public boost::noncopyable
{
// Internal
private:
	template <typename S, bool r>
	class SingletonCreator
	{
	// Operations
	public:
		inline void join();
	};

	template <typename S>
	class SingletonCreator<S, false>
	{
	// Construction
	public:
		SingletonCreator();

	// Operations
	public:
		inline void join();
	};

	typedef SingletonCreator<T, lazy> Creator;

	class Impl : public T { friend class StaticSingleton<T, lazy>; };

// Construction
protected:
	StaticSingleton();
	virtual ~StaticSingleton();

// Attributes
public:
	static T * instance();

private:
	inline static scoped_ptr<T> & ptr();

// Operations
private:
	static void initInstance();

private:
	static Creator m_creator;
	static boost::once_flag m_initializer;
};

//////////////////////////////////////////////////////////////////////

template <typename T, bool lazy>
typename StaticSingleton<T, lazy>::Creator StaticSingleton<T, lazy>::m_creator;

template <typename T, bool lazy>
boost::once_flag StaticSingleton<T, lazy>::m_initializer = BOOST_ONCE_INIT;

//////////////////////////////////////////////////////////////////////

template <typename T, bool lazy>
template <typename S, bool r>
inline void StaticSingleton<T, lazy>::SingletonCreator<S, r>::join()
{
	// Funzione di appoggio, vedere le note in "StaticSingleton()"
}

//////////////////////////////////////////////////////////////////////

template <typename T, bool lazy>
template <typename S>
StaticSingleton<T, lazy>::SingletonCreator<S, false>::SingletonCreator()
{
	// Assicura la creazione del singleton con lo stesso scope di "m_creator"
	S::instance();
}

template <typename T, bool lazy>
template <typename S>
inline void StaticSingleton<T, lazy>::SingletonCreator<S, false>::join()
{
	// Funzione di appoggio, vedere le note in "StaticSingleton()"
}

//////////////////////////////////////////////////////////////////////

template <typename T, bool lazy>
StaticSingleton<T, lazy>::StaticSingleton()
{
	// Assicura che in versione NON runtime il singleton venga preistanziato
	m_creator.join();
}

template <typename T, bool lazy>
StaticSingleton<T, lazy>::~StaticSingleton()
{

}

template <typename T, bool lazy>
T * StaticSingleton<T, lazy>::instance()
{
	// Versione non thread-safe

//	static Impl i;
//	return &i;

	// Versione thread-safe

	// N.B.: non tenere "instance" a livello globale in quanto non POD (m_initializer è POD) e pertanto potrebbe causare problemi di inizializzazione (instance.ptr verrebbe reinizializzato a 0 dopo la initInstance)
	boost::call_once(m_initializer, &initInstance);
	OS_ASSERT(ptr().get() != null);
	return ptr().get();
}

template <typename T, bool lazy>
inline scoped_ptr<T> & StaticSingleton<T, lazy>::ptr()
{
	// N.B.: non creare lo scoped_ptr statico passandolo alla initInstance per evitare problemi di "ottimizzazione" di compilazione
	static scoped_ptr<T> instance;
	return instance;
}

template <typename T, bool lazy>
void StaticSingleton<T, lazy>::initInstance()
{
	OS_ASSERT(ptr().get() == null);
	ptr().reset(new Impl());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_SINGLETON_H
