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
#include "application.h"

#include "algorithms.h"
#include "boost/bind.hpp"
#include "boost/format.hpp"
#include "condition.h"
#include "conversions.h"
#include "engine.h"
#include "extensionssystem.h"
#include "file.h"
#include "filesystem.h"
#include "idesystem.h"
#include "idemainjobs.h"
#include "ihttpserver.h"
#include "languagefolder.h"
#include "languagemanager.h"
#include "log.h"
#include "logmessage.h"
#include "logmanager.h"
#include "logger.h"
#include "logwindow.h"
#include "mainwindow.h"
#include "mimemanager.h"
#include "notificationsmanager.h"
#include "options.h"
#include "osiriscommon.h"
#include "osirislink.h"
#include "portalsimporterjob.h"
#include "portalsserializer.h"
#include "portalsportalssystem.h"
#include "platformmanager.h"
#include "splashscreen.h"
#include "thirdpartylibrary.h"
#include "thirdpartylibrariesreporter.h"
#include "uiapi.h"
#include "uistandards.h"
#include "unicode.h"
#include "upnpserver.h"
#include "utils.h"
#include "wx/evtloop.h"

#include "omlmanager.h"

//////////////////////////////////////////////////////////////////////

#if OS_PLATFORM == OS_PLATFORM_WIN
	#include "Shlobj.h"
#endif

#if OS_PLATFORM == OS_PLATFORM_MAC
	#include <CoreFoundation/CFBundle.h>
    #include <ApplicationServices/ApplicationServices.h>    // For LSRegisterURL
#endif

//////////////////////////////////////////////////////////////////////

OS_IMPLEMENT_SINGLETON(OS_NAMESPACE_NAME::Application)

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

static const uint32 cmdFlushLogs = wxNewId();
static const uint32 cmdHandleProcessData = wxNewId();

//////////////////////////////////////////////////////////////////////

IMPLEMENT_APP(Application)

BEGIN_EVENT_TABLE(Application, wxApp)
	EVT_COMMAND(cmdFlushLogs, OS_UI_EVENT_NOTIFY, Application::onFlushLogs)
	EVT_COMMAND(cmdHandleProcessData, OS_UI_EVENT_NOTIFY, Application::onHandleProcessData)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

Application::Application() : ApplicationBase(PlatformManager::applicationGUI),
							 m_configureOptions(false),
							 m_startBackground(false),
							 m_exiting(false),
							 m_mimeTypesManagerInitialized(false)
{
	// Disabilita il log
	wxLog::EnableLogging(false);

	//Options::instance()->ensureOption<bool>(OS_SYSTEM_OPTION_REGISTER_PROTOCOL, true);
	//Options::instance()->ensureOption<bool>(OS_SYSTEM_OPTION_REGISTER_FILES_ASSOCIATIONS, true);

	// Sottoscrive l'evento di configurazione delle opzioni
	Options::instance()->subscribeInitHandler(boost::bind(&Application::onConfigureOptions, this));

	m_logger = Logger::create(boost::bind(&Application::log, this, _1));	
}

Application::~Application()
{
	m_logger.reset();
}

String Application::getRootPath() const
{
	// URGENT: viene richiamata prima che venga caricato il file system.xml...
	return utils::makeFolderPath(Options::instance()->getSharePath(), OS_UI_ROOT_PATH);
}

String Application::getResourcesPath() const
{
	return utils::makeFolderPath(getRootPath(), OS_UI_RESOURCES_PATH);
}

String Application::getResourcesFilePath(const String &filename) const
{
    String path = utils::makeFilePath(getResourcesPath(), filename);
    OS_ASSERT(FileSystem::instance()->fileExists(path));	// TODO: temporaneo per il check delle risorse
	return path;
}

String Application::getTemplatesPath() const
{
	return utils::makeFolderPath(getRootPath(), OS_UI_TEMPLATES_PATH);
}

String Application::getText(const String &key) const
{
	/*
	shared_ptr<LanguageFolder> folder = LanguageManager::instance()->getRootFolder();
	OS_ASSERT(folder != null);
	return folder->getText(Options::instance()->getDefaultLanguage(), key);
	*/
	//return LanguageManager::instance()->getText(Options::instance()->getDefaultLanguage(), key);
	return Engine::instance()->getText(key);
}

