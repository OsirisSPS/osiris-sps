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
#include "languagefolder.h"

#include "constants.h"
#include "filesystem.h"
#include "languageculture.h"
#include "languagefolderlistener.h"
#include "languagemanager.h"
#include "languagevocabolary.h"
#include "log.h"
#include "utils.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const String LanguageFolder::UNKNOWNTEXT = _S("{unknown_text}");

//////////////////////////////////////////////////////////////////////

LanguageFolder::LanguageFolder()
{

}

LanguageFolder::~LanguageFolder()
{
	clear();
}

void LanguageFolder::clear()
{
	m_vocabolaries.clear();
}

void LanguageFolder::addPath(const String &path)
{
	addPath("inv", path);
}

void LanguageFolder::addPath(const String &culture, const String &path)
{
	shared_ptr<LanguageCulture> currentCulture = LanguageManager::instance()->getCulture(culture);
	if(currentCulture == null)
		return;

	String sPath = utils::makeFilePath(path, culture + _S(".xml"));

	if(FileSystem::instance()->fileExists(sPath))
	{
		Vocabolaries::const_iterator i = m_vocabolaries.find(culture);
		if(i == m_vocabolaries.end())
		{
			std::pair<Vocabolaries::iterator, bool> result = m_vocabolaries.insert(Vocabolaries::value_type(culture, shared_ptr<LanguageVocabolary>(OS_NEW LanguageVocabolary())));
			if(result.second == false)
				return;

			i = result.first;
		}

		currentCulture->setTranslationAvaible(true);

		i->second->add(sPath);
	}

	const LanguageCulture::Cultures &cultures = currentCulture->getCultures();
	for(LanguageCulture::Cultures::const_iterator i = cultures.begin(); i != cultures.end(); ++i)
	{
		addPath(i->second->getID(), path);
	}
}

String LanguageFolder::findText(const String &culture, const String &name, bool searchingInAlternative) const
{
	// Lo cerco nella cultura specificata
	Vocabolaries::const_iterator i = m_vocabolaries.find(culture);
	if(i != m_vocabolaries.end())
	{
		const String &value = i->second->getText(name);
		if(value.empty() == false)
			return value;		
	}

	if(searchingInAlternative == false)
	{
		// Lo cerco nella cultura padre
		shared_ptr<LanguageCulture> currentCulture = LanguageManager::instance()->getCulture(culture);
		if(currentCulture != null)
		{
			shared_ptr<LanguageCulture> parentCulture = currentCulture->getParent();
			if(parentCulture != null)
				return findText(parentCulture->getID(), name, false);			
		}
	}

	return String::EMPTY;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
