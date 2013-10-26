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
#include "testapp.h"

#include "filesystem.h"
#include "utils.h"

////////////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

////////////////////////////////////////////////////////////////////////////

TestApp::TestApp()
{

}

TestApp::~TestApp()
{

}

String TestApp::getRootPath() const
{
	return m_rootPath;
}

String TestApp::getTestPath() const
{
	return m_testPath;
}

String TestApp::getRelativeTestPath(const String &path) const
{
	return utils::makeFolderPath(m_testPath, path);
}

void TestApp::init(const String &rootPath)
{
	m_rootPath = rootPath;
	m_testPath = utils::makeFolderPath(rootPath, OS_TESTAPP_TEST_DIR);
	FileSystem::instance()->removeDirectory(m_testPath);
	FileSystem::instance()->ensureDirectory(m_testPath);
}

void TestApp::cleanup()
{
	if(m_testPath.empty())
		return;

	FileSystem::instance()->removeDirectory(m_testPath);
	m_testPath.clear();
}

////////////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

////////////////////////////////////////////////////////////////////////////
