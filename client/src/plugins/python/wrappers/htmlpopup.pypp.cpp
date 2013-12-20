// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "htmlpopup.h"
#include "ideportalcontrol.h"
#include "datatree.h"
#include "htmlattributes.h"
#include "htmlevent.h"
#include "eventssource.h"
#include "htmlvirtualpage.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "htmlpopup.pypp.hpp"

namespace bp = boost::python;

struct HtmlPopup_wrapper : ::osiris::HtmlPopup, ::osiris::PythonWrapper< ::osiris::HtmlPopup > {

    HtmlPopup_wrapper( )
    : ::osiris::HtmlPopup( )
      , ::osiris::PythonWrapper< ::osiris::HtmlPopup >(){
        // nullptr constructor
    
    }

    static boost::python::object getType( ::osiris::HtmlPopup const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlPopup::Type result = inst.getType();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getHeader( ::osiris::HtmlPopup & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::HtmlDiv> result = inst.getHeader();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getBody( ::osiris::HtmlPopup & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::HtmlDiv> result = inst.getBody();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual void onPreRender(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onPreRender = this->get_override( "onPreRender" ) )
            func_onPreRender(  );
        else{
            __pystate.leave();
            this->::osiris::HtmlPopup::onPreRender(  );
        }
    }
    
    virtual void default_onPreRender(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlPopup::onPreRender( );
    }

    static void setType( ::osiris::HtmlPopup & inst, ::osiris::HtmlPopup::Type type ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setType(type);
        __pythreadSaver.restore();
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
            this->::osiris::IHtmlControl::onInit(  );
        }
    }
    
    virtual void default_onInit(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onInit( );
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
            this->::osiris::HtmlDiv::onRender( boost::ref(writer) );
        }
    }
    
    virtual void default_onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlDiv::onRender( boost::ref(writer) );
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

void register_HtmlPopup_class(){

    { //::osiris::HtmlPopup
        typedef ::boost::python::class_< HtmlPopup_wrapper, ::boost::python::bases< ::osiris::HtmlDiv >, ::boost::noncopyable > HtmlPopup_exposer_t;
        HtmlPopup_exposer_t HtmlPopup_exposer = HtmlPopup_exposer_t( "HtmlPopup", ::boost::python::init< >() );
        ::boost::python::scope HtmlPopup_scope( HtmlPopup_exposer );
        ::boost::python::enum_< ::osiris::HtmlPopup::Type>("Type")
            .value("ptStatic", ::osiris::HtmlPopup::ptStatic)
            .value("ptStaticClick", ::osiris::HtmlPopup::ptStaticClick)
            .value("ptTooltip", ::osiris::HtmlPopup::ptTooltip)
            .value("ptPopup", ::osiris::HtmlPopup::ptPopup)
            .export_values()
            ;
        { //::osiris::HtmlPopup::getType
        
            typedef boost::python::object ( *getType_function_type )( ::osiris::HtmlPopup const & );
            
            HtmlPopup_exposer.def( 
                "getType"
                , getType_function_type( &HtmlPopup_wrapper::getType ) );
        
        }
        { //::osiris::HtmlPopup::getHeader
        
            typedef boost::python::object ( *getHeader_function_type )( ::osiris::HtmlPopup & );
            
            HtmlPopup_exposer.def( 
                "getHeader"
                , getHeader_function_type( &HtmlPopup_wrapper::getHeader ) );
        
        }
        { //::osiris::HtmlPopup::getBody
        
            typedef boost::python::object ( *getBody_function_type )( ::osiris::HtmlPopup & );
            
            HtmlPopup_exposer.def( 
                "getBody"
                , getBody_function_type( &HtmlPopup_wrapper::getBody ) );
        
        }
        { //::osiris::HtmlPopup::onPreRender
        
            typedef void ( HtmlPopup_wrapper::*onPreRender_function_type )(  ) ;
            
            HtmlPopup_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &HtmlPopup_wrapper::default_onPreRender ) );
        
        }
        { //::osiris::HtmlPopup::setType
        
            typedef void ( *setType_function_type )( ::osiris::HtmlPopup &,::osiris::HtmlPopup::Type );
            
            HtmlPopup_exposer.def( 
                "setType"
                , setType_function_type( &HtmlPopup_wrapper::setType )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("type") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( HtmlPopup_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            HtmlPopup_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &HtmlPopup_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( HtmlPopup_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            HtmlPopup_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &HtmlPopup_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( HtmlPopup_wrapper::*default_getSession_function_type )(  ) const;
            
            HtmlPopup_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&HtmlPopup_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( HtmlPopup_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            HtmlPopup_exposer.def( 
                "onEvent"
                , onEvent_function_type( &HtmlPopup_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onInit
        
            typedef void ( HtmlPopup_wrapper::*onInit_function_type )(  ) ;
            
            HtmlPopup_exposer.def( 
                "onInit"
                , onInit_function_type( &HtmlPopup_wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( HtmlPopup_wrapper::*onLoad_function_type )(  ) ;
            
            HtmlPopup_exposer.def( 
                "onLoad"
                , onLoad_function_type( &HtmlPopup_wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( HtmlPopup_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            HtmlPopup_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &HtmlPopup_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::HtmlDiv::onRender
        
            typedef void ( HtmlPopup_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlPopup_exposer.def( 
                "onRender"
                , onRender_function_type( &HtmlPopup_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( HtmlPopup_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlPopup_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &HtmlPopup_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlPopup_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlPopup_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&HtmlPopup_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlPopup_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlPopup_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&HtmlPopup_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( HtmlPopup_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlPopup_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &HtmlPopup_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "type"[fget=::osiris::HtmlPopup::getType, fset=::osiris::HtmlPopup::setType]
        
            typedef ::osiris::HtmlPopup::Type ( ::osiris::HtmlPopup::*fget )(  ) const;
            typedef void ( ::osiris::HtmlPopup::*fset )( ::osiris::HtmlPopup::Type ) ;
            
            HtmlPopup_exposer.add_property( 
                "type"
                , fget( &::osiris::HtmlPopup::getType )
                , fset( &::osiris::HtmlPopup::setType )
                , "get\\set property, built on top of \"osiris::HtmlPopup::Type osiris::HtmlPopup::getType() const [member function]\" and \"void osiris::HtmlPopup::setType(osiris::HtmlPopup::Type type) [member function]\"" );
        
        }
    }

}
