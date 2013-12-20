// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "htmlpager.h"
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
#include "htmlpager.pypp.hpp"

namespace bp = boost::python;

struct HtmlPager_wrapper : ::osiris::HtmlPager, ::osiris::PythonWrapper< ::osiris::HtmlPager > {

    HtmlPager_wrapper( )
    : ::osiris::HtmlPager( )
      , ::osiris::PythonWrapper< ::osiris::HtmlPager >(){
        // nullptr constructor
    
    }

    static boost::python::object getOptions( ::osiris::HtmlPager const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint32 const & result = inst.getOptions();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::uint32 const & >( result ) );
    }

    static boost::python::object getCurrent( ::osiris::HtmlPager const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint32 const & result = inst.getCurrent();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::uint32 const & >( result ) );
    }

    static boost::python::object getCount( ::osiris::HtmlPager const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::uint32 const & result = inst.getCount();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::uint32 const & >( result ) );
    }

    static boost::python::object addPage( ::osiris::HtmlPager & inst, ::osiris::String const & href=(osiris::String::EMPTY) ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::HtmlPager::Entry> result = inst.addPage(href);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual ::osiris::String formatName( ::osiris::uint32 index, ::boost::shared_ptr< osiris::HtmlPager::Entry > const & entry ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_formatName = this->get_override( "formatName" ) )
            return func_formatName( index, entry );
        else{
            __pystate.leave();
            return this->::osiris::HtmlPager::formatName( index, entry );
        }
    }
    
    virtual ::osiris::String default_formatName( ::osiris::uint32 index, ::boost::shared_ptr< osiris::HtmlPager::Entry > const & entry ) const {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::HtmlPager::formatName( index, entry );
    }

    virtual void onLoad(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onLoad = this->get_override( "onLoad" ) )
            func_onLoad(  );
        else{
            __pystate.leave();
            this->::osiris::HtmlPager::onLoad(  );
        }
    }
    
    virtual void default_onLoad(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::HtmlPager::onLoad( );
    }

    static boost::python::object getOption( ::osiris::HtmlPager const & inst, ::osiris::uint32 option ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.getOption(option);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static void setCount( ::osiris::HtmlPager & inst, ::osiris::uint32 const & count ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setCount(count);
        __pythreadSaver.restore();
    }

    static void setCurrent( ::osiris::HtmlPager & inst, ::osiris::uint32 const & current ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setCurrent(current);
        __pythreadSaver.restore();
    }

    static void setOption( ::osiris::HtmlPager & inst, ::osiris::uint32 option ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setOption(option);
        __pythreadSaver.restore();
    }

    static void setOptions( ::osiris::HtmlPager & inst, ::osiris::uint32 const & options ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setOptions(options);
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

static boost::python::object getCaption_e8832718161e519a7d005cfc69bb1c01( ::osiris::HtmlPager::Entry const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::String const & result = inst.getCaption();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
}

static boost::python::object getHref_b997f85a6635371d8f3fb90e874ae976( ::osiris::HtmlPager::Entry const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::String const & result = inst.getHref();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
}

static void setCaption_75f3eba7b5342cc7ae3b4a7339051427( ::osiris::HtmlPager::Entry & inst, ::osiris::String const & caption ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.setCaption(caption);
    __pythreadSaver.restore();
}

static void setHref_dc4e97ce709c4352a66fcf8ad7ec7f35( ::osiris::HtmlPager::Entry & inst, ::osiris::String const & href ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.setHref(href);
    __pythreadSaver.restore();
}

void register_HtmlPager_class(){

    { //::osiris::HtmlPager
        typedef ::boost::python::class_< HtmlPager_wrapper, ::boost::python::bases< ::osiris::HtmlDiv >, ::boost::noncopyable > HtmlPager_exposer_t;
        HtmlPager_exposer_t HtmlPager_exposer = HtmlPager_exposer_t( "HtmlPager", ::boost::python::init< >() );
        ::boost::python::scope HtmlPager_scope( HtmlPager_exposer );
        ::boost::python::enum_< ::osiris::HtmlPager::Format>("Format")
            .value("pfPagesNumbers", ::osiris::HtmlPager::pfPagesNumbers)
            .value("pfPagesCaptions", ::osiris::HtmlPager::pfPagesCaptions)
            .export_values()
            ;
        ::boost::python::enum_< ::osiris::HtmlPager::Options>("Options")
            .value("poShowFirstPage", ::osiris::HtmlPager::poShowFirstPage)
            .value("poShowPrevPage", ::osiris::HtmlPager::poShowPrevPage)
            .value("poShowNextPage", ::osiris::HtmlPager::poShowNextPage)
            .value("poShowLastPage", ::osiris::HtmlPager::poShowLastPage)
            .value("poShowAll", ::osiris::HtmlPager::poShowAll)
            .export_values()
            ;
        { //::osiris::HtmlPager::Entry
            typedef ::boost::python::class_< ::osiris::HtmlPager::Entry, ::boost::noncopyable > Entry_exposer_t;
            Entry_exposer_t Entry_exposer = Entry_exposer_t( "Entry", ::boost::python::init< ::osiris::String const & >(( ::boost::python::arg("href") )) );
            ::boost::python::scope Entry_scope( Entry_exposer );
            ::boost::python::implicitly_convertible< ::osiris::String const &, ::osiris::HtmlPager::Entry >();
            { //::osiris::HtmlPager::Entry::getCaption
            
                typedef boost::python::object ( *getCaption_function_type )( ::osiris::HtmlPager::Entry const & );
                
                Entry_exposer.def( 
                    "getCaption"
                    , getCaption_function_type( &getCaption_e8832718161e519a7d005cfc69bb1c01 ) );
            
            }
            { //::osiris::HtmlPager::Entry::getHref
            
                typedef boost::python::object ( *getHref_function_type )( ::osiris::HtmlPager::Entry const & );
                
                Entry_exposer.def( 
                    "getHref"
                    , getHref_function_type( &getHref_b997f85a6635371d8f3fb90e874ae976 ) );
            
            }
            { //::osiris::HtmlPager::Entry::setCaption
            
                typedef void ( *setCaption_function_type )( ::osiris::HtmlPager::Entry &,::osiris::String const & );
                
                Entry_exposer.def( 
                    "setCaption"
                    , setCaption_function_type( &setCaption_75f3eba7b5342cc7ae3b4a7339051427 )
                    , ( ::boost::python::arg("inst"), ::boost::python::arg("caption") ) );
            
            }
            { //::osiris::HtmlPager::Entry::setHref
            
                typedef void ( *setHref_function_type )( ::osiris::HtmlPager::Entry &,::osiris::String const & );
                
                Entry_exposer.def( 
                    "setHref"
                    , setHref_function_type( &setHref_dc4e97ce709c4352a66fcf8ad7ec7f35 )
                    , ( ::boost::python::arg("inst"), ::boost::python::arg("href") ) );
            
            }
            { //property "caption"[fget=::osiris::HtmlPager::Entry::getCaption, fset=::osiris::HtmlPager::Entry::setCaption]
            
                typedef ::osiris::String const & ( ::osiris::HtmlPager::Entry::*fget )(  ) const;
                typedef void ( ::osiris::HtmlPager::Entry::*fset )( ::osiris::String const & ) ;
                
                Entry_exposer.add_property( 
                    "caption"
                    , ::boost::python::make_function( 
                          fget( &::osiris::HtmlPager::Entry::getCaption )
                        , bp::return_value_policy< bp::copy_const_reference >() ) 
                    , fset( &::osiris::HtmlPager::Entry::setCaption )
                    , "get\\set property, built on top of \"osiris::String const & osiris::HtmlPager::Entry::getCaption() const [member function]\" and \"void osiris::HtmlPager::Entry::setCaption(osiris::String const & caption) [member function]\"" );
            
            }
            { //property "href"[fget=::osiris::HtmlPager::Entry::getHref, fset=::osiris::HtmlPager::Entry::setHref]
            
                typedef ::osiris::String const & ( ::osiris::HtmlPager::Entry::*fget )(  ) const;
                typedef void ( ::osiris::HtmlPager::Entry::*fset )( ::osiris::String const & ) ;
                
                Entry_exposer.add_property( 
                    "href"
                    , ::boost::python::make_function( 
                          fget( &::osiris::HtmlPager::Entry::getHref )
                        , bp::return_value_policy< bp::copy_const_reference >() ) 
                    , fset( &::osiris::HtmlPager::Entry::setHref )
                    , "get\\set property, built on top of \"osiris::String const & osiris::HtmlPager::Entry::getHref() const [member function]\" and \"void osiris::HtmlPager::Entry::setHref(osiris::String const & href) [member function]\"" );
            
            }
            ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::HtmlPager::Entry > >();
            ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlPager::Entry >, boost::shared_ptr< ::osiris::Object > >();
            ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlPager::Entry >, boost::shared_ptr< ::osiris::AllocWrapper< osiris::Object, osiris::SysAllocator< osiris::Object > > > >();
        }
        { //::osiris::HtmlPager::getOptions
        
            typedef boost::python::object ( *getOptions_function_type )( ::osiris::HtmlPager const & );
            
            HtmlPager_exposer.def( 
                "getOptions"
                , getOptions_function_type( &HtmlPager_wrapper::getOptions ) );
        
        }
        { //::osiris::HtmlPager::getCurrent
        
            typedef boost::python::object ( *getCurrent_function_type )( ::osiris::HtmlPager const & );
            
            HtmlPager_exposer.def( 
                "getCurrent"
                , getCurrent_function_type( &HtmlPager_wrapper::getCurrent ) );
        
        }
        { //::osiris::HtmlPager::getCount
        
            typedef boost::python::object ( *getCount_function_type )( ::osiris::HtmlPager const & );
            
            HtmlPager_exposer.def( 
                "getCount"
                , getCount_function_type( &HtmlPager_wrapper::getCount ) );
        
        }
        { //::osiris::HtmlPager::addPage
        
            typedef boost::python::object ( *addPage_function_type )( ::osiris::HtmlPager &,::osiris::String const & );
            
            HtmlPager_exposer.def( 
                "addPage"
                , addPage_function_type( &HtmlPager_wrapper::addPage )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("href")=(osiris::String::EMPTY) ) );
        
        }
        { //::osiris::HtmlPager::formatName
        
            typedef ::osiris::String ( HtmlPager_wrapper::*formatName_function_type )( ::osiris::uint32,::boost::shared_ptr< osiris::HtmlPager::Entry > const & ) const;
            
            HtmlPager_exposer.def( 
                "formatName"
                , formatName_function_type( &HtmlPager_wrapper::default_formatName )
                , ( ::boost::python::arg("index"), ::boost::python::arg("entry") ) );
        
        }
        { //::osiris::HtmlPager::onLoad
        
            typedef void ( HtmlPager_wrapper::*onLoad_function_type )(  ) ;
            
            HtmlPager_exposer.def( 
                "onLoad"
                , onLoad_function_type( &HtmlPager_wrapper::default_onLoad ) );
        
        }
        { //::osiris::HtmlPager::getOption
        
            typedef boost::python::object ( *getOption_function_type )( ::osiris::HtmlPager const &,::osiris::uint32 );
            
            HtmlPager_exposer.def( 
                "getOption"
                , getOption_function_type( &HtmlPager_wrapper::getOption )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("option") ) );
        
        }
        { //::osiris::HtmlPager::setCount
        
            typedef void ( *setCount_function_type )( ::osiris::HtmlPager &,::osiris::uint32 const & );
            
            HtmlPager_exposer.def( 
                "setCount"
                , setCount_function_type( &HtmlPager_wrapper::setCount )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("count") ) );
        
        }
        { //::osiris::HtmlPager::setCurrent
        
            typedef void ( *setCurrent_function_type )( ::osiris::HtmlPager &,::osiris::uint32 const & );
            
            HtmlPager_exposer.def( 
                "setCurrent"
                , setCurrent_function_type( &HtmlPager_wrapper::setCurrent )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("current") ) );
        
        }
        { //::osiris::HtmlPager::setOption
        
            typedef void ( *setOption_function_type )( ::osiris::HtmlPager &,::osiris::uint32 );
            
            HtmlPager_exposer.def( 
                "setOption"
                , setOption_function_type( &HtmlPager_wrapper::setOption )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("option") ) );
        
        }
        { //::osiris::HtmlPager::setOptions
        
            typedef void ( *setOptions_function_type )( ::osiris::HtmlPager &,::osiris::uint32 const & );
            
            HtmlPager_exposer.def( 
                "setOptions"
                , setOptions_function_type( &HtmlPager_wrapper::setOptions )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("options") ) );
        
        }
        { //::osiris::IHtmlControl::decodeEvent
        
            typedef bool ( HtmlPager_wrapper::*decodeEvent_function_type )( ::osiris::String const &,::osiris::String &,::osiris::HtmlEvent & ) const;
            
            HtmlPager_exposer.def( 
                "decodeEvent"
                , decodeEvent_function_type( &HtmlPager_wrapper::decodeEvent )
                , ( ::boost::python::arg("command"), ::boost::python::arg("eventName"), ::boost::python::arg("e") ) );
        
        }
        { //::osiris::IHtmlControl::encodeEvent
        
            typedef ::osiris::String ( HtmlPager_wrapper::*encodeEvent_function_type )( ::osiris::String const &,::osiris::HtmlEvent const * ) const;
            
            HtmlPager_exposer.def( 
                "encodeEvent"
                , encodeEvent_function_type( &HtmlPager_wrapper::encodeEvent )
                , ( ::boost::python::arg("eventName"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::getSession
        
            typedef ::boost::shared_ptr< osiris::HttpSession > ( ::osiris::IHtmlControl::*getSession_function_type )(  ) const;
            typedef ::boost::shared_ptr< osiris::HttpSession > ( HtmlPager_wrapper::*default_getSession_function_type )(  ) const;
            
            HtmlPager_exposer.def( 
                "getSession"
                , getSession_function_type(&::osiris::IHtmlControl::getSession)
                , default_getSession_function_type(&HtmlPager_wrapper::default_getSession) );
        
        }
        { //::osiris::IHtmlControl::onEvent
        
            typedef void ( HtmlPager_wrapper::*onEvent_function_type )( ::osiris::String const &,::osiris::IEvent * ) ;
            
            HtmlPager_exposer.def( 
                "onEvent"
                , onEvent_function_type( &HtmlPager_wrapper::default_onEvent )
                , ( ::boost::python::arg("name"), ::boost::python::arg("e")=(nullptr) ) );
        
        }
        { //::osiris::IHtmlControl::onInit
        
            typedef void ( HtmlPager_wrapper::*onInit_function_type )(  ) ;
            
            HtmlPager_exposer.def( 
                "onInit"
                , onInit_function_type( &HtmlPager_wrapper::default_onInit ) );
        
        }
        { //::osiris::IHtmlControl::onLoadViewState
        
            typedef void ( HtmlPager_wrapper::*onLoadViewState_function_type )( ::osiris::DataTree const & ) ;
            
            HtmlPager_exposer.def( 
                "onLoadViewState"
                , onLoadViewState_function_type( &HtmlPager_wrapper::default_onLoadViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::onPreRender
        
            typedef void ( HtmlPager_wrapper::*onPreRender_function_type )(  ) ;
            
            HtmlPager_exposer.def( 
                "onPreRender"
                , onPreRender_function_type( &HtmlPager_wrapper::default_onPreRender ) );
        
        }
        { //::osiris::HtmlDiv::onRender
        
            typedef void ( HtmlPager_wrapper::*onRender_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlPager_exposer.def( 
                "onRender"
                , onRender_function_type( &HtmlPager_wrapper::default_onRender )
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::onSaveViewState
        
            typedef void ( HtmlPager_wrapper::*onSaveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlPager_exposer.def( 
                "onSaveViewState"
                , onSaveViewState_function_type( &HtmlPager_wrapper::default_onSaveViewState )
                , ( ::boost::python::arg("state") ) );
        
        }
        { //::osiris::IHtmlControl::renderAttributes
        
            typedef void ( ::osiris::IHtmlControl::*renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlPager_wrapper::*default_renderAttributes_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlPager_exposer.def( 
                "renderAttributes"
                , renderAttributes_function_type(&::osiris::IHtmlControl::renderAttributes)
                , default_renderAttributes_function_type(&HtmlPager_wrapper::default_renderAttributes)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::renderChilds
        
            typedef void ( ::osiris::IHtmlControl::*renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            typedef void ( HtmlPager_wrapper::*default_renderChilds_function_type )( ::osiris::HtmlWriter & ) ;
            
            HtmlPager_exposer.def( 
                "renderChilds"
                , renderChilds_function_type(&::osiris::IHtmlControl::renderChilds)
                , default_renderChilds_function_type(&HtmlPager_wrapper::default_renderChilds)
                , ( ::boost::python::arg("writer") ) );
        
        }
        { //::osiris::IHtmlControl::saveViewState
        
            typedef void ( HtmlPager_wrapper::*saveViewState_function_type )( ::osiris::DataTree & ) ;
            
            HtmlPager_exposer.def( 
                "saveViewState"
                , saveViewState_function_type( &HtmlPager_wrapper::saveViewState )
                , ( ::boost::python::arg("states") ) );
        
        }
        { //property "options"[fget=::osiris::HtmlPager::getOptions, fset=::osiris::HtmlPager::setOptions]
        
            typedef ::osiris::uint32 const & ( ::osiris::HtmlPager::*fget )(  ) const;
            typedef void ( ::osiris::HtmlPager::*fset )( ::osiris::uint32 const & ) ;
            
            HtmlPager_exposer.add_property( 
                "options"
                , ::boost::python::make_function( 
                      fget( &::osiris::HtmlPager::getOptions )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::HtmlPager::setOptions )
                , "get\\set property, built on top of \"osiris::uint32 const & osiris::HtmlPager::getOptions() const [member function]\" and \"void osiris::HtmlPager::setOptions(osiris::uint32 const & options) [member function]\"" );
        
        }
        { //property "current"[fget=::osiris::HtmlPager::getCurrent, fset=::osiris::HtmlPager::setCurrent]
        
            typedef ::osiris::uint32 const & ( ::osiris::HtmlPager::*fget )(  ) const;
            typedef void ( ::osiris::HtmlPager::*fset )( ::osiris::uint32 const & ) ;
            
            HtmlPager_exposer.add_property( 
                "current"
                , ::boost::python::make_function( 
                      fget( &::osiris::HtmlPager::getCurrent )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::HtmlPager::setCurrent )
                , "get\\set property, built on top of \"osiris::uint32 const & osiris::HtmlPager::getCurrent() const [member function]\" and \"void osiris::HtmlPager::setCurrent(osiris::uint32 const & current) [member function]\"" );
        
        }
        { //property "count"[fget=::osiris::HtmlPager::getCount, fset=::osiris::HtmlPager::setCount]
        
            typedef ::osiris::uint32 const & ( ::osiris::HtmlPager::*fget )(  ) const;
            typedef void ( ::osiris::HtmlPager::*fset )( ::osiris::uint32 const & ) ;
            
            HtmlPager_exposer.add_property( 
                "count"
                , ::boost::python::make_function( 
                      fget( &::osiris::HtmlPager::getCount )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::HtmlPager::setCount )
                , "get\\set property, built on top of \"osiris::uint32 const & osiris::HtmlPager::getCount() const [member function]\" and \"void osiris::HtmlPager::setCount(osiris::uint32 const & count) [member function]\"" );
        
        }
    }

}
