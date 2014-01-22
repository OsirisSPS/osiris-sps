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

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_buffer_construction)
{
	Buffer b1(64);

	BOOST_CHECK(b1.empty());
	BOOST_CHECK(b1.getData() == nullptr);
	BOOST_CHECK(b1.getPosition() == nullptr);
	BOOST_CHECK(b1.getEndPosition() == nullptr);
	BOOST_CHECK(b1.getAllocEndPosition() == nullptr);
	BOOST_CHECK(b1.getAvailable() == 0);
	BOOST_CHECK(b1.isReadable() == false);
	BOOST_CHECK(b1.getOffset() == 0);
	BOOST_CHECK(b1.getSize() == 0);
	BOOST_CHECK(b1.getAllocSize() == 0);
	BOOST_CHECK(b1.getGrowSize() == 64);

	b1.reserve(100);
	BOOST_CHECK(b1.empty() == false);
	BOOST_CHECK(b1.getData() != nullptr);
	BOOST_CHECK(b1.getPosition() != nullptr);
	BOOST_CHECK(b1.getEndPosition() != nullptr);
	BOOST_CHECK(b1.getAllocEndPosition() != nullptr);
	BOOST_CHECK(b1.getAvailable() == 100);
	BOOST_CHECK(b1.isReadable());
	BOOST_CHECK(b1.getSize() == 100);	
	BOOST_CHECK(b1.getAllocSize() == 128);	// growsize * 2
	BOOST_CHECK(b1.getGrowSize() == 64);

	Buffer b2(b1);
	BOOST_CHECK(b1 == b2);
	BOOST_CHECK(b2.empty() == false);
	BOOST_CHECK(b2.getData() != nullptr);
	BOOST_CHECK(b2.getPosition() != nullptr);
	BOOST_CHECK(b2.getEndPosition() != nullptr);
	BOOST_CHECK(b2.getAllocEndPosition() != nullptr);
	BOOST_CHECK(b2.getAvailable() == 100);
	BOOST_CHECK(b2.isReadable());
	BOOST_CHECK(b2.getSize() == 100);	
	BOOST_CHECK(b2.getAllocSize() == 128);	// growsize * 2
	BOOST_CHECK(b2.getGrowSize() == 64);
	
	Buffer b3;
	b3 = b2;
	BOOST_CHECK(b2 == b3);
	BOOST_CHECK(b3.empty() == false);
	BOOST_CHECK(b3.getData() != nullptr);
	BOOST_CHECK(b3.getPosition() != nullptr);
	BOOST_CHECK(b3.getEndPosition() != nullptr);
	BOOST_CHECK(b3.getAllocEndPosition() != nullptr);
	BOOST_CHECK(b3.getAvailable() == 100);
	BOOST_CHECK(b3.isReadable());
	BOOST_CHECK(b3.getSize() == 100);	
	BOOST_CHECK(b3.getAllocSize() == 128);	// growsize * 2
	BOOST_CHECK(b3.getGrowSize() == 64);

	b3.clear();
	BOOST_CHECK(b3.empty());
	BOOST_CHECK(b3.getData() == nullptr);
	BOOST_CHECK(b3.getPosition() == nullptr);
	BOOST_CHECK(b3.getEndPosition() == nullptr);
	BOOST_CHECK(b3.getAllocEndPosition() == nullptr);
	BOOST_CHECK(b3.getAvailable() == 0);
	BOOST_CHECK(b3.isReadable() == false);
	BOOST_CHECK(b3.getOffset() == 0);
	BOOST_CHECK(b3.getSize() == 0);
	BOOST_CHECK(b3.getAllocSize() == 0);
	BOOST_CHECK(b3.getGrowSize() == 64);
}

BOOST_AUTO_TEST_CASE(test_buffer_io)
{
	Buffer b1;

	uint8 v1 = 9;
	BOOST_CHECK(b1.write(&v1, sizeof(uint8)) == 1);
	BOOST_CHECK(b1.getSize() == 1);
	BOOST_CHECK(b1.getAvailable() == 0);
	BOOST_CHECK(b1.seekToBegin());	
	BOOST_CHECK(b1.getAvailable() == 1);

	uint8 v2 = 0;
	BOOST_CHECK(b1.read(&v2, sizeof(uint8)) == 1);
	BOOST_CHECK(v2 == v1);
}

