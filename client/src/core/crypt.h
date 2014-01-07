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

#ifndef _OS_CORE_CRYPT_H
#define _OS_CORE_CRYPT_H

//////////////////////////////////////////////////////////////////////

namespace CryptoPP {

//////////////////////////////////////////////////////////////////////

class AutoSeededRandomPool;

//////////////////////////////////////////////////////////////////////

} // CryptoPP

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Enumerated types

enum CompressorType
{
	compressorTypeUnknown		= 0,
	compressorTypeDeflate		= 1,
	compressorTypeZlib			= 2,
	compressorTypeGzip			= 3,
};

enum CompressionLevel
{
	compressionLevelNone		= 0,
	compressionLevelLow			= 1,
	compressionLevelNormal		= 2,
	compressionLevelHigh		= 3,
};

enum RsaType
{
	rsaType1024					= 1024,
	rsaType2048					= 2048,
	rsaType4096					= 4096,
};

enum HashType
{
	hashTypeUnknown				= 0,
	hashTypeMD5					= 1,
	hashTypeSHA					= 2,
	hashTypeSHA256				= 3,
	hashTypeSHA512				= 4,
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_CRYPT_H
