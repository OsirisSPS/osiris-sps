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
#include "ideskin.h"

#include "convert.h"
#include "engine.h"
#include "ideide.h"
#include "idesystem.h"
#include "ids.h"
#include "filesystem.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "options.h"
#include "portalsportals.h"
#include "htmlparser.h"
#include "htmlpage.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String IdeSkin::ID = _S("id");
const String IdeSkin::NAME = _S("name");
const String IdeSkin::PREVIEW = _S("preview");
const String IdeSkin::DESCRIPTION = _S("description");
const String IdeSkin::VERSION = _S("version");
const String IdeSkin::AUTHOR = _S("author");
const String IdeSkin::COMPATIBILITY = _S("compatibility");
//const String IdeSkin::DEFAULT = _S("default");

const String IdeSkin::STYLES = _S("styles");
const String IdeSkin::STYLE = _S("style");
const String IdeSkin::CSS = _S("css");
const String IdeSkin::SCRIPTS = _S("scripts");
const String IdeSkin::SCRIPT = _S("script");
const String IdeSkin::JS = _S("js");


//////////////////////////////////////////////////////////////////////

IdeSkin::IdeSkin() : m_valid(false)
{	
}

IdeSkin::~IdeSkin()
{
	_clear();
}

String IdeSkin::_getSchemaPath()
{
	return utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(), OS_SCHEMAS_PATH), OS_IDE_SKIN_SCHEMA);
}

uint32 IdeSkin::getPagerPages() const
{
	return Options::instance()->getOption<uint32>(Options::ide_options::pager_pages);
}

uint32 IdeSkin::getPagerItems() const
{
	return Options::instance()->getOption<uint32>(Options::ide_options::pager_items);
}

String IdeSkin::getFaviconUrl() const
{
	return getResourceUrl(utils::makeFilePath(OS_SKIN_PATH_ICONS, OS_FAVICON));
}

String IdeSkin::getResourcePath(const String &resource) const
{
	return findResource(resource, false);
}

String IdeSkin::getResourceUrl(const String &resource) const
{
	return findResource(resource, true);
}

String IdeSkin::getImageUrl(const String &filename) const
{
	return findResource(utils::makeFilePath(OS_SKIN_PATH_IMAGES, filename), true);
}

String IdeSkin::getTemplatePath(const String &filename) const
{
	return findResource(utils::makeFilePath(OS_SKIN_PATH_TEMPLATES, filename), false);
}

bool IdeSkin::read(shared_ptr<XMLDocument> document)
{
	shared_ptr<XMLNode> root = document->getRoot();
	if(_parseAttributes(root) == false)
		return false;

	shared_ptr<XMLNode> styles = root->getNode(STYLES);
	if(styles != null)	// gli stili sono opzionali
	{
		if(_parseStyles(styles) == false)
			return false;
	}

	shared_ptr<XMLNode> scripts = root->getNode(SCRIPTS);
	if(scripts != null)	// gli scripts sono opzionali
	{
		if(_parseScripts(scripts) == false)
			return false;
	}

	return true;
}

bool IdeSkin::loadXml(const String &path)
{
	shared_ptr<XMLSchema> schema(OS_NEW XMLSchema());
	schema->parseFile(_getSchemaPath());

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument(schema));
	// Carica l'xml della skin
	if(document->parseFile(path) == false)
		return false;

	// Carica la skin
	if(read(document) == false) 
	{
		unload();		// N.B.: resetta i dati eventualmente caricati (non rimuovere)
		return false;
	}

	m_valid = true;
	return true; 
}

void IdeSkin::unload()
{
	_clear();
}

void IdeSkin::apply(shared_ptr<HtmlPage> page)
{
	OS_ASSERT(page != null);
	if(page == null || isValid() == false)
		return;
	
	const StringList &styles = getStyles();
	// Scorre gli stylesheet dello schema
	for(StringList::const_iterator i = styles.begin(); i != styles.end(); ++i)
	{
		// Nota: i css specifici della skin non devono essere caricati dalla base se non vengono trovati

		// Inizializza l'url del css
		String css_url = findResource(*i, true);
		if(css_url.empty() == false)
			page->addCss(css_url);
	}

	const StringList &scripts = getScripts();
	// Scorre gli scripts dello schema
	for(StringList::const_iterator i = scripts.begin(); i != scripts.end(); ++i)
	{
		// Nota: i js specifici della skin non devono essere caricati dalla base se non vengono trovati

		// Inizializza l'url del js
		String js_url = findResource(*i, true);
		if(js_url.empty() == false)
			page->addJavascript(js_url);
	}	
}

