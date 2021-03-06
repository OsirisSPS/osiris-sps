// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "idepickerculture.h"
#include "ideskin.h"
#include "idesession.h"
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
#include "idepickerculture.pypp.hpp"

namespace bp = boost::python;

struct IdePickerCulture_wrapper : ::osiris::IdePickerCulture, ::osiris::PythonWrapper< ::osiris::IdePickerCulture > {

    IdePickerCulture_wrapper( )
    : ::osiris::IdePickerCulture( )
      , ::osiris::PythonWrapper< ::osiris::IdePickerCulture >(){
        // null constructor
    
    }

    static boost::python::object getEventSelect( ::osiris::IdePickerCulture const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::EventSource> result = inst.getEventSelect();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual ::osiris::String getValue(  ) const  {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getValue = this->get_override( "getValue" ) )
            return func_getValue(  );
        else{
            __pystate.leave();
            return this->::osiris::IdePickerCulture::getValue(  );
        }
    }
    
    ::osiris::String default_getValue(  ) const  {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IdePickerCulture::getValue( );
    }

    static boost::python::object getShowSystem( ::osiris::IdePickerCulture const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.getShowSystem();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    void addCulture( ::boost::shared_ptr< osiris::XMLNode > node, ::boost::shared_ptr< osiris::LanguageCulture > culture, ::osiris::String const & current, ::osiris::uint32 level ){
        ::osiris::IdePickerCulture::addCulture( node, culture, boost::ref(current), level );
    }

    virtual void onLoad(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onLoad = this->get_override( "onLoad" ) )
            func_onLoad(  );
        else{
            __pystate.leave();
            this->::osiris::IdePickerCulture::onLoad(  );
        }
    }
    
    virtual void default_onLoad(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdePickerCulture::onLoad( );
    }