String Application::getLocalUrl(const String &relativeUrl) const
{
	return IdeSystem::instance()->getLocalUrl(relativeUrl);
}

bool Application::getExiting() const
{
	OS_LOCK(m_cs);
	return m_exiting;
}

void Application::setExiting(bool exiting)
{
	OS_LOCK(m_cs);
	m_exiting = exiting;
}

String Application::getShellOpenCommand(bool withFile) const
{
	//wxString applicationPath = argv[0];
	String applicationPath = FileSystem::instance()->getExecutableName();
	applicationPath.replace_all(_S("%"), _S("%%"));
	String command;
	command.append("\"");
	command.append(applicationPath);
	command.append("\"");
	if(withFile)
		command.append(" \"%1\"");

	return command;
}

void Application::setupAutostart(bool autoStartEnabled)
{
#if OS_PLATFORM == OS_PLATFORM_WIN

	TCHAR szPath[MAX_PATH];

	if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, TRUE)))
	{
		String path = szPath + _S("\\Osiris.lnk");
		String applicationPath = FileSystem::instance()->getExecutableName();

		if(autoStartEnabled)
		{
			if(FileSystem::instance()->exists(path) == false)
			{
				String arguments = applicationPath + _S(" /") + OS_CMDLINE_BACKGROUND;
				utils::CreateWindowsShortcut(applicationPath.c_str(), arguments.c_str(), _S("Osiris - Serverless Portal System").c_str(), path.c_str());
			}
		}
		else
		{
			if(FileSystem::instance()->exists(path) == true)
			{
				FileSystem::instance()->remove(path);
			}
		}
	}

	/*
	if(PlatformManager::instance()->isPrivilegedUser() == false)
		return;

	if(autoStartEnabled)
	{
		wxRegKey keyAutostart(conversions::from_utf16<wxString>(_S("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run")));
		if(keyAutostart.Exists() == false)
			return;

		String applicationPath = FileSystem::instance()->getExecutableName();
		String command = String(_S("\"")) + applicationPath + _S("\"");
		if(autoStartBackground)
			command += String(_S(" /")) + OS_CMDLINE_BACKGROUND;

		wxString currentValue;
		keyAutostart.QueryValue(conversions::from_utf16<wxString>(_S("osiris")), currentValue);

		if(currentValue != command)
			keyAutostart.SetValue(conversions::from_utf16<wxString>(_S("osiris")), conversions::from_utf16<wxString>(command));
	}
	else
	{
#if OS_PLATFORM == OS_PLATFORM_WIN
		wxRegKey keyAutostart(conversions::from_utf16<wxString>(_S("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run")));
		if(keyAutostart.Exists() == false)
			return;

		keyAutostart.DeleteValue(conversions::from_utf16<wxString>(_S("osiris")));
#endif
	}
	*/
#endif
}

void Application::setupFilesAssociations(bool enable, bool verbose)
{
	if(enable)
	{
		if(registerFilesAssociations(true) == false)
			reportError("Cannot register osiris files associations, please check the software privileges", verbose);
	}
	else
	{
		registerFilesAssociations(false);
	}
}

void Application::setupProtocolAssociations(bool enable, bool verbose)
{
	if(enable)
	{
		if(registerProtocol() == false)			
			reportError("Cannot register osiris protocol, please check the software privileges", verbose);
	}
	else
	{
#if OS_PLATFORM == OS_PLATFORM_WIN
		wxRegKey keyProtocol(wxRegKey::HKCR, conversions::from_utf16<wxString>(_S("osiris")));
		if(keyProtocol.Exists())
			keyProtocol.DeleteSelf();
#endif
	}
}

void Application::reloadExtensions()
{
	wxBusyCursor busyCursor;
	ExtensionsSystem::instance()->reloadExtensions();

	Application::instance()->reportMessage(getText("ui.extensions.extensions_reloaded"));
}


void Application::viewHome()
{
	openLocalUrl(_S("/"));
}

void Application::openLocalUrl(const String &relativePath)
{
	shared_ptr<IHttpServer> server = IdeSystem::instance()->getServer();
	if(server == null || server->running() == false)
	{
		wxMessageBox(conversions::from_utf16<wxString>(_S("Http server is not running")), conversions::from_utf16<wxString>(_S("Error")), wxOK | wxICON_ERROR);
		return;
	}

	openUrl(getLocalUrl(relativePath));
}

