// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "idemailbox.h"
#include "xmlstylesheet.h"
#include "ideskin.h"
#include "idesession.h"
#include "datatree.h"
#include "htmlattributes.h"
#include "htmlevent.h"
#include "eventssource.h"
#include "htmlvirtualpage.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "idemailbox.pypp.hpp"

namespace bp = boost::python;

struct IdeMailBox_wrapper : ::osiris::IdeMailBox, ::osiris::PythonWrapper< ::osiris::IdeMailBox > {

    IdeMailBox_wrapper(::osiris::MessageFolder folder=::osiris::messageFolderInbox )
    : ::osiris::IdeMailBox( folder )
      , ::osiris::PythonWrapper< ::osiris::IdeMailBox >(){
        // constructor
    
    }

    IdeMailBox_wrapper(::osiris::UniqueID const & message )
    : ::osiris::IdeMailBox( boost::ref(message) )
      , ::osiris::PythonWrapper< ::osiris::IdeMailBox >(){
        // constructor
    
    }

    static boost::python::object getFolder( ::osiris::IdeMailBox const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::MessageFolder result = inst.getFolder();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getMessage( ::osiris::IdeMailBox const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::UniqueID const & result = inst.getMessage();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::UniqueID const & >( result ) );
    }

    static boost::python::object getOffset( ::osiris::IdeMailBox const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint32 result = inst.getOffset();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getSecure( ::osiris::IdeMailBox const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.getSecure();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual ::osiris::String getTemplatePath(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getTemplatePath = this->get_override( "getTemplatePath" ) )
            return func_getTemplatePath(  );
        else{
            __pystate.leave();
            return this->::osiris::IdeMailBox::getTemplatePath(  );
        }
    }
    
    ::osiris::String default_getTemplatePath(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IdeMailBox::getTemplatePath( );
    }

    virtual void onPreRender(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onPreRender = this->get_override( "onPreRender" ) )
            func_onPreRender(  );
        else{
            __pystate.leave();
            this->::osiris::IdeMailBox::onPreRender(  );
        }
    }
    
    void default_onPreRender(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdeMailBox::onPreRender( );
    }

