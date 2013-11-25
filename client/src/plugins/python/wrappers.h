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

#ifndef _OS_PYTHON_WRAPPERS_H
#define _OS_PYTHON_WRAPPERS_H

#include "gccxml.h"

//////////////////////////////////////////////////////////////////////

// Prerequisites

#include "osiriscommon.h"

#include "archivesconfig.h"
#include "coreconfig.h"
#include "dbconfig.h"
#include "engineconfig.h"
#include "httpconfig.h"
#include "langconfig.h"
#include "netconfig.h"
#include "webconfig.h"
#include "xmlconfig.h"

// --------------------------------------------------------------------------------
// Archives 
#include "iarchivesarchive.h"
#include "iarchivesarchiveentry.h"
#include "iarchivesarchivefactory.h"
#include "archivesmanager.h"
#include "archivesziparchive.h"
#include "archivesziparchivefactory.h"

// --------------------------------------------------------------------------------
// Core 
#include "ievent.h"
#include "ieventcallback.h"
#include "ijob.h"
#include "ioptionvalidator.h"
#include "buffer.h"
#include "cryptmanager.h"
#include "dataitem.h"
#include "datatable.h"
#include "datatree.h"
#include "eventcallback.h"
#include "eventssource.h"
#include "file.h"
#include "filelogger.h"
#include "filesystem.h"
#include "hash.h"
#include "image.h"
#include "regexmanager.h"
#include "textfile.h"
#include "thirdpartylibrary.h"
#include "thirdpartylibrariesreporter.h"
#include "uniqueid.h"
#include "version.h"

// --------------------------------------------------------------------------------
// DB 
#include "idbconnection.h"
#include "idbdriver.h"
#include "idboptions.h"
#include "idbparameter.h"
#include "idbresult.h"
#include "idbstatement.h"

// --------------------------------------------------------------------------------
// Engine 

// Engine - General
#include "id.h"
#include "ids.h"

// Engine - Data
#include "dataaccount.h"

// Engine - Extensions
#include "iextensionsextension.h"
#include "iextensionscomponent.h"
#include "iextensionsmodule.h"
#include "extensionsextension.h"
#include "extensionssystem.h"
#include "extensionscomponentcontrol.h"
#include "extensionscomponenteditor.h"
#include "extensionscomponentviewer.h"
#include "extensionsinvalidcomponent.h"
#include "extensionsinvalidmodule.h"
#include "extensionsmodulecontrol.h"
#include "extensionsmoduleeditor.h"
#include "extensionsmoduleviewer.h"
#include "extensionsmoduleviewerhidden.h"

// Engine - OML
#include "iomlcode.h"
#include "iomlxsl.h"
#include "omla.h"
#include "omlanchor.h"
#include "omlcode.h"
#include "omlcontext.h"
#include "omldate.h"
#include "omldebug.h"
#include "omlemail.h"
#include "omlemoticon.h"
#include "omlgoto.h"
#include "omlhtml.h"
#include "omlhtmlwrapper.h"
#include "omlif.h"
#include "omlimg.h"
#include "omlitem.h"
#include "omllang.h"
#include "omlless.h"
#include "omllist.h"
#include "omlmanager.h"
#include "omlmore.h"
#include "omlnobr.h"
#include "omlnoparse.h"
#include "omlobject.h"
#include "omlpopup.h"
#include "omlquote.h"
#include "omlreplace.h"
#include "omlsize.h"
#include "omlsystem.h"
#include "omltab.h"
#include "omltext.h"
#include "omlurl.h"
#include "omluser.h"
#include "omlview.h"

// Engine - Pickers
#include "idepickerbool.h"
#include "idepickercolor.h"
#include "idepickercomponent.h"
#include "idepickerculture.h"
#include "idepickerdatetime.h"
#include "idepickerdatetimespecial.h"
#include "idepickernumber.h"
#include "idepickerobject.h"
#include "idepickerobjectex.h"
#include "idepickersections.h"
#include "idepickerselect.h"
#include "idepickerskin.h"

