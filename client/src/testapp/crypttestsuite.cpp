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
#include "cryptkey.h"
#include "cryptkeyagreement.h"
#include "cryptmanager.h"
#include "memfile.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static void test_dh(CryptKeyAgreement &first)
{
	BOOST_CHECK(first.getModulus().empty() == false);
	BOOST_CHECK(first.getGenerator().empty() == false);
	BOOST_CHECK(first.getPublicKey().empty() == false);
	BOOST_CHECK(first.getPrivateKey().empty() == false);

	CryptKeyAgreement second;
	BOOST_CHECK(second.init(first.getModulus(), first.getGenerator()));
	BOOST_CHECK(second.getModulus().empty() == false);
	BOOST_CHECK(second.getGenerator().empty() == false);
	BOOST_CHECK(second.getPublicKey().empty() == false);
	BOOST_CHECK(second.getPrivateKey().empty() == false);

	CryptKey firstKey;
	BOOST_CHECK(first.agree(second.getPublicKey(), firstKey));
	BOOST_CHECK(firstKey.getKey().empty() == false);

	CryptKey secondKey;
	BOOST_CHECK(second.agree(first.getPublicKey(), secondKey));
	BOOST_CHECK(secondKey.getKey().empty() == false);

	BOOST_CHECK(firstKey == secondKey);
}

static void test_dh(uint32 size)
{
	CryptKeyAgreement first;
	BOOST_CHECK(first.init(size));
	test_dh(first);
}

BOOST_AUTO_TEST_CASE(test_crypt_dh)
{
	{
		CryptKeyAgreement key;
		BOOST_CHECK(key.init(Buffer::EMPTY, Buffer::EMPTY) == false);

		Buffer buff1;
		buff1.reserve(113);
		OS_ZEROMEMORY(buff1.getData(), 113);

		Buffer buff2;
		buff2.reserve(59);
		OS_ZEROMEMORY(buff2.getData(), 59);
		
		BOOST_CHECK(key.init(buff1, buff2) == false);

		BOOST_CHECK(key.init(16));
		test_dh(key);

		BOOST_CHECK(key.init(32));
		test_dh(key);
	}

	test_dh(64);
	test_dh(128);
	test_dh(256);		
}

BOOST_AUTO_TEST_CASE(test_crypt_rsa)
{
	Buffer privateKey;
	Buffer publicKey;
	BOOST_CHECK(CryptManager::instance()->rsaGenerateKeys(rsaType1024, privateKey, publicKey));
	BOOST_CHECK(CryptManager::instance()->rsaCheckKeys(privateKey, publicKey));

	Buffer publicKeyCheck;
	BOOST_CHECK(CryptManager::instance()->rsaGetPublicKey(privateKey, publicKeyCheck));
	BOOST_CHECK(publicKeyCheck == publicKey);

	std::string foo("foo");

	Buffer encrypted;
	BOOST_CHECK(CryptManager::instance()->rsaEncrypt(publicKey, foo.data(), foo.size(), encrypted));
	Buffer decrypted;
	BOOST_CHECK(CryptManager::instance()->rsaDecrypt(privateKey, encrypted.getData(), encrypted.getSize(), decrypted));
	BOOST_CHECK(std::string(reinterpret_cast<const char *>(decrypted.getData()), decrypted.getSize()) == foo);
	Buffer signature;
	BOOST_CHECK(CryptManager::instance()->rsaSign(privateKey, foo.data(), foo.size(), signature));
	BOOST_CHECK(CryptManager::instance()->rsaVerify(publicKey, foo.data(), foo.size(), signature));
}

BOOST_AUTO_TEST_CASE(test_crypt_hex)
{
	std::string foo("foo");

	std::string encoded;
	BOOST_CHECK(CryptManager::instance()->encodeHex(foo.data(), foo.size(), encoded));
	
	Buffer decoded;
	BOOST_CHECK(CryptManager::instance()->decodeHex(encoded, decoded));
	BOOST_CHECK(std::string(reinterpret_cast<const char *>(decoded.getData()), decoded.getSize()) == foo);
}