BOOST_AUTO_TEST_CASE(test_buffer_erase)
{
	uint32 u = 50;
	std::string alfa = "alfa";
	byte b = 3;	
	std::string beta = "beta";
	
	Buffer templateBuffer;
	BOOST_REQUIRE(templateBuffer.write(&u, sizeof(uint32)) == sizeof(uint32));
	BOOST_REQUIRE(templateBuffer.write(alfa.data(), static_cast<uint32>(alfa.size())) == static_cast<uint32>(alfa.size()));
	BOOST_REQUIRE(templateBuffer.write(&b, sizeof(byte)) == sizeof(byte));
	BOOST_REQUIRE(templateBuffer.write(beta.data(), static_cast<uint32>(beta.size())) == static_cast<uint32>(beta.size()));
	BOOST_REQUIRE(templateBuffer.seekToBegin());
	BOOST_REQUIRE(templateBuffer.getSize() == (sizeof(uint32) + alfa.size() + sizeof(byte) + beta.size()));

	//	0 | 0 | 0 | 50 | a | l | f | a | 3 | b | e | t | a |

	{
		// Caso pos < start
		// Pos = 0
		// Start = 8
		// Count = npos
		// Test = offset == 0

		Buffer buffer(templateBuffer);

		BOOST_CHECK(buffer.erase(sizeof(uint32) + static_cast<uint32>(alfa.size())));
		BOOST_CHECK(buffer.getSize() == (sizeof(uint32) + static_cast<uint32>(alfa.size())));
		BOOST_CHECK(buffer.getAvailable() == (sizeof(uint32) + static_cast<uint32>(alfa.size())));
		BOOST_CHECK(buffer.getOffset() == 0);

		uint32 test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(uint32)) == sizeof(uint32));
		BOOST_CHECK(test == u);
	}

	{
		// pos < start
		// Pos = 4
		// Start = 8
		// Count = npos
		// Test = offset == 4

		Buffer buffer(templateBuffer);

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32)));	
		BOOST_CHECK(buffer.erase(sizeof(uint32) + static_cast<uint32>(alfa.size())));
		BOOST_CHECK(buffer.getSize() == (sizeof(uint32) + static_cast<uint32>(alfa.size())));
		BOOST_CHECK(buffer.getAvailable() == (static_cast<uint32>(alfa.size())));
		BOOST_CHECK(buffer.getOffset() == sizeof(uint32));

		char test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(char)) == sizeof(char));
		BOOST_CHECK(test == alfa[0]);
	}

	{
		// pos == start
		// Pos = 0
		// Start = 0
		// Count = 4
		// Test = offset == 0

		Buffer buffer(templateBuffer);

		BOOST_CHECK(buffer.erase(0, sizeof(uint32)));

		BOOST_CHECK(buffer.getOffset() == 0);
		BOOST_CHECK(buffer.getSize() == static_cast<uint32>(alfa.size() + sizeof(byte) + beta.size()));
		BOOST_CHECK(buffer.getAvailable() == static_cast<uint32>(alfa.size() + sizeof(byte) + beta.size()));

		char test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(char)) == sizeof(char));
		BOOST_CHECK(test == alfa[0]);
	}

	{
		// pos == start
		// Pos = 4
		// Start = 4
		// Count = 4
		// Test = offset == 4

		Buffer buffer(templateBuffer);

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32)));	
		BOOST_CHECK(buffer.erase(sizeof(uint32), static_cast<uint32>(alfa.size())));

		BOOST_CHECK(buffer.getOffset() == sizeof(uint32));
		BOOST_CHECK(buffer.getSize() == static_cast<uint32>(sizeof(uint32) + sizeof(byte) + beta.size()));
		BOOST_CHECK(buffer.getAvailable() == static_cast<uint32>(sizeof(byte) + beta.size()));

		byte test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(byte)) == sizeof(byte));
		BOOST_CHECK(test == b);
	}

	{
		// pos = start + count
		// Pos = 4
		// Start = 0
		// Count = 4
		// Test = offset == 0

		Buffer buffer(templateBuffer);		

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32)));	
		BOOST_CHECK(buffer.erase(0, sizeof(uint32)));

		BOOST_CHECK(buffer.getSize() == (static_cast<uint32>(alfa.size()) + sizeof(byte) + static_cast<uint32>(beta.size())));
		BOOST_CHECK(buffer.getAvailable() == (static_cast<uint32>(alfa.size()) + sizeof(byte) + static_cast<uint32>(beta.size())));
		BOOST_CHECK(buffer.getOffset() == 0);

		char test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(char)) == sizeof(char));
		BOOST_CHECK(test == alfa[0]);
	}

	{
		// pos = start + count
		// Pos = 8
		// Start = 4
		// Count = 4
		// Test = offset == 4

		Buffer buffer(templateBuffer);		

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32) + static_cast<uint32>(alfa.size())));	
		BOOST_CHECK(buffer.erase(sizeof(uint32), static_cast<uint32>(alfa.size())));
		
		BOOST_CHECK(buffer.getSize() == (sizeof(uint32) + sizeof(byte) + static_cast<uint32>(beta.size())));
		BOOST_CHECK(buffer.getAvailable() == sizeof(byte) + static_cast<uint32>(beta.size()));
		BOOST_CHECK(buffer.getOffset() == sizeof(uint32));

		byte test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(byte)) == sizeof(byte));
		BOOST_CHECK(test == b);
	}

	{
		// pos > (start + count)
		// Pos = 9
		// Start = 4
		// Count = 4
		// Test = offset == 5

		Buffer buffer(templateBuffer);		

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32) + static_cast<uint32>(alfa.size()) + sizeof(byte)));
		BOOST_CHECK(buffer.erase(sizeof(uint32), static_cast<uint32>(alfa.size())));
		
		BOOST_CHECK(buffer.getSize() == (sizeof(uint32) + sizeof(byte) + static_cast<uint32>(beta.size())));
		BOOST_CHECK(buffer.getAvailable() == static_cast<uint32>(beta.size()));
		BOOST_CHECK(buffer.getOffset() == (sizeof(uint32) + sizeof(byte)));

		char test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(char)) == sizeof(char));
		BOOST_CHECK(test == beta[0]);
	}

	{
		// pos > (start + count)
		// Pos = 8
		// Start = 0
		// Count = 4
		// Test = offset == 4

		Buffer buffer(templateBuffer);

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32) + static_cast<uint32>(alfa.size())));	
		BOOST_CHECK(buffer.erase(0, sizeof(uint32)));

		BOOST_CHECK(buffer.getOffset() == alfa.size());
		BOOST_CHECK(buffer.getAvailable() == (sizeof(byte) + static_cast<uint32>(beta.size())));

		byte test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(byte)) == sizeof(byte));
		BOOST_CHECK(test == b);
	}

	{
		// pos > (start + count)
		// Pos = 8
		// Start = 4
		// Count = 4
		// Test = offset == 4

		Buffer buffer(templateBuffer);

		BOOST_CHECK(buffer.seekOffset(sizeof(uint32) + static_cast<uint32>(alfa.size())));	
		BOOST_CHECK(buffer.erase(sizeof(uint32), static_cast<uint32>(alfa.size())));

		BOOST_CHECK(buffer.getSize() == (sizeof(uint32) + sizeof(byte) + static_cast<uint32>(beta.size())));
		BOOST_CHECK(buffer.getOffset() == sizeof(uint32));

		byte test = 0;
		BOOST_CHECK(buffer.peek(&test, sizeof(byte)) == sizeof(byte));
		BOOST_CHECK(test == b);
	}	
}

BOOST_AUTO_TEST_CASE(test_buffer_serialization)
{
	String first = _S("alfa");
	Buffer buffer;
	BOOST_CHECK(buffer.writeString(first));
	buffer.seekToBegin();
	String second;
	BOOST_CHECK(buffer.readString(second));
	BOOST_CHECK(first == second);	
}

BOOST_AUTO_TEST_CASE(test_buffer_xor)
{
	String check(_S("osiris"));

	Buffer b1;
	BOOST_CHECK(b1.writeString(check));;
	b1.seekToBegin();

	uint32 v = 123;
	b1.xor(&v, sizeof(uint32));

	Buffer b2(b1);
	b2.xor(&v, sizeof(uint32));

	String str;
	BOOST_CHECK(b2.readString(str));

	BOOST_CHECK(check == str);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
