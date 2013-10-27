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
#include "languagemanager.h"

#include "boost/bind.hpp"
#include "languageculture.h"
#include "languagefolder.h"
#include "languagefolderlistener.h"
#include "languageresult.h"
#include "log.h"
#include "platformmanager.h"
#include "timerperformance.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String LanguageManager::LANGUAGES("languages");
const String LanguageManager::LANGUAGE("language");
const String LanguageManager::ID("id");
const String LanguageManager::NAME("name");
const String LanguageManager::PARENT("parent");

//////////////////////////////////////////////////////////////////////

LanguageManager::LanguageManager() : m_invariant(OS_NEW LanguageCulture()),
									 m_root(OS_NEW LanguageFolder())
{
	addFolder(m_root);
}

LanguageManager::~LanguageManager()
{

}

bool LanguageManager::hasCulture(const String &id) const
{
	return m_cultures.find(algorithms::to_lower_copy(id)) != m_cultures.end();
}

bool LanguageManager::hasCultureNotNeutral(const String &id) const
{
	shared_ptr<LanguageCulture> culture = getCulture(id);
	if(culture != null)
		return !culture->isNeutralCulture();

	return false;
}

shared_ptr<LanguageCulture> LanguageManager::getCulture(const String &id) const
{
	Cultures::const_iterator i = m_cultures.find(String(id).to_lower());
	if(i == m_cultures.end())
		return null;

	return i->second;
}

void LanguageManager::clear()
{
	m_cultures.clear();
}

String LanguageManager::getText(const String &culture, const LanguageResult &result) const
{	
	String text = getText(culture, String(result.getId()));

	// CLODOURGENT, qua devo scorrere i parametri di 'result' e fare replace di {@key}

	return text;
}

String LanguageManager::getText(const String &culture, const String &name) const
{
	if(name.substr(0,1) == _S("~"))
		return name;

	String value = findText(culture, name, false);

	if(value.empty())
	{
		notifyUnknownKey(culture, name);				
		return "~" + name + "~";
	}
	else
	{
		notifyKey(culture, name, value);

		#ifdef OS_DEBUG
			return _S("{") + value + _S("}");		// Identifica visivamente le voci tradotte rispetto a quelle scritte dirette nel codice
		#else	
			return value;
		#endif		
	}
}

String LanguageManager::findText(const String &culture, const String &name, bool searchingInAlternative) const
{
	OS_ASSERT(hasCulture(culture));

	String value;

	const Folders &folders = getFolders();
	for(Folders::const_iterator i = folders.begin(); i != folders.end(); ++i)
	{
		value = (*i)->findText(culture, name, false);
		if(value.empty() == false)
			break;
	}

	if( (searchingInAlternative == false) && (value.empty() == true) )
		value = findText(getAlternative(), name, true);

	return value;	
}

String LanguageManager::localize(const String &id) const
{
	shared_ptr<LanguageCulture> culture = getCulture(id);	
	if(culture != null)
	{
		if(culture->isNeutralCulture())
			return culture->getDefaultNotNeutral();
		else
			return id;
	}

	return String::EMPTY;
}

void LanguageManager::loadCultures(const String &path)
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(document->parseFile(path) == false)
		return;

	shared_ptr<XMLNode> cultures = document->getRoot();
	if(cultures == null)
		return;

	OS_ASSERT(cultures->getAttributeString(ID) == "inv");
	m_invariant->parse(cultures);

	// Eccezione: la lingua inglese ha le traduzioni in quanto sono quelle dell'invariant.
	getCulture("en")->setTranslationAvaible(true);
}

void LanguageManager::addListener(shared_ptr<LanguageFolderListener> listener)
{
	if(listener == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	boost::mutex::scoped_lock lock(m_cs);

	OS_ASSERT(m_listeners.find(listener) == m_listeners.end());
	m_listeners.insert(listener);
}

void LanguageManager::removeListener(shared_ptr<LanguageFolderListener> listener)
{
	boost::mutex::scoped_lock lock(m_cs);

	Listeners::iterator i = m_listeners.find(listener);
	if(i == m_listeners.end())
	{
		OS_ASSERTFALSE();
		return;
	}

	m_listeners.erase(i);
}


void LanguageManager::notifyKey(const String &culture, const String &name, const String &value) const
{
	{
		boost::mutex::scoped_lock lock(m_cs);
		utils::for_each(m_listeners, boost::bind(&LanguageFolderListener::handleKey, _1, culture, name, value));
	}
}

void LanguageManager::notifyUnknownKey(const String &culture, const String &name) const
{
	{
		boost::mutex::scoped_lock lock(m_cs);
		utils::for_each(m_listeners, boost::bind(&LanguageFolderListener::handleUnknownKey, _1, culture, name));	
	}	
}

void LanguageManager::addFolder(shared_ptr<LanguageFolder> folder)
{
	m_folders.push_front(folder);
}

shared_ptr<LanguageFolder> LanguageManager::addFolder() 
{
	shared_ptr<LanguageFolder> folder(OS_NEW LanguageFolder());
	addFolder(folder);
	return folder;
}

void LanguageManager::removeFolder(shared_ptr<LanguageFolder> folder)
{
	m_folders.remove(folder);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