BOOST_AUTO_TEST_CASE(test_crypt_base64)
{
	std::string foo("foo");

	std::string encoded;
	BOOST_CHECK(CryptManager::instance()->encodeBase64(foo.data(), foo.size(), encoded));
	
	Buffer decoded;
	BOOST_CHECK(CryptManager::instance()->decodeBase64(encoded, decoded));
	BOOST_CHECK(std::string(reinterpret_cast<const char *>(decoded.getData()), decoded.getSize()) == foo);
}

BOOST_AUTO_TEST_CASE(test_crypt_aes)
{
	CryptKey key("key");

	std::string foo("foo");

	Buffer encrypted;
	Buffer decrypted;

	BOOST_CHECK(CryptManager::instance()->aesEncrypt(key, foo.data(), foo.size(), encrypted, true));
	BOOST_CHECK(CryptManager::instance()->aesDecrypt(key, encrypted.getData(), encrypted.getSize(), decrypted, true));
	BOOST_CHECK(std::string(reinterpret_cast<const char *>(decrypted.getData()), decrypted.getSize()) == foo);
	
	encrypted.clear();
	decrypted.clear();

	BOOST_CHECK(CryptManager::instance()->aesEncrypt(key, foo.data(), foo.size(), encrypted, false));
	BOOST_CHECK(CryptManager::instance()->aesDecrypt(key, encrypted.getData(), encrypted.getSize(), decrypted, false));
	BOOST_CHECK(std::string(reinterpret_cast<const char *>(decrypted.getData()), decrypted.getSize()) == foo);
}

