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

#include "buffer.h"
#include "cryptmanager.h"
#include "memfile.h"
#include "streamfilter.h"
#include "streamlayergzip.h"
#include "streamlayerzlib.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_streams_buffer)
{
	shared_ptr<Buffer> buffer(OS_NEW Buffer());

	uint32 value = 0;

	{
		shared_ptr<MemFile> file(OS_NEW MemFile(buffer));
		
		shared_ptr<StreamFilter> filter(OS_NEW StreamFilter(file));
		filter->addLayer(shared_ptr<IStreamLayer>(OS_NEW StreamLayerGzip()));
		filter->addLayer(shared_ptr<IStreamLayer>(OS_NEW StreamLayerZlib()));

		value = 10;
		BOOST_CHECK(filter->write(&value, sizeof(uint32)) == sizeof(uint32));
	}

	buffer->seekToBegin();

	{
		shared_ptr<MemFile> file(OS_NEW MemFile(buffer));

		shared_ptr<StreamFilter> filter(OS_NEW StreamFilter(file));
		filter->addLayer(shared_ptr<IStreamLayer>(OS_NEW StreamLayerGzip()));
		filter->addLayer(shared_ptr<IStreamLayer>(OS_NEW StreamLayerZlib()));

		value = 0;
		BOOST_CHECK(filter->read(&value, sizeof(uint32)) == sizeof(uint32));
		BOOST_CHECK(value == 10);
	}

	buffer->seekToBegin();

	Buffer decompressedGzip;
	BOOST_CHECK(CryptManager::instance()->decompress(compressorTypeGzip, buffer->getData(), buffer->getSize(), decompressedGzip));

	Buffer decompressedZlib;
	BOOST_CHECK(CryptManager::instance()->decompress(compressorTypeZlib, decompressedGzip.getData(), decompressedGzip.getSize(), decompressedZlib));

	value = 0;
	BOOST_CHECK(decompressedZlib.read(&value, sizeof(uint32)) == sizeof(uint32));
	BOOST_CHECK(value == 10);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
