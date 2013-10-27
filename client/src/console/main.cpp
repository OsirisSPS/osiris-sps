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

#include "condition.h"
#include <signal.h>
#include <iostream>

// TODO: abolire la dipendenza dal core

////////////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

////////////////////////////////////////////////////////////////////////////

Condition g_quitEvent;

////////////////////////////////////////////////////////////////////////////

static void logHandler(const char *message, os_uint8 level, os_time timestamp)
{
	String str;
	str.from_utf8(message);

	// Clodo 0.14 : With unicode text, for example with arabian  characters, crash under Windows.
	//std::wcout << str.to_wide() << std::endl;
	std::cout << str.to_ascii() << std::endl;	
}

////////////////////////////////////////////////////////////////////////////

static void quitHandler(int e)
{
	std::wcout << "Quit detected, stopping systems..." << std::endl; // 0.14

	g_quitEvent.reset();
}

////////////////////////////////////////////////////////////////////////////

static bool run()
{
	if(OS_SDK_FAILED(osiris_start()))
	{
		std::cerr << "Osiris start error" << std::endl;
		return false;
	}

	g_quitEvent.wait();

	if(OS_SDK_FAILED(osiris_stop()))
	{
		std::cerr << "Osiris stop error" << std::endl;
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	osiris_log_handler(&logHandler);

	if(OS_SDK_FAILED(osiris_init(argc, argv, NULL)))
	{
		std::cerr << "Osiris initialization error" << std::endl;
		return 1;
	}

	signal(SIGINT, quitHandler);
	signal(SIGTERM, quitHandler);
	signal(SIGABRT, quitHandler);

	bool result = run();

	if(OS_SDK_FAILED(osiris_cleanup()))
	{
		std::cerr << "Osiris cleanup error" << std::endl;
		return 1;
	}

	std::wcout << "Osiris terminated." << std::endl;

	return result ? 0 : 1;
}
