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

#ifndef _OS_CORE_RANDOMNUMBERGENERATOR_H
#define _OS_CORE_RANDOMNUMBERGENERATOR_H

#include "boost/thread/thread.hpp"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport RandomNumberGenerator : public StaticSingleton<RandomNumberGenerator>
{
	typedef set<boost::thread::id>::type ThreadsMap;

// Construction
protected:
	RandomNumberGenerator();

public:
	virtual ~RandomNumberGenerator();

// Operations
public:
	// Genera un nuovo valore casuale
	template <typename T>
	T generate(const T &lowerBound, const T &upperBound);

	// Dealloca i dati sul thread che ha richiesto un valore casuale (opzionale, può servire nel caso vengano creati threads in continuazione)
	void dispose();
	void dispose(const boost::thread::id &id);

	// Svuota la cache
	void clear();

private:
	void initGenerator();

private:
	boost::mutex m_cs;
	ThreadsMap m_threads;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
T RandomNumberGenerator::generate(const T &lowerBound, const T &upperBound)
{	
	initGenerator();
	return static_cast<T>((rand()%(upperBound - lowerBound + 1)) + (lowerBound));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_RANDOMNUMBERGENERATOR_H
