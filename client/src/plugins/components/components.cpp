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

#include "componentsextension.h"
#include "extensionssystem.h"
#include "documentationdocumentation.h"
#include "forumforum.h"
#include "forumsection.h"
#include "modulesfeed.h"
#include "moduleslanguageswitcher.h"
#include "modulesmirror.h"
#include "modulespathway.h"
#include "modulesquicksearch.h"
#include "modulessearch.h"
#include "modulestext.h"
#include "modulesxslttransform.h"
#include "osirisplugins.h"
#include "plugin.h"

//////////////////////////////////////////////////////////////////////

OS_MODULE_ENTRY_POINT()

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

String g_componentsPath;

//////////////////////////////////////////////////////////////////////

static bool pluginInitialize(Plugin &plugin)
{
	g_componentsPath = plugin.getPath();

	ExtensionsSystem::instance()->loadExtension(shared_ptr<IExtensionsExtension>(OS_NEW components::ComponentsExtension(g_componentsPath)));

	// Components
	ExtensionsSystem::instance()->registerComponent(shared_ptr<IExtensionsComponent>(OS_NEW components::documentation::Documentation()));
	ExtensionsSystem::instance()->registerComponent(shared_ptr<IExtensionsComponent>(OS_NEW components::forum::Forum()));
	ExtensionsSystem::instance()->registerComponent(shared_ptr<IExtensionsComponent>(OS_NEW components::forum::Section()));

	// Modules
	//ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::Pathway()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::Mirror()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::Text()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::XsltTransform()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::LanguageSwitcher()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::QuickSearch()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::Search()));
	ExtensionsSystem::instance()->registerModule(shared_ptr<IExtensionsModule>(OS_NEW components::modules::Feed()));	

	return true;
}

static bool pluginTerminate()
{
	return true;
}

//////////////////////////////////////////////////////////////////////

OS_PLUGIN_DECLARE(pluginInitialize, pluginTerminate)

//////////////////////////////////////////////////////////////////////
