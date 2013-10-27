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

#include "options.h"
#include "osirisplugins.h"
#include "translations.h"
#include "translationslanguagefolderlistener.h"

//////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

static bool pluginInitialize(Plugin &plugin)
{	
	Options::instance()->ensureOption<String>(OS_TRANSLATIONS_OPTION_URL, "https://www.osiris-sps.org/anubis/translations-razor.php", false);		
	Options::instance()->ensureOption<String>(OS_TRANSLATIONS_OPTION_PASSWORD, String::EMPTY, false);		
	Options::instance()->ensureOption<bool>(OS_TRANSLATIONS_OPTION_REPORT_ENABLED, true, false);		
	Options::instance()->ensureOption<uint32>(OS_TRANSLATIONS_OPTION_REPORT_DELAY, 60000, false);

	TranslationsLanguageFolderListener::install();

	return true;
}

static bool pluginTerminate()
{
	TranslationsLanguageFolderListener::uninstall();

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_PLUGIN_DECLARE(pluginInitialize, pluginTerminate)

//////////////////////////////////////////////////////////////////////