// Engine - Misc
#include "cachesystem.h"
#include "isissystem.h"
#include "reputationsfeedback.h"
#include "reputationsmanager.h"
#include "reputationsscore.h"

// Engine - Search
#include "isearchoptions.h"
#include "isearchresults.h"
#include "searchattachmentoptions.h"
#include "searchcalendareventoptions.h"
#include "searchfileoptions.h"
#include "searchinstanceoptions.h"
#include "searchmodeloptions.h"
#include "searchpolloptionoptions.h"
#include "searchpolloptions.h"
#include "searchpostoptions.h"
#include "searchquery.h"
#include "searchresult.h"
#include "searchresultsdatabase.h"
#include "searchresultslucene.h"
#include "searchsectionoptions.h"
#include "searchtagoptions.h"
#include "searchtextoptions.h"

// --------------------------------------------------------------------------------
// HTTP 
#include "ihttpclient.h"
#include "ihttpdirectory.h"
#include "ihttpdirectoryhandler.h"
#include "ihttpdirectoryregexhandler.h"
#include "ihttpmethod.h"
#include "ihttpserver.h"
#include "httpclient.h"
#include "httpcontent.h"
#include "httpcookie.h"
#include "httpdata.h"
#include "httpdirectorycallback.h"
#include "httpenumerateddirectorycallback.h"
#include "httpparser.h"
#include "httppath.h"
#include "httpphysicaldirectory.h"
#include "httppostparam.h"
#include "httpproxy.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httprootdirectory.h"
#include "httpsession.h"
#include "httpsessionstate.h"
#include "httpsocketproxy.h"
#include "httpurl.h"
#include "httpvirtualdirectory.h"

// --------------------------------------------------------------------------------
// IRC 

// --------------------------------------------------------------------------------
// Lang 
#include "languageculture.h"
#include "languagefolder.h"
#include "languagefolderlistener.h"
#include "languagemanager.h"
#include "languageresult.h"
#include "languagevocabolary.h"

// --------------------------------------------------------------------------------
// Net 
#include "connectionsmanager.h"
#include "ipaddress.h"
#include "networksystem.h"

// --------------------------------------------------------------------------------
// SDK

// --------------------------------------------------------------------------------
// UI

// --------------------------------------------------------------------------------
// UPNP
//#include "upnpmanager.h"
//#include "upnpserver.h"
//#include "upnpservice.h"

// --------------------------------------------------------------------------------
// Web
#include "ihtmlcontrol.h"
#include "ihtmlinput.h"
#include "ihtmlselect.h"
#include "htmlattribute.h"
#include "htmlattributes.h"
#include "htmlbr.h"
#include "htmlbutton.h"
#include "htmlcheckbox.h"
#include "htmlcombobox.h"
#include "htmlcontrols.h"
#include "htmldiv.h"
#include "htmlevent.h"
#include "htmlfilebrowser.h"
#include "htmlimage.h"
#include "htmllink.h"
#include "htmllinkbutton.h"
#include "htmllistbox.h"
#include "htmllinktag.h"
#include "htmlliteral.h"
#include "htmlmetatag.h"
#include "htmloption.h"
#include "htmloptiongroup.h"
#include "htmlpage.h"
#include "htmlpager.h"
#include "htmlpanel.h"
#include "htmlparagraph.h"
#include "htmlparser.h"
#include "htmlpopup.h"
#include "htmlroller.h"
#include "htmlscript.h"
#include "htmlspace.h"
#include "htmlspan.h"
#include "htmltab.h"
#include "htmltabctrl.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltableheader.h"
#include "htmltablerow.h"
#include "htmltabpage.h"
#include "htmltext.h"
#include "htmltextarea.h"
#include "htmltextbox.h"
#include "htmltooltip.h"
#include "htmlunit.h"
#include "htmlvirtualpage.h"
#include "htmlwriter.h"
#include "htmlxslcontrol.h"

