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

#include "extensionssystem.h"
#include "httpphysicaldirectory.h"
#include "httpvirtualdirectory.h"
#include "idesystem.h"
#include "ihttpserver.h"
#include "log.h"
#include "options.h"
#include "osirisplugins.h"
#include "pythoncodeprovider.h"
#include "pythonengine.h"
#include "pythoninterpreter.h"
#include "platformmanager.h"
#include "plugin.h"
#include "portalsportalssystem.h"
#include "thirdpartylibrariesreporter.h"
#include "thirdpartylibrary.h"

//////////////////////////////////////////////////////////////////////

BOOST_PYTHON_DECLARE_MODULE(osirislib)

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

static bool pluginInitialize(Plugin &plugin)
{
	// Py_GetVersion
	// Py_GetBuildNumber
	// Py_GetCopyright
	// Py_GetCompiler
	// Py_GetBuildInfo

	shared_ptr<ThirdPartyLibrary> pythonLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
	OS_ASSERT(pythonLibrary != nullptr);
	pythonLibrary->setName("Python");
	pythonLibrary->setVersion(PY_VERSION);
	pythonLibrary->setDescription("Python is a remarkably powerful dynamic programming language that is used in a wide variety of application domains.");
	pythonLibrary->setUrl("http://www.python.org");
    pythonLibrary->setLicense("Python Software Foundation");
	pythonLibrary->setLicenseUrl("http://www.python.org/psf/license/");

	if(PythonEngine::instance()->init(Options::instance()->getRootPath(), plugin.getPath()) == false)
	{
		OS_ASSERTFALSE();
		return false;
	}

	if(PythonEngine::instance()->loadModule(BOOST_PYTHON_MODULE_NAME(osirislib), BOOST_PYTHON_MODULE_FUNCTION(osirislib)) == false)
	{
		OS_ASSERTFALSE();
		return false;
	}

	PythonEngine::instance()->run();

	if(ExtensionsSystem::instance()->registerCodeProvider(shared_ptr<IExtensionsCodeProvider>(new PythonCodeProvider())) == false)
		return false;

	return true;
}

static bool pluginTerminate()
{
	PythonEngine::instance()->cleanup();

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_PLUGIN_DECLARE(pluginInitialize, pluginTerminate)

//////////////////////////////////////////////////////////////////////
