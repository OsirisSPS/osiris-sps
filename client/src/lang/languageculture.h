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

#ifndef _OS_LANG_LANGUAGECULTURE_H
#define _OS_LANG_LANGUAGECULTURE_H

#include "base/object.h"
#include "datetime.h"
#include "enable_this_ptr.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LanguageFolder;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class LangExport LanguageCulture : public enable_this_ptr<LanguageCulture>,
								   public Object
{
public:
	typedef unordered_map<String, shared_ptr<LanguageCulture> >::type Cultures;

// Construction
public:
	LanguageCulture(shared_ptr<LanguageCulture> parent = nullptr);
	virtual ~LanguageCulture();

// Attributes
public:
	inline const String & getID() const;
	inline const String & getEnglishName() const;
	inline const String & getNativeName() const;
	inline bool isNeutralCulture() const;
	inline const String & getDefaultNotNeutral() const;
	inline const String & getIsoName() const;
	inline uint32 getLCID() const;

	inline int32 getDefaultTimeOffset() const;

	inline bool getTranslationAvaible() const;
	inline void setTranslationAvaible(bool value);

	inline shared_ptr<LanguageCulture> getParent() const;
	inline Cultures & getCultures();

	bool getTranslationDerivedAvaible();
	bool isParentOf(const String &id);

// Operations
public:
	shared_ptr<LanguageCulture> addCulture(const String &id);
	shared_ptr<LanguageCulture> getCulture(const String &id) const;

	void parse(shared_ptr<XMLNode> node);

	String formatDate(const DateTime &dt, const DateTime::DateTimePattern &pattern, int32 timeOffset, int32 timeDST) const;
	String formatDate(const DateTime &dt, const String &pattern, int32 timeOffset, int32 timeDST) const;
	String formatTime(double time) const;

protected:
	weak_ptr<LanguageCulture> m_parent;

	String m_id;
	String m_englishName;
	String m_nativeName;
	bool m_isNeutralCulture;
	String m_defaultNotNeutral;
	String m_isoName;
	uint32 m_lcid;

	bool m_translationAvaible;
	int32 m_defaultTimeOffset;

	String m_dayNamesLong[7];
	String m_dayNamesShort[7];
	String m_monthNamesLong[12];
	String m_monthNamesShort[12];
	String m_pmDesignator;
	String m_amDesignator;

	String m_longDatePattern;
    String m_shortDatePattern;
    String m_longTimePattern;
    String m_shortTimePattern;

	Cultures m_cultures;
};

//////////////////////////////////////////////////////////////////////

inline const String & LanguageCulture::getID() const { return m_id; }
inline const String & LanguageCulture::getEnglishName() const { return m_englishName; }
inline const String & LanguageCulture::getNativeName() const { return m_nativeName; }
inline bool LanguageCulture::isNeutralCulture() const { return m_isNeutralCulture; }
inline const String & LanguageCulture::getDefaultNotNeutral() const { return m_defaultNotNeutral; }
inline const String & LanguageCulture::getIsoName() const { return m_isoName; }
inline uint32 LanguageCulture::getLCID() const { return m_lcid; }

inline int32 LanguageCulture::getDefaultTimeOffset() const { return m_defaultTimeOffset; }

inline bool LanguageCulture::getTranslationAvaible() const { return m_translationAvaible; }
inline void LanguageCulture::setTranslationAvaible(bool value) { m_translationAvaible = value; }

inline shared_ptr<LanguageCulture> LanguageCulture::getParent() const { return m_parent.lock(); }
inline LanguageCulture::Cultures & LanguageCulture::getCultures() { return m_cultures; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OSIRIS_LANG_LANGUAGECULTURE_H
