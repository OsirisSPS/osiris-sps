// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "htmltooltip.h"
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
#include "htmltooltip.pypp.hpp"

namespace bp = boost::python;

struct HtmlTooltip_wrapper : ::osiris::HtmlTooltip, ::osiris::PythonWrapper< ::osiris::HtmlTooltip > {

    HtmlTooltip_wrapper( )
    : ::osiris::HtmlTooltip( )
      , ::osiris::PythonWrapper< ::osiris::HtmlTooltip >(){
        // nullptr constructor
    
    }

    static boost::python::object getType( ::osiris::HtmlTooltip const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTooltip::Type result = inst.getType();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getConnect( ::osiris::HtmlTooltip const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IHtmlControl> result = inst.getConnect();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getValue( ::osiris::HtmlTooltip const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.getValue();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual void onLoad(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onLoad = this->get_override( "onLoad" ) )
            func_onLoad(  );
        else{
            __pystate.leave();
            this->::osiris::HtmlTooltip::onLoad(  );
        }
    }
    
    virtual void default_onLoad(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTooltip::onLoad( );
    }

    virtual void onPreRender(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onPreRender = this->get_override( "onPreRender" ) )
            func_onPreRender(  );
        else{
            __pystate.leave();
            this->::osiris::HtmlTooltip::onPreRender(  );
        }
    }
    
    virtual void default_onPreRender(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTooltip::onPreRender( );
    }

    static void setConnect( ::osiris::HtmlTooltip & inst, ::boost::shared_ptr< osiris::IHtmlControl > connect ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setConnect(connect);
        __pythreadSaver.restore();
    }

    static void setType( ::osiris::HtmlTooltip & inst, ::osiris::HtmlTooltip::Type type ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setType(type);
        __pythreadSaver.restore();
    }

    static void setValue( ::osiris::HtmlTooltip & inst, ::osiris::String const & value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setValue(value);
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

void register_HtmlTooltip_class(){

    { //::osiris::HtmlTooltip
        typedef ::boost::python::class_< HtmlTooltip_wrapper, ::boost::python::bases< ::osiris::HtmlDiv >, ::boost::noncopyable > HtmlTooltip_exposer_t;
        HtmlTooltip_exposer_t HtmlTooltip_exposer = HtmlTooltip_exposer_t( "HtmlTooltip", ::boost::python::no_init );
        ::boost::python::scope HtmlTooltip_scope( HtmlTooltip_exposer );
        ::boost::python::enum_< ::osiris::HtmlTooltip::Type>("Type")
            .value("ttInfo", ::osiris::HtmlTooltip::ttInfo)
            .value("ttWarning", ::osiris::HtmlTooltip::ttWarning)
            .export_values()
            ;
        HtmlTooltip_exposer.def( ::boost::python::init< >() );
        { //::osiris::HtmlTooltip::getType
        
            typedef boost::python::object ( *getType_function_type )( ::osiris::HtmlTooltip const & );
            
            HtmlTooltip_exposer.def( 
                "getType"
                , getType_function_type( &HtmlTooltip_wrapper::getType ) );
        
        }
        { //::osiris::HtmlTooltip::getConnect
        
            typedef boost::python::object ( *getConnect_function_type )( ::osiris::HtmlTooltip const & );
            
            HtmlTooltip_exposer.def( 
                "getConnect"
                , getConnect_function_type( &HtmlTooltip_wrapper::getConnect ) );
        
        }
        { //::osiris::HtmlTooltip::getValue
        
            typedef boost::python::object ( *getValue_function_type )( ::osiris::HtmlTooltip const & );
            
            HtmlTooltip_exposer.def( 
                "getValue"
                , getValue_function_type( &HtmlTooltip_wrapper::getValue ) );
        
        }
        { //::osiris::HtmlTooltip::onLoad
        
            typedef void ( HtmlTooltip_wrapper::*onLoad_function_type )(  ) ;
            
            HtmlTooltip_exposer.def( 
                "onLoad"
                , onLoad_function_type( &HtmlTooltip_wrapper::default_onLoad ) );
        
        }
        { //::osiris::HtmlTooltip::onPreRender
        
            typedef void ( HtmlTooltip_wrapper::*onPreRender_function_type )(  ) ;
            
            HtmlTooltip_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &HtmlTooltip_wrapper::default_onPreRender ) );
        
        }
        { //::osiris::HtmlTooltip::setConnect
        
            typedef void ( *setConnect_function_type )( ::osiris::HtmlTooltip &,::boost::shared_ptr<osiris::IHtmlControl> );
            
            HtmlTooltip_exposer.def( 
                "setConnect"
                , setConnect_function_type( &HtmlTooltip_wrapper::setConnect )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("connect") ) );
        
        }
        { //::osiris::HtmlTooltip::setType
        
            typedef void ( *setType_function_type )( ::osiris::HtmlTooltip &,::osiris::HtmlTooltip::Type );
            
            HtmlTooltip_exposer.def( 
                "setType"
                , setType_function_type( &HtmlTooltip_wrapper::setType )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("type") ) );
        
        }
        { //::osiris::HtmlTooltip::setValue
        
            typedef void ( *setValue_function_type )( ::osiris::HtmlTooltip &,::osiris::String const & );
            
            HtmlTooltip_exposer.def( 
                "setValue"
                , setValue_function_type( &HtmlTooltip_wrapper::setValue )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( HtmlTooltip_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            HtmlTooltip_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &HtmlTooltip_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( HtmlTooltip_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            HtmlTooltip_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &HtmlTooltip_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( HtmlTooltip_wrapper::*default_getSession_function_type )(  ) const;
            
            HtmlTooltip_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&HtmlTooltip_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( HtmlTooltip_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            HtmlTooltip_exposer.def( 
                "onEvent"
                , onEvent_function_type( &HtmlTooltip_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onInit
        
            typedef void ( HtmlTooltip_wrapper::*onInit_function_type )(  ) ;
            
            HtmlTooltip_exposer.def( 
                "onInit"
                , onInit_function_type( &HtmlTooltip_wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( HtmlTooltip_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            HtmlTooltip_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &HtmlTooltip_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::HtmlDiv::onRender
        
            typedef void ( HtmlTooltip_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTooltip_exposer.def( 
                "onRender"
                , onRender_function_type( &HtmlTooltip_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( HtmlTooltip_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlTooltip_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &HtmlTooltip_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlTooltip_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTooltip_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&HtmlTooltip_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlTooltip_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTooltip_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&HtmlTooltip_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( HtmlTooltip_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlTooltip_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &HtmlTooltip_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "type"[fget=::osiris::HtmlTooltip::getType, fset=::osiris::HtmlTooltip::setType]
        
            typedef ::osiris::HtmlTooltip::Type ( ::osiris::HtmlTooltip::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTooltip::*fset )( ::osiris::HtmlTooltip::Type ) ;
            
            HtmlTooltip_exposer.add_property( 
                "type"
                , fget( &::osiris::HtmlTooltip::getType )
                , fset( &::osiris::HtmlTooltip::setType )
                , "get\\set property, built on top of \"osiris::HtmlTooltip::Type osiris::HtmlTooltip::getType() const [member function]\" and \"void osiris::HtmlTooltip::setType(osiris::HtmlTooltip::Type type) [member function]\"" );
        
        }
        { //property "connect"[fget=::osiris::HtmlTooltip::getConnect, fset=::osiris::HtmlTooltip::setConnect]
        
            typedef ::boost::shared_ptr<osiris::IHtmlControl> ( ::osiris::HtmlTooltip::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTooltip::*fset )( ::boost::shared_ptr<osiris::IHtmlControl> ) ;
            
            HtmlTooltip_exposer.add_property( 
                "connect"
                , fget( &::osiris::HtmlTooltip::getConnect )
                , fset( &::osiris::HtmlTooltip::setConnect )
                , "get\\set property, built on top of \"boost::shared_ptr<osiris::IHtmlControl> osiris::HtmlTooltip::getConnect() const [member function]\" and \"void osiris::HtmlTooltip::setConnect(boost::shared_ptr<osiris::IHtmlControl> connect) [member function]\"" );
        
        }
        { //property "value"[fget=::osiris::HtmlTooltip::getValue, fset=::osiris::HtmlTooltip::setValue]
        
            typedef ::osiris::String ( ::osiris::HtmlTooltip::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTooltip::*fset )( ::osiris::String const & ) ;
            
            HtmlTooltip_exposer.add_property( 
                "value"
                , fget( &::osiris::HtmlTooltip::getValue )
                , fset( &::osiris::HtmlTooltip::setValue )
                , "get\\set property, built on top of \"osiris::String osiris::HtmlTooltip::getValue() const [member function]\" and \"void osiris::HtmlTooltip::setValue(osiris::String const & value) [member function]\"" );
        
        }
    }

}
