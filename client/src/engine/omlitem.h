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

#ifndef _IDE_BBCODES_ITEM_H
#define _IDE_BBCODES_ITEM_H

#include "base/object.h"
#include "collections.h"
#include "enable_this_ptr.h"
#include "ideide.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class OMLContext;

//////////////////////////////////////////////////////////////////////

class EngineExport OMLItem : public enable_this_ptr<OMLItem>,
									public Object
{
public:
	typedef list<shared_ptr<OMLItem> >::type Items;

// Construction
public:
	OMLItem();
	virtual ~OMLItem();

// Attributes
	//inline const bool isTag() const;
	//inline const bool isTextSpacer() const;
	inline shared_ptr<OMLItem> getParent() const;
	inline const String getTagName() const;
	
	inline size_t getStartPos() const;
	inline void setEndPos(size_t pos);
	inline size_t getEndPos() const;
	inline const Items & getChilds() const;

	inline bool getPostProcess() const;
	inline void setPostProcess(bool postProcess);
	inline bool getDirectClose() const;
	inline bool getSkipped() const;
	inline void setSkipped(bool val);

	//inline const String getParam() const;
	inline const String getParam(const String &name) const;
	inline bool hasParam(const String &name) const;
	inline bool hasParams() const;
	inline const StringCollection<String>& getParams() const;
	void setParam(const String &name, const String& value);
	void removeParam(const String &name);

// Operations
	void add(shared_ptr<OMLItem> i);
	//void clear();
	//void setText(const String &text);
	String getSingleText();
	String getHtml(shared_ptr<OMLContext> context);
	String getOsml(shared_ptr<OMLContext> context);

	String getHtmlChildsPlainWithAttributes(shared_ptr<OMLContext> context);
	String getHtmlChilds(shared_ptr<OMLContext> context);
	String getOsmlChilds(shared_ptr<OMLContext> context);

	std::wstring::size_type extract(const std::wstring& text, const std::wstring& textLower, std::wstring::size_type posStart, std::wstring::size_type posEnd = std::wstring::npos);
	std::wstring::size_type extract2(const std::wstring& text, const std::wstring& textLower, std::wstring::size_type posStart, std::wstring::size_type posEnd = std::wstring::npos);

// Internal
private:

	weak_ptr<OMLItem> m_parent;
	String m_tagName;
	//String m_text;
	bool m_skipped;
	bool m_postProcess;
	bool m_directClose;
	StringCollection<String> m_params;
	//Params m_params;

	size_t m_startPos;	// Posizione di inizio nel context->m_input, non comprende il tag di apertura.
	size_t m_endPos;	// Posizione di fine nel context->m_input, comprende figli ma non il tag di chiusura.

	Items m_childs;
	shared_ptr<OMLItem> m_lastAdded;	// L'ultimo item aggiunto

	void _extractSetParam(std::wstring name, std::wstring value);
	void _extractCheckClosed(const std::wstring& text, std::wstring::size_type posClose);

public:
	static const String ITEM_TAG_TEXT;
	static const String ITEM_TAG_PARAM;
	//static const String ITEM_TAG_NULL;
	static const String ITEM_PARAM_TEXT;
	static const String ITEM_PARAM_DEFAULT;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<OMLItem> OMLItem::getParent() const { return m_parent.lock(); }
inline const String OMLItem::getTagName() const { return m_tagName; }

inline size_t OMLItem::getStartPos() const { return m_startPos; }
inline void OMLItem::setEndPos(size_t pos) { m_endPos = pos; }
inline size_t OMLItem::getEndPos() const { return m_endPos; }
inline const OMLItem::Items & OMLItem::getChilds() const { return m_childs; }

inline bool OMLItem::getPostProcess() const { return m_postProcess; }
inline void OMLItem::setPostProcess(bool postProcess) { m_postProcess = postProcess; }
inline bool OMLItem::getDirectClose() const { return m_directClose; }
inline bool OMLItem::getSkipped() const { return m_skipped; }
inline void OMLItem::setSkipped(bool val) { m_skipped = val; }

//inline const String OMLItem::getParam() const { return m_params.get(OMLItem::ITEM_PARAM_DEFAULT); }
inline const String OMLItem::getParam(const String &name) const { return m_params.get(name); }
inline bool OMLItem::hasParam(const String &name) const { return m_params.get(name) != String::EMPTY; }
inline bool OMLItem::hasParams() const { return m_params.size() != 0; }
inline const StringCollection<String>& OMLItem::getParams() const { return m_params; }



//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_BBCODES_ITEM_H
