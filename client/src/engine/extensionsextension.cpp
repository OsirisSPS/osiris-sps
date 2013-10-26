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
#include "extensionsextension.h"

#include "engine.h"
#include "extensionssystem.h"
#include "iextensionscodeprovider.h"
#include "iextensionscodecontext.h"
#include "idesystem.h"
#include "ideskinsimple.h"
#include "ihttpserver.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "filesystem.h"
#include "options.h"
#include "platformmanager.h"
#include "notificationsmanager.h"
#include "path.h"
#include "utils.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ExtensionsExtension::ExtensionsExtension() : m_loaded(false)
{	
}

ExtensionsExtension::~ExtensionsExtension()
{

}
/*
shared_ptr<LanguageFolder> Extension::getExtensionsFolder() const
{
	shared_ptr<LanguageFolder> folder = LanguageManager::instance()->getRootFolder()->getFolder(_S("extensions"));
	if(folder == null)
		folder = LanguageManager::instance()->getRootFolder()->addFolder(_S("extensions"));

	return folder;
}
*/

bool ExtensionsExtension::isActive()
{
	if(Options::instance()->exists(getActiveOptionName()))
		return Options::instance()->getOption<bool>(getActiveOptionName());
	else
		return true;
}

String ExtensionsExtension::getStorePath() const
{
	return utils::makeFolderPath(Options::instance()->getDataPath(), utils::makeFolderPath(OS_STORE_PATH, getID().toUTF16()));
}

String ExtensionsExtension::getActiveOptionName()
{
	return _S("extensions.") + getID().toUTF16() + _S(".active");
}

bool ExtensionsExtension::init(const ExtensionID &id, const Path &path)
{
	OS_ASSERT(path.empty() == false);
	OS_ASSERT(m_path.empty());

	m_path = path;

	//String filename = utils::makeFilePath(path, id.toUTF16() + ".xml"); // 0.13
	String filename = utils::makeFilePath(path, OS_MANIFESTXML); // 0.14
	if(FileSystem::instance()->fileExists(filename) == false)
		return false;
	
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(XMLSchema::fromFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(), OS_SCHEMAS_PATH), OS_EXTENSIONS_EXTENSION_SCHEMA))));
	if(document->parseFile(filename) == false)
		return false;

	String languagesPath = utils::makeFolderPath(path.path(), OS_LANGUAGES_PATH);
	if(FileSystem::instance()->directoryExists(languagesPath))
	{
		m_languageFolder.reset(OS_NEW LanguageFolder());
		m_languageFolder->addPath(languagesPath);
	}

	String htdocsPath = utils::makeFolderPath(path.path(), OS_HTDOCS_PATH);
	if(FileSystem::instance()->directoryExists(htdocsPath))
	{
		// TODO: Qui dovrei fare un'opzione a livello di xml extension, che stabilisce il nome della virtual-directory. Se omesso, è l'ID.
		// Per ora, forzo l'unico caso in cui mi servirebbe.
		String virtualName = id.toUTF16();
		if(id.toUTF16() == OS_EXTENSIONS_CORE)
			virtualName = OS_HTDOCS_PATH;

		m_httpDirectory.reset(OS_NEW HttpPhysicalDirectory(virtualName, htdocsPath));
	}

	// Auto-discovery IdeSkinSimple
	String skinsPath = utils::makeFolderPath(path.path(), "skins");
	{
		if(FileSystem::instance()->directoryExists(skinsPath))
		{
			StringList skins;
			FileSystem::instance()->getFiles(skinsPath, skins, false);
			for(StringList::const_iterator i = skins.begin(); i != skins.end(); ++i)
			{
				String config = "/skins/" + *i;
				String title = FileSystem::instance()->getFileTitle(*i);
				shared_ptr<IdeSkinSimple> skin(OS_NEW IdeSkinSimple());
				if(skin->init(get_this_ptr(), config, title))
					m_skins.push_back(skin);
			}
		}
	}

	shared_ptr<XMLNode> root = document->getRoot();

	if(root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_ID) != id.getString()) // Ensure that the directory name is equal to ID.
		return false;

	m_id = id;
	m_name = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_NAME);	
	m_description = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_DESCRIPTION);
	m_content = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_CONTENT);
	m_category = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_CATEGORY);
	m_tags = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_TAGS);
	m_trust = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_TRUST);
	m_author = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_AUTHOR);
	m_version = root->getAttributeFloat(OS_EXTENSION_XML_NODE_ROOT_VERSION);
	m_compatibility = root->getAttributeFloat(OS_EXTENSION_XML_NODE_ROOT_COMPATIBILITY);
	/*
	if(m_version.fromString(root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_VERSION).to_ascii()) == false)
		return false;
	String compatibility = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_COMPATIBILITY);
	if( (compatibility.empty() == false) && (m_compatibility.fromString(compatibility.to_ascii()) == false) )
		return false;
	*/
	m_homepage = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_HOMEPAGE);
	m_icon = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_ICON);
	m_logo = root->getAttributeString(OS_EXTENSION_XML_NODE_ROOT_LOGO);
	
	NotificationsManager::instance()->notify(_S("Loading extension: ") + m_name);

	shared_ptr<XMLNode> nodeFiles = document->getRoot()->getNode(OS_EXTENSION_XML_NODE_SCRIPTS);
	if(nodeFiles != null)
	{
		shared_ptr<XMLNodes> files = nodeFiles->getNodes();
		for(XMLNodes::const_iterator i = files->begin(); i != files->end(); ++i)
		{
			String scriptPath = utils::makeFilePath(path, (*i)->getAttributeString(OS_EXTENSION_XML_NODE_SCRIPT_PATH));
			String scriptLanguage = (*i)->getAttributeString(OS_EXTENSION_XML_NODE_SCRIPT_LANGUAGE);

			shared_ptr<IExtensionsCodeProvider> codeProvider = ExtensionsSystem::instance()->getCodeProvider(scriptLanguage);
			if(codeProvider == null)
			{
				OS_LOG_ERROR(_S("Invalid script language '") + scriptLanguage + _S("'"));
				return false;
			}

			shared_ptr<IExtensionsCodeContext> context = codeProvider->createContext();
			if(context == null)
			{
				OS_LOG_ERROR(_S("Cannot create context for script language '") + scriptLanguage + _S("'"));
				return false;
			}

			if(context->parseFile(scriptPath))
				m_contexts.push_back(context);				
			else
				OS_LOG_ERROR(_S("Cannot parse extension file '") + scriptPath + _S("'"));
		}
	}
	
	return true;
}

