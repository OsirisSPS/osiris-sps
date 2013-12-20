// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
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
#include "idepickerobject.h"
#include "idehelpbox.h"
#include "idebutton.h"
#include "ideomleditor.h"
#include "ideblock.h"
#include "idepickerdatetime.h"
#include "idepickernumber.h"
#include "idepickerdatetimespecial.h"
#include "idetablequery.h"
#include "idesearchglobalparams.h"
#include "ideassistant.h"
#include "idesearchquery.h"
#include "idepickerobjectex.h"
#include "idemailbox.h"
#include "idesearchresults.h"
#include "idevoteeditor.h"
#include "idepickercolor.h"
#include "ipagehtmlcontrol.pypp.hpp"

namespace bp = boost::python;

struct IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper : ::osiris::IPageControl< osiris::IHtmlControl >, ::osiris::PythonWrapper< ::osiris::IPageControl< osiris::IHtmlControl > > {

    IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper( )
    : ::osiris::IPageControl<osiris::IHtmlControl>( )
      , ::osiris::PythonWrapper< ::osiris::IPageControl< osiris::IHtmlControl > >(){
        // null constructor
    
    }

    static boost::python::object getPage( ::osiris::IPageControl< osiris::IHtmlControl > const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IPage> result = inst.getPage();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getSkin( ::osiris::IPageControl< osiris::IHtmlControl > const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IdeSkin> result = inst.getSkin();
        __pythreadSaver.restore();
        return boost::python::object( result );
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

    virtual void onPreRender(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onPreRender = this->get_override( "onPreRender" ) )
            func_onPreRender(  );
        else{
            __pystate.leave();
            this->::osiris::IHtmlControl::onPreRender(  );
        }
    }
    
    virtual void default_onPreRender(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlControl::onPreRender( );
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

void register_IPageHtmlControl_class(){

    { //::osiris::IPageControl< osiris::IHtmlControl >
        typedef ::boost::python::class_< IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper, ::boost::python::bases< ::osiris::IHtmlControl >, ::boost::noncopyable > IPageHtmlControl_exposer_t;
        IPageHtmlControl_exposer_t IPageHtmlControl_exposer = IPageHtmlControl_exposer_t( "IPageHtmlControl", ::boost::python::init< >() );
        ::boost::python::scope IPageHtmlControl_scope( IPageHtmlControl_exposer );
        { //::osiris::IPageControl< osiris::IHtmlControl >::getPage
        
            typedef ::osiris::IPageControl< osiris::IHtmlControl > exported_class_t;
            typedef boost::python::object ( *getPage_function_type )( ::osiris::IPageControl<osiris::IHtmlControl> const & );
            
            IPageHtmlControl_exposer.def( 
                "getPage"
                , getPage_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::getPage ) );
        
        }
        { //::osiris::IPageControl< osiris::IHtmlControl >::getSkin
        
            typedef ::osiris::IPageControl< osiris::IHtmlControl > exported_class_t;
            typedef boost::python::object ( *getSkin_function_type )( ::osiris::IPageControl<osiris::IHtmlControl> const & );
            
            IPageHtmlControl_exposer.def( 
                "getSkin"
                , getSkin_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::getSkin ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            IPageHtmlControl_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            IPageHtmlControl_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*default_getSession_function_type )(  ) const;
            
            IPageHtmlControl_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            IPageHtmlControl_exposer.def( 
                "onEvent"
                , onEvent_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onInit
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onInit_function_type )(  ) ;
            
            IPageHtmlControl_exposer.def( 
                "onInit"
                , onInit_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onLoad_function_type )(  ) ;
            
            IPageHtmlControl_exposer.def( 
                "onLoad"
                , onLoad_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            IPageHtmlControl_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::onPreRender
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onPreRender_function_type )(  ) ;
            
            IPageHtmlControl_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onPreRender ) );
        
        }
        { //::osiris::IHtmlControl::onRender
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            IPageHtmlControl_exposer.def( 
                "onRender"
                , onRender_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IPageHtmlControl_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            IPageHtmlControl_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            IPageHtmlControl_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IPageHtmlControl_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &IPageControl_less__osiris_scope_IHtmlControl__greater__wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "page"[fget=::osiris::IPageControl<osiris::IHtmlControl>::getPage]
        
            typedef ::osiris::IPageControl<osiris::IHtmlControl> fget_class_t;
            
            typedef ::boost::shared_ptr<osiris::IPage> ( fget_class_t::*fget )(  ) const;
            
            IPageHtmlControl_exposer.add_property( 
                "page"
                , fget( &::osiris::IPageControl<osiris::IHtmlControl>::getPage )
                , "get property, built on top of \"boost::shared_ptr<osiris::IPage> osiris::IPageControl<osiris::IHtmlControl>::getPage() const [member function]\"" );
        
        }
        { //property "skin"[fget=::osiris::IPageControl<osiris::IHtmlControl>::getSkin]
        
            typedef ::osiris::IPageControl<osiris::IHtmlControl> fget_class_t;
            
            typedef ::boost::shared_ptr<osiris::IdeSkin> ( fget_class_t::*fget )(  ) const;
            
            IPageHtmlControl_exposer.add_property( 
                "skin"
                , fget( &::osiris::IPageControl<osiris::IHtmlControl>::getSkin )
                , "get property, built on top of \"boost::shared_ptr<osiris::IdeSkin> osiris::IPageControl<osiris::IHtmlControl>::getSkin() const [member function]\"" );
        
        }
    }

}