    virtual void renderFolder( ::osiris::MessageFolder folder, ::boost::shared_ptr< osiris::XMLNode > node ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_renderFolder = this->get_override( "renderFolder" ) )
            func_renderFolder( folder, node );
        else{
            __pystate.leave();
            this->::osiris::IdeMailBox::renderFolder( folder, node );
        }
    }
    
    virtual void default_renderFolder( ::osiris::MessageFolder folder, ::boost::shared_ptr< osiris::XMLNode > node ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdeMailBox::renderFolder( folder, node );
    }

    virtual void renderMessage( ::osiris::UniqueID const & id, ::boost::shared_ptr< osiris::XMLNode > node ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_renderMessage = this->get_override( "renderMessage" ) )
            func_renderMessage( boost::ref(id), node );
        else{
            __pystate.leave();
            this->::osiris::IdeMailBox::renderMessage( boost::ref(id), node );
        }
    }
    
    virtual void default_renderMessage( ::osiris::UniqueID const & id, ::boost::shared_ptr< osiris::XMLNode > node ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdeMailBox::renderMessage( boost::ref(id), node );
    }

    virtual ::osiris::String getPageHref( ::osiris::uint32 offset ) const  {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getPageHref = this->get_override( "getPageHref" ) )
            return func_getPageHref( offset );
        else{
            __pystate.leave();
            return this->::osiris::IdeMailBox::getPageHref( offset );
        }
    }
    
    ::osiris::String default_getPageHref( ::osiris::uint32 offset ) const  {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IdeMailBox::getPageHref( offset );
    }

    static void setFolder( ::osiris::IdeMailBox & inst, ::osiris::MessageFolder folder ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setFolder(folder);
        __pythreadSaver.restore();
    }

    static void setMessage( ::osiris::IdeMailBox & inst, ::osiris::UniqueID const & message ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setMessage(message);
        __pythreadSaver.restore();
    }

    static void setOffset( ::osiris::IdeMailBox & inst, ::osiris::uint32 offset ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setOffset(offset);
        __pythreadSaver.restore();
    }

    static void setSecure( ::osiris::IdeMailBox & inst, bool secure ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setSecure(secure);
        __pythreadSaver.restore();
    }

    void createPager( ::boost::shared_ptr< osiris::IdeSkin > skin, ::boost::shared_ptr< osiris::XMLNode > node, ::osiris::uint32 total_items, ::osiris::uint32 items_to_show, ::osiris::uint32 offset ){
        ::osiris::IIdePageable::createPager( skin, node, total_items, items_to_show, offset );
    }

    bool decodeEvent( ::osiris::String const & command, ::osiris::String & eventName, ::osiris::HtmlEvent & e ) const {
        return ::osiris::IHtmlControl::decodeEvent( boost::ref(command), boost::ref(eventName), boost::ref(e) );
    }

    ::osiris::String encodeEvent( ::osiris::String const & eventName, ::osiris::HtmlEvent const * e=(nullptr) ) const {
        return ::osiris::IHtmlControl::encodeEvent( boost::ref(eventName), boost::python::ptr(e) );
    }

    virtual ::boost::shared_ptr< osiris::HttpSession > getSession(  ) const  {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getSession = this->get_override( "getSession" ) )
            return func_getSession(  );
        else{
            __pystate.leave();
            return this->::osiris::IHtmlControl::getSession(  );
        }
    }
    
    ::boost::shared_ptr< osiris::HttpSession > default_getSession(  ) const  {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IHtmlControl::getSession( );
    }

    virtual void onEvent( ::osiris::String const & name, ::osiris::IEvent * e=(nullptr) ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onEvent = this->get_override( "onEvent" ) )
            func_onEvent( boost::ref(name), boost::python::ptr(e) );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::onEvent( boost::ref(name), boost::python::ptr(e) );
        }
    }
    
    virtual void default_onEvent( ::osiris::String const & name, ::osiris::IEvent * e=(nullptr) ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onEvent( boost::ref(name), boost::python::ptr(e) );
    }

    virtual void onInit(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onInit = this->get_override( "onInit" ) )
            func_onInit(  );
        else{
            __pystate.leave();
            this->::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInit(  );
        }
    }
    
    virtual void default_onInit(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInit( );
    }

    virtual void onInitStylesheet(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onInitStylesheet = this->get_override( "onInitStylesheet" ) )
            func_onInitStylesheet(  );
        else{
            __pystate.leave();
            this->::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInitStylesheet(  );
        }
    }
    
    virtual void default_onInitStylesheet(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInitStylesheet( );
    }

    virtual void onLoad(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onLoad = this->get_override( "onLoad" ) )
            func_onLoad(  );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::onLoad(  );
        }
    }
    
    virtual void default_onLoad(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onLoad( );
    }

    virtual void onLoadViewState( ::osiris::DataTree const & state ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onLoadViewState = this->get_override( "onLoadViewState" ) )
            func_onLoadViewState( boost::ref(state) );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::onLoadViewState( boost::ref(state) );
        }
    }
    
    virtual void default_onLoadViewState( ::osiris::DataTree const & state ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onLoadViewState( boost::ref(state) );
    }

    virtual void onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onRender = this->get_override( "onRender" ) )
            func_onRender( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::onRender( boost::ref(writer) );
        }
    }
    
    virtual void default_onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onRender( boost::ref(writer) );
    }

    virtual void onSaveViewState( ::osiris::DataTree & state ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onSaveViewState = this->get_override( "onSaveViewState" ) )
            func_onSaveViewState( boost::ref(state) );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::onSaveViewState( boost::ref(state) );
        }
    }
    
    virtual void default_onSaveViewState( ::osiris::DataTree & state ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onSaveViewState( boost::ref(state) );
    }

    virtual void renderAttributes( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_renderAttributes = this->get_override( "renderAttributes" ) )
            func_renderAttributes( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::renderAttributes( boost::ref(writer) );
        }
    }
    
    void default_renderAttributes( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::renderAttributes( boost::ref(writer) );
    }

    virtual void renderChilds( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_renderChilds = this->get_override( "renderChilds" ) )
            func_renderChilds( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::renderChilds( boost::ref(writer) );
        }
    }
    
    void default_renderChilds( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::renderChilds( boost::ref(writer) );
    }

    void saveViewState( ::osiris::DataTree & states ){
        ::osiris::IHtmlControl::saveViewState( boost::ref(states) );
    }

};