void ExtensionsExtension::activate()
{
	if(isActive())
		return;

	Options::instance()->setOption<bool>(getActiveOptionName(), true);

	if(m_loaded == false)
		load();
}

void ExtensionsExtension::deactivate()
{
	if(isActive() == false)
		return;

	Options::instance()->setOption<bool>(getActiveOptionName(), false);
	
	if(m_loaded)
		unload();
}

void ExtensionsExtension::executeContexts(bool loaded)
{
	for(CodeContexts::const_iterator i = m_contexts.begin(); i != m_contexts.end(); (*i)->execute(get_this_ptr(), loaded), ++i);
}

ExtensionID ExtensionsExtension::getID() const
{
	return m_id;
}

String ExtensionsExtension::getName() const
{
	return m_name;
}

String ExtensionsExtension::getDescription() const
{
	return m_description;
}

String ExtensionsExtension::getContent() const
{
	return m_content;
}

String ExtensionsExtension::getCategory() const
{
	return m_category;
}

String ExtensionsExtension::getTags() const
{
	return m_tags;
}

String ExtensionsExtension::getTrust() const
{
	return m_trust;
}

String ExtensionsExtension::getAuthor() const
{
	return m_author;
}

float ExtensionsExtension::getVersion() const
{
	return m_version;
}

float ExtensionsExtension::getCompatibility() const
{
	return m_compatibility;
}

String ExtensionsExtension::getHomePage() const
{
	return m_homepage;
}

String ExtensionsExtension::getIcon() const
{
	return m_icon;
}

String ExtensionsExtension::getLogo() const
{
	return m_logo;
}


String ExtensionsExtension::getPath() const
{
	return m_path;
}

bool ExtensionsExtension::getInternal() const
{
	// Le extension caricate da manifest son sempre non interne (cioè compaiono nella lista addons).
	return false;
}

/*
String Extension::getText(const String &culture, const String &name)
{
	OS_ASSERT(m_languageFolder != null);
	return m_languageFolder->getText(culture, name);
}
*/

void ExtensionsExtension::load()
{
	if( (m_loaded == false) && (isActive()) )
	{
		if(m_languageFolder)
			LanguageManager::instance()->addFolder(m_languageFolder);

		if(m_httpDirectory)
			IdeSystem::instance()->getServer()->addDirectory(m_httpDirectory);

		for(Skins::iterator i = m_skins.begin(); i != m_skins.end(); ++i)
		{
			IdeSystem::instance()->registerSkin(*i);
		}

		executeContexts(true);

		m_loaded = true;
	}
}

void ExtensionsExtension::unload()
{
	if(m_loaded)
	{
		executeContexts(false);

		if(m_languageFolder)
			LanguageManager::instance()->removeFolder(m_languageFolder);

		if(m_httpDirectory)
			IdeSystem::instance()->getServer()->removeDirectory(m_httpDirectory);

		for(Skins::const_iterator i = m_skins.begin(); i != m_skins.end(); ++i)
		{
			IdeSystem::instance()->unregisterSkin(*i);
		}
		
		m_loaded = false;
	}
}

void ExtensionsExtension::reload()
{
	unload();
	load();
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
