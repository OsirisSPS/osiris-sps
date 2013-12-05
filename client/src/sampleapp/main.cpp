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


#include "utils.h"

#include "archivesmanager.h"

#include "conversions.h"

#include "osirislink.h"

#include "filesystem.h"

#include "iculayer.h"
#include "unicode/ustdio.h"
#include "unicode/ustring.h"


/*
#include "conversions.h"
#include "dtl/dtl.hpp"
#include "platformmanager.h"
#include "randomnumbergenerator.h"
#include "htmlevent.h"
#include "mainaddonsjob.h"
#include "filesystem.h"
#include "utils.h"
*/

//#include "httpclient.h"
//#include "xmldocument.h"

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

/*
std::wstring generateHashMerge(std::wstring hash1, std::wstring hash2, bool add)
{
	// L'hash deve essere valorizzato. I portali esistenti saranno vuoti, e quindi mai funzionanti. Me ne fotto, se li re-importeranno per ora.
	if(hash1.length() == 0)
		return String::EMPTY.to_wide();

	if(hash1.length() != hash2.length())
		return String::EMPTY.to_wide();

	std::wstring result = hash1;
	for(size_t i=0;i<hash1.length();i++)
	{
		int32 a = conversions::hexToInt(hash1[i]);
		int32 b = conversions::hexToInt(hash2[i]);

		int32 c = 0;
		if(add)
			c = (a + b)%16;
		else
			c = (16 + a - b)%16;

		result[i] = conversions::intToHex(c);
	}

	return result;
}
*/
/*
void test_hash()
{
	std::wstring hash = _S("4A7F130B4A5C42CC5D928D157641596A89543C65").to_wide();
	std::wstring hash2 = _S("9EF66A96C4A73540B8DB46A14D56FF49802ED82F").to_wide();

	std::wstring result1 = utils::generateHashMergeW(hash,hash2,true);
	std::wstring result2 = utils::generateHashMergeW(result1,hash2,false);

	bool ok1 = (hash == result2);

	bool ok2 = true;


	std::string Xhash = "4A7F130B4A5C42CC5D928D157641596A89543C65";
	std::string Xhash2 = "9EF66A96C4A73540B8DB46A14D56FF49802ED82F";

	std::string Xresult1 = utils::generateHashMerge(Xhash,Xhash2,true);
	std::string Xresult2 = utils::generateHashMerge(Xresult1,Xhash2,false);

	bool Xok1 = (Xhash == Xresult2);

	bool Xok2 = true;

}
*/
/*
void test_httpclient()
{
	String url = _S("http://tnt-overtime.tk/isis/request.php");
	shared_ptr<boost::asio::io_service> service = createAsioService();

	shared_ptr<TCPSocket> socket(OS_NEW TCPSocket(*service));

	shared_ptr<HttpClient> client(OS_NEW HttpClient(service, socket));
	shared_ptr<HttpRequest> request = client->getRequest();
	OS_ASSERT(request != null);

	request->setMethod(httpMethodPost);

	request->setPostParamString("portal", "010000010CA3D28F949D9787A321A2A59E1295E4716F7CE2");

	client->perform(url.to_ascii());
}
*/
/*
void test_dtl()
{
    std::cout << "enter first string: ";
    std::string s1;
    std::cin >> s1;

    std::cout << "enter second string: ";
    std::string s2;
    std::cin >> s2;

    dtl::Diff<char, std::string> d(s1, s2);
    d.compose();

    std::cout << "editDistance:" << d.getEditDistance() << std::endl;

    // Longest Common Subsequence
    std::vector<char> lcs_v = d.getLcsVec();
    std::string lcs_s(lcs_v.begin(), lcs_v.end());
    std::cout << "LCS:" << lcs_s << std::endl;

    // Shortest Edit Script
    std::cout << "SES" << std::endl;
    d.printSES();

	std::cout << "UF" << std::endl;
	d.printUnifiedFormat();
}
*/
/*
void test_unordered_map()
{
	unordered_map<std::string, shared_ptr<std::string> >::type map;

	for(;;)
	{
		map.clear();

		uint32 count = RandomNumberGenerator::instance()->generate<uint32>(1, 1000);
		for(uint32 i = 0; i < count; ++i)
		{
			std::string key = conversions::to_string(i);
			map[key] = shared_ptr<std::string>(new std::string(conversions::to_string(i)));
		}
	}
}
*/

/*
void check_job(shared_ptr<IBackgroundJob> job)
{
	shared_ptr<MainAddonsJob> check = boost::dynamic_pointer_cast<MainAddonsJob>(job);
	if(check != null)
	{
		// job punta effettivamente ad un MainAddonsJob o ad una ulteriore derivata da MainAddonsJob
	}
	else
	{
		// job non è di tipo MainAddonsJob
	}
}

void test_jobs()
{
	//shared_ptr<IBackgroundJob> job(OS_NEW MainAddonsJob(123, ""));
	//check_job(job);
}
*/

