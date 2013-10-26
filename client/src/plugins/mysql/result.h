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

#ifndef _MYSQL_RESULT_H
#define _MYSQL_RESULT_H

#include "boost/thread/recursive_mutex.hpp"
#include "dbiresult.h"
#include "lock.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

namespace mysql {

//////////////////////////////////////////////////////////////////////

class Result : public IDatabaseResult
{
	typedef std::pair<enum_field_types, unsigned int> field_type;
	typedef vector<field_type>::type FieldTypes;

// Construction
public:
	Result(boost::recursive_mutex &cs);
	virtual ~Result();

// Attributes
public:
	uint32 columnLength(uint32 index);

// Operations
public:
	void prepare(Connection *connection);
	void prepare(MYSQL_RES *result);

// IResult interface
public:
	virtual uint32 columns();
	virtual String columnName(uint32 index);

	virtual bool end();

	virtual void moveNext();

	virtual void get(uint32 index, DataItem &value);

private:
	static int32 _toInt32(char *data);
	static uint32 _toUint32(char *data);
	static double _toDouble(char *data);
	static String _toString(char *data, unsigned long length);
	static Buffer _toBuffer(char *data, unsigned long length);

protected:
	boost::recursive_mutex::scoped_lock m_lock;
	MYSQL_RES *m_result;
	MYSQL_ROW m_row;
	unsigned long *m_lengths;
	StringVector m_fieldNames;
	FieldTypes m_fieldTypes;	
};

//////////////////////////////////////////////////////////////////////

} // mysql

//////////////////////////////////////////////////////////////////////

#endif // _MYSQL_RESULT_H
