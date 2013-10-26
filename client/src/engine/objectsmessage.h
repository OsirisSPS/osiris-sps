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

#ifndef _OBJECTS_MESSAGE_H
#define _OBJECTS_MESSAGE_H

#include "base/object.h"
#include "buffer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsMessage : public Object
{
    typedef Object BaseClass;

// Construction
public:
	ObjectsMessage();
	ObjectsMessage(const ObjectsMessage &second);
	virtual ~ObjectsMessage();

// Attributes
public:

// Operations
public:
	static Buffer generateSession();

	bool encode(const Buffer &public_key, Buffer &encrypted_session, Buffer &encrypted_content) const;
	bool decode(const Buffer &private_key, const Buffer &encrypted_session, const Buffer &encrypted_content);

private:
	bool _toString(String &str) const;
	bool _fromString(const String &str);

// Operators
public:
	ObjectsMessage & operator =(const ObjectsMessage &second);

public:
	String subject;
	String body;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OBJECTS_MESSAGE_H