    virtual void onPreRender(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onPreRender = this->get_override( "onPreRender" ) )
            func_onPreRender(  );
        else{
            __pystate.leave();
            this->::osiris::IdePickerCulture::onPreRender(  );
        }
    }
    
    virtual void default_onPreRender(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdePickerCulture::onPreRender( );
    }

    void onSelect( ::osiris::IEvent * e ){
        ::osiris::IdePickerCulture::onSelect( boost::python::ptr(e) );
    }

    static void setShowSystem( ::osiris::IdePickerCulture & inst, bool const showSystem ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setShowSystem(showSystem);
        __pythreadSaver.restore();
    }

    virtual void setValue( ::osiris::String const & id ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_setValue = this->get_override( "setValue" ) )
            func_setValue( boost::ref(id) );
        else{
            __pystate.leave();
            this->::osiris::IdePickerCulture::setValue( boost::ref(id) );
        }
    }
    
    void default_setValue( ::osiris::String const & id ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdePickerCulture::setValue( boost::ref(id) );
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

void register_IdePickerCulture_class(){

    { //::osiris::IdePickerCulture
        typedef ::boost::python::class_< IdePickerCulture_wrapper, ::boost::python::bases< ::osiris::IPageControl< osiris::HtmlDiv > >, ::boost::noncopyable > IdePickerCulture_exposer_t;
        IdePickerCulture_exposer_t IdePickerCulture_exposer = IdePickerCulture_exposer_t( "IdePickerCulture", ::boost::python::init< >() );
        ::boost::python::scope IdePickerCulture_scope( IdePickerCulture_exposer );
        { //::osiris::IdePickerCulture::getEventSelect
        
            typedef boost::python::object ( *getEventSelect_function_type )( ::osiris::IdePickerCulture const & );
            
            IdePickerCulture_exposer.def( 
                "getEventSelect"
                , getEventSelect_function_type( &IdePickerCulture_wrapper::getEventSelect ) );
        
        }
        { //::osiris::IdePickerCulture::getValue
        
            typedef ::osiris::String ( ::osiris::IdePickerCulture::*getValue_function_type )(  ) const;
            typedef ::osiris::String ( IdePickerCulture_wrapper::*default_getValue_function_type )(  ) const;
            
            IdePickerCulture_exposer.def( 
                "getValue"
                , getValue_function_type(&::osiris::IdePickerCulture::getValue)
                , default_getValue_function_type(&IdePickerCulture_wrapper::default_getValue) );
        
        }
        { //::osiris::IdePickerCulture::getShowSystem
        
            typedef boost::python::object ( *getShowSystem_function_type )( ::osiris::IdePickerCulture const & );
            
            IdePickerCulture_exposer.def( 
                "getShowSystem"
                , getShowSystem_function_type( &IdePickerCulture_wrapper::getShowSystem ) );
        
        }
        { //::osiris::IdePickerCulture::addCulture
        
            typedef void ( IdePickerCulture_wrapper::*addCulture_function_type )( ::boost::shared_ptr< osiris::XMLNode >,::boost::shared_ptr< osiris::LanguageCulture >,::osiris::String const &,::osiris::uint32 ) ;
            
            IdePickerCulture_exposer.def( 
                "addCulture"
                , addCulture_function_type( &IdePickerCulture_wrapper::addCulture )
                , ( ::boost::python::arg("node"), ::boost::python::arg("culture"), ::boost::python::arg("current"), ::boost::python::arg("level") ) );
        
        }
        { //::osiris::IdePickerCulture::onLoad
        
            typedef void ( IdePickerCulture_wrapper::*onLoad_function_type )(  ) ;
            
            IdePickerCulture_exposer.def( 
                "onLoad"
                , onLoad_function_type( &IdePickerCulture_wrapper::default_onLoad ) );
        
        }
        { //::osiris::IdePickerCulture::onPreRender
        
            typedef void ( IdePickerCulture_wrapper::*onPreRender_function_type )(  ) ;
            
            IdePickerCulture_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &IdePickerCulture_wrapper::default_onPreRender ) );
        
        }
        { //::osiris::IdePickerCulture::onSelect
        
            typedef void ( IdePickerCulture_wrapper::*onSelect_function_type )( ::osiris::IEvent * ) ;
            
            IdePickerCulture_exposer.def( 
                "onSelect"
                , onSelect_function_type( &IdePickerCulture_wrapper::onSelect )
                , ( ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IdePickerCulture::setShowSystem
        
            typedef void ( *setShowSystem_function_type )( ::osiris::IdePickerCulture &,bool const );
            
            IdePickerCulture_exposer.def( 
                "setShowSystem"
                , setShowSystem_function_type( &IdePickerCulture_wrapper::setShowSystem )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("showSystem") ) );
        
        }
        { //::osiris::IdePickerCulture::setValue
        
            typedef void ( ::osiris::IdePickerCulture::*setValue_function_type )( ::osiris::String const & ) ;
            typedef void ( IdePickerCulture_wrapper::*default_setValue_function_type )( ::osiris::String const & ) ;
            
            IdePickerCulture_exposer.def( 
                "setValue"
                , setValue_function_type(&::osiris::IdePickerCulture::setValue)
                , default_setValue_function_type(&IdePickerCulture_wrapper::default_setValue)
                , ( ::boost::python::arg("id") ) );
        
        }
        IdePickerCulture_exposer.def_readonly( "EVENT_ONSELECT", ::osiris::IdePickerCulture::EVENT_ONSELECT );
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( IdePickerCulture_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            IdePickerCulture_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &IdePickerCulture_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( IdePickerCulture_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            IdePickerCulture_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &IdePickerCulture_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( IdePickerCulture_wrapper::*default_getSession_function_type )(  ) const;
            
            IdePickerCulture_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&IdePickerCulture_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( IdePickerCulture_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            IdePickerCulture_exposer.def( 
                "onEvent"
                , onEvent_function_type( &IdePickerCulture_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onInit
        
            typedef void ( IdePickerCulture_wrapper::*onInit_function_type )(  ) ;
            
            IdePickerCulture_exposer.def( 
                "onInit"
                , onInit_function_type( &IdePickerCulture_wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( IdePickerCulture_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            IdePickerCulture_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &IdePickerCulture_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::HtmlDiv::onRender
        
            typedef void ( IdePickerCulture_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdePickerCulture_exposer.def( 
                "onRender"
                , onRender_function_type( &IdePickerCulture_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( IdePickerCulture_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IdePickerCulture_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &IdePickerCulture_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IdePickerCulture_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdePickerCulture_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&IdePickerCulture_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IdePickerCulture_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdePickerCulture_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&IdePickerCulture_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( IdePickerCulture_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IdePickerCulture_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &IdePickerCulture_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "value"[fget=::osiris::IdePickerCulture::getValue, fset=::osiris::IdePickerCulture::setValue]
        
            typedef ::osiris::String ( ::osiris::IdePickerCulture::*fget )(  ) const;
            typedef void ( ::osiris::IdePickerCulture::*fset )( ::osiris::String const & ) ;
            
            IdePickerCulture_exposer.add_property( 
                "value"
                , fget( &::osiris::IdePickerCulture::getValue )
                , fset( &::osiris::IdePickerCulture::setValue )
                , "get\\set property, built on top of \"osiris::String osiris::IdePickerCulture::getValue() const [member function]\" and \"void osiris::IdePickerCulture::setValue(osiris::String const & id) [member function]\"" );
        
        }
        { //property "eventSelect"[fget=::osiris::IdePickerCulture::getEventSelect]
        
            typedef ::boost::shared_ptr<osiris::EventSource> ( ::osiris::IdePickerCulture::*fget )(  ) const;
            
            IdePickerCulture_exposer.add_property( 
                "eventSelect"
                , fget( &::osiris::IdePickerCulture::getEventSelect )
                , "get property, built on top of \"boost::shared_ptr<osiris::EventSource> osiris::IdePickerCulture::getEventSelect() const [member function]\"" );
        
        }
        { //property "showSystem"[fget=::osiris::IdePickerCulture::getShowSystem]
        
            typedef bool ( ::osiris::IdePickerCulture::*fget )(  ) const;
            
            IdePickerCulture_exposer.add_property( 
                "showSystem"
                , fget( &::osiris::IdePickerCulture::getShowSystem )
                , "get property, built on top of \"bool osiris::IdePickerCulture::getShowSystem() const [member function]\"" );
        
        }
    }

}
