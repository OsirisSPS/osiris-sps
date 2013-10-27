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

#ifndef _IDE_BBCODES_EMOTICON_H
#define _IDE_BBCODES_EMOTICON_H

#include "base/object.h"
#include "enable_this_ptr.h"
#include "ideide.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport OMLEmoticon : public enable_this_ptr<OMLEmoticon>,
										public Object
{
// Construction
public:
	OMLEmoticon(const String &code, const String &name);
	virtual ~OMLEmoticon();

// Attributes
public:
	inline const String & getCode() const;
	inline const String & getName() const;
	
// Internal
private:
	String m_code;
	String m_name;	
};

//////////////////////////////////////////////////////////////////////

inline const String & OMLEmoticon::getCode() const { return m_code; }
inline const String & OMLEmoticon::getName() const { return m_name; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_BBCODES_EMOTICON_H