void Application::openUrl(const String &url)
{	
	if(url.empty())
	{
		OS_ASSERTFALSE();
		return;
	}

	wxBusyCursor busyCursor;

	String browser = Options::instance()->getOption<String>(Options::web_options::browser);
	if(browser.empty())
	{
		#if OS_PLATFORM == OS_PLATFORM_MAC
            String command = _S("open \"") + url + _S("\"");
			PlatformManager::instance()->execute(command);
		#elif OS_PLATFORM == OS_PLATFORM_WIN
			//wxExecute(conversions::from_utf16<wxString>(url), wxEXEC_ASYNC);
			//Application::instance()->shell(url);
			PlatformManager::instance()->execute(url);
		#else
			wxLaunchDefaultBrowser(conversions::from_utf16<wxString>(url));
		#endif
	}
	else
	{
		String command = browser + _S(" ") + url;
		wxExecute(conversions::from_utf16<wxString>(command), wxEXEC_ASYNC);
	}
}

void Application::shell(const String &path)
{
    // VERYURGENT: è la stessa parte presa dai sorgenti delle wxWidgets per wxLaunchDefaultBrowser, ma stranamente chiamando wxLaunchDefaultBrowser viene aperto il browser al posto del filemanager...
#if OS_PLATFORM == OS_PLATFORM_LINUX
    wxString envPath, xdgOpen;
    if(wxGetEnv(_T("PATH"), &envPath) && wxFindFileInPath(&xdgOpen, envPath, _T("xdg-open")))
    {
        if(wxExecute(xdgOpen + _T(" ") + conversions::from_utf16<wxString>(path)))
            return;
    }
#elif OS_PLATFORM == OS_PLATFORM_MAC
	String command = _S("open \"") + path + _S("\"");
	PlatformManager::instance()->execute(command);
#else
	wxLaunchDefaultBrowser(conversions::from_utf16<wxString>(path));
#endif
}

void Application::processFile(const String &filename, bool verbose)
{
	String password;
	{
		MainWindow *window = MainWindow::instance();
		if(window != null)
			window->showMode(false);

		wxPasswordEntryDialog passwordDialog(window, conversions::from_utf16<wxString>(getText("ui.file_import.portal_password.message")), conversions::from_utf16<wxString>(filename));
		if(passwordDialog.ShowModal() != wxID_OK)
			return;

		password = conversions::to_utf16(passwordDialog.GetValue());
	}

	uint32 jobID = Engine::instance()->processFile(filename, password);
	if(jobID == 0)
	{
		reportError(String::format(_S("Invalid file '%S'").c_str(), filename.c_str()), verbose);
		return;
	}

	/*
    shared_ptr<File> file(OS_NEW File());
	if(file->open(filename, File::ofRead) == false)
	{
		reportError(String::format(_S("Cannot open file '%S'").c_str(), filename.c_str()), verbose);
		return;
	}

	shared_ptr<PortalsSerializer> serializer(OS_NEW PortalsSerializer());

	shared_ptr<PortalsSerializer::IResult> result = serializer->parsePortalLink(file);
	if(result == null)
	{
		reportError(String::format(_S("Invalid file '%S'").c_str(), filename.c_str()), verbose);
		return;
	}

	shared_ptr<OsirisLink> portalLink = result->getPortalLink();
	if(portalLink == null)
	{
		reportError(String::format(_S("Invalid portal link found in file '%S'").c_str(), filename.c_str()), verbose);
		return;
	}

	

	shared_ptr<Portal> portal = PortalsSystem::instance()->ensurePortal(portalLink, password);
	if(portal == null)
	{
		reportError(String::format(_S("Cannot create portal from file '%S'").c_str(), filename.c_str()), verbose);
		return;
	}

	uint32 jobID = Engine::instance()->peekBackgroundJobID();

	shared_ptr<PortalsImporterJob> job(OS_NEW PortalsImporterJob(jobID, portal, file));

	Engine::instance()->addBackgroundJob(job);
	*/
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_PAGEJOBS_PARAM_JOB, conversions::to_wstring(jobID));
	openUrl(IdeSystem::instance()->getLocalUrl(PortalsSystem::instance()->getMainLink("jobs", params)));	
}

void Application::processLink(const OsirisLink &link, bool verbose)
{
	String href;
	if(Engine::instance()->processLink(link, href))
		openUrl(href);
	else
		reportError(String::format(_S("Cannot process link '%S'").c_str(), link.toString().c_str()), verbose);
}

