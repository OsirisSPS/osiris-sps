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
#include "logmanager.h"
#include "platformmanager.h"
#include "testapp.h"

#include <iostream>

////////////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

////////////////////////////////////////////////////////////////////////////

bool init_unit_test_suite()
{
	return true;
}

void dump_informations(char** envp)
{
    std::cout << "-------------------------------------" << std::endl;
	std::cout << "Informations" << std::endl;
	std::cout << "" << std::endl;

    std::cout << "lang: " << PlatformManager::instance()->getSystemCulture().to_ascii() << std::endl;

	char** env;
	for (env = envp; *env != 0; env++)
	{
		char* thisEnv = *env;
		std::cout << "env: " << thisEnv << std::endl;		
	}

    std::cout << "-------------------------------------" << std::endl;
}

int main(int argc, char *argv[], char** envp)
{
	PlatformManager::instance()->init(PlatformManager::applicationConsole);
	LogManager::instance()->init();
	LogManager::instance()->setEnabled(false);
	//TestApp::instance()->init(FileSystem::instance()->getFilePath(argv[0]));
	FileSystem::instance()->initExecutableName(argv[0]);
	TestApp::instance()->init(FileSystem::instance()->getExecutablePath());	

    dump_informations(envp);

	int result = boost::unit_test::unit_test_main(&init_unit_test_suite, argc, argv);

	TestApp::instance()->cleanup();
	LogManager::instance()->cleanup();
	PlatformManager::instance()->cleanup();

	return result;
}

////////////////////////////////////////////////////////////////////////////
