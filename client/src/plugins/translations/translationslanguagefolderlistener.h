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

#ifndef _OS_TRANSLATIONS_TRANSLATIONSLANGUAGEFOLDERLISTENER_H
#define _OS_TRANSLATIONS_TRANSLATIONSLANGUAGEFOLDERLISTENER_H

#include "boost/thread/mutex.hpp"
#include "enable_this_ptr.h"
#include "languagefolderlistener.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class TranslationsLanguageFolderListener : public LanguageFolderListener,
										   public enable_this_ptr<TranslationsLanguageFolderListener>
{
	typedef unordered_map<std::wstring, bool>::type Keys;

// Construction
private:
	TranslationsLanguageFolderListener();

public:
	virtual ~TranslationsLanguageFolderListener();

// Interface
public:
	virtual void handleKey(const String &culture, const String &name, const String &value);
	virtual void handleUnknownKey(const String &culture, const String &name);

// Operations
public:
	static void install();
	static void uninstall(bool report = true);

private:
	void trackKey(const String &name, bool known);
	void reportKeys();
	void reportKeys(const Keys &keys);

private:
	static shared_ptr<TranslationsLanguageFolderListener> g_instance;

private:
	boost::mutex m_cs;
	Keys m_keys;	
	boost::posix_time::ptime m_clock;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_TRANSLATIONS_TRANSLATIONSLANGUAGEFOLDERLISTENER_H
