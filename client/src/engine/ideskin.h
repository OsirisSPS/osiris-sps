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

#ifndef _IDE_SKIN_H
#define _IDE_SKIN_H

#include "base/object.h"
#include "collections.h"
#include "constants.h"
#include "ids.h"
#include "path.h"
#include "version.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

//#define OS_SKINS_PATH_ROOT							_S("skins")
#define OS_SKIN_PATH_ICONS							_S("icons")
#define OS_SKIN_PATH_IMAGES							_S("images")
#define OS_SKIN_PATH_TEMPLATES						_S("templates")
#define OS_SKIN_PATH_SCRIPTS						_S("scripts")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlPage;
class LanguageFolder;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeSkin :	public Object,
								public enable_this_ptr<IdeSkin>
{
public:
	
// Construction
public:
	IdeSkin();
	virtual ~IdeSkin();

// Attributes
public:
	inline bool isValid() const;

	inline const SkinID & getID() const;
	inline const String & getName() const;
	inline const String & getDescription() const;
	inline const String & getPreview() const;
	inline bool hasPreview() const;
	
	inline const StringList & getStyles() const;
	inline const StringList & getScripts() const;
	
	uint32 getPagerPages() const;
	uint32 getPagerItems() const;
	
// Operations
public:
	String getFaviconUrl() const;
	
	// Restituisce il path della risorsa richiesta
	String getResourcePath(const String &resource) const;
	// Restituisce l'url della risorsa richiesta
	String getResourceUrl(const String &resource) const;

	// Restituisce l'url dell'immagine richiesta
	String getImageUrl(const String &filename) const;

	// Restituisce il path del template richiesto
	String getTemplatePath(const String &filename) const;

	bool read(shared_ptr<XMLDocument> document);
		
	bool loadXml(const String &path);
	void unload();

	// Applica la skin ad una pagina
	virtual void apply(shared_ptr<HtmlPage> page);	

private:
	void _clear();

	bool _parseAttributes(const shared_ptr<XMLNode> &node);
	bool _parseStyles(const shared_ptr<XMLNode> &styles);
	bool _parseStyle(const shared_ptr<XMLNode> &style);
	bool _parseScripts(const shared_ptr<XMLNode> &scripts);
	bool _parseScript(const shared_ptr<XMLNode> &script);

protected:
	String _getSchemaPath();
	virtual String findResource(String resource, bool httpPath) const;		

protected:
	SkinID m_id;									// ID univoco della skin
	bool m_valid;									// flag di stato
	String m_name;									// nome della skin
	String m_description;							// descrizione della skin
	String m_preview;								// preview della skin	
	String m_version;								// versione
	String m_author;								// autore
	Version m_compatibility;						// versione minima di compatibilit
	StringList m_styles;	// css da applicare allo schema
	StringList m_scripts;	// scripts da applicare allo schema

private:
	static const String ID;
	static const String NAME;
	static const String DESCRIPTION;
	static const String PREVIEW;
	static const String VERSION;
	static const String AUTHOR;
	static const String COMPATIBILITY;
	static const String DEFAULT;

	static const String STYLES;
	static const String STYLE;
	static const String CSS;
	static const String SCRIPTS;
	static const String SCRIPT;
	static const String JS;
};

//////////////////////////////////////////////////////////////////////

inline bool IdeSkin::isValid() const { return m_valid; }

inline const SkinID & IdeSkin::getID() const { return m_id; }
inline const String & IdeSkin::getName() const { return m_name; }
inline const String & IdeSkin::getDescription() const { return m_description; }
inline const String & IdeSkin::getPreview() const { return m_preview; }
inline bool IdeSkin::hasPreview() const { return m_preview.empty() == false; }
inline const StringList & IdeSkin::getStyles() const { return m_styles; }
inline const StringList & IdeSkin::getScripts() const { return m_scripts; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_SKIN_H
