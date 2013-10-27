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

#ifndef _EXTENSIONSEXTENSION_H
#define _EXTENSIONSEXTENSION_H

#include "iextensionsextension.h"
#include "httpphysicaldirectory.h"
#include "ids.h"
#include "version.h"

//////////////////////////////////////////////////////////////////////

#define OS_EXTENSION_XML_NODE_ROOT					"extension"
#define OS_EXTENSION_XML_NODE_ROOT_ID				"id"
#define OS_EXTENSION_XML_NODE_ROOT_NAME				"name"
#define OS_EXTENSION_XML_NODE_ROOT_DESCRIPTION		"description"
#define OS_EXTENSION_XML_NODE_ROOT_CONTENT			"content"
#define OS_EXTENSION_XML_NODE_ROOT_CATEGORY			"category"
#define OS_EXTENSION_XML_NODE_ROOT_TAGS				"tags"
#define OS_EXTENSION_XML_NODE_ROOT_TRUST			"trust"
#define OS_EXTENSION_XML_NODE_ROOT_AUTHOR			"author"
#define OS_EXTENSION_XML_NODE_ROOT_VERSION			"version"
#define OS_EXTENSION_XML_NODE_ROOT_COMPATIBILITY	"compatibility"
#define OS_EXTENSION_XML_NODE_ROOT_HOMEPAGE			"homepage"
#define OS_EXTENSION_XML_NODE_ROOT_ICON				"icon"
#define OS_EXTENSION_XML_NODE_ROOT_LOGO				"logo"

#define OS_EXTENSION_XML_NODE_SCRIPTS				"scripts"
#define OS_EXTENSION_XML_NODE_SCRIPT				"script"
#define OS_EXTENSION_XML_NODE_SCRIPT_PATH			"path"
#define OS_EXTENSION_XML_NODE_SCRIPT_LANGUAGE		"language"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IExtensionsCodeContext;
class IdeSkin;
class LanguageFolder;
class Path;

//////////////////////////////////////////////////////////////////////

class EngineExport ExtensionsExtension : public IExtensionsExtension
{
	typedef list<shared_ptr<IExtensionsCodeContext> >::type CodeContexts;
	typedef list<shared_ptr<IdeSkin> >::type Skins;

// Construction
public:
	ExtensionsExtension();
	virtual ~ExtensionsExtension();

// Attributes
public:
	//shared_ptr<LanguageFolder> getExtensionsFolder() const;
	bool isActive();

	String getStorePath() const;
	inline void setEnterUrl(const String &url);
	inline String getEnterUrl() const;


// Operations
public:
	bool init(const ExtensionID &id, const Path &path);
	void activate();
	void deactivate();

private:
	String getActiveOptionName();

private:
	void executeContexts(bool loaded);

// IExtension interface
public:
	virtual ExtensionID getID() const;
	virtual String getName() const;
	virtual String getDescription() const;
	virtual String getContent() const;
	virtual String getCategory() const;
	virtual String getTags() const;
	virtual String getTrust() const;
	virtual String getAuthor() const;
	virtual float getVersion() const;
	virtual float getCompatibility() const;
	virtual String getHomePage() const;
	virtual String getIcon() const;
	virtual String getLogo() const;
	
	virtual bool getInternal() const;
	virtual String getPath() const;

	virtual void load();
	virtual void unload();

// Operations
public:
	// Restituisce un testo tradotto in base alla skin
	//String getText(const String &culture, const String &name);
	void reload();

private:
	bool m_loaded;

protected:	
	ExtensionID m_id;
	String m_name;	
	String m_description;
	String m_content;
	String m_category;	
	String m_tags;
	String m_trust;
	String m_author;
	float m_version;
	float m_compatibility;
	String m_homepage;
	String m_icon;
	String m_logo;

	String m_enterUrl;
	
	String m_path;
	CodeContexts m_contexts;
	Skins m_skins;

	shared_ptr<LanguageFolder> m_languageFolder;
	shared_ptr<HttpPhysicalDirectory> m_httpDirectory;
};

//////////////////////////////////////////////////////////////////////

void ExtensionsExtension::setEnterUrl(const String &url) { m_enterUrl = url; }
String ExtensionsExtension::getEnterUrl() const { return m_enterUrl; }

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _EXTENSIONSEXTENSION_H