// --------------------------------------------------------------------------------
// XML
#include "ixmlfunction.h"
#include "ixmlhandler.h"
#include "ixmlresolver.h"
#include "ixmlserializable.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmldetails.h"
#include "xmldocument.h"
#include "xmlmanager.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlschema.h"
#include "xmlstylesheet.h"
#include "xmlwriter.h"

// --------------------------------------------------------------------------------
// Other wrappers, to clean

#include "dbdatabasessystem.h"
#include "engine.h"
#include "ibackgroundjob.h"
#include "ideide.h"
#include "ideaccount.h"
#include "ideaccountsmanager.h"
#include "ideassistant.h"
#include "ideomleditor.h"
#include "ideblock.h"
#include "idebutton.h"
#include "idehelpbox.h"
#include "idemailbox.h"
#include "idepathway.h"
#include "idepositioneditor.h"
#include "idesearchglobalparams.h"
#include "idesearchparam.h"
#include "idesearchquery.h"
#include "idesearchresults.h"
#include "idesession.h"
#include "idesessionportal.h"
#include "ideskin.h"
#include "ideskinsimple.h"
#include "idesystem.h"
#include "idetablequery.h"
#include "idevoteeditor.h"
#include "idexslresolver.h"
#include "iidemainpage.h"
#include "iidepageable.h"
#include "iideportalpage.h"
#include "iidexslrenderer.h"
#include "iportalbackgroundjob.h"
#include "iportaldatabase.h"
#include "isisendpoint.h"
#include "istream.h"
#include "locked.h"
#include "logmanager.h"
#include "mainaddonsjob.h"
#include "options.h"
#include "ordered_map.h"
//#include "portalsexporterjob.h"
#include "portalsimporterjob.h"
#include "portalspeer.h"
#include "portalspeersmanager.h"
#include "portalsoptionsshared.h"
#include "portalsportal.h"
#include "portalsportaloptions.h"
#include "portalsportalssystem.h"
#include "portalstransaction.h"
#include "portalsstabilityjob.h"
#include "tcpsocket.h"
#include "thread.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class Foo
{
public:
	Foo()
	{
		
	}

	virtual ~Foo()
	{

	}

public:
	void call_public()
	{
		virtual_public();
	}

	static void static_call_public(Foo *foo)
	{
		foo->call_public();
	}

	bool call_public_with_result()
	{
		return virtual_public_with_result();
	}

	void call_protected()
	{
		virtual_protected();
	}

	static void static_call_protected(Foo *foo)
	{
		foo->call_protected();
	}

	bool call_protected_with_result()
	{
		return virtual_protected_with_result();
	}
/*
	void call_log_pure()
	{
		log_pure();
	}

	void call_log_protected_pure()
	{
		log_protected_pure();
	}
*/
	void normal()
	{
		OS_LOG_INFO("Foo::normal");
	}

	bool normal_with_result()
	{
		OS_LOG_INFO("Foo::normal_with_result");
		return true;
	}

	virtual void virtual_public()
	{
		OS_LOG_INFO("Foo::virtual_public");
	}

	virtual bool virtual_public_with_result()
	{
		OS_LOG_INFO("Foo::virtual_public_with_result");
		return true;
	}

	bool non_static_method_public()
	{
		return true;
	}

	static bool static_method_public()
	{
		return true;
	}

//	virtual void log_pure() = 0;
//	virtual bool log_pure_with_result() = 0;

protected:
	static bool static_method_protected()
	{
		return true;
	}

	bool non_static_method_protected()
	{
		return true;
	}

	virtual void virtual_protected()
	{
		OS_LOG_INFO("Foo::virtual_protected");
	}

	virtual bool virtual_protected_with_result()
	{
		OS_LOG_INFO("Foo::virtual_protected_with_result");
		return true;
	}

//	virtual void log_protected_pure() = 0;
//	virtual bool log_protected_pure_with_result() = 0;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#ifdef OS_GCCXML
	#include "pyplusplusaliases.h"
	#include "pyplusplusmanualtemplates.h"
#endif // OS_GCCXML

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_WRAPPERS_H