void Application::reportMessage(const String &message, bool verbose)
{
	if(verbose)
		wxMessageBox(conversions::from_utf16<wxString>(message), conversions::from_utf16<wxString>(Engine::getVersionName(true)), wxOK | wxICON_INFORMATION);
	else
		OS_LOG_INFO(message);
}

void Application::reportError(const String &error, bool verbose)
{
	if(verbose)
		wxMessageBox(conversions::from_utf16<wxString>(error), conversions::from_utf16<wxString>(_S("Error")), wxOK | wxICON_ERROR);
	else
		OS_LOG_ERROR(error);
}

void Application::dispatchEvents()
{
	shared_ptr<wxEventLoopBase> active(wxEventLoopBase::GetActive(), null_deleter());
	if(active == null)
		active = shared_ptr<wxEventLoopBase>(OS_NEW_T(wxEventLoop), os_delete_t());

    wxEventLoopActivator activator(active.get());

	while(active->Pending())
		active->Dispatch();
}

void Application::handleProcessCommands(const std::string &commands)
{
	OS_ASSERT(commands.empty() == false);

	OS_LOG_DEBUG("Processing command(s): " + commands);

	// Processa il comando nel thread principale
	wxCommandEvent e(OS_UI_EVENT_NOTIFY, cmdHandleProcessData);
	e.SetString(conversions::from_string<wxString>(commands));
	wxPostEvent(this, e);
}

void Application::flushLogs()
{
	if(MainWindow::exists() == false || MainWindow::instance()->getInitialized() == false)
		return;

	list<shared_ptr<LogMessage> >::type logs;
	peekLogs(logs);

	if(logs.empty())
		return;

	MainWindow::instance()->logMessages(logs);
}

void Application::log(shared_ptr<LogMessage> message)
{
	// N.B.: i log devo essere accodati e processati in un secondo momento (la finestra potrebbe ad esempio
	// non essere ancora stata creata, in ogni caso meglio restituire subito il controllo...)

	{
		OS_LOCK(m_cs);
		// Accoda il log
		m_logs.push_back(message);
	}

	postFlushLogsEvent();
}

void Application::peekLogs(list<shared_ptr<LogMessage> >::type &logs)
{
	OS_LOCK(m_cs);

	utils::copy(logs, m_logs);
	m_logs.clear();
}

void Application::postFlushLogsEvent()
{
	if(getExiting() == false)
	{
	    wxCommandEvent e(OS_UI_EVENT_NOTIFY, cmdFlushLogs);
		wxPostEvent(this, e);
	}
}

bool Application::registerFilesAssociations(bool associate)
{
	initMimeTypesManager();

	String mime_type;
	MimeManager::instance()->getMimeType(_S("osiris"), mime_type);

	if(associate)
	{
		wxString openCommand = conversions::from_utf16<wxString>(getShellOpenCommand(false));	// Il parametro file viene già inserito in automatico da wxFileTypeInfo

		// N.B.: non usare qui null (causa problemi nel costruttore di wxFileTypeInfo va_start/va_arg)
		wxFileTypeInfo file(conversions::from_utf16<wxString>(mime_type), openCommand, wxEmptyString, wxEmptyString, wxT(OS_PORTALFILE_EXTENSION), wxNullPtr);
		file.SetShortDesc(wxT("osiris.file"));

		/*
		URGENT: NON FUNZIONA: il file è nella dir "share", ma le opzioni le legge dopo se lancio Osiris con "/install".
		String iconPath;
#if OS_PLATFORM	== OS_PLATFORM_WIN
		iconPath = getResourcesFilePath("osiris_file.ico");
#elif OS_PLATFORM == OS_PLATFORM_MAC
		iconPath = getResourcesFilePath("osiris_file.icns");
#else
		iconPath = getResourcesFilePath("osiris_file.png");
#endif
		file.SetIcon(conversions::from_utf16<wxString>(iconPath));
		*/
		
		return wxTheMimeTypesManager->Associate(file) != null;
	}

	wxFileType *file = wxTheMimeTypesManager->GetFileTypeFromMimeType(conversions::from_utf16<wxString>(mime_type));
	if(file == null)
		return true;

	return wxTheMimeTypesManager->Unassociate(file);
}

