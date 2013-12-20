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
#include "extensionswizard.h"

#include "algorithms.h"
#include "application.h"
#include "buffer.h"
#include "conversions.h"
#include "cryptmanager.h"
#include "filesystem.h"
#include "extensionsextension.h"
#include "extensionssystem.h"
#include "extensionswizardpagefinish.h"
#include "extensionswizardpagegeneral.h"
#include "extensionswizardpagetemplate.h"
#include "options.h"
#include "osiriscommon.h"
#include "uniqueid.h"
#include "textfile.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(ExtensionsWizard, WizardBase)
    EVT_WIZARD_FINISHED(static_cast<wxWindowID>(wxID_ANY), ExtensionsWizard::OnWizardFinished)
END_EVENT_TABLE()

//////////////////////////////////////////////////////////////////////

ExtensionsWizard::ExtensionsWizard(wxWindow *parent, const String &title) : WizardBase(parent, title)
{
	m_pageGeneral = new ExtensionsWizardPageGeneral(this);
	m_pageTemplate = new ExtensionsWizardPageTemplate(this);
	m_pageFinish = new ExtensionsWizardPageFinish(this);

	wxWizardPageSimple::Chain(m_pageGeneral, m_pageTemplate);
	wxWizardPageSimple::Chain(m_pageTemplate, m_pageFinish);

	SetBorder(0);
}

ExtensionsWizard::~ExtensionsWizard()
{

}

void ExtensionsWizard::run()
{
	wxSizer *sizer = GetSizer();
	if(sizer != nullptr)
	{
		// Il bordo della colonna principale nel wizard viene settato a 5px e, non essendo al momento
		// regolabile, viene qui modificato in questo modo...

		wxSizerItemList &sizerItems = sizer->GetChildren();
		if(sizerItems.empty() == false)
		{
			sizerItems[0]->SetBorder(0);
		}
	}

	// Scorre tutte le pagine a partire da quella di welcome e adatta il wizard alla dimensione migliore
	FitToPage(m_pageGeneral);

	Layout();

	// Avvia il wizard mostrando la pagina di welcome
	RunWizard(m_pageGeneral);
}

void ExtensionsWizard::OnWizardFinished(wxWizardEvent &e)
{
	String extensionName = conversions::to_utf16(m_pageGeneral->getExtensionName());
	String extensionDescription = conversions::to_utf16(m_pageGeneral->getExtensionDescription());
	String extensionAuthor = conversions::to_utf16(m_pageGeneral->getExtensionAuthor());
	shared_ptr<ExtensionsWizardPageTemplate::TemplateDetails> extensionTemplate = m_pageTemplate->getTemplate();
	const ExtensionsWizardPageTemplate::TemplateDetails::Files &extensionFiles = extensionTemplate->getFiles();

	ExtensionID extensionID = ExtensionID::generate();

	String rootPath = ExtensionsSystem::instance()->getDataPath();
	String extensionPath = utils::makeFolderPath(rootPath, extensionID.toUTF16());

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLNode> nodeRoot = document->create(OS_EXTENSION_XML_NODE_ROOT);
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_ID, extensionID.getString());
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_NAME, extensionName);
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_DESCRIPTION, extensionDescription);
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_AUTHOR, extensionAuthor);
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_VERSION_CODE, _S("1"));
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_VERSION_NAME, _S("0.1"));
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_ICON, _S("icon.png"));
	nodeRoot->setAttributeString(OS_EXTENSION_XML_NODE_ROOT_LOGO, _S("logo.png"));

	shared_ptr<XMLNode> nodeScripts;
	ExtensionsWizardPageTemplate::TemplateDetails::Files::const_iterator i;
	for(i = extensionFiles.begin(); i != extensionFiles.end(); ++i)
	{
		if((*i).script)
		{
			if(nodeScripts == nullptr)
				nodeScripts = nodeRoot->addChild(OS_EXTENSION_XML_NODE_SCRIPTS);

			shared_ptr<XMLNode> nodeScript = nodeScripts->addChild(OS_EXTENSION_XML_NODE_SCRIPT);
			nodeScript->setAttributeString(OS_EXTENSION_XML_NODE_SCRIPT_PATH, (*i).target);
			// TODO: temp
			nodeScript->setAttributeString(OS_EXTENSION_XML_NODE_SCRIPT_LANGUAGE, "python");
		}
	}

	String documentString;
	if(document->writeString(documentString) == false)
	{
		OS_ASSERTFALSE();
		Application::instance()->reportError("Internal error generating extension's xml, please report it", true);
		return;
	}

	if(XMLManager::instance()->validateString(documentString, XMLSchema::fromFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(), OS_SCHEMAS_PATH), OS_EXTENSIONS_EXTENSION_SCHEMA))) == false)
	{
		OS_ASSERTFALSE();
		Application::instance()->reportError("Internal error validating extension's xml, please report it", true);
		return;
	}

	if(FileSystem::instance()->ensureDirectory(extensionPath) == false)
	{
		Application::instance()->reportError("Cannot create directory '" + extensionPath + "'", true);
		return;
	}

	for(i = extensionFiles.begin(); i != extensionFiles.end(); ++i)
	{
		String targetFile = utils::makeFilePath(extensionPath, (*i).target);
		String sourceFile = utils::makeFilePath(extensionTemplate->getFolder(), (*i).source);

		String targetPath = FileSystem::instance()->getFilePath(targetFile);
		if(FileSystem::instance()->ensureDirectory(targetPath) == false)
		{
			Application::instance()->reportError("Cannot create directory '" + targetPath + "'", true);
			return;
		}

		if(FileSystem::instance()->copyFile(sourceFile, targetFile) == false)
		{
			Application::instance()->reportError("Cannot copy file '" + sourceFile + "' to '" + targetFile + "'", true);
			return;
		}

		if((*i).macro)
		{
			// Open file, do replaces and save.
			String data;
			TextFile::readFile(targetFile, data);

			data = data.replace_all("{extension_id}", extensionID.getString());
			data = data.replace_all("{extension_name}", extensionName);
			data = data.replace_all("{extension_description}", extensionDescription);
			data = data.replace_all("{extension_author}", extensionAuthor);
			UniqueID uniqueID = UniqueID::generate();
			data = data.replace_all("{unique_id}", uniqueID.getString());

			TextFile file;
			file.open(targetFile, TextFile::ofWrite);
			file.writeLine(data);
			file.close();
		}
	}

	String documentFile = utils::makeFilePath(extensionPath, OS_MANIFESTXML);
	if(document->writeFile(documentFile) == false)
	{
		Application::instance()->reportError("Error writing file '" + documentFile + "'", true);
		return;
	}

	OS_ASSERT(XMLManager::instance()->validateFile(documentFile, XMLSchema::fromFile(utils::makeFilePath(utils::makeFolderPath(Options::instance()->getSharePath(), OS_SCHEMAS_PATH), OS_EXTENSIONS_EXTENSION_SCHEMA))));

	if(m_pageFinish->getOpenFolder())
		Application::instance()->shell(extensionPath);

	Application::instance()->reloadExtensions();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
