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

#ifndef _OS_CORE_STREAMFILTER_H
#define _OS_CORE_STREAMFILTER_H

#include "istream.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IStreamLayer;

//////////////////////////////////////////////////////////////////////

class CoreExport StreamFilter : public IStream
{
// Construction
public:
	StreamFilter(shared_ptr<IStream> stream);
	virtual ~StreamFilter();

// Operations
public:
	void addLayer(shared_ptr<IStreamLayer> layer);

// IStream interface
public:
	virtual bool is_open() const;
	virtual bool close();
	virtual uint32 read(void *v, uint32 size) const;
	virtual uint32 write(const void *v, uint32 size);
	virtual bool seek(uint64 offset, SeekPosition from) const;
	virtual uint64 position() const;
	virtual uint64 size() const;
	virtual bool flush();
	virtual bool eof() const;

protected:
	pimpl_of<StreamFilter>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_STREAMFILTER_H
