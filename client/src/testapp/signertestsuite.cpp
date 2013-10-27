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
#include "dbvalue.h"
#include "objectssigner.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static void populate_signer(ObjectsSigner &signer)
{
	signer.add(true);

	signer.add(static_cast<uint32>(400));

	signer.add(String::EMPTY);
	signer.add(Buffer::EMPTY);

	DateTime emptyDateTime;
	signer.add(emptyDateTime);

	signer.add(false);

	signer.add(static_cast<int32>(-11));

	signer.add(static_cast<double>(-500.28991));

	DateTime validDateTime("19801017053259");
	signer.add(validDateTime);

	signer.add(static_cast<uint32>(123));

	signer.add(static_cast<double>(-159.254));

	String str("alnfalknalkmna)lks(dmada_DAdasdAdajdbu123123;--asda[]@<>");
	signer.add(str);

	Buffer buff;
	for(uint32 i = 0; i < 100; i++)
		BOOST_REQUIRE(buff.write(&i, sizeof(uint32)) == sizeof(uint32));
	
	signer.add(buff);

	DbValue<String> emptyDBString;
	signer.add(emptyDBString);

	DbValue<String> validDBString(str);
	signer.add(validDBString);

	DbValue<Buffer> emptyDBBuffer;
	signer.add(emptyDBBuffer);

	DbValue<Buffer> validDBBuffer(buff);
	signer.add(validDBBuffer);

	// TODO: aggiungere ComponentID, ModuleID, ObjectID e PortalID
}

BOOST_AUTO_TEST_CASE(test_signer_operations)
{
	Buffer privateKey;
	Buffer publicKey;
	BOOST_REQUIRE(CryptManager::instance()->rsaGenerateKeys(rsaType2048, privateKey, publicKey));

	ObjectsSigner signer;
	populate_signer(signer);

	Buffer signature;
	BOOST_CHECK(signer.sign(privateKey, signature));
	BOOST_CHECK(signer.validate(publicKey, signature));

	ObjectsSigner checker;
	populate_signer(checker);
	BOOST_CHECK(checker.validate(publicKey, signature));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