/*
void testUnicode()
{
	unsigned char* pSource = new unsigned char[28];
	pSource[0] = 84;
	pSource[1] = 0;
	pSource[2] = 101;
	pSource[3] = 0;
	pSource[4] = 115;
	pSource[5] = 0;
	pSource[6] = 116;
	pSource[7] = 0;
	pSource[8] = 32;
	pSource[9] = 0;
	pSource[10] = 179;
	pSource[11] = 111;
	pSource[12] = 128;
	pSource[13] = 149;
	pSource[14] = 121;
	pSource[15] = 114;
	pSource[16] = 43;
	pSource[17] = 82;
	pSource[18] = 76;
	pSource[19] = 136;
	pSource[20] = 63;
	pSource[21] = 101;
	pSource[22] = 64;
	pSource[23] = 83;
	pSource[24] = 125;
	pSource[25] = 0;
	pSource[26] = 0;
	pSource[27] = 0;

	int32_t nChars = 100;
	wchar_t* pDest = new wchar_t[nChars];
	memset(pDest, 0, nChars * sizeof(wchar_t));

	UErrorCode status = U_ZERO_ERROR;
	u_strToWCS(pDest, nChars, &nChars, (const UChar*) pSource, -1, &status);
	if(U_SUCCESS(status))
	{
		wchar_t c1 = pDest[2];	// Ascii char. Win: 115, Linux: 115, OS-X: 115
		wchar_t c2 = pDest[5];	// Japan char. Win: 28595, Linux: 28595, OS-X: 26
		wchar_t c3 = pDest[6];   // Japan char. Win: 38272, Linux: 38272, OS-X: 26

		printf("sc1: %lu\n",c1);
        printf("sc2: %lu\n",c2);
        printf("sc3: %lu\n",c3);
	}



	printf("--- Start Clodo hack on SampleApp\n");

	printf("sizeof wchar_t: %d\n", sizeof(wchar_t));
	printf("sizeof UChar: %d\n", sizeof(UChar));

	printf("--- End Clodo hack on SampleApp\n");

	u_strFromWCS(null, 0, null, null, 0, &status);
}
*/

void dumpLink(std::string tag, std::string url)
{
	OsirisLink o;
	o.parse(url);

	std::string out = o.generate();

	printf("-------------------------");

}

void dump()
{
	// Portal - Definitivi.
	//dumpLink("ok","osiris://|portal|0000000197DCF94088CEFBC0D8EBB4995087EE65B4805589|name=Serverless%20News|description=|user=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8|");
	//dumpLink("ok","osiris://|portal|0000000197DCF94088CEFBC0D8EBB4995087EE65B4805589|name=Serverless%20News|description=|user=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8|/");
	//dumpLink("ok in 0X, no in 1X","osiris://|portal|0000000197DCF94088CEFBC0D8EBB4995087EE65B4805589|name=Serverless%20News|description=|/");
	//dumpLink("ok","osiris:?type=portal&portal=00000001ADC379E5F7BB3BB80536266456F7EEC8FEE2A377&name=Disaster%20Portal");
	//dumpLink("ok","osiris:?type=portal&portal=00000001ADC379E5F7BB3BB80536266456F7EEC8FEE2A377&pov=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8&name=Disaster%20Portal&description=Portale%20di%20test");
	//dumpLink("ok","osiris:?type=portal&portal=00000001ADC379E5F7BB3BB80536266456F7EEC8FEE2A377&pov=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8&name=Disaster%20Portal");
	//dumpLink("ok","osiris:?type=portal&portal=00000001ADC379E5F7BB3BB80536266456F7EEC8FEE2A377&pov=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8");

	// Isis - Definitivi.
	//dumpLink("ok in 0X, no in 1X","osiris://|isis|0000000197DCF94088CEFBC0D8EBB4995087EE65B4805589|name=Official%20Isis|url=http://isis.kodeware.net/|");
	//dumpLink("ok","osiris:?type=isis&portal=000000017D3FD0686252D640EB2FFE59F34511198F811940&pov=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8&name=Official%20Isis&url=http://isis.kodeware.net/");

	// File - Definitivi.
	//dumpLink("ok","osiris://|file|08000002F7BC65083796982049EA1A3435C2AAA385C3691E");
	//dumpLink("ok","osiris:?type=file&id=08000002F7BC65083796982049EA1A3435C2AAA385C3691E");
	//dumpLink("ok","osiris:?type=file&id=08000002F7BC65083796982049EA1A3435C2AAA385C3691E&portal=00000001ADC379E5F7BB3BB80536266456F7EEC8FEE2A377");
	//dumpLink("ok","osiris:?type=file&id=08000002F7BC65083796982049EA1A3435C2AAA385C3691E&portal=00000001ADC379E5F7BB3BB80536266456F7EEC8FEE2A377&pov=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8");

	// Url - Da sistemare
	//dumpLink("wrong","osiris://|url|portals/view|id=0500000208C6C0EE776629CDF0466689A0ADCC9EC964601C&portal=010000010CA3D28F949D9787A321A2A59E1295E4716F7CE2");
	//dumpLink("ok","osiris://|url|portals/view");
	dumpLink("ok","osiris://|url|/portals/view|id=05000002E60BD8F42FECD0CBF67A5891B745E0BD1EF3318F&portal=010000018E61CF26177EB8EA6835B6A85D1D9831B64CF3AB|");
	//dumpLink("ok","osiris:?type=url&url=portals%2Fview%3Fid%3D0500000208C6C0EE776629CDF0466689A0ADCC9EC964601C&portal=010000010CA3D28F949D9787A321A2A59E1295E4716F7CE2&pov=01000002A9D3C2D0FA4A9357933EBB7CE12BBA843FFFB4E8");
	//dumpLink("ok","osiris:?type=url&url=portals%2Fview%3Fid%3D0500000208C6C0EE776629CDF0466689A0ADCC9EC964601C&portal=010000010CA3D28F949D9787A321A2A59E1295E4716F7CE2");
	//dumpLink("ok","osiris:?type=url&url=portals%2Fview%3Fid%3D0500000208C6C0EE776629CDF0466689A0ADCC9EC964601C");

	// Res - Definitivi.
	//dumpLink("wrong","osiris://|skin|images/flags/icon/en.png");
	//dumpLink("wrong","osiris:?type=skin&path=images/flags/icon/en.png");
	//dumpLink("ok","osiris://|skin|images%2Fflags%2Ficon%2Fen.png");
	//dumpLink("ok","osiris:?type=res&path=images%2Fflags%2Ficon%2Fen.png");

}



