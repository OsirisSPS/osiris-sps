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
#include "idemainimport.h"

#include "boost/bind.hpp"
#include "buffer.h"
#include "conversions.h"
#include "eventssource.h"
#include "engine.h"
#include "file.h"
#include "htmlevent.h"
#include "htmlcontrols.h"
#include "htmlfilebrowser.h"
#include "htmltextbox.h"
#include "htmlxslcontrol.h"
#include "idebutton.h"
#include "idemainjobs.h"
#include "idepages.h"
#include "ideskin.h"
#include "memfile.h"
#include "options.h"
#include "osirislink.h"
#include "portalsportalssystem.h"
#include "temporaryfile.h"
#include "utils.h"
#include "xmldocument.h"
#include "xmlexporter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_OPEN((ide)(main))

//////////////////////////////////////////////////////////////////////

Import::Import(shared_ptr<HttpSession> session) : PageBase(session),
												  m_fileBrowser(OS_NEW HtmlFileBrowser()),
												  m_portalPassword(OS_NEW HtmlTextBox())
{
	m_fileBrowser->setID(_S("fileBrowser"));
	m_fileBrowser->setCss(_S("os_input_full"));

	m_portalPassword->setID(_S("portalPassword"));
	m_portalPassword->setCss(_S("os_input_full"));
}

Import::~Import()
{

}

void Import::onImport()
{
	const Buffer *buffer = m_fileBrowser->getFileBufferPtr();
	if(buffer == nullptr)
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}

	shared_ptr<TemporaryFile> file = Options::instance()->createTemporaryFile();
	if(file == nullptr)
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}

	if(file->write(buffer->getData(), buffer->getSize()) != buffer->getSize())
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}

	if(file->seekToBegin() == false)
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}

	/*
	shared_ptr<PortalsSerializer> serializer(OS_NEW PortalsSerializer());

	shared_ptr<PortalsSerializer::IResult> result = serializer->parsePortalLink(file);
	if(result == nullptr)
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}

	shared_ptr<OsirisLink> portalLink = result->getPortalLink();
	if(portalLink == nullptr)
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}
	*/

	String password = m_portalPassword->getValue();
	
	uint32 jobID = Engine::instance()->processFile(file, password);
	if(jobID == 0)
	{
		showError(getText("main.pages.import.errors.invalid_file"));
		return;
	}
	
	/*
	shared_ptr<Portal> portal = PortalsSystem::instance()->ensurePortal(portalLink, password);
	if(portal == nullptr)
	{
		showError(getText("main.pages.import.errors.cannot_create_portal"));
		return;
	}

	uint32 jobID = Engine::instance()->peekBackgroundJobID();

	shared_ptr<PortalsImporterJob> job(OS_NEW PortalsImporterJob(jobID, portal, file));

	Engine::instance()->addBackgroundJob(job);
	*/

	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_PAGEJOBS_PARAM_JOB, conversions::to_wstring(jobID));
	redirect(PortalsSystem::instance()->getMainLink("jobs", params));	
}

String Import::getPageName() const
{
    return "main.pages.import";
}

void Import::onLoad()
{
	PageBase::onLoad();

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	shared_ptr<XMLExporter> exporter(OS_NEW XMLExporter(document->create(_S("page"))));
	shared_ptr<HtmlXSLControl> pageTemplate(OS_NEW HtmlXSLControl(loadStylesheet(getSkin()->getTemplatePath(_S("import.xsl"))), document));
	getArea(pageAreaContent)->getControls()->add(pageTemplate);

	shared_ptr<IdeButton> cmdImport(OS_NEW IdeButton(getText("main.pages.import.actions.import")));
	cmdImport->setID("cmdImport");
	cmdImport->setIsDefault(true);
	cmdImport->getEventClick()->connect(boost::bind(&Import::onImport, this));
		
	pageTemplate->addChildParam(m_fileBrowser);
	pageTemplate->addChildParam(m_portalPassword);
	pageTemplate->addChildParam(cmdImport);	
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_CLOSE((ide)(main))

//////////////////////////////////////////////////////////////////////
