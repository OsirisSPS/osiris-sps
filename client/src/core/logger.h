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

#ifndef _OS_CORE_LOGGER_H
#define _OS_CORE_LOGGER_H

#include "base/object.h"
#include "boost/signals.hpp"
#include "sealed.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LogMessage;

//////////////////////////////////////////////////////////////////////

class CoreExport Logger : Sealed<Logger>,
						  public Object
{
// Construction
private:
	Logger();
	Logger(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot);

public:
	~Logger();

// Attributes
public:
	// Indica se il logger è stato inizializzato
	bool connected() const;

private:
	boost::recursive_mutex & getMutex() const;

// Operations
public:
	static shared_ptr<Logger> create();
	static shared_ptr<Logger> create(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot);

	// Blocca il logger
	void block();
	// Sblocca il logger
	void unblock();

	// Effettua la connessione ad uno slot
	bool connect(const boost::signal<void (shared_ptr<LogMessage> message)>::slot_type &slot);
	// Effettua la disconnessione dallo slot corrente
	void disconnect();

private:
	boost::signals::connection m_connection;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_LOGGER_H
