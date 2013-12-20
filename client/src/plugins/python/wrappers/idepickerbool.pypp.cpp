// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "idepickerbool.h"
#include "datatree.h"
#include "htmlattributes.h"
#include "htmlevent.h"
#include "eventssource.h"
#include "htmlvirtualpage.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "idepickerbool.pypp.hpp"

namespace bp = boost::python;

struct IdePickerBool_wrapper : ::osiris::IdePickerBool, ::osiris::PythonWrapper< ::osiris::IdePickerBool > {

    IdePickerBool_wrapper( )
    : ::osiris::IdePickerBool( )
      , ::osiris::PythonWrapper< ::osiris::IdePickerBool >(){
        // nullptr constructor
    
    }

    static boost::python::object getAutoPostBack( ::osiris::IdePickerBool const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool const & result = inst.getAutoPostBack();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, bool const & >( result ) );
    }

    static boost::python::object getCheck( ::osiris::IdePickerBool const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.getCheck();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual void onPreRender(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onPreRender = this->get_override( "onPreRender" ) )
            func_onPreRender(  );
        else{
            __pystate.leave();
            this->::osiris::IdePickerBool::onPreRender(  );
        }
    }
    
    void default_onPreRender(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdePickerBool::onPreRender( );
    }

    virtual void renderAttributes( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_renderAttributes = this->get_override( "renderAttributes" ) )
            func_renderAttributes( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::IdePickerBool::renderAttributes( boost::ref(writer) );
        }
    }
    
    void default_renderAttributes( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdePickerBool::renderAttributes( boost::ref(writer) );
    }

    static void setAutoPostBack( ::osiris::IdePickerBool & inst, bool const & autoPostBack ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setAutoPostBack(autoPostBack);
        __pythreadSaver.restore();
    }

    static void setCheck( ::osiris::IdePickerBool & inst, bool check ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setCheck(check);
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
            this->::osiris::IHtmlInput::onInit(  );
        }
    }
    
    virtual void default_onInit(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlInput::onInit( );
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
            this->::osiris::IHtmlInput::onRender( boost::ref(writer) );
        }
    }
    
    virtual void default_onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IHtmlInput::onRender( boost::ref(writer) );
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

void register_IdePickerBool_class(){

    { //::osiris::IdePickerBool
        typedef ::boost::python::class_< IdePickerBool_wrapper, ::boost::python::bases< ::osiris::IHtmlInput >, ::boost::noncopyable > IdePickerBool_exposer_t;
        IdePickerBool_exposer_t IdePickerBool_exposer = IdePickerBool_exposer_t( "IdePickerBool", ::boost::python::init< >() );
        ::boost::python::scope IdePickerBool_scope( IdePickerBool_exposer );
        { //::osiris::IdePickerBool::getAutoPostBack
        
            typedef boost::python::object ( *getAutoPostBack_function_type )( ::osiris::IdePickerBool const & );
            
            IdePickerBool_exposer.def( 
                "getAutoPostBack"
                , getAutoPostBack_function_type( &IdePickerBool_wrapper::getAutoPostBack ) );
        
        }
        { //::osiris::IdePickerBool::getCheck
        
            typedef boost::python::object ( *getCheck_function_type )( ::osiris::IdePickerBool const & );
            
            IdePickerBool_exposer.def( 
                "getCheck"
                , getCheck_function_type( &IdePickerBool_wrapper::getCheck ) );
        
        }
        { //::osiris::IdePickerBool::onPreRender
        
            typedef void ( ::osiris::IdePickerBool::*onPreRender_function_type )(  ) ;
            typedef void ( IdePickerBool_wrapper::*default_onPreRender_function_type )(  ) ;
            
            IdePickerBool_exposer.def( 
                "onPreRender"
                , onPreRender_function_type(&::osiris::IdePickerBool::onPreRender)
                , default_onPreRender_function_type(&IdePickerBool_wrapper::default_onPreRender) );
        
        }
        { //::osiris::IdePickerBool::renderAttributes
        
            typedef void ( ::osiris::IdePickerBool::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IdePickerBool_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdePickerBool_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IdePickerBool::renderAttributes)
                , default_renderAttributes_function_type(&IdePickerBool_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IdePickerBool::setAutoPostBack
        
            typedef void ( *setAutoPostBack_function_type )( ::osiris::IdePickerBool &,bool const & );
            
            IdePickerBool_exposer.def( 
                "setAutoPostBack"
                , setAutoPostBack_function_type( &IdePickerBool_wrapper::setAutoPostBack )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("autoPostBack") ) );
        
        }
        { //::osiris::IdePickerBool::setCheck
        
            typedef void ( *setCheck_function_type )( ::osiris::IdePickerBool &,bool );
            
            IdePickerBool_exposer.def( 
                "setCheck"
                , setCheck_function_type( &IdePickerBool_wrapper::setCheck )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("check") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( IdePickerBool_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            IdePickerBool_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &IdePickerBool_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( IdePickerBool_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            IdePickerBool_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &IdePickerBool_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( IdePickerBool_wrapper::*default_getSession_function_type )(  ) const;
            
            IdePickerBool_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&IdePickerBool_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( IdePickerBool_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            IdePickerBool_exposer.def( 
                "onEvent"
                , onEvent_function_type( &IdePickerBool_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlInput::onInit
        
            typedef void ( IdePickerBool_wrapper::*onInit_function_type )(  ) ;
            
            IdePickerBool_exposer.def( 
                "onInit"
                , onInit_function_type( &IdePickerBool_wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( IdePickerBool_wrapper::*onLoad_function_type )(  ) ;
            
            IdePickerBool_exposer.def( 
                "onLoad"
                , onLoad_function_type( &IdePickerBool_wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( IdePickerBool_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            IdePickerBool_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &IdePickerBool_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlInput::onRender
        
            typedef void ( IdePickerBool_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdePickerBool_exposer.def( 
                "onRender"
                , onRender_function_type( &IdePickerBool_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( IdePickerBool_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IdePickerBool_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &IdePickerBool_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( IdePickerBool_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            IdePickerBool_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&IdePickerBool_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( IdePickerBool_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            IdePickerBool_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &IdePickerBool_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "autoPostBack"[fget=::osiris::IdePickerBool::getAutoPostBack, fset=::osiris::IdePickerBool::setAutoPostBack]
        
            typedef bool const & ( ::osiris::IdePickerBool::*fget )(  ) const;
            typedef void ( ::osiris::IdePickerBool::*fset )( bool const & ) ;
            
            IdePickerBool_exposer.add_property( 
                "autoPostBack"
                , ::boost::python::make_function( 
                      fget( &::osiris::IdePickerBool::getAutoPostBack )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::IdePickerBool::setAutoPostBack )
                , "get\\set property, built on top of \"bool const & osiris::IdePickerBool::getAutoPostBack() const [member function]\" and \"void osiris::IdePickerBool::setAutoPostBack(bool const & autoPostBack) [member function]\"" );
        
        }
        { //property "check"[fget=::osiris::IdePickerBool::getCheck, fset=::osiris::IdePickerBool::setCheck]
        
            typedef bool ( ::osiris::IdePickerBool::*fget )(  ) const;
            typedef void ( ::osiris::IdePickerBool::*fset )( bool ) ;
            
            IdePickerBool_exposer.add_property( 
                "check"
                , fget( &::osiris::IdePickerBool::getCheck )
                , fset( &::osiris::IdePickerBool::setCheck )
                , "get\\set property, built on top of \"bool osiris::IdePickerBool::getCheck() const [member function]\" and \"void osiris::IdePickerBool::setCheck(bool check) [member function]\"" );
        
        }
    }

}