bool Application::registerProtocol()
{
#if OS_PLATFORM == OS_PLATFORM_WIN
	wxRegKey keyProtocol(wxRegKey::HKCR, conversions::from_utf16<wxString>(_S("osiris")));
	if(keyProtocol.Exists() == false)
	{
		if(keyProtocol.Create() == false)
			return false;

		if(keyProtocol.SetValue(wxEmptyString, conversions::from_utf16<wxString>(_S("URL: osiris Protocol"))) == false)
			return false;

		if(keyProtocol.SetValue(conversions::from_utf16<wxString>(_S("URL Protocol")), wxEmptyString) == false)
			return false;
	}

	wxString applicationPath = conversions::from_utf16<wxString>(FileSystem::instance()->getExecutableName());

	wxRegKey keyIcon(wxRegKey::HKCR, conversions::from_utf16<wxString>(_S("osiris\\DefaultIcon")));
	if(keyIcon.Exists())
	{
		wxString currentValue;
		if(keyIcon.QueryValue(wxEmptyString, currentValue) == false)
			return false;

		if(currentValue != applicationPath)
		{
			if(keyIcon.SetValue(wxEmptyString, applicationPath) == false)
				return false;
		}
	}
	else
	{
		if(keyIcon.Create() == false)
			return false;

		if(keyIcon.SetValue(wxEmptyString, applicationPath) == false)
			return false;
	}

	String checkValue = getShellOpenCommand(true);

	wxRegKey keyCommand(wxRegKey::HKCR, conversions::from_utf16<wxString>(_S("osiris\\shell\\open\\command")));
	if(keyCommand.Exists())
	{
		wxString currentValue;
		if(keyCommand.QueryValue(wxEmptyString, currentValue) == false)
			return false;

		if(currentValue != conversions::from_utf16<wxString>(checkValue))
		{
			if(keyCommand.SetValue(wxEmptyString, conversions::from_utf16<wxString>(checkValue)) == false)
				return false;
		}
	}
	else
	{
		if(keyCommand.Create() == false)
			return false;

		if(keyCommand.SetValue(wxEmptyString, conversions::from_utf16<wxString>(checkValue)) == false)
			return false;
	}
#endif

#if OS_PLATFORM == OS_PLATFORM_MAC
	CFURLRef cfurl = NULL;
    //const char *url = "osiris://"; // 0.14
	const char *url = "osiris:"; // 0.15
    size_t len = 9;
    cfurl = CFURLCreateWithBytes(NULL, (const UInt8 *) url, len, kCFStringEncodingUTF8, NULL);
    if (cfurl == NULL)
		return false;
    else
    {
        OSStatus rc = LSRegisterURL(cfurl, true);
        if (rc != noErr)
		    printf("registering url protocol failed! (rc==%d)\n", (int) rc);
        CFRelease(cfurl);

		if (rc != noErr)
			return false;
    }
#endif

	return true;
}

bool Application::getAutoStart()
{
#if OS_PLATFORM == OS_PLATFORM_WIN
	TCHAR szPath[MAX_PATH];

	if(SUCCEEDED(SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, TRUE)))
	{
		String path = szPath + _S("\\Osiris.lnk");

		return (FileSystem::instance()->exists(path) == true);		
	}
	else
		return false;
#else
	return false;
#endif
}

bool Application::getAssociationsProtocols()
{
#if OS_PLATFORM == OS_PLATFORM_WIN
	wxRegKey keyCommand(wxRegKey::HKCR, conversions::from_utf16<wxString>(_S("osiris\\shell\\open\\command")));
	if(keyCommand.Open(wxRegKey::Read) == false)
		return false;

	wxString currentValue;
	if(keyCommand.QueryValue(wxEmptyString, currentValue) == false)
		return false;

	if(currentValue != conversions::from_utf16<wxString>(getShellOpenCommand(true)))
		return false;

	return true;
#elif OS_PLATFORM == OS_PLATFORM_MAC
	return true;
#else
	return false;
#endif
}

bool Application::getAssociationsFiles()
{
#if OS_PLATFORM == OS_PLATFORM_WIN
	wxRegKey keyCommand(wxRegKey::HKCR, conversions::from_utf16<wxString>(_S("osiris.file\\shell\\open\\command")));
	if(keyCommand.Open(wxRegKey::Read) == false)
		return false;

	wxString currentValue;
	if(keyCommand.QueryValue(wxEmptyString, currentValue) == false)
		return false;

	if(currentValue != conversions::from_utf16<wxString>(getShellOpenCommand(true)))
		return false;

	return true;
#elif OS_PLATFORM == OS_PLATFORM_MAC
	return true;
#else
	return false;
#endif
}

