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

#ifndef _OS_CORE_STREAMLAYERZLIB_H
#define _OS_CORE_STREAMLAYERZLIB_H

#include "crypt.h"
#include "istreamlayer.h"
#include "pimpl_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class CoreExport StreamLayerZlib : public IStreamLayer
{
	typedef IStreamLayer LayerBase;

// Construction
public:
	StreamLayerZlib(CompressionLevel level = compressionLevelNormal);
	virtual ~StreamLayerZlib();

// IStreamLayer interface
public:
	virtual uint32 read(void *v, uint32 size) const;
	virtual uint32 write(const void *v, uint32 size);

// IStreamLayer overrides
public:
	virtual bool close();

private:
	pimpl_of<StreamLayerZlib>::type m_impl;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_STREAMLAYERZLIB_H