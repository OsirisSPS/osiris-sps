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

#ifndef _OS_CORE_IMAGE_H
#define _OS_CORE_IMAGE_H

#include "base/object.h"
#include "boost/noncopyable.hpp"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Buffer;

//////////////////////////////////////////////////////////////////////

class CoreExport Image : public Object,
						 public boost::noncopyable					
{
	typedef Object BaseClass;

// Enumerated types
public:
	enum ImageType
	{
		itUnknown		= 0,
		itBmp			= 1,
		itIco			= 2,
		itJpg			= 3,
		itGif			= 4,
		itPcx			= 5,
		itPng			= 6,
		itTiff			= 7,
	};

// Construction
public:
	Image();
	Image(const String &filename, ImageType type = itUnknown);
	virtual ~Image();

// Attributes
public:
	ImageType getType() const;

	uint32 getWidth() const;
	uint32 getHeight() const;

// Operations
public:
	bool load(const String &filename, ImageType type = itUnknown);
	bool load(const Buffer &buffer, ImageType type = itUnknown);
	bool save(const String &filename, ImageType type = itUnknown);
	bool save(Buffer &buffer, ImageType type = itUnknown);

protected:
	struct impl;
	pimpl_ptr<impl> m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_IMAGE_H