void register_IdeMailBox_class(){

    { //::osiris::IdeMailBox
        typedef ::boost::python::class_< IdeMailBox_wrapper, ::boost::python::bases< ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >, ::osiris::IIdePageable >, ::boost::noncopyable > IdeMailBox_exposer_t;
        IdeMailBox_exposer_t IdeMailBox_exposer = IdeMailBox_exposer_t( "IdeMailBox", ::boost::python::init< ::boost::python::optional< ::osiris::MessageFolder > >(( ::boost::python::arg("folder")=::osiris::messageFolderInbox )) );
        ::boost::python::scope IdeMailBox_scope( IdeMailBox_exposer );
        ::boost::python::implicitly_convertible< ::osiris::MessageFolder, ::osiris::IdeMailBox >();
        IdeMailBox_exposer.def( ::boost::python::init< ::osiris::UniqueID const & >(( ::boost::python::arg("message") )) );
        ::boost::python::implicitly_convertible< ::osiris::UniqueID const &, ::osiris::IdeMailBox >();
        { //::osiris::IdeMailBox::getFolder
        
            typedef boost::python::object ( *getFolder_function_type )( ::osiris::IdeMailBox const & );
            
            IdeMailBox_exposer.def( 
                "getFolder"
                , getFolder_function_type( &IdeMailBox_wrapper::getFolder ) );
        
        }
        { //::osiris::IdeMailBox::getMessage
        
            typedef boost::python::object ( *getMessage_function_type )( ::osiris::IdeMailBox const & );
            
            IdeMailBox_exposer.def( 
                "getMessage"
                , getMessage_function_type( &IdeMailBox_wrapper::getMessage ) );
        
        }
        { //::osiris::IdeMailBox::getOffset
        
            typedef boost::python::object ( *getOffset_function_type )( ::osiris::IdeMailBox const & );
            
            IdeMailBox_exposer.def( 
                "getOffset"
                , getOffset_function_type( &IdeMailBox_wrapper::getOffset ) );
        
        }
        { //::osiris::IdeMailBox::getSecure
        
            typedef boost::python::object ( *getSecure_function_type )( ::osiris::IdeMailBox const & );
            
            IdeMailBox_exposer.def( 
                "getSecure"
                , getSecure_function_type( &IdeMailBox_wrapper::getSecure ) );
        
        }
        { //::osiris::IdeMailBox::getTemplatePath
        
            typedef ::osiris::String ( ::osiris::IdeMailBox::*getTemplatePath_function_type )(  ) ;
            typedef ::osiris::String ( IdeMailBox_wrapper::*default_getTemplatePath_function_type )(  ) ;
            
            IdeMailBox_exposer.def( 
                "getTemplatePath"
                , getTemplatePath_function_type(&::osiris::IdeMailBox::getTemplatePath)
                , default_getTemplatePath_function_type(&IdeMailBox_wrapper::default_getTemplatePath) );
        
        }
        { //::osiris::IdeMailBox::onPreRender
        
            typedef void ( ::osiris::IdeMailBox::*onPreRender_function_type )(  ) ;
            typedef void ( IdeMailBox_wrapper::*default_onPreRender_function_type )(  ) ;
            
            IdeMailBox_exposer.def( 
                "onPreRender"
                , onPreRender_function_type(&::osiris::IdeMailBox::onPreRender)
                , default_onPreRender_function_type(&IdeMailBox_wrapper::default_onPreRender) );
        
        }
        { //::osiris::IdeMailBox::renderFolder
        
            typedef void ( IdeMailBox_wrapper::*renderFolder_function_type )( ::osiris::MessageFolder,::boost::shared_ptr< osiris::XMLNode > ) ;
            
            IdeMailBox_exposer.def( 
                "renderFolder"
                , renderFolder_function_type( &IdeMailBox_wrapper::default_renderFolder )
                , ( ::boost::python::arg("folder"), ::boost::python::arg("node") ) );
        
        }
        { //::osiris::IdeMailBox::renderMessage
        
            typedef void ( IdeMailBox_wrapper::*renderMessage_function_type )( ::osiris::UniqueID const &,::boost::shared_ptr< osiris::XMLNode > ) ;
            
            IdeMailBox_exposer.def( 
                "renderMessage"
                , renderMessage_function_type( &IdeMailBox_wrapper::default_renderMessage )
                , ( ::boost::python::arg("id"), ::boost::python::arg("node") ) );
        
        }
        { //::osiris::IdeMailBox::getPageHref
        
            typedef ::osiris::String ( ::osiris::IdeMailBox::*getPageHref_function_type )( ::osiris::uint32 ) const;
            typedef ::osiris::String ( IdeMailBox_wrapper::*default_getPageHref_function_type )( ::osiris::uint32 ) const;
            
            IdeMailBox_exposer.def( 
                "getPageHref"
                , getPageHref_function_type(&::osiris::IdeMailBox::getPageHref)
                , default_getPageHref_function_type(&IdeMailBox_wrapper::default_getPageHref)
                , ( ::boost::python::arg("offset") ) );
        
        }
        { //::osiris::IdeMailBox::setFolder
        
            typedef void ( *setFolder_function_type )( ::osiris::IdeMailBox &,::osiris::MessageFolder );
            
            IdeMailBox_exposer.def( 
                "setFolder"
                , setFolder_function_type( &IdeMailBox_wrapper::setFolder )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("folder") ) );
        
        }
        { //::osiris::IdeMailBox::setMessage
        
            typedef void ( *setMessage_function_type )( ::osiris::IdeMailBox &,::osiris::UniqueID const & );
            
            IdeMailBox_exposer.def( 
                "setMessage"
                , setMessage_function_type( &IdeMailBox_wrapper::setMessage )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("message") ) );
        
        }
        { //::osiris::IdeMailBox::setOffset
        
            typedef void ( *setOffset_function_type )( ::osiris::IdeMailBox &,::osiris::uint32 );
            
            IdeMailBox_exposer.def( 
                "setOffset"
                , setOffset_function_type( &IdeMailBox_wrapper::setOffset )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("offset") ) );
        
        }
        { //::osiris::IdeMailBox::setSecure
        
            typedef void ( *setSecure_function_type )( ::osiris::IdeMailBox &,bool );
            
            IdeMailBox_exposer.def( 
                "setSecure"
                , setSecure_function_type( &IdeMailBox_wrapper::setSecure )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("secure") ) );
        
        }
        { //::osiris::IIdePageable::createPager
        
            typedef void ( IdeMailBox_wrapper::*createPager_function_type )( ::boost::shared_ptr< osiris::IdeSkin >,::boost::shared_ptr< osiris::XMLNode >,::osiris::uint32,::osiris::uint32,::osiris::uint32 ) ;
            
            IdeMailBox_exposer.def( 
                "createPager"
                , createPager_function_type( &IdeMailBox_wrapper::createPager )
                , ( ::boost::python::arg("skin"), ::boost::python::arg("node"), ::boost::python::arg("total_items"), ::boost::python::arg("items_to_show"), ::boost::python::arg("offset") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( IdeMailBox_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            IdeMailBox_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &IdeMailBox_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( IdeMailBox_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            IdeMailBox_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &IdeMailBox_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( IdeMailBox_wrapper::*default_getSession_function_type )(  ) const;
            
            IdeMailBox_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&IdeMailBox_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( IdeMailBox_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            IdeMailBox_exposer.def( 
                "onEvent"
                , onEvent_function_type( &IdeMailBox_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInit
        
            typedef ::osiris::IdeMailBox exported_class_t;
            typedef void ( IdeMailBox_wrapper::*onInit_function_type )(  ) ;
            
            IdeMailBox_exposer.def( 
                "onInit"
                , onInit_function_type( &IdeMailBox_wrapper::default_onInit ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInitStylesheet
        
            typedef ::osiris::IdeMailBox exported_class_t;
            typedef void ( IdeMailBox_wrapper::*onInitStylesheet_function_type )(  ) ;
            
            IdeMailBox_exposer.def( 
                "onInitStylesheet"
                , onInitStylesheet_function_type( &IdeMailBox_wrapper::default_onInitStylesheet ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( IdeMailBox_wrapper::*onLoad_function_type )(  ) ;
            
            IdeMailBox_exposer.def( 
                "onLoad"
                , onLoad_function_type( &IdeMailBox_wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( IdeMailBox_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            IdeMailBox_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &IdeMailBox_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::onRender
        
            typedef void ( IdeMailBox_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdeMailBox_exposer.def( 
                "onRender"
                , onRender_function_type( &IdeMailBox_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( IdeMailBox_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IdeMailBox_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &IdeMailBox_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IdeMailBox_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdeMailBox_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&IdeMailBox_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IdeMailBox_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdeMailBox_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&IdeMailBox_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( IdeMailBox_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IdeMailBox_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &IdeMailBox_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "folder"[fget=::osiris::IdeMailBox::getFolder, fset=::osiris::IdeMailBox::setFolder]
        
            typedef ::osiris::MessageFolder ( ::osiris::IdeMailBox::*fget )(  ) const;
            typedef void ( ::osiris::IdeMailBox::*fset )( ::osiris::MessageFolder ) ;
            
            IdeMailBox_exposer.add_property( 
                "folder"
                , fget( &::osiris::IdeMailBox::getFolder )
                , fset( &::osiris::IdeMailBox::setFolder )
                , "get\\set property, built on top of \"osiris::MessageFolder osiris::IdeMailBox::getFolder() const [member function]\" and \"void osiris::IdeMailBox::setFolder(osiris::MessageFolder folder) [member function]\"" );
        
        }
        { //property "message"[fget=::osiris::IdeMailBox::getMessage, fset=::osiris::IdeMailBox::setMessage]
        
            typedef ::osiris::UniqueID const & ( ::osiris::IdeMailBox::*fget )(  ) const;
            typedef void ( ::osiris::IdeMailBox::*fset )( ::osiris::UniqueID const & ) ;
            
            IdeMailBox_exposer.add_property( 
                "message"
                , ::boost::python::make_function( 
                      fget( &::osiris::IdeMailBox::getMessage )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::IdeMailBox::setMessage )
                , "get\\set property, built on top of \"osiris::UniqueID const & osiris::IdeMailBox::getMessage() const [member function]\" and \"void osiris::IdeMailBox::setMessage(osiris::UniqueID const & message) [member function]\"" );
        
        }
        { //property "offset"[fget=::osiris::IdeMailBox::getOffset, fset=::osiris::IdeMailBox::setOffset]
        
            typedef ::osiris::uint32 ( ::osiris::IdeMailBox::*fget )(  ) const;
            typedef void ( ::osiris::IdeMailBox::*fset )( ::osiris::uint32 ) ;
            
            IdeMailBox_exposer.add_property( 
                "offset"
                , fget( &::osiris::IdeMailBox::getOffset )
                , fset( &::osiris::IdeMailBox::setOffset )
                , "get\\set property, built on top of \"osiris::uint32 osiris::IdeMailBox::getOffset() const [member function]\" and \"void osiris::IdeMailBox::setOffset(osiris::uint32 offset) [member function]\"" );
        
        }
        { //property "secure"[fget=::osiris::IdeMailBox::getSecure, fset=::osiris::IdeMailBox::setSecure]
        
            typedef bool ( ::osiris::IdeMailBox::*fget )(  ) const;
            typedef void ( ::osiris::IdeMailBox::*fset )( bool ) ;
            
            IdeMailBox_exposer.add_property( 
                "secure"
                , fget( &::osiris::IdeMailBox::getSecure )
                , fset( &::osiris::IdeMailBox::setSecure )
                , "get\\set property, built on top of \"bool osiris::IdeMailBox::getSecure() const [member function]\" and \"void osiris::IdeMailBox::setSecure(bool secure) [member function]\"" );
        
        }
    }

}
