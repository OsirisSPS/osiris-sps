// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
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
#include "idepickerobjectex.h"
#include "idemailbox.h"
#include "idesearchresults.h"
#include "idetablequery.h"
#include "idesearchglobalparams.h"
#include "idesearchquery.h"
#include "ideassistant.h"
#include "iportalpagexslrendererhtmlcontrol.pypp.hpp"

namespace bp = boost::python;

struct IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper : ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >, ::osiris::PythonWrapper< ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > > {

    IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper( )
    : ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >( )
      , ::osiris::PythonWrapper< ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > >(){
        // nullptr constructor
    
    }

    static boost::python::object getDocument( ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::XMLDocument> result = inst.getDocument();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getStylesheet( ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::XMLStylesheet> result = inst.getStylesheet();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getTemplate( ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::HtmlXSLControl> result = inst.getTemplate();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual ::osiris::String getTemplatePath(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_getTemplatePath = this->get_override( "getTemplatePath" );
        return func_getTemplatePath(  );
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

void register_IPortalPageXSLRendererHtmlControl_class(){

    { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >
        typedef ::boost::python::class_< IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper, ::boost::python::bases< ::osiris::IPortalPageControl< osiris::IHtmlControl > >, ::boost::noncopyable > IPortalPageXSLRendererHtmlControl_exposer_t;
        IPortalPageXSLRendererHtmlControl_exposer_t IPortalPageXSLRendererHtmlControl_exposer = IPortalPageXSLRendererHtmlControl_exposer_t( "IPortalPageXSLRendererHtmlControl", ::boost::python::no_init );
        ::boost::python::scope IPortalPageXSLRendererHtmlControl_scope( IPortalPageXSLRendererHtmlControl_exposer );
        IPortalPageXSLRendererHtmlControl_exposer.def( ::boost::python::init< >() );
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::getDocument
        
            typedef ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > exported_class_t;
            typedef boost::python::object ( *getDocument_function_type )( ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> > const & );
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "getDocument"
                , getDocument_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::getDocument ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::getStylesheet
        
            typedef ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > exported_class_t;
            typedef boost::python::object ( *getStylesheet_function_type )( ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> > const & );
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "getStylesheet"
                , getStylesheet_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::getStylesheet ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::getTemplate
        
            typedef ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > exported_class_t;
            typedef boost::python::object ( *getTemplate_function_type )( ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> > const & );
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "getTemplate"
                , getTemplate_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::getTemplate ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::getTemplatePath
        
            typedef ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > exported_class_t;
            typedef ::osiris::String ( exported_class_t::*getTemplatePath_function_type )(  ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "getTemplatePath"
                , ( getTemplatePath_function_type(&::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::getTemplatePath) ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInitStylesheet
        
            typedef ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > exported_class_t;
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onInitStylesheet_function_type )(  ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onInitStylesheet"
                , onInitStylesheet_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onInitStylesheet ) );
        
        }
        { //::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > >::onInit
        
            typedef ::osiris::IXSLRenderer< osiris::IPortalPageControl< osiris::IHtmlControl > > exported_class_t;
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onInit_function_type )(  ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onInit"
                , onInit_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*default_getSession_function_type )(  ) const;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onEvent"
                , onEvent_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onLoad_function_type )(  ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onLoad"
                , onLoad_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::onPreRender
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onPreRender_function_type )(  ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onPreRender ) );
        
        }
        { //::osiris::IHtmlControl::onRender
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onRender"
                , onRender_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IPortalPageXSLRendererHtmlControl_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &IXSLRenderer_less__osiris_scope_IPortalPageControl_less__osiris_scope_IHtmlControl__greater___greater__wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "document"[fget=::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getDocument]
        
            typedef ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> > fget_class_t;
            
            typedef ::boost::shared_ptr<osiris::XMLDocument> ( fget_class_t::*fget )(  ) const;
            
            IPortalPageXSLRendererHtmlControl_exposer.add_property( 
                "document"
                , fget( &::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getDocument )
                , "get property, built on top of \"boost::shared_ptr<osiris::XMLDocument> osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getDocument() const [member function]\"" );
        
        }
        { //property "stylesheet"[fget=::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getStylesheet]
        
            typedef ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> > fget_class_t;
            
            typedef ::boost::shared_ptr<osiris::XMLStylesheet> ( fget_class_t::*fget )(  ) const;
            
            IPortalPageXSLRendererHtmlControl_exposer.add_property( 
                "stylesheet"
                , fget( &::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getStylesheet )
                , "get property, built on top of \"boost::shared_ptr<osiris::XMLStylesheet> osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getStylesheet() const [member function]\"" );
        
        }
        { //property "template"[fget=::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getTemplate]
        
            typedef ::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> > fget_class_t;
            
            typedef ::boost::shared_ptr<osiris::HtmlXSLControl> ( fget_class_t::*fget )(  ) const;
            
            IPortalPageXSLRendererHtmlControl_exposer.add_property( 
                "template"
                , fget( &::osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getTemplate )
                , "get property, built on top of \"boost::shared_ptr<osiris::HtmlXSLControl> osiris::IXSLRenderer<osiris::IPortalPageControl<osiris::IHtmlControl> >::getTemplate() const [member function]\"" );
        
        }
    }

}
