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

#ifndef _OS_PYTHON_PYPLUSPLUSALIASES_H
#define _OS_PYTHON_PYPLUSPLUSALIASES_H

//////////////////////////////////////////////////////////////////////

#ifndef OS_GCCXML
	#error pyplusplusaliases.h must be included only from GCCXML !
#endif // OS_GCCXML

//////////////////////////////////////////////////////////////////////

namespace pyplusplus {
namespace aliases {

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////


// Short - TODO: check naming
typedef IHtmlControl::HtmlEvents HtmlEvents;
typedef HttpResponse::HttpCookies HttpCookies;
typedef SearchQuery::Options SearchQueryOptions;

// Ptr	
typedef shared_ptr<IEvent> IEventPtr;
typedef shared_ptr<IHtmlControl::HtmlEvents> HtmlEventsPtr;
typedef shared_ptr<IDbDriver> IDbDriverPtr;
typedef shared_ptr<IDbParameter> IDbParameterPtr;
typedef shared_ptr<IHttpDirectoryHandler> IHttpDirectoryHandlerPtr;
typedef shared_ptr<ISearchOptions> ISearchOptionsPtr;
typedef shared_ptr<LanguageFolder> LanguageFolderPtr;
typedef shared_ptr<IBackgroundJob> IBackgroundJobPtr;
typedef shared_ptr<OMLEmoticon> OMLEmoticonPtr;
typedef shared_ptr<OMLItem> OMLItemPtr;
typedef shared_ptr<Portal> PortalPtr;
typedef shared_ptr<PortalsPeer> PortalsPeerPtr;
typedef shared_ptr<XMLAttribute> XMLAttributePtr;

// Singleton	
typedef DynamicSingleton<DatabasesSystem> DatabasesSystemSingleton;
typedef DynamicSingleton<Engine> EngineSingleton;
typedef DynamicSingleton<ExtensionsSystem> ExtensionsSystemSingleton;
typedef DynamicSingleton<IdeSystem> IdeSystemSingleton;
typedef DynamicSingleton<CacheSystem> CacheSystemSingleton;
typedef DynamicSingleton<IsisSystem> IsisSystemSingleton;
typedef DynamicSingleton<PortalsSystem> PortalsSystemSingleton;
typedef StaticSingleton<FileSystem> FileSystemSingleton;
typedef StaticSingleton<IdeAccountsManager> IdeAccountsManagerSingleton;
typedef StaticSingleton<LanguageManager> LanguageManagerSingleton;
typedef StaticSingleton<LogManager> LogManagerSingleton;
typedef StaticSingleton<OMLManager> OMLManagerSingleton;
typedef StaticSingleton<RegexManager> RegexManagerSingleton; 
typedef StaticSingleton<XMLManager> XMLManagerSingleton; // ClodoLap1
typedef StaticSingleton<HtmlParser> HtmlParserSingleton; // ClodoLap1
typedef StaticSingleton<NetworkSystem> NetworkSystemSingleton; // ClodoLap1
typedef StaticSingleton<ArchivesManager> ArchivesManagerSingleton; // ClodoLap1
typedef StaticSingleton<Options> OptionsSingleton;
typedef StaticSingleton<CryptManager> CryptManagerSingleton; // ClodoLap1
typedef StaticSingleton<ThirdPartyLibrariesReporter> ThirdPartyLibrariesReporterSingleton; // ClodoLap1


// Template - TODO: check naming
typedef list<String>::type StringList;
typedef map<String, String>::type StringMap;
typedef vector<String>::type StringVector;
	
typedef vector<DataItem>::type DataItemVector;

typedef HtmlVirtualPage<IMainPage> IVirtualMainPage;
typedef IPageControl<IHtmlControl> IPageHtmlControl;
typedef IPageControl<HtmlDiv> IPageHtmlDiv;
typedef IXSLRenderer<IPageControl<IHtmlControl> > IPageXSLRendererHtmlControl;
typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > IPortalPageXSLRendererHtmlControl;
typedef IPortalPageControl<HtmlDiv>	IPortalPageHtmlDiv;
typedef IPortalPageControl<IHtmlControl> IPortalPageHtmlControl;

// List & Deque
//clodolap1
/*
typedef list<shared_ptr<IDbDriver> >::type IDbDriversList;
typedef deque<shared_ptr<IDbParameter> >::type DbParametersDeque;
typedef deque<shared_ptr<IBackgroundJob> >::type IBackgroundJobsDeque;	
typedef list<shared_ptr<IHttpDirectoryHandler> >::type IHttpDirectoryHandlersList;

typedef list<shared_ptr<OMLEmoticon> >::type OMLEmoticonsList;
typedef list<shared_ptr<OMLItem> >::type OMLItemsList;
typedef list<shared_ptr<Portal> >::type PortalsList;
typedef list<shared_ptr<PortalsPeer> >::type PortalsPeersList;
typedef list<shared_ptr<LanguageFolder> >::type LanguageManagerFoldersList;
*/
typedef list<shared_ptr<IDbDriver> >::type IDbDriverList;
typedef deque<shared_ptr<IDbParameter> >::type IDbParameterDeque;
typedef deque<shared_ptr<IBackgroundJob> >::type IBackgroundJobDeque;	
typedef list<shared_ptr<IHttpDirectoryHandler> >::type IHttpDirectoryHandlerList;
typedef list<shared_ptr<ThirdPartyLibrary> >::type ThirdPartyLibraryList;
typedef list<shared_ptr<OMLEmoticon> >::type OMLEmoticonList;
typedef list<shared_ptr<OMLItem> >::type OMLItemList;
typedef list<shared_ptr<Portal> >::type PortalList;
typedef list<shared_ptr<PortalsPeer> >::type PortalsPeerList;
typedef list<shared_ptr<ReputationsFeedback> >::type ReputationsFeedbackList;
typedef list<shared_ptr<LanguageFolder> >::type LanguageFolderList;
typedef deque<shared_ptr<IArchivesArchiveEntry> >::type IArchivesArchiveEntryDeque; // #ErrorC: error C2678: binary '==' : no operator found which takes a left-hand operand of type 'osiris::IArchivesArchiveEntry' (or there is no acceptable conversion)	o:\osiris\client\src\plugins\python\wrappers\indexing_suite\value_traits.hpp	65	
typedef unordered_map<uint32, shared_ptr<IsisEndpoint> >::type IsisEndpointMap;
typedef list<shared_ptr<IdeAccount> >::type IdeAccountsList;



// Temp, test
//typedef unordered_map<uint32, shared_ptr<IsisEndpoint> >::type IsisEndpointsMap;
//typedef LockPtr<const PortalOptions::IsisEndpoints, boost::recursive_mutex, boost::unique_lock<boost::recursive_mutex> > IsisEndpointsLocked;

//typedef LockPtr<const osiris::PortalOptions::IsisEndpoints, boost::recursive_mutex, boost::unique_lock<boost::recursive_mutex> > IsisEndpointsLocked;
//typedef Locked<boost::unordered_map<unsigned int, boost::shared_ptr<osiris::IsisEndpoint>, boost::hash<unsigned int>, std::equal_to<unsigned int>, osiris::StdAllocator<std::pair<unsigned int const, boost::shared_ptr<osiris::IsisEndpoint> >, osiris::NedAllocator<std::pair<unsigned int const, boost::shared_ptr<osiris::IsisEndpoint> > > > > const, boost::recursive_mutex> IsisEndpointsLocked;

//////////////////////////////////////////////////////////////////////

} // aliases
} // pyplusplus

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYPLUSPLUSALIASES_H
