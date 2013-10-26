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
#include "translationslanguagefolderlistener.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "engine.h"
#include "httpclient.h"
#include "httprequest.h"
#include "httpurl.h"
#include "ibackgroundjob.h"
#include "languagemanager.h"
#include "log.h"
#include "options.h"
#include "translations.h"
#include "utils.h"

//////////////////////////////////////////////////////////////////////

#define OS_TRANSLATIONS_KEYS_SEPARATOR				_W(";")

#define OS_TRANSLATIONS_PARAM_PASSWORD				_W("password")
#define OS_TRANSLATIONS_PARAM_KEYS_KNOWN			_W("keys_known")
#define OS_TRANSLATIONS_PARAM_KEYS_UNKNOWN			_W("keys_unknown")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

shared_ptr<TranslationsLanguageFolderListener> TranslationsLanguageFolderListener::g_instance;

//////////////////////////////////////////////////////////////////////

TranslationsLanguageFolderListener::TranslationsLanguageFolderListener() : m_clock(boost::date_time::not_a_date_time)
{

}

TranslationsLanguageFolderListener::~TranslationsLanguageFolderListener()
{
	
}

void TranslationsLanguageFolderListener::handleKey(const String &culture, const String &name, const String &value)
{
	trackKey(name, true);
}

void TranslationsLanguageFolderListener::handleUnknownKey(const String &culture, const String &name)
{
	OS_LOG_DEBUG("Unknown language key '" + name + "'");
	
	trackKey(name, false);
}

void TranslationsLanguageFolderListener::trackKey(const String &name, bool known)
{
	boost::mutex::scoped_lock lock(m_cs);
	
	OS_ASSERT(name.find(OS_TRANSLATIONS_KEYS_SEPARATOR) == String::npos);
	m_keys[name.to_wide()] = known;

	uint32 reportDelay = Options::instance()->getOption<uint32>(OS_TRANSLATIONS_OPTION_REPORT_DELAY);
	if(reportDelay > 0)
	{
		boost::posix_time::ptime now = utils::utc_time();
		if(m_clock.is_not_a_date_time())
		{
			m_clock = now;
		}
		else if((now - m_clock) >= boost::posix_time::millisec(reportDelay))
		{
			m_clock = now;
			Engine::instance()->addBackgroundJob(shared_ptr<IBackgroundJob>(OS_NEW BackgroundJobCallback<>(L"translator reporter", false, Engine::instance()->peekBackgroundJobID(), boost::bind(&TranslationsLanguageFolderListener::reportKeys, get_this_ptr()))));
		}
	}
}

void TranslationsLanguageFolderListener::install()
{
	if(g_instance != null)
	{
		OS_ASSERTFALSE();
		return;
	}	

	g_instance.reset(OS_NEW TranslationsLanguageFolderListener());
	LanguageManager::instance()->addListener(g_instance);
}

void TranslationsLanguageFolderListener::uninstall(bool report)
{
	if(g_instance == null)
	{
		OS_ASSERTFALSE();
		return;
	}	

	LanguageManager::instance()->removeListener(g_instance);

	g_instance->reportKeys();
	g_instance.reset();
}

void TranslationsLanguageFolderListener::reportKeys()
{
	boost::mutex::scoped_lock lock(m_cs);

	if(m_keys.empty())
		return;
	
	reportKeys(m_keys);
	m_keys.clear();
}

void TranslationsLanguageFolderListener::reportKeys(const Keys &keys)
{
	OS_ASSERT(keys.empty() == false);		

	bool enabled = Options::instance()->getOption<bool>(OS_TRANSLATIONS_OPTION_REPORT_ENABLED);
	if(enabled == false)
		return;

	String url = Options::instance()->getOption<String>(OS_TRANSLATIONS_OPTION_URL);
	if(url.empty())
	{
		OS_LOG_WARNING("Cannot report language keys statistics: empty url");		
		return;
	}

	String password = Options::instance()->getOption<String>(OS_TRANSLATIONS_OPTION_PASSWORD);
	if(password.empty())
	{
		OS_LOG_WARNING("Cannot report language keys statistics: empty password");		
		return;
	}	

	std::wstring keysKnown;
	std::wstring keysUnknown;

	for(Keys::const_iterator i = keys.begin(); i != keys.end(); ++i)
	{
		std::wstring key = i->first;
		bool known = i->second;
		if(known)
			keysKnown = algorithms::combine<std::wstring>(keysKnown, key, OS_TRANSLATIONS_KEYS_SEPARATOR);
		else
			keysUnknown = algorithms::combine<std::wstring>(keysUnknown, key, OS_TRANSLATIONS_KEYS_SEPARATOR);
	}

	OS_ASSERT(keysKnown.empty() == false || keysUnknown.empty() == false);

	shared_ptr<boost::asio::io_service> service = createAsioService();
	if(service == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<TCPSocket> socket = Engine::instance()->createTCPSocket(service, true, true);
	if(socket == null)
	{
		OS_ASSERTFALSE();
		return;
	}

	shared_ptr<HttpClient> client(OS_NEW HttpClient(service, socket));
	shared_ptr<HttpRequest> request = client->getRequest();
	OS_ASSERT(request != null);	

	request->setMethod(httpMethodPost);
	
	request->setPostParamString(OS_TRANSLATIONS_PARAM_PASSWORD, password.to_wide());

	if(keysKnown.empty() == false)
		request->setPostParamString(OS_TRANSLATIONS_PARAM_KEYS_KNOWN, keysKnown);

	if(keysUnknown.empty() == false)
		request->setPostParamString(OS_TRANSLATIONS_PARAM_KEYS_UNKNOWN, keysUnknown);
	
	if(client->perform(url.to_ascii()))
		OS_LOG_DEBUG("Language keys statistics's reported");	
	else
		OS_LOG_WARNING("Error reporting language keys statistics to '" + url + "'");	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