/*
void Application::validateOptions()
{
#if OS_PLATFORM == OS_PLATFORM_WIN
	
	if(Options::instance()->getOption<bool>(Options::ui_options::start_system))
		if(Application::instance()->checkAutostart() == false)
				Options::instance()->setOption(Options::ui_options::start_system, false);

	if(Options::instance()->getOption<bool>(Options::ui_options::associations_protocols))
	{
		boost::optional<bool> result = checkProtocolRegistered();
		if(result)
			if(result.get() == false)
				Options::instance()->setOption(Options::ui_options::associations_protocols, false);
	}

	if(Options::instance()->getOption<bool>(Options::ui_options::associations_files))
	{
		boost::optional<bool> result = checkFilesRegistered();
		if(result)
			if(result.get() == false)
				Options::instance()->setOption(Options::ui_options::associations_files, false);
	}

#endif
}
*/

void Application::initLibraries()
{
	shared_ptr<ThirdPartyLibrary> wxwidgetsLibrary = ThirdPartyLibrariesReporter::instance()->addLibrary();
	wxwidgetsLibrary->setName("wxWidgets");
	wxwidgetsLibrary->setVersion((boost::format("%d.%d.%d") % wxMAJOR_VERSION % wxMINOR_VERSION % wxRELEASE_NUMBER).str());
	wxwidgetsLibrary->setDescription("wxWidgets (formerly wxWindows) is a widget toolkit for creating graphical user interfaces (GUIs) for cross-platform applications");
	wxwidgetsLibrary->setUrl("http://www.wxwidgets.org/");
	wxwidgetsLibrary->setLicense("LGPL");
	wxwidgetsLibrary->setLicenseUrl("http://en.wikipedia.org/wiki/GNU_Lesser_General_Public_License");
}

void Application::initMimeTypesManager()
{
	if(m_mimeTypesManagerInitialized)
		return;

	m_mimeTypesManagerInitialized = true;
	wxTheMimeTypesManager->Initialize();
}

void Application::processCommand(const std::string &command, bool verbose)
{
	OS_LOG_INFO(_S("Processing command: ") + command);

	OsirisLink link;
	if(link.parse(command))
		processLink(link, verbose);
	else
		processFile(command, verbose);
}

void Application::onConfigureOptions()
{
	m_configureOptions = true;
}

void Application::onFlushLogs(wxCommandEvent &e)
{
	flushLogs();
}

void Application::onHandleProcessData(wxCommandEvent &e)
{
	vector<std::string>::type commands;
	algorithms::split_regex(conversions::to_string(e.GetString()), commands, boost::regex(OS_IAPPLICATION_COMMANDS_SEPARATOR));
	std::for_each(commands.begin(), commands.end(), boost::bind(&Application::processCommand, this, _1, false));
}

bool Application::OnInit()
{
#ifdef OS_OSIRIS_VERSION_RAZOR
	// URGENT: http://www.osiris-sps.org/forums/index.php?showtopic=1799	
	setlocale(LC_ALL,"C");
#endif
	
	FileSystem::instance()->initExecutableName(conversions::to_string(argv[0])); // Also here because with "/install" doesn't reach the IApplication::init.

	if(wxApp::OnInit() == false)
		return false;

	// N.B.: le opzioni vanno caricate prima dei check sul processo in quanto settano eventuali variabili di ambiente sulle directory temporanee utilizzate nel boost
	if(init(argc, static_cast<wchar_t **>(argv)) == false)
		return false;

	wxSocketBase::Initialize();
	wxInitAllImageHandlers();

	MainWindow::create();	// Va creata prima per mostrare il wizard in caso di primo avvio

	scoped_ptr<SplashScreen> splash;
#ifndef OS_DEBUG
	{
		wxImage bitmap;
		if(bitmap.LoadFile(conversions::from_utf16<wxString>(getResourcesFilePath(_S("splash.png"))), wxBITMAP_TYPE_PNG))
		{
		    splash.reset(new SplashScreen(conversions::from_utf16<wxString>(Engine::getVersionName(false)), bitmap/*, MainWindow::instance()*/));
			splash->setStatus(_S("Initializing..."));
		}
	}
#endif // OS_DEBUG

	initLibraries();

	if(start() == false)
	{
		if(splash != null)
		{
	        splash->setStatus(_S("Initialization failed"));
			PlatformManager::instance()->sleep(1000);
		}

		return false;
	}

	if(m_configureOptions)
	{
		splash.reset();
		MainWindow::instance()->configureOptions(false);
	}

	if(Options::instance()->getOption<bool>(Options::ui_options::start_minimized))
		m_startBackground = true;

	MainWindow::instance()->init(m_startBackground, getRecoveryMode() == false);

	String applicationPath = FileSystem::instance()->getExecutableName();
	OS_LOG_DEBUG(_S("Executable path: ") + applicationPath);

	wxYield();

	// TOCLEAN
	/*
	bool enableVersionCheck = Options::instance()->getOption<bool>(Options::system_options::last_version_check_enable);
	if(enableVersionCheck)
		MainWindow::instance()->checkForUpdates();
	*/

	postFlushLogsEvent();

	return true;
}