int main(int argc, char *argv[])
{
	ObjectID id1("0D2C4E505C1D98B221E222CABE4E167BECB2D669");
	PovID id2("0D2C4E505C1D98B221E222CABE4E167BECB2D669");

	//String v1 = _S("72480651");
	String v1 = _S("72480651.123");
	double v2 = conversions::from_utf16<double>(v1);

	std::string va1 = "72480651.123";
	float va2 = conversions::from_string<double>(va1);

	bool X1 = (id1 == id2);
	bool X2 = (id1.getString() == id2.getString());
	bool X3 = X1;
//	FileSystem::instance()->initExecutablePath(argv[0]);
//	std::cout << "4:" << FileSystem::instance()->getExecutablePath().to_ascii() << std::endl;
//	test_hash();
//	test_httpclient();
//  test_dtl();
//	test_unordered_map();
//	test_jobs();

	/*
	int x =0;
#ifdef _WIN64
	x=1;
#error 64
#elif _WIN32
	x=2;
#error 32
#else
	x=3;
#error unknown
#endif
	*/


	//ArchivesManager::instance()->extract("O:\\osiris\\client\\share\\extensions\\9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704.osiris", "d:\\out");
	//ArchivesManager::instance()->extract("d:\\9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704.osiris", "d:\\out");
	//ArchivesManager::instance()->extract("d:\\9F4EBA0F1F02D218D684D92A39EF8F52E1DA9704.zip", "d:\\out");
	//ArchivesManager::instance()->extract("d:\\test.zip", "d:\\out");


	//String dataPath = _S("O:/osiris-sps/razor/bin/windows/i386/vc10/debug/../../../../../data/");
	//String dataPath = _S("O:/osiris-sps/razor/data/");

	/*
	String dataPath = _S("D:/Projects/Osiris/osiris-sps/razor/bin/windows/i386/vc10/debug/../../../../../../client/data/");

	bool x = FileSystem::instance()->isDirectory(_S("c:"));

	if(FileSystem::instance()->directoryExists(dataPath) == false)
	{	
		if(FileSystem::instance()->ensureDirectory(dataPath) == false)
		{
			String str1 = _S("test");
		}
	}

	String str1 = _S("test");
	//double dbl1 = 3.123;
	double dbl1 = 0;

	String sql1 = String::format(_S("MyStr: %S, MyFloat: %f, MyStr: %S").c_str(), str1.c_str(), dbl1, str1.c_str());
	String sql2 = String::format(_S("MyFloat: %f").c_str(), dbl1);
	*/

	dump();
	//testUnicode();

	return 0;
}
