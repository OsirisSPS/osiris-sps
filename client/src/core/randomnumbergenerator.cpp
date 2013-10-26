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
#include "randomnumbergenerator.h"

#include <time.h>

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

RandomNumberGenerator::RandomNumberGenerator()
{

}

RandomNumberGenerator::~RandomNumberGenerator()
{

}

void RandomNumberGenerator::initGenerator()
{
	boost::mutex::scoped_lock lock(m_cs);

	boost::thread::id id = boost::this_thread::get_id();
	
	if(m_threads.find(id) != m_threads.end())
		return;

	// Salva il thread nella cache
	m_threads.insert(id);
	// Se qui entra in assert l'applicazione sta creando di continuo threads e la cache è probabile che cresca all'infinito, 
	// pertanto sarebbe necessario la funzione dispose() all'uscita di ogni thread
	OS_ASSERT(m_threads.size() < 1000);	
	// Effettua il seed per il thread corrente
	srand((unsigned) time(null));
}

void RandomNumberGenerator::dispose()
{
	// Rimuove il thread corrente dalla cache
	dispose(boost::this_thread::get_id());
}

void RandomNumberGenerator::dispose(const boost::thread::id &id)
{
	boost::mutex::scoped_lock lock(m_cs);

	// Rimuove il thread specificato dalla cache se presente

	ThreadsMap::iterator i = m_threads.find(id);
	if(i != m_threads.end())
		m_threads.erase(i);
}

void RandomNumberGenerator::clear()
{
	boost::mutex::scoped_lock lock(m_cs);
	
	m_threads.clear();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
