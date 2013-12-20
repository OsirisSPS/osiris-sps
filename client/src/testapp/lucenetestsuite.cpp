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

#include "filesystem.h"
#include "lucene.h"
#include "testapp.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LuceneTester : public StaticSingleton<LuceneTester>
{
// Construction
protected:
	LuceneTester();

public:
	~LuceneTester();

// Attributes
public:
	inline std::string getLucenePath() const;

private:
	String m_path;
};

//////////////////////////////////////////////////////////////////////

LuceneTester::LuceneTester()
{
	m_path = TestApp::instance()->getRelativeTestPath(_S("lucene"));
	FileSystem::instance()->ensureDirectory(m_path);
}

LuceneTester::~LuceneTester()
{
	// TODO: da chiamare in una funzione di cleanup
//	_lucene_shutdown();
}

inline std::string LuceneTester::getLucenePath() const
{
	String path = m_path;
	path.trim_right(_S("/\\"));
	return path.to_ascii();
}

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_lucene)
{
	std::string path = LuceneTester::instance()->getLucenePath();

    lucene::analysis::standard::StandardAnalyzer luceneAnalyzer;
	bool create = true;

	if(lucene::index::IndexReader::indexExists(path.c_str()))
	{
		create = false;

		if(lucene::index::IndexReader::isLocked(path.c_str()))
			lucene::index::IndexReader::unlock(path.c_str());
	}

    shared_ptr<lucene::index::IndexWriter> luceneIndexWriter(new lucene::index::IndexWriter(path.c_str(), &luceneAnalyzer, create));

	luceneIndexWriter->setMergeFactor(10);
	luceneIndexWriter->setMinMergeDocs(1000);

	if(luceneIndexWriter != nullptr)
	{
		luceneIndexWriter->optimize();
		luceneIndexWriter->close();
		luceneIndexWriter = nullptr;
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
