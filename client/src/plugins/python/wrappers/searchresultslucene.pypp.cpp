// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "searchresultslucene.h"
#include "searchresult.h"
#include "searchresultslucene.pypp.hpp"

namespace bp = boost::python;

struct SearchResultsLucene_wrapper : ::osiris::SearchResultsLucene, ::osiris::PythonWrapper< ::osiris::SearchResultsLucene > {

    SearchResultsLucene_wrapper( )
    : ::osiris::SearchResultsLucene( )
      , ::osiris::PythonWrapper< ::osiris::SearchResultsLucene >(){
        // null constructor
    
    }

    static boost::python::object getQueryText( ::osiris::SearchResultsLucene const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.getQueryText();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual void exportXML( ::boost::shared_ptr< osiris::XMLNode > node, ::boost::shared_ptr< osiris::SearchQuery > query, ::boost::shared_ptr< osiris::IPortalPage > page, bool external ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_exportXML = this->get_override( "exportXML" ) )
            func_exportXML( node, query, page, external );
        else{
            __pystate.leave();
            this->::osiris::SearchResultsLucene::exportXML( node, query, page, external );
        }
    }
    
    void default_exportXML( ::boost::shared_ptr< osiris::XMLNode > node, ::boost::shared_ptr< osiris::SearchQuery > query, ::boost::shared_ptr< osiris::IPortalPage > page, bool external ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::SearchResultsLucene::exportXML( node, query, page, external );
    }

    virtual void clear(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_clear = this->get_override( "clear" ) )
            func_clear(  );
        else{
            __pystate.leave();
            this->::osiris::SearchResultsLucene::clear(  );
        }
    }
    
    void default_clear(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::SearchResultsLucene::clear( );
    }

    virtual ::boost::shared_ptr< osiris::SearchResult > getResult( ::boost::shared_ptr< osiris::IPortalDatabase > database, ::osiris::uint32 index, bool asGroup=false ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getResult = this->get_override( "getResult" ) )
            return func_getResult( database, index, asGroup );
        else{
            __pystate.leave();
            return this->::osiris::SearchResultsLucene::getResult( database, index, asGroup );
        }
    }
    
    ::boost::shared_ptr< osiris::SearchResult > default_getResult( ::boost::shared_ptr< osiris::IPortalDatabase > database, ::osiris::uint32 index, bool asGroup=false ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::SearchResultsLucene::getResult( database, index, asGroup );
    }

    virtual ::osiris::uint32 getResults(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getResults = this->get_override( "getResults" ) )
            return func_getResults(  );
        else{
            __pystate.leave();
            return this->::osiris::SearchResultsLucene::getResults(  );
        }
    }
    
    ::osiris::uint32 default_getResults(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::SearchResultsLucene::getResults( );
    }

    virtual ::osiris::uint32 getTotalResults(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getTotalResults = this->get_override( "getTotalResults" ) )
            return func_getTotalResults(  );
        else{
            __pystate.leave();
            return this->::osiris::SearchResultsLucene::getTotalResults(  );
        }
    }
    
    ::osiris::uint32 default_getTotalResults(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::SearchResultsLucene::getTotalResults( );
    }

    void _addDateRange( ::osiris::String & text, ::osiris::String const & field, ::osiris::DateTime const & from, ::osiris::DateTime const & to ){
        ::osiris::SearchResultsLucene::_addDateRange( boost::ref(text), boost::ref(field), boost::ref(from), boost::ref(to) );
    }

    void _addFieldIn( ::osiris::String & text, ::osiris::String const & field, ::osiris::StringList const & list ){
        ::osiris::SearchResultsLucene::_addFieldIn( boost::ref(text), boost::ref(field), boost::ref(list) );
    }

    virtual void executeQuery( ::boost::shared_ptr< osiris::IPortalDatabase > database, ::boost::shared_ptr< osiris::Portal > portal, ::boost::shared_ptr< osiris::SearchQuery > query, ::osiris::StringMap const & objectsMap ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_executeQuery = this->get_override( "executeQuery" ) )
            func_executeQuery( database, portal, query, boost::ref(objectsMap) );
        else{
            __pystate.leave();
            this->::osiris::SearchResultsLucene::executeQuery( database, portal, query, boost::ref(objectsMap) );
        }
    }
    
    virtual void default_executeQuery( ::boost::shared_ptr< osiris::IPortalDatabase > database, ::boost::shared_ptr< osiris::Portal > portal, ::boost::shared_ptr< osiris::SearchQuery > query, ::osiris::StringMap const & objectsMap ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::SearchResultsLucene::executeQuery( database, portal, query, boost::ref(objectsMap) );
    }

    virtual void search( ::boost::shared_ptr< osiris::IPortalDatabase > database, ::boost::shared_ptr< osiris::Portal > portal, ::boost::shared_ptr< osiris::SearchQuery > query, ::osiris::StringMap const & objectsMap ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_search = this->get_override( "search" ) )
            func_search( database, portal, query, boost::ref(objectsMap) );
        else{
            __pystate.leave();
            this->::osiris::ISearchResults::search( database, portal, query, boost::ref(objectsMap) );
        }
    }
    
    void default_search( ::boost::shared_ptr< osiris::IPortalDatabase > database, ::boost::shared_ptr< osiris::Portal > portal, ::boost::shared_ptr< osiris::SearchQuery > query, ::osiris::StringMap const & objectsMap ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::ISearchResults::search( database, portal, query, boost::ref(objectsMap) );
    }

};

