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

#ifndef _IOMLCODE_H
#define _IOMLCODE_H

#include "base/object.h"
#include "ideide.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class OMLContext;
class OMLItem;

//////////////////////////////////////////////////////////////////////

class EngineExport IOMLCode : public Object
{
// Construction
public:
	IOMLCode(const String& tag);
	virtual ~IOMLCode();

// Attributes
public:
	inline String getTag() const;

// Operations
public:
	static String extractProtocol(const String &url);
	//String extractObjectId(const String &url) const;

	void mapDefaultParamTo(shared_ptr<OMLItem> i, const String &paramName) const;
	void removeTag(shared_ptr<OMLItem> i, const String &tag, const bool recursive) const;
	bool allowedParentTags(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String &tags) const;
	void allowedChildsTags(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String &tags) const;
	void removeTextSpacer2(shared_ptr<OMLItem> i, const bool recursive, const bool alsoBR) const;
	void removeBR(shared_ptr<OMLItem> i) const;
	void removeChilds(shared_ptr<OMLItem> i) const;

	static String getText(shared_ptr<OMLContext> context, const String &id);

	// Funzioni per controllare il testo da renderizzare, per evitare injection

	// Pulizia/controllo URL
	static String cleanUrl(shared_ptr<OMLContext> context, const String& url);
	// Encoding html estesa
	static String encode(shared_ptr<OMLContext> context, const String& text);
	// Da chiamare se la destinazione è un parametro di una funzione javascript
	static String encodeToJavascriptString(shared_ptr<OMLContext> context, const String& text);
	// Da chiamare se la destinazione è un'unità di misura
	static String encodeToMeasure(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& text);
	// Da chiamare se la destinazione è in un attributo style
	static String encodeToCss(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& text);
	// Da chiamare se la destinazione è un valore di un attributo html
	static String encodeToAttribute(shared_ptr<OMLContext> context, const String& text);
	// Da chiamare se la destinazione sono dei parametri GET di un'url
	static String encodeToUrlGet(shared_ptr<OMLContext> context, const String& text);
	// Da chiamare se la destinazione è come testo tra tags
	static String encodeBody(shared_ptr<OMLContext> context, const String& text, bool postProcess, bool preserveStartLineSpaces = false, bool convertCR = true);
	// Da chiamare se il sorgente è un url ad un'immagine, video, risorsa osiris://|file|
	static String encodeResourceUrl(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href);
	static String encodeResourceUrlEx(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href, String& entityID);
	// Da chiamare se il sorgente è un url
	//static String encodeUrl(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href, OMLRenderUrlDestination& destination, bool onlyExternalWithoutConfirm);
	static String encodeUrl(shared_ptr<OMLContext> context, shared_ptr<OMLItem> i, const String& href, OMLRenderUrlDestination destination, bool onlyExternalWithoutConfirm);
	// Da chiamare se il sorgente è un sotto-oml
	static String encodeOML(shared_ptr<OMLContext> context, const String& text);

	

	virtual String processOsml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const;
// Interface
public:

	virtual bool allowRowMode() const;
	virtual String processHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context) const = 0;

// Private:
	String m_tag;
};

//////////////////////////////////////////////////////////////////////

inline String IOMLCode::getTag() const { return m_tag; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IOMLCODE_H
