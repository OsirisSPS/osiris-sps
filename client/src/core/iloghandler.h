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

#ifndef _OS_CORE_ILOGHANDLER_H
#define _OS_CORE_ILOGHANDLER_H

#include "boost/bind.hpp"
#include "logger.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T>
class ILogHandler
{
	class Handler : public T
	{
		friend class ILogHandler;	// Consente di definire costruttore/distruttore di T protetti

		typedef T BaseClass;

	// Construction
	public:
		Handler();
		virtual ~Handler();

		// Se spostati all'esterno msvc da un "internal compiler error"

		template <typename P1>
		Handler(const P1 &p1) : BaseClass(p1) { }
		template <typename P1, typename P2>
		Handler(const P1 &p1, const P2 &p2) : BaseClass(p1, p2) { }
		template <typename P1, typename P2, typename P3>
		Handler(const P1 &p1, const P2 &p2, const P3 &p3) : BaseClass(p1, p2, p3) { }
		template <typename P1, typename P2, typename P3, typename P4>
		Handler(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4) : BaseClass(p1, p2, p3, p4) { }
		template <typename P1, typename P2, typename P3, typename P4, typename P5>
		Handler(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5) : BaseClass(p1, p2, p3, p4, p5) { }		
		
	// Operations
	private:
		void createLogger();		
	};

// Construction
protected:
	ILogHandler();

public:
	virtual ~ILogHandler();

// Attributes
public:
	inline shared_ptr<Logger> getLogger() const;

// Operations
public:
	static shared_ptr<T> create();
	template <typename P1>
	static shared_ptr<T> create(const P1 &p1);
	template <typename P1, typename P2>
	static shared_ptr<T> create(const P1 &p1, const P2 &p2);
	template <typename P1, typename P2, typename P3>
	static shared_ptr<T> create(const P1 &p1, const P2 &p2, const P3 &p3);
	template <typename P1, typename P2, typename P3, typename P4>
	static shared_ptr<T> create(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4);
	template <typename P1, typename P2, typename P3, typename P4, typename P5>
	static shared_ptr<T> create(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5);

private:
	void initLogger(shared_ptr<Logger> logger);	
	void resetLogger();
	
private:
	static void destroy(ILogHandler *instance);

protected:
	virtual void log(shared_ptr<LogMessage> message) = 0;

private:
	shared_ptr<Logger> m_logger;	// L'accesso privato è per garantire che l'istanza venga creata solo all'interno di ILogHandler
};

//////////////////////////////////////////////////////////////////////

template <typename T>
ILogHandler<T>::Handler::Handler()
{

}

template <typename T>
ILogHandler<T>::Handler::~Handler()
{

}

template <typename T>
void ILogHandler<T>::Handler::createLogger()
{
	// N.B.: se si cambia implementazione non passare boost::bind lo shared_ptr esterno o si crea un riferimento ciclico
	initLogger(Logger::create(boost::bind(&ILogHandler::log, this, _1)));
}

//////////////////////////////////////////////////////////////////////

template <typename T>
ILogHandler<T>::ILogHandler()
{
	
}

template <typename T>
ILogHandler<T>::~ILogHandler()
{

}

template <typename T>
inline shared_ptr<Logger> ILogHandler<T>::getLogger() const
{
	return m_logger;
}

template <typename T>
shared_ptr<T> ILogHandler<T>::create()
{
	shared_ptr<Handler> instance(OS_NEW_T(Handler), &ILogHandler::destroy);		
	instance->createLogger();
	return instance;
}

template <typename T>
template <typename P1>
shared_ptr<T> ILogHandler<T>::create(const P1 &p1)
{
	shared_ptr<Handler> instance(OS_NEW_T(Handler)(p1), &ILogHandler::destroy);		
	instance->createLogger();
	return instance;
}

template <typename T>
template <typename P1, typename P2>
shared_ptr<T> ILogHandler<T>::create(const P1 &p1, const P2 &p2)
{
	shared_ptr<Handler> instance(OS_NEW_T(Handler)(p1, p2), &ILogHandler::destroy);		
	instance->createLogger();
	return instance;
}

template <typename T>
template <typename P1, typename P2, typename P3>
shared_ptr<T> ILogHandler<T>::create(const P1 &p1, const P2 &p2, const P3 &p3)
{
	shared_ptr<Handler> instance(OS_NEW_T(Handler)(p1, p2, p3), &ILogHandler::destroy);		
	instance->createLogger();
	return instance;
}

template <typename T>
template <typename P1, typename P2, typename P3, typename P4>
shared_ptr<T> ILogHandler<T>::create(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4)
{
	shared_ptr<Handler> instance(OS_NEW_T(Handler)(p1, p2, p3, p4), &ILogHandler::destroy);		
	instance->createLogger();
	return instance;
}

template <typename T>
template <typename P1, typename P2, typename P3, typename P4, typename P5>
shared_ptr<T> ILogHandler<T>::create(const P1 &p1, const P2 &p2, const P3 &p3, const P4 &p4, const P5 &p5)
{
	shared_ptr<Handler> instance(OS_NEW_T(Handler)(p1, p2, p3, p4, p5), &ILogHandler::destroy);		
	instance->createLogger();
	return instance;
}

template <typename T>
void ILogHandler<T>::initLogger(shared_ptr<Logger> logger)
{
	m_logger = logger;
}

template <typename T>
void ILogHandler<T>::resetLogger()
{
	m_logger.reset();
}

template <typename T>
void ILogHandler<T>::destroy(ILogHandler *instance)
{
	instance->resetLogger();
	OS_DELETE_T(instance);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_ILOGHANDLER_H
