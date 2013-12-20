// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "htmltextarea.h"
#include "datatree.h"
#include "htmlattributes.h"
#include "htmlevent.h"
#include "eventssource.h"
#include "htmlvirtualpage.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "htmltextarea.pypp.hpp"

namespace bp = boost::python;

struct HtmlTextArea_wrapper : ::osiris::HtmlTextArea, ::osiris::PythonWrapper< ::osiris::HtmlTextArea > {

    HtmlTextArea_wrapper( )
    : ::osiris::HtmlTextArea( )
      , ::osiris::PythonWrapper< ::osiris::HtmlTextArea >(){
        // null constructor
    
    }

    static boost::python::object getColumns( ::osiris::HtmlTextArea const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint32 result = inst.getColumns();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getRows( ::osiris::HtmlTextArea const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint32 result = inst.getRows();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getValue( ::osiris::HtmlTextArea const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String const & result = inst.getValue();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
    }

    virtual void onInit(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onInit = this->get_override( "onInit" ) )
            func_onInit(  );
        else{
            __pystate.leave();
            this->::osiris::HtmlTextArea::onInit(  );
        }
    }
    
    virtual void default_onInit(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTextArea::onInit( );
    }

    virtual void onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onRender = this->get_override( "onRender" ) )
            func_onRender( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::HtmlTextArea::onRender( boost::ref(writer) );
        }
    }
    
    virtual void default_onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTextArea::onRender( boost::ref(writer) );
    }

    virtual void renderAttributes( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_renderAttributes = this->get_override( "renderAttributes" ) )
            func_renderAttributes( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::HtmlTextArea::renderAttributes( boost::ref(writer) );
        }
    }
    
    void default_renderAttributes( ::osiris::HtmlWriter & writer ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTextArea::renderAttributes( boost::ref(writer) );
    }

    static void setColumns( ::osiris::HtmlTextArea & inst, ::osiris::uint32 const & value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setColumns(value);
        __pythreadSaver.restore();
    }

    static void setRows( ::osiris::HtmlTextArea & inst, ::osiris::uint32 const & value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setRows(value);
        __pythreadSaver.restore();
    }

    static void setValue( ::osiris::HtmlTextArea & inst, ::osiris::String const & text ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setValue(text);
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

void register_HtmlTextArea_class(){

    { //::osiris::HtmlTextArea
        typedef ::boost::python::class_< HtmlTextArea_wrapper, ::boost::python::bases< ::osiris::IHtmlControl >, ::boost::noncopyable > HtmlTextArea_exposer_t;
        HtmlTextArea_exposer_t HtmlTextArea_exposer = HtmlTextArea_exposer_t( "HtmlTextArea", ::boost::python::no_init );
        ::boost::python::scope HtmlTextArea_scope( HtmlTextArea_exposer );
        HtmlTextArea_exposer.def( ::boost::python::init< >() );
        { //::osiris::HtmlTextArea::getColumns
        
            typedef boost::python::object ( *getColumns_function_type )( ::osiris::HtmlTextArea const & );
            
            HtmlTextArea_exposer.def( 
                "getColumns"
                , getColumns_function_type( &HtmlTextArea_wrapper::getColumns ) );
        
        }
        { //::osiris::HtmlTextArea::getRows
        
            typedef boost::python::object ( *getRows_function_type )( ::osiris::HtmlTextArea const & );
            
            HtmlTextArea_exposer.def( 
                "getRows"
                , getRows_function_type( &HtmlTextArea_wrapper::getRows ) );
        
        }
        { //::osiris::HtmlTextArea::getValue
        
            typedef boost::python::object ( *getValue_function_type )( ::osiris::HtmlTextArea const & );
            
            HtmlTextArea_exposer.def( 
                "getValue"
                , getValue_function_type( &HtmlTextArea_wrapper::getValue ) );
        
        }
        { //::osiris::HtmlTextArea::onInit
        
            typedef void ( HtmlTextArea_wrapper::*onInit_function_type )(  ) ;
            
            HtmlTextArea_exposer.def( 
                "onInit"
                , onInit_function_type( &HtmlTextArea_wrapper::default_onInit ) );
        
        }
        { //::osiris::HtmlTextArea::onRender
        
            typedef void ( HtmlTextArea_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTextArea_exposer.def( 
                "onRender"
                , onRender_function_type( &HtmlTextArea_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::HtmlTextArea::renderAttributes
        
            typedef void ( ::osiris::HtmlTextArea::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlTextArea_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTextArea_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::HtmlTextArea::renderAttributes)
                , default_renderAttributes_function_type(&HtmlTextArea_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::HtmlTextArea::setColumns
        
            typedef void ( *setColumns_function_type )( ::osiris::HtmlTextArea &,::osiris::uint32 const & );
            
            HtmlTextArea_exposer.def( 
                "setColumns"
                , setColumns_function_type( &HtmlTextArea_wrapper::setColumns )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::HtmlTextArea::setRows
        
            typedef void ( *setRows_function_type )( ::osiris::HtmlTextArea &,::osiris::uint32 const & );
            
            HtmlTextArea_exposer.def( 
                "setRows"
                , setRows_function_type( &HtmlTextArea_wrapper::setRows )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::HtmlTextArea::setValue
        
            typedef void ( *setValue_function_type )( ::osiris::HtmlTextArea &,::osiris::String const & );
            
            HtmlTextArea_exposer.def( 
                "setValue"
                , setValue_function_type( &HtmlTextArea_wrapper::setValue )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("text") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( HtmlTextArea_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            HtmlTextArea_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &HtmlTextArea_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( HtmlTextArea_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            HtmlTextArea_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &HtmlTextArea_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( HtmlTextArea_wrapper::*default_getSession_function_type )(  ) const;
            
            HtmlTextArea_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&HtmlTextArea_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( HtmlTextArea_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            HtmlTextArea_exposer.def( 
                "onEvent"
                , onEvent_function_type( &HtmlTextArea_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( HtmlTextArea_wrapper::*onLoad_function_type )(  ) ;
            
            HtmlTextArea_exposer.def( 
                "onLoad"
                , onLoad_function_type( &HtmlTextArea_wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( HtmlTextArea_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            HtmlTextArea_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &HtmlTextArea_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::onPreRender
        
            typedef void ( HtmlTextArea_wrapper::*onPreRender_function_type )(  ) ;
            
            HtmlTextArea_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &HtmlTextArea_wrapper::default_onPreRender ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( HtmlTextArea_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlTextArea_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &HtmlTextArea_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlTextArea_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTextArea_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&HtmlTextArea_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( HtmlTextArea_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlTextArea_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &HtmlTextArea_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "columns"[fget=::osiris::HtmlTextArea::getColumns, fset=::osiris::HtmlTextArea::setColumns]
        
            typedef ::osiris::uint32 ( ::osiris::HtmlTextArea::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTextArea::*fset )( ::osiris::uint32 const & ) ;
            
            HtmlTextArea_exposer.add_property( 
                "columns"
                , fget( &::osiris::HtmlTextArea::getColumns )
                , fset( &::osiris::HtmlTextArea::setColumns )
                , "get\\set property, built on top of \"osiris::uint32 osiris::HtmlTextArea::getColumns() const [member function]\" and \"void osiris::HtmlTextArea::setColumns(osiris::uint32 const & value) [member function]\"" );
        
        }
        { //property "rows"[fget=::osiris::HtmlTextArea::getRows, fset=::osiris::HtmlTextArea::setRows]
        
            typedef ::osiris::uint32 ( ::osiris::HtmlTextArea::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTextArea::*fset )( ::osiris::uint32 const & ) ;
            
            HtmlTextArea_exposer.add_property( 
                "rows"
                , fget( &::osiris::HtmlTextArea::getRows )
                , fset( &::osiris::HtmlTextArea::setRows )
                , "get\\set property, built on top of \"osiris::uint32 osiris::HtmlTextArea::getRows() const [member function]\" and \"void osiris::HtmlTextArea::setRows(osiris::uint32 const & value) [member function]\"" );
        
        }
        { //property "value"[fget=::osiris::HtmlTextArea::getValue, fset=::osiris::HtmlTextArea::setValue]
        
            typedef ::osiris::String const & ( ::osiris::HtmlTextArea::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTextArea::*fset )( ::osiris::String const & ) ;
            
            HtmlTextArea_exposer.add_property( 
                "value"
                , ::boost::python::make_function( 
                      fget( &::osiris::HtmlTextArea::getValue )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::HtmlTextArea::setValue )
                , "get\\set property, built on top of \"osiris::String const & osiris::HtmlTextArea::getValue() const [member function]\" and \"void osiris::HtmlTextArea::setValue(osiris::String const & text) [member function]\"" );
        
        }
    }

}