BOOST_AUTO_TEST_CASE(test_crypt_aes_values)
{
	{
		CryptKey key("sfdlkj2342lk");
		std::string data("jlhefljh2348234uih2384283482yh4blajksflj3");
		Buffer encrypted;
		BOOST_CHECK(CryptManager::instance()->aesEncrypt(key, data.data(), data.size(), encrypted, true));
		BOOST_CHECK(encrypted.toBase64() == "BZl6SD/CW7rlYAO/b1mQICp72ia6h54I/31QSuiKO1d9WEzZOKRYGDLcpAKGjNmtm72Uimq5vwI++wNUJA==");
	}

	{
		CryptKey key("_[]^'''\"fjoanme");
		std::string data("~fe[]asdlk,.,.;:;:;.[]+]]---");
		Buffer encrypted;
		BOOST_CHECK(CryptManager::instance()->aesEncrypt(key, data.data(), data.size(), encrypted, true));
		BOOST_CHECK(encrypted.toHex() == "50721777E1B872A0891F746C1399235D8336E807EDC085B79E4046A3D5CC8C6C2F3AD31F0097AC8BFF22176E6B4D6841");
	}

	{
		Buffer encryptedPrivateKey;
		BOOST_CHECK(encryptedPrivateKey.fromHex("EE45B4FA64250CEE71E2434392F1DA750E085C7656CBD7AB1E10BD52DDC449A5C21419F5677A6A4ACB789CE106382F5B712394D06225E7A3245E6B350A6C1C8BB022EBA8E2DE36E7B933AA6935690A4604788BBD849FFF1D02262325B7C1E778600806C37D7757D17D9CEA569DD14612E9819D4D02427341A866ADC2F9E2F59073027D096A9C0092F6F989DDC045151992529031DACCCBFF468B494AD399F808F5C4CA3D3B8F1161383A716D83DF134D0E8F007403D6E9F93CAF558D9667D4F159C46E8406BF268F9B04E97F9DD5459D55CB4A443410B529797E5BF49D3E8AA99ADFB60EE6200B7E39733D54D91222B29ABF4EEB02C8F0530EFD24175ED13ABAD6862A98E263EA24AB4CE0B2FDC0564FA43EBC1676FA3A4FFA7DB400714C1818D9F67629D66C9D6A1758EB389EA035F7E676EE7907F6D95D1B9065D89A5674C600ED0A4B34B828D92D5E6CA0829B60D28C936C1E52FF6F57B4DE99148EBF2154DEE0BBBD31B8FBD3CD058122078D00E6ED39856B046A648AB69A026866472AE242B824D4BE0E0D97FDBB81290F3961FBF7F07C3E0F3CB94C76DDEA8FA1D02AB2462B84068E04CCA3A78E15690350EE99E93C055EC5D5EBB97FAF309675DAEF48CDDA4C4DD0AC481AAD0821439A1267FE25CCFFB19CDAB6EF828571F12B7E1C3C6D02019A63D843D20A55E50AF5F3A7B7C9F6A01F035B03E48CCF7C93E02D23FAF9C88C7A7E0939C21DDC9733CD02157F27D72C4746DEDC6961899862055AA89DFFEDA54AE17BE8A5FE1226308AAA1215D8493B66C53E933DE2DACCA7A8953DFE576732D8CA0CD6D26A1211BC1CE1000C12FABF7F7E1C31ACCE2D44BFE193770A82AA1EAB773DA8D1CB67FD3AEA87C6BD80192FED66520DE4FF2CD68F0058621B3A961B526A5C1921E76EB8BC349CAFAAD3EC7400EC7F1F5EBFD8A027CAD792B91E6AB8601AA07CA8EC6659AD85D6F61515C148DB45602F41B467A6148802F08F77112B59D4E2AD83EA21B7DAC3B2DC43C56E72D8E4BE631F14B1F71FE51936F36A1FEB19F56E5F061E336A2C1EC691DF3924D25BE85AE5079EEA6D68A9405CDBD0D21F255BA7EA0F6FD576815A64D115C6FD68D3297C07FAD1EE27040A898754DBDCF8D44710046ED18CDD69D0EAFE35F0BA40B1E03B537929431B32E56F1B3A4C3DBB1E412893074C9A39F793A6CDE5323D0C360DE98124106F71F0AA1849F5F5FE795714623D5B2B5843374A8CEBA7B86040FE6497B55B065A3847D9870309EAE2FAA0C78F405656BAF697F350843116EE6F79CE7494EB43F734A4D32A5E6EC37B735007B219DC7EB5BBC4AD4BB401987D4BF35BDF93B7B16EF1D4A53D42BAA7B7D12B1FB9B04C0A3F21F88232A7AEE7CDE6A0C2BD8B4114DA93D5BEB65BBC14659420124E198A4B5D4450060D9CBADABDA3E9C26F1D279E40F63A6B76B3224C7C736176283E9EABD857D4F46B30C32FBC0AB2F0156ADDBA90532FE7E77E860AAB9F94C9A1B0153E5207E85B68CA362DD9B1A51DC0647164235D407553E0929659FD676F3B1EEA337D3D663D67214F34A4998DE2B74B8339438D37C10371C7860DC2C20A7F93D20C16AFD19320D8ADF8592F09CF818412B57BFEAFF5096F3EE0E85D6E8DD8754D4A198ABC9F040B53CA5A04D5DF957703F91686323FE6E376CDC25C118E891BF3934636CC1B989A6287FD1B92144DA19018179130ACAF9AF9A60616B1716A59677094DA9B864B251AE87D"));

		Buffer encryptedPublicKey;
		BOOST_CHECK(encryptedPublicKey.fromHex("F14CA17F7C45951B3C11BAE7F5209448DAE2938056CBD2312C1CBB6BFA440809B3519E036B7E6B484C799563321E1ED1770197D0628E8620973EFEF54B4625F097C35EA75179D60B7B8FD58FB2465BA79188E856989D48F91217DFD900B3A562329B777B57C8A0A3C42E46F46C23FC031CE31DED5D86A8FCD530280A1A624B2D2D066277E329E7FCB8F6F1935F1FC6123ADA8E41B61AA35E1F3BD62B17BAE4F3A4C501FCCE02E5F20EB021BA0314203DA9B5EB852BC7215C416D1C22EF48B45E0F5E0E5CB9EE7FA3B8A295BD55111A9AD769B8FBDD0FBFB9E6E54C6A0E649F11BFF2CB8EE4E38333E79865988BE4536F8CB9B0D8E4E294727533A7CCDFA7A4216C77CF4375CAFB3F82B2652089FBD35EB091D869E91AD40BD32F30448AC304F5BD8E8547F44C5902E3D44BD417643416569669037D46AF5D"));			

		CryptKey key("osiris");

		Buffer privateKey;
		BOOST_CHECK(CryptManager::instance()->aesDecrypt(key, encryptedPrivateKey.getData(), encryptedPrivateKey.getSize(), privateKey, true));

		Buffer publicKey;
		BOOST_CHECK(CryptManager::instance()->aesDecrypt(key, encryptedPublicKey.getData(), encryptedPublicKey.getSize(), publicKey, true));

		BOOST_CHECK(CryptManager::instance()->rsaCheckKeys(privateKey, publicKey));
	}
}