void register_SearchResultsLucene_class(){

    { //::osiris::SearchResultsLucene
        typedef ::boost::python::class_< SearchResultsLucene_wrapper, ::boost::python::bases< ::osiris::ISearchResults >, ::boost::noncopyable > SearchResultsLucene_exposer_t;
        SearchResultsLucene_exposer_t SearchResultsLucene_exposer = SearchResultsLucene_exposer_t( "SearchResultsLucene", ::boost::python::no_init );
        ::boost::python::scope SearchResultsLucene_scope( SearchResultsLucene_exposer );
        SearchResultsLucene_exposer.def( ::boost::python::init< >() );
        { //::osiris::SearchResultsLucene::getQueryText
        
            typedef boost::python::object ( *getQueryText_function_type )( ::osiris::SearchResultsLucene const & );
            
            SearchResultsLucene_exposer.def( 
                "getQueryText"
                , getQueryText_function_type( &SearchResultsLucene_wrapper::getQueryText ) );
        
        }
        { //::osiris::SearchResultsLucene::exportXML
        
            typedef void ( ::osiris::SearchResultsLucene::*exportXML_function_type )( ::boost::shared_ptr< osiris::XMLNode >,::boost::shared_ptr< osiris::SearchQuery >,::boost::shared_ptr< osiris::IPortalPage >,bool ) ;
            typedef void ( SearchResultsLucene_wrapper::*default_exportXML_function_type )( ::boost::shared_ptr< osiris::XMLNode >,::boost::shared_ptr< osiris::SearchQuery >,::boost::shared_ptr< osiris::IPortalPage >,bool ) ;
            
            SearchResultsLucene_exposer.def( 
                "exportXML"
                , exportXML_function_type(&::osiris::SearchResultsLucene::exportXML)
                , default_exportXML_function_type(&SearchResultsLucene_wrapper::default_exportXML)
                , ( ::boost::python::arg("node"), ::boost::python::arg("query"), ::boost::python::arg("page"), ::boost::python::arg("external") ) );
        
        }
        { //::osiris::SearchResultsLucene::clear
        
            typedef void ( ::osiris::SearchResultsLucene::*clear_function_type )(  ) ;
            typedef void ( SearchResultsLucene_wrapper::*default_clear_function_type )(  ) ;
            
            SearchResultsLucene_exposer.def( 
                "clear"
                , clear_function_type(&::osiris::SearchResultsLucene::clear)
                , default_clear_function_type(&SearchResultsLucene_wrapper::default_clear) );
        
        }
        { //::osiris::SearchResultsLucene::getResult
        
            typedef ::boost::shared_ptr< osiris::SearchResult > ( ::osiris::SearchResultsLucene::*getResult_function_type )( ::boost::shared_ptr< osiris::IPortalDatabase >,::osiris::uint32,bool ) ;
            typedef ::boost::shared_ptr< osiris::SearchResult > ( SearchResultsLucene_wrapper::*default_getResult_function_type )( ::boost::shared_ptr< osiris::IPortalDatabase >,::osiris::uint32,bool ) ;
            
            SearchResultsLucene_exposer.def( 
                "getResult"
                , getResult_function_type(&::osiris::SearchResultsLucene::getResult)
                , default_getResult_function_type(&SearchResultsLucene_wrapper::default_getResult)
                , ( ::boost::python::arg("database"), ::boost::python::arg("index"), ::boost::python::arg("asGroup")=(bool)(false) ) );
        
        }
        { //::osiris::SearchResultsLucene::getResults
        
            typedef ::osiris::uint32 ( ::osiris::SearchResultsLucene::*getResults_function_type )(  ) ;
            typedef ::osiris::uint32 ( SearchResultsLucene_wrapper::*default_getResults_function_type )(  ) ;
            
            SearchResultsLucene_exposer.def( 
                "getResults"
                , getResults_function_type(&::osiris::SearchResultsLucene::getResults)
                , default_getResults_function_type(&SearchResultsLucene_wrapper::default_getResults) );
        
        }
        { //::osiris::SearchResultsLucene::getTotalResults
        
            typedef ::osiris::uint32 ( ::osiris::SearchResultsLucene::*getTotalResults_function_type )(  ) ;
            typedef ::osiris::uint32 ( SearchResultsLucene_wrapper::*default_getTotalResults_function_type )(  ) ;
            
            SearchResultsLucene_exposer.def( 
                "getTotalResults"
                , getTotalResults_function_type(&::osiris::SearchResultsLucene::getTotalResults)
                , default_getTotalResults_function_type(&SearchResultsLucene_wrapper::default_getTotalResults) );
        
        }
        { //::osiris::SearchResultsLucene::_addDateRange
        
            typedef void ( SearchResultsLucene_wrapper::*_addDateRange_function_type )( ::osiris::String &,::osiris::String const &,::osiris::DateTime const &,::osiris::DateTime const & ) ;
            
            SearchResultsLucene_exposer.def( 
                "_addDateRange"
                , _addDateRange_function_type( &SearchResultsLucene_wrapper::_addDateRange )
                , ( ::boost::python::arg("text"), ::boost::python::arg("field"), ::boost::python::arg("from"), ::boost::python::arg("to") ) );
        
        }
        { //::osiris::SearchResultsLucene::_addFieldIn
        
            typedef void ( SearchResultsLucene_wrapper::*_addFieldIn_function_type )( ::osiris::String &,::osiris::String const &,::osiris::StringList const & ) ;
            
            SearchResultsLucene_exposer.def( 
                "_addFieldIn"
                , _addFieldIn_function_type( &SearchResultsLucene_wrapper::_addFieldIn )
                , ( ::boost::python::arg("text"), ::boost::python::arg("field"), ::boost::python::arg("list") ) );
        
        }
        { //::osiris::SearchResultsLucene::executeQuery
        
            typedef void ( SearchResultsLucene_wrapper::*executeQuery_function_type )( ::boost::shared_ptr< osiris::IPortalDatabase >,::boost::shared_ptr< osiris::Portal >,::boost::shared_ptr< osiris::SearchQuery >,::osiris::StringMap const & ) ;
            
            SearchResultsLucene_exposer.def( 
                "executeQuery"
                , executeQuery_function_type( &SearchResultsLucene_wrapper::default_executeQuery )
                , ( ::boost::python::arg("database"), ::boost::python::arg("portal"), ::boost::python::arg("query"), ::boost::python::arg("objectsMap") ) );
        
        }
        { //::osiris::ISearchResults::search
        
            typedef void ( ::osiris::ISearchResults::*search_function_type )( ::boost::shared_ptr< osiris::IPortalDatabase >,::boost::shared_ptr< osiris::Portal >,::boost::shared_ptr< osiris::SearchQuery >,::osiris::StringMap const & ) ;
            typedef void ( SearchResultsLucene_wrapper::*default_search_function_type )( ::boost::shared_ptr< osiris::IPortalDatabase >,::boost::shared_ptr< osiris::Portal >,::boost::shared_ptr< osiris::SearchQuery >,::osiris::StringMap const & ) ;
            
            SearchResultsLucene_exposer.def( 
                "search"
                , search_function_type(&::osiris::ISearchResults::search)
                , default_search_function_type(&SearchResultsLucene_wrapper::default_search)
                , ( ::boost::python::arg("database"), ::boost::python::arg("portal"), ::boost::python::arg("query"), ::boost::python::arg("objectsMap") ) );
        
        }
        { //property "queryText"[fget=::osiris::SearchResultsLucene::getQueryText]
        
            typedef ::osiris::String ( ::osiris::SearchResultsLucene::*fget )(  ) const;
            
            SearchResultsLucene_exposer.add_property( 
                "queryText"
                , fget( &::osiris::SearchResultsLucene::getQueryText )
                , "get property, built on top of \"osiris::String osiris::SearchResultsLucene::getQueryText() const [member function]\"" );
        
        }
    }

}