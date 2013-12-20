// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "htmltableheader.h"
#include "datatree.h"
#include "htmlattributes.h"
#include "htmlevent.h"
#include "eventssource.h"
#include "htmlvirtualpage.h"
#include "htmlwriter.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpsession.h"
#include "htmltableheader.pypp.hpp"

namespace bp = boost::python;

struct HtmlTableHeader_wrapper : ::osiris::HtmlTableHeader, ::osiris::PythonWrapper< ::osiris::HtmlTableHeader > {

    HtmlTableHeader_wrapper(::osiris::String const & caption=osiris::String::EMPTY )
    : ::osiris::HtmlTableHeader( boost::ref(caption) )
      , ::osiris::PythonWrapper< ::osiris::HtmlTableHeader >(){
        // constructor
    
    }

    static boost::python::object getCaption( ::osiris::HtmlTableHeader const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String const & result = inst.getCaption();
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
            this->::osiris::HtmlTableHeader::onInit(  );
        }
    }
    
    virtual void default_onInit(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTableHeader::onInit( );
    }

    virtual void onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onRender = this->get_override( "onRender" ) )
            func_onRender( boost::ref(writer) );
        else{
            __pystate.leave();
            this->::osiris::HtmlTableHeader::onRender( boost::ref(writer) );
        }
    }
    
    virtual void default_onRender( ::osiris::HtmlWriter & writer ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlTableHeader::onRender( boost::ref(writer) );
    }

    static void setCaption( ::osiris::HtmlTableHeader & inst, ::osiris::String const & caption ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setCaption(caption);
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

void register_HtmlTableHeader_class(){

    { //::osiris::HtmlTableHeader
        typedef ::boost::python::class_< HtmlTableHeader_wrapper, ::boost::python::bases< ::osiris::HtmlTableCell >, ::boost::noncopyable > HtmlTableHeader_exposer_t;
        HtmlTableHeader_exposer_t HtmlTableHeader_exposer = HtmlTableHeader_exposer_t( "HtmlTableHeader", ::boost::python::init< ::boost::python::optional< ::osiris::String const & > >(( ::boost::python::arg("caption")=osiris::String::EMPTY )) );
        ::boost::python::scope HtmlTableHeader_scope( HtmlTableHeader_exposer );
        ::boost::python::implicitly_convertible< ::osiris::String const &, ::osiris::HtmlTableHeader >();
        { //::osiris::HtmlTableHeader::getCaption
        
            typedef boost::python::object ( *getCaption_function_type )( ::osiris::HtmlTableHeader const & );
            
            HtmlTableHeader_exposer.def( 
                "getCaption"
                , getCaption_function_type( &HtmlTableHeader_wrapper::getCaption ) );
        
        }
        { //::osiris::HtmlTableHeader::onInit
        
            typedef void ( HtmlTableHeader_wrapper::*onInit_function_type )(  ) ;
            
            HtmlTableHeader_exposer.def( 
                "onInit"
                , onInit_function_type( &HtmlTableHeader_wrapper::default_onInit ) );
        
        }
        { //::osiris::HtmlTableHeader::onRender
        
            typedef void ( HtmlTableHeader_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTableHeader_exposer.def( 
                "onRender"
                , onRender_function_type( &HtmlTableHeader_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::HtmlTableHeader::setCaption
        
            typedef void ( *setCaption_function_type )( ::osiris::HtmlTableHeader &,::osiris::String const & );
            
            HtmlTableHeader_exposer.def( 
                "setCaption"
                , setCaption_function_type( &HtmlTableHeader_wrapper::setCaption )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("caption") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( HtmlTableHeader_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            HtmlTableHeader_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &HtmlTableHeader_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( HtmlTableHeader_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            HtmlTableHeader_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &HtmlTableHeader_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( HtmlTableHeader_wrapper::*default_getSession_function_type )(  ) const;
            
            HtmlTableHeader_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&HtmlTableHeader_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( HtmlTableHeader_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            HtmlTableHeader_exposer.def( 
                "onEvent"
                , onEvent_function_type( &HtmlTableHeader_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onLoad
        
            typedef void ( HtmlTableHeader_wrapper::*onLoad_function_type )(  ) ;
            
            HtmlTableHeader_exposer.def( 
                "onLoad"
                , onLoad_function_type( &HtmlTableHeader_wrapper::default_onLoad ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( HtmlTableHeader_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            HtmlTableHeader_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &HtmlTableHeader_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::onPreRender
        
            typedef void ( HtmlTableHeader_wrapper::*onPreRender_function_type )(  ) ;
            
            HtmlTableHeader_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &HtmlTableHeader_wrapper::default_onPreRender ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( HtmlTableHeader_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlTableHeader_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &HtmlTableHeader_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlTableHeader_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTableHeader_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&HtmlTableHeader_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlTableHeader_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlTableHeader_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&HtmlTableHeader_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( HtmlTableHeader_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlTableHeader_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &HtmlTableHeader_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "caption"[fget=::osiris::HtmlTableHeader::getCaption, fset=::osiris::HtmlTableHeader::setCaption]
        
            typedef ::osiris::String const & ( ::osiris::HtmlTableHeader::*fget )(  ) const;
            typedef void ( ::osiris::HtmlTableHeader::*fset )( ::osiris::String const & ) ;
            
            HtmlTableHeader_exposer.add_property( 
                "caption"
                , ::boost::python::make_function( 
                      fget( &::osiris::HtmlTableHeader::getCaption )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::HtmlTableHeader::setCaption )
                , "get\\set property, built on top of \"osiris::String const & osiris::HtmlTableHeader::getCaption() const [member function]\" and \"void osiris::HtmlTableHeader::setCaption(osiris::String const & caption) [member function]\"" );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::HtmlTableHeader > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlTableHeader >, boost::shared_ptr< ::boost::noncopyable_::noncopyable > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlTableHeader >, boost::shared_ptr< ::boost::signals::trackable > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlTableHeader >, boost::shared_ptr< ::osiris::enable_this_ptr< osiris::IHtmlControl > > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlTableHeader >, boost::shared_ptr< ::osiris::Object > >();
    }

}