int Application::OnExit()
{
	OS_ASSERT(isPrimaryProcess());

    stop();
	cleanup();

	setExiting(true);

	wxImage::CleanUpHandlers();

	return wxApp::OnExit();
}

void Application::OnUnhandledException()
{
	OS_LOG_ERROR(_S("Unhandled exception"));
	// Rilancia l'eccezione
	throw;
}

bool Application::OnExceptionInMainLoop()
{
	OS_LOG_ERROR(_S("Unhandled exception"));
	return true;	// Ignora l'eccezione
}

void Application::OnInitCmdLine(wxCmdLineParser &parser)
{
	wxApp::OnInitCmdLine(parser);

	parser.AddSwitch(conversions::from_utf16<wxString>(OS_CMDLINE_INSTALL));
	parser.AddSwitch(conversions::from_utf16<wxString>(OS_CMDLINE_UNINSTALL));
	parser.AddSwitch(conversions::from_utf16<wxString>(OS_CMDLINE_BACKGROUND));
	parser.AddSwitch(conversions::from_utf16<wxString>(OS_CMDLINE_RECOVERY));
	parser.AddSwitch(conversions::from_utf16<wxString>(OS_CMDLINE_VERBOSE));
	parser.AddParam(wxEmptyString, wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL | wxCMD_LINE_PARAM_MULTIPLE);
}

bool Application::OnCmdLineParsed(wxCmdLineParser &parser)
{
	if(wxApp::OnCmdLineParsed(parser) == false)
		return false;

	/*
	if(parser.Found(conversions::from_utf16<wxString>(OS_CMDLINE_INSTALL)))
	{
		setupFilesAssociations(false);
		setupRegistryProtocol(false);
		return false;
	}
	*/
	
	if(parser.Found(conversions::from_utf16<wxString>(OS_CMDLINE_INSTALL)))
	{
		setupProtocolAssociations(true, false);
		setupFilesAssociations(true, false);

		return false;
	}

	if(parser.Found(conversions::from_utf16<wxString>(OS_CMDLINE_UNINSTALL)))
	{
		setupAutostart(false);
		setupFilesAssociations(false, false);		
		setupProtocolAssociations(false, false);

		return false;
	}
	
	if(parser.Found(conversions::from_utf16<wxString>(OS_CMDLINE_BACKGROUND)))
	{
		m_startBackground = true;
	}

	// TODO: la command line andrebbe gestita in IApplication (leggendo qui eventuali switch specifici della UI) in modo da supportare in particolare /recovery e /verbose anche da console

	if(parser.Found(conversions::from_utf16<wxString>(OS_CMDLINE_RECOVERY)))
	{
		NotificationsManager::instance()->notify(_S("Start in Recovery mode"));
		setRecoveryMode(true);
	}

	if(parser.Found(conversions::from_utf16<wxString>(OS_CMDLINE_VERBOSE)))
	{
		NotificationsManager::instance()->notify(_S("Start in Verbose mode"));
		setVerbose(true);
	}

	size_t params = parser.GetParamCount();
	for(size_t i = 0; i < params; ++i)
	{
		OS_LOG_DEBUG("Command line param: " + conversions::to_utf16(parser.GetParam(i)));

		addProcessCommand(conversions::to_utf16(parser.GetParam(i)));
	}

	return true;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
