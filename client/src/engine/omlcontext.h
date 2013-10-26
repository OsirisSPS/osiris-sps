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

#ifndef _IDE_BBCODES_CONTEXT_H
#define _IDE_BBCODES_CONTEXT_H

#include "base/object.h"
#include "collections.h"
#include "enable_this_ptr.h"
#include "ideide.h"
#include "portalsportals.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class OMLItem;
class IPage;
class IPortalPage;

//////////////////////////////////////////////////////////////////////

class EngineExport OMLContext : public enable_this_ptr<OMLContext>,
										public Object
{
public:
	typedef list<String>::type Warnings;
	typedef list<String>::type Urls;

// Construction
public:
	OMLContext(shared_ptr<IPage> page, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl);
	virtual ~OMLContext();

// Attributes
public:
	const String getContextID() const;	// Da usare quando serve un'ID univoco a livello di contesto
	const String getContextNextID(); // Da usare quando serve un'ID univoco solo a livello di tag	
	inline const String & getOutput() const;
	inline void setOutput(const String& html);
    inline const Warnings & getWarnings() const;
	inline const Urls & getUrls() const;

	//inline const OutputMode getOutputMode() const;
	shared_ptr<IPage> getPage() const;
	shared_ptr<IPortalPage> getPortalPage() const;

	inline bool getFull() const;
	inline void setFull(bool full);
	inline bool getSecure() const;
	inline void setSecure(bool secure);
	inline bool getRow() const;
	inline void setRow(bool row);
	inline OMLRenderMode getMode() const;
	inline void setMode(OMLRenderMode mode);

	inline String getRefID() const;
	inline void setRefID(String refID);

	inline String getFullUrl() const;
	inline void setFullUrl(String fullUrl);

	inline bool getSecureCheck() const;
	inline void setSecureCheck(bool secure);

	// Identifica se deve essere mostrato il link "Show More",
	// ovvero se sono stati incontrati tag che definiscono visualizzazioni diverse tra Lite & Full.
	inline bool getShowMore() const;
	inline void setShowMore(bool showMore);

    String getReport(const String& text, bool detailed) const;

// Operations
public:

	void parseOml(const String& oml);
	void parseCss(const String& cssText, bool encode);

	String getFriendlyName(shared_ptr<OMLItem> i) const;
	void addWarning(const String& msg);
	void addWarningSyntax();
	void addWarningCantHaveParams(shared_ptr<OMLItem> i);
	void addWarningParamRequired(shared_ptr<OMLItem> i);
	void addWarningMustBeInside(shared_ptr<OMLItem> i, const String &parentTag);
	void addWarningCantBeHere(shared_ptr<OMLItem> i, shared_ptr<OMLItem> ref);
	void addWarningInvalidUrl(shared_ptr<OMLItem> i, const String &url);
	void addWarningInvalidParam(shared_ptr<OMLItem> i, const String &name);
	void addWarningRowModeNotAllowed(shared_ptr<OMLItem> i);
	bool incrementAndCheckRecursive();
	void decrementRecursive();

	bool checkPortalPageAvailable(shared_ptr<OMLItem> i);

	String generateReportTree(shared_ptr<OMLItem> i, uint32 depth) const;

	void generateItems(shared_ptr<OMLItem> root, const std::wstring& text);
	shared_ptr<OMLItem> closeItem(shared_ptr<OMLItem> i);

	void addUrl(shared_ptr<OMLItem> i, const String &url);

// Utils:
public:
	//static String::size_type findNotEscaped(const std::wstring& text, const uchar &what, String::size_type posStart);
	static std::wstring::size_type findNotEscaped(const std::wstring& text, const std::wstring& what, String::size_type posStart);
	static std::wstring::size_type findToken(const std::wstring& text, const std::wstring& what, String::size_type posStart);
	static std::wstring::size_type findTokens(const std::wstring& text, const std::wstring& tokens, String::size_type posStart);

// Internal:
private:

// Private
	uint32 m_id;					// ID univoco del contesto
	uint32 m_subid;					// Sub-ID a livello di contesto
	//String m_input;
	String m_output;
	shared_ptr<OMLItem> m_root;

	//OutputMode m_outputMode;
	bool m_full;				// Versione normale o di riepilogo?
	bool m_secure;				// Versione sicura?
	bool m_row;					// Versione a singola riga?
	OMLRenderMode m_mode;			// "osiris" = Default, for Osiris (with os_* css, for example)
								// "search" = Plain text for search indexes
								// "external" = Html for external (without os_* css, with full-url), for example for RSS Feed.
	String m_refID;				// ID di riferimento
	String m_fullUrl;			// Link alla versione full.

	bool m_secureCheck;

	shared_ptr<IPage> m_page;

	bool m_enableWarning;
	Warnings m_warnings;
	int m_recursiveLimit;
	int m_recursiveCounter;
	
	bool m_showMore;
	Urls m_urls;
};

//////////////////////////////////////////////////////////////////////

inline const String & OMLContext::getOutput() const { return m_output; }
inline void OMLContext::setOutput(const String& html) { m_output = html; }
inline const OMLContext::Warnings & OMLContext::getWarnings() const { return m_warnings; }
inline const OMLContext::Urls & OMLContext::getUrls() const { return m_urls; }
inline bool OMLContext::getFull() const { return m_full; }
inline void OMLContext::setFull(bool full) { m_full = full; }
inline bool OMLContext::getSecure() const { return m_secure; }
inline void OMLContext::setSecure(bool secure) { m_secure = secure; }
inline bool OMLContext::getRow() const { return m_row; }
inline void OMLContext::setRow(bool row) { m_row = row; }
inline OMLRenderMode OMLContext::getMode() const { return m_mode; }
inline void OMLContext::setMode(OMLRenderMode mode) { m_mode = mode; }

inline String OMLContext::getRefID() const { return m_refID; }
inline void OMLContext::setRefID(String refID) { m_refID = refID; }

inline String OMLContext::getFullUrl() const { return m_fullUrl; }
inline void OMLContext::setFullUrl(String fullUrl) { m_fullUrl = fullUrl; }

inline bool OMLContext::getSecureCheck() const { return m_secureCheck; }
inline void OMLContext::setSecureCheck(bool secure) { m_secureCheck = secure; }

inline bool OMLContext::getShowMore() const { return m_showMore; }
inline void OMLContext::setShowMore(bool showMore) { m_showMore = showMore; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_BBCODES_CONTEXT_H
