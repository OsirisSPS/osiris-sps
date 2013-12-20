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
#include "modulesfeedviewer.h"

#include "engine.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpurl.h"
#include "ideviewiobjectviewer.h"
#include "log.h"
#include "modulesfeed.h"
#include "modulesmodulesconfig.h"
#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "platformmanager.h"
#include "idesession.h"
#include "portalsportal.h"
#include "htmlliteral.h"
#include "htmltext.h"
#include "httpsession.h"
#include "mimemanager.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

extern OS_NAMESPACE_NAME::String g_componentsPath;

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {

//////////////////////////////////////////////////////////////////////

namespace modules {

//////////////////////////////////////////////////////////////////////

FeedViewer::FeedViewer()
{

}

FeedViewer::~FeedViewer()
{

}

void FeedViewer::onLoad()
{
	ViewerBase::onLoad();

	shared_ptr<XMLNode> root = getModuleDocument()->getRoot();
	OS_ASSERT(root != nullptr);

	// Ajax:
	// - Creo un controllo per contenere quanto sotto
	// - Chiamo un "isDirect" a livello di ModuleViewer, che restituisce un "true" se son puntato diretto.
	// - Se si, leggo il parametro "mode", se è "direct" restituisco il contenuto del controllo secco via transmit-buffer
	// - Qui dumpo una chiamata ad ajax passandogli l'url di me in direct mode
	String mode = _S("direct");  // Proprietà?
	mode = _S("ajax");
	
	shared_ptr<HtmlDiv> divData(OS_NEW HtmlDiv());

	String controlID = getInstance().toUTF16();
	if(controlID.empty())
	{
		controlID = _S("preview"); // Se il modulo è in preview, non ha un'ID di istanza. Ma un ID mi serve x forza per gli eventi.
		mode = _S("direct"); // Ajax non è supportato nell'anteprima, perchè non ho un id da referenziare secco per avere il contenuto in un secondo momento.
	}
	setID(controlID);

	if(mode == _S("ajax"))
	{
		// - Chiamo un "isDirect" a livello di ModuleViewer, che restituisce un "true" se son puntato diretto.
		// - Se si, leggo il parametro "mode", se è "direct" restituisco il contenuto del controllo secco via transmit-buffer
		if(getPage()->getRequest()->getUrlParam(_W("mode")) == _W("content"))
			mode = _S("content");
		else
			mode = _S("delayed");
	}

	if( (mode == _S("direct")) || (mode == _S("content")) )
	{	
		String url = root->getAttributeString(OS_MODULES_FEED_URL);
		bool showTitle = root->getAttributeBool(OS_MODULES_FEED_SHOWTITLE);
		bool showDescription = root->getAttributeBool(OS_MODULES_FEED_SHOWDESCRIPTION);
		bool showImage = root->getAttributeBool(OS_MODULES_FEED_SHOWIMAGE);
		int32 nItems = root->getAttributeInt32(OS_MODULES_FEED_NITEMS);
		String showItemDescription = root->getAttributeString(OS_MODULES_FEED_SHOWITEMDESCRIPTION);
		String templateModel = root->getAttributeString(OS_MODULES_FEED_TEMPLATEMODEL);
		String templateCustom = root->getAttributeString(OS_MODULES_FEED_TEMPLATECUSTOM);

		shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
		shared_ptr<XMLStylesheet> transformer(OS_NEW XMLStylesheet());

		try
		{
			shared_ptr<boost::asio::io_service> service = getSession()->getService();

			String output;
			String error;

			String userAgent = Engine::getDefaultHttpUserAgent();	
			document->parseUrl(url.to_ascii(), userAgent, service, Engine::instance()->createTCPSocket(service, true, true));

			shared_ptr<XMLNode> rssRoot = document->getRoot();

			if(rssRoot == nullptr)
			{
				// Error reading RSS Feed.				
				error = _S("Feed reading error.");				
			}
			else
			{		
				// Aggiungo all'xml RSS le proprietà del modulo.
				rssRoot->setAttributeString(OS_MODULES_FEED_URL, url);
				rssRoot->setAttributeBool(OS_MODULES_FEED_SHOWTITLE, showTitle);
				rssRoot->setAttributeBool(OS_MODULES_FEED_SHOWDESCRIPTION, showDescription);
				rssRoot->setAttributeBool(OS_MODULES_FEED_SHOWIMAGE, showImage);
				rssRoot->setAttributeInt32(OS_MODULES_FEED_NITEMS, nItems);
				rssRoot->setAttributeString(OS_MODULES_FEED_SHOWITEMDESCRIPTION, showItemDescription);
				rssRoot->setAttributeString(OS_MODULES_FEED_TEMPLATEMODEL, templateModel);
				
				if(templateModel.empty())
				{
					shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
					if(stylesheet->parseString(templateCustom))
					{
						// Inizializza le funzioni del template
						getPage()->initStylesheet(stylesheet);

						if(stylesheet->applyToString(document, output))
						{
							//getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getPage()->parseBBCode(output, true, false, false, false))));
						}
						else
						{
							// Transformazione fallita.
							error = _S("Transformation failed.");
						}
					}
					else
					{
						// Template custom invalido.
						error = _S("Invalid custom template.");
					}
				}
				else
				{
					shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());
					// Inizializza le funzioni del template
					getPage()->initStylesheet(stylesheet);

					String path = utils::makeFilePath(utils::makeFolderPath(g_componentsPath, _S("feed")), _S("feed_viewer_") + templateModel + _S(".xsl"));

					// Carica l'xsl
					if(stylesheet->parseFile(path))
					{
						if(stylesheet->applyToString(document, output))
						{
							// Qui potrei aggiungere un literal diretto, non parserizzato bb...
							//getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getPage()->parseBBCode(output, true, false, false, false))));
						}
						else
						{
							// Transformazione fallita.
							error = _S("Transformation failed.");
						}
					}
					else
					{
						// Template non valido.
						error = _S("Invalid template.");
					}
				}

				if(output.empty() == true)
				{
					error = _S("Unknown error.");										
				}
			}

			if(error.empty() == false)
			{
				divData->setCss(_S("os_plugins_components_feed_error_reading"));
				divData->getControls()->add(shared_ptr<HtmlText>(OS_NEW HtmlText(error)));
			}
			else
			{
				divData->setCss(_S("os_plugins_components_feed_ok"));
				output = output.trim();
				//output = _S("[nobr]") + output + _S("[/nobr]");

				// Debugging
				//output = _S("[code]") + output + _S("[/code]") + output;
				
				divData->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(getPage()->parseOml(output, true, false, false, omlRenderModeOsiris, getInstance().getString()))));
			}
		}
		catch(std::exception &e)
		{
			OS_LOG_ERROR(e.what());
		}	
	}

	if(mode == _S("direct"))
	{
		getControls()->add(divData);		
	}
	else if(mode == _S("delayed"))
	{
		String urlAjaxContent = getPage()->getPortal()->getViewObjectLink(getInstance()) + _S("&mode=content");
		divData->getControls()->add(shared_ptr<HtmlLiteral>(OS_NEW HtmlLiteral(_S("<div data-os-wait=\"center\" data-os-url=\"") + urlAjaxContent + _S("\"></div>"))));		
		getControls()->add(divData);		
	}
	else if(mode == _S("content"))
	{
		// trasmitbuffer solo di divData.
		HtmlWriter writer;
		divData->render(getPage(), writer);		
		
		getPage()->getSession()->transmitHtml(writer.getHtml().to_utf8(), httpStatusOK);
	}
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
