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

#ifndef _OS_LANG_LANGUAGEMANAGER_H
#define _OS_LANG_LANGUAGEMANAGER_H

#include "singleton.h"
#include "boost/thread/mutex.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class LanguageCulture;
class LanguageFolder;
class LanguageFolderListener;
class LanguageResult;

//////////////////////////////////////////////////////////////////////

class LangExport LanguageManager : public StaticSingleton<LanguageManager>
{
public:	
	typedef unordered_map<String, shared_ptr<LanguageCulture> >::type Cultures;
	typedef list<shared_ptr<LanguageFolder> >::type Folders;
	typedef unordered_set<shared_ptr<LanguageFolderListener> >::type Listeners;

// Construction
public:
	LanguageManager();
	virtual ~LanguageManager();

// Attributes
public:
	bool hasCulture(const String &id) const;
	bool hasCultureNotNeutral(const String &id) const;
	
	shared_ptr<LanguageCulture> getCulture(const String &id) const;
	
	inline String getAlternative() const;
	inline void setAlternative(const String &alt);

	inline shared_ptr<LanguageCulture> getInvariant() const;
	inline shared_ptr<LanguageFolder> getRootFolder() const;

	inline Folders getFolders() const;

	inline Cultures & getCultures();

// Operations
public:
	void clear();

	String getText(const String &culture, const String &name) const;
	String getText(const String &culture, const LanguageResult &result) const;

	// Restituisce la cultura non-neutrale raccomandata per quella specificata. Ad esempio specificando "it" viene restituita "it-IT".
	String localize(const String &culture) const;

	void loadCultures(const String &path);

	void addListener(shared_ptr<LanguageFolderListener> listener);
	void removeListener(shared_ptr<LanguageFolderListener> listener);

	void notifyKey(const String &culture, const String &name, const String &value) const;
	void notifyUnknownKey(const String &culture, const String &name) const;

	void addFolder(shared_ptr<LanguageFolder>);
	shared_ptr<LanguageFolder> addFolder();
	void removeFolder(shared_ptr<LanguageFolder> folder);

private:
	String findText(const String &culture, const String &name, bool searchingInAlternative) const;

protected:
	Cultures m_cultures;
	shared_ptr<LanguageCulture> m_invariant;
	shared_ptr<LanguageFolder> m_root;
	Folders m_folders;
	Listeners m_listeners;
	mutable boost::mutex m_cs;
	String m_alternative;

protected:
	static const String LANGUAGES;
	static const String LANGUAGE;
	static const String ID;
	static const String NAME;
	static const String PARENT;
};

//////////////////////////////////////////////////////////////////////

inline String LanguageManager::getAlternative() const { return m_alternative; }
inline void LanguageManager::setAlternative(const String &alt) { m_alternative = alt; }

inline shared_ptr<LanguageCulture> LanguageManager::getInvariant() const { return m_invariant; }
inline shared_ptr<LanguageFolder> LanguageManager::getRootFolder() const { return m_root; }
inline LanguageManager::Folders LanguageManager::getFolders() const { return m_folders; }

inline LanguageManager::Cultures & LanguageManager::getCultures() { return m_cultures; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_LANG_LANGUAGEMANAGER_H