/*
String IdeSkin::getText(const String &culture, const String &name)
{
	OS_ASSERT(m_languageFolder != null);
	return m_languageFolder->getText(culture, name);
}
*/

void IdeSkin::_clear()
{
	m_valid = false;
	//m_path.clear();
	m_description.clear();
	m_version.clear();
	m_author.clear();
	m_compatibility = 0;
}

bool IdeSkin::_parseAttributes(const shared_ptr<XMLNode> &node)
{
	OS_ASSERT(node != null);

	m_id = node->getAttributeString(ID).to_ascii();
	if(m_id.validate(false) == false)
		return false;
	
	/*
	if(m_id != id)
		return false;
	*/

	m_name = node->getAttributeString(NAME);
	if(m_name.empty())
		return false;

	m_preview = node->getAttributeString(PREVIEW);
	m_description = node->getAttributeString(DESCRIPTION);
	m_version = node->getAttributeString(VERSION);
	m_author = node->getAttributeString(AUTHOR);

	if(m_compatibility.fromString(node->getAttributeString(COMPATIBILITY).to_ascii()) == false)
		return false;

	if(m_compatibility > OS_OSIRIS_VERSION())
	{
		OS_LOG_ERROR(_S("Incompatible skin"));
		return false;
	}

	return true;
}

bool IdeSkin::_parseStyles(const shared_ptr<XMLNode> &styles)
{
	OS_ASSERT(styles != null);

	shared_ptr<XMLNodes> schema_styles = styles->getNodes();
	// Scorre tutti gli stili dello schema
	for(XMLNodes::const_iterator i = schema_styles->begin(); i != schema_styles->end(); ++i)
	{
		// Carica lo stile corrente
		if(_parseStyle(*i) == false)
			return false;
	}

	return true;
}

bool IdeSkin::_parseStyle(const shared_ptr<XMLNode> &style)
{
	OS_ASSERT(style != null);

	String css = style->getAttributeString(CSS);
	if(css.empty() == false)
		m_styles.push_back(css);

	return true;
}

bool IdeSkin::_parseScripts(const shared_ptr<XMLNode> &scripts)
{
	OS_ASSERT(scripts != null);

	shared_ptr<XMLNodes> schema_scripts = scripts->getNodes();
	// Scorre tutti gli scripts dello schema
	for(XMLNodes::const_iterator i = schema_scripts->begin(); i != schema_scripts->end(); ++i)
	{
		// Carica lo script corrente
		if(_parseScript(*i) == false)
			return false;
	}

	return true;
}

bool IdeSkin::_parseScript(const shared_ptr<XMLNode> &script)
{
	OS_ASSERT(script != null);

	String js = script->getAttributeString(JS);
	if(js.empty() == false)
		m_scripts.push_back(js);

	return true;
}

String IdeSkin::findResource(String resource, bool httpPath) const
{	
	if(isValid() == false)
		return String::EMPTY;

	String res = resource;

	// Scatta l'evento. Se risponde, tengo quello.

	{
		String path = utils::makeFilePath(IdeSystem::instance()->getHtdocsPath(), resource);
		if(FileSystem::instance()->exists(path))
		{
			if(httpPath)
				return _S("/") + utils::makeFilePath(OS_HTDOCS_PATH, resource);		
			else
				return path;
		}
		else
		{
			/*
			// Se è richiesto un path HTTP, e la risorsa richiesta inizia per /,
			// allora la considero assoluta.
			// Ad esempio, nell'elenco CSS di una skin in una extension.
			// E' logico, dato che non ha senso che il nome di una risorsa inizi per /.
			if( (httpPath) && (resource.starts_with(_S("/"))) )
				return resource;
			*/

			OS_LOG_WARNING(String::format(_S("Resource '%S' not found.").c_str(), resource.c_str()));
			return String::EMPTY;
		}
	}	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