static void test_compressor_type(CompressorType compressor, CompressionLevel level, bool headerless, const Buffer &data, const set<CompressorType>::type &compressors)
{
	OS_ASSERT(data.empty() == false);

	shared_ptr<Buffer> compressed(OS_NEW Buffer());
	BOOST_CHECK(CryptManager::instance()->compress(compressor, data.getData(), data.getSize(), *compressed, level));

	Buffer decompressed;
	BOOST_CHECK(CryptManager::instance()->decompress(compressor, compressed->getData(), compressed->getSize(), decompressed));

	BOOST_CHECK(data == decompressed);

	compressed->seekToBegin();
	shared_ptr<MemFile> stream(OS_NEW MemFile(compressed));

	if(headerless == false)
	{		
		BOOST_CHECK(CryptManager::instance()->detectCompressor(compressed->getData(), compressed->getSize()) == compressor);
		BOOST_CHECK(CryptManager::instance()->detectCompressor(stream) == compressor);
	}

	for(set<CompressorType>::type::const_iterator i = compressors.begin(); i != compressors.end(); ++i)
	{
		OS_ASSERT(*i != compressorTypeDeflate);

		if(*i == compressor)
			continue;

		BOOST_CHECK(CryptManager::instance()->matchCompressor(compressed->getData(), compressed->getSize(), *i) == false);
		BOOST_CHECK(CryptManager::instance()->matchCompressor(stream, *i) == false);
	}
}

static void test_compressor_type(CompressorType compressor, bool headerless, const set<CompressorType>::type &compressors)
{
	for(uint32 i = 0; i < 16; ++i)
	{
		uint32 size = 1 << i;

		Buffer data(size);
		data.reserve(size);
		CryptManager::instance()->randomBlock(data.getData(), data.getSize());

		test_compressor_type(compressor, compressionLevelLow, headerless, data, compressors);
		test_compressor_type(compressor, compressionLevelNormal, headerless, data, compressors);
		test_compressor_type(compressor, compressionLevelHigh, headerless, data, compressors);
	}	
}

BOOST_AUTO_TEST_CASE(test_crypt_compressors)
{
	set<CompressorType>::type compressors;
	// non include compressorTypeDeflate in quanto headerless
	compressors.insert(compressorTypeZlib);
	compressors.insert(compressorTypeGzip);

	test_compressor_type(compressorTypeDeflate, true, compressors);
	test_compressor_type(compressorTypeZlib, false, compressors);
	test_compressor_type(compressorTypeGzip, false, compressors);

	for(uint32 i = 0; i < 16; ++i)
	{
		uint32 size = 1 << i;

		shared_ptr<Buffer> buffer(OS_NEW Buffer(size));
		buffer->reserve(size);
		OS_ZEROMEMORY(buffer->getData(), buffer->getSize());	
		BOOST_CHECK(CryptManager::instance()->detectCompressor(buffer->getData(), buffer->getSize()) == compressorTypeUnknown);
		BOOST_CHECK(CryptManager::instance()->detectCompressor(shared_ptr<MemFile>(OS_NEW MemFile(buffer))) == compressorTypeUnknown);		
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
