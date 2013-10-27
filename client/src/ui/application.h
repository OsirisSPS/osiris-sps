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

#ifndef _OS_UI_APPLICATION_H
#define _OS_UI_APPLICATION_H

#include "boost/signals/trackable.hpp"
#include "boost/thread/recursive_mutex.hpp"
#include "iapplication.h"
#include "singleton.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Logger;
class LogMessage;
class OsirisLink;

//////////////////////////////////////////////////////////////////////

class Application : public wxApp,
					public DynamicSingleton<Application>,
					public IApplication,
					public boost::signals::trackable
{
	DECLARE_EVENT_TABLE()
	
	typedef IApplication ApplicationBase;

// Construction
public:
	Application();
	virtual ~Application();

// Attributes
public:
	String getRootPath() const;
	String getResourcesPath() const;
	String getResourcesFilePath(const String &filename) const;
	String getTemplatesPath() const;

	String getText(const String &key) const;

	String getLocalUrl(const String &relativeUrl) const;

private:
	bool getExiting() const;
	void setExiting(bool exiting);	

	String getShellOpenCommand(bool withFile) const;

// Operations
public:
	bool checkAutostart();
	void setupAutostart(bool autoStartEnabled);	
	void setupFilesAssociations(bool enabled, bool verbose);	
	void setupProtocolAssociations(bool enabled, bool verbose);

	void reloadExtensions();
	void viewHome();
	void openLocalUrl(const String &relativePath);
	void openUrl(const String &url);
	void shell(const String &path);

	void processFile(const String &filename, bool verbose);
	void processLink(const OsirisLink &link, bool verbose);

	void reportMessage(const String &message, bool verbose = true);
	void reportError(const String &error, bool verbose = true);

	void dispatchEvents();

	//void validateOptions();
	bool getAutoStart();
	bool getAssociationsProtocols();
	bool getAssociationsFiles();

// IApplication interface
protected:
	virtual void handleProcessCommands(const std::string &commands);

private:
	void flushLogs();
	void log(shared_ptr<LogMessage> message);
	void peekLogs(list<shared_ptr<LogMessage> >::type &logs);
	void postFlushLogsEvent();
	
	bool registerFilesAssociations(bool associate);

	boost::optional<bool> checkProtocolRegistered();
	boost::optional<bool> checkFilesRegistered();
	bool registerProtocol();
	
	void initLibraries();
	void initMimeTypesManager();
					
	void processCommand(const std::string &command, bool verbose);

// Events
private:
	void onConfigureOptions();
	void onFlushLogs(wxCommandEvent &e);
	void onHandleProcessData(wxCommandEvent &e);

// wxApp overrides
public:
    virtual bool OnInit();
	virtual int OnExit();
	virtual void OnUnhandledException();
	virtual bool OnExceptionInMainLoop();

	virtual void OnInitCmdLine(wxCmdLineParser &parser);
	virtual bool OnCmdLineParsed(wxCmdLineParser &parser);

protected:
	bool m_configureOptions;
	bool m_startBackground;
	bool m_exiting;
	bool m_mimeTypesManagerInitialized;
	list<shared_ptr<LogMessage> >::type m_logs;
	mutable boost::recursive_mutex m_cs;
	shared_ptr<Logger> m_logger;	
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_APPLICATION_H
