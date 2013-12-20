// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "idesystem.h"
#include "ideskin.h"
#include "ihttpserver.h"
#include "idesystem.pypp.hpp"

namespace bp = boost::python;

struct IdeSystem_wrapper : ::osiris::IdeSystem, ::osiris::PythonWrapper< ::osiris::IdeSystem > {

    IdeSystem_wrapper( )
    : ::osiris::IdeSystem( )
      , ::osiris::PythonWrapper< ::osiris::IdeSystem >(){
        // nullptr constructor
    
    }

    static boost::python::object getServer( ::osiris::IdeSystem const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IHttpServer> result = inst.getServer();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getDefaultSkin( ::osiris::IdeSystem const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IdeSkin> result = inst.getDefaultSkin();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getAvailableSkins( ::osiris::IdeSystem const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::LockPtr<const osiris::ordered_map<osiris::UniqueID, boost::shared_ptr<osiris::IdeSkin> >,boost::recursive_mutex,boost::unique_lock<boost::recursive_mutex> > result = inst.getAvailableSkins();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getLocalUrl( ::osiris::IdeSystem const & inst, ::osiris::String const & relativeUrl=(osiris::String::EMPTY) ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.getLocalUrl(relativeUrl);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getHtdocsPath( ::osiris::IdeSystem const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.getHtdocsPath();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getEventBeforePageRender( ::osiris::IdeSystem const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::EventSource> result = inst.getEventBeforePageRender();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getEventAfterPageRender( ::osiris::IdeSystem const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::EventSource> result = inst.getEventAfterPageRender();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object createSession( ::osiris::IdeSystem & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IdeSession> result = inst.createSession();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual ::osiris::String getName(  ) const  {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getName = this->get_override( "getName" ) )
            return func_getName(  );
        else{
            __pystate.leave();
            return this->::osiris::IdeSystem::getName(  );
        }
    }
    
    ::osiris::String default_getName(  ) const  {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IdeSystem::getName( );
    }

    virtual void stop(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_stop = this->get_override( "stop" ) )
            func_stop(  );
        else{
            __pystate.leave();
            this->::osiris::IdeSystem::stop(  );
        }
    }
    
    void default_stop(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IdeSystem::stop( );
    }

    static boost::python::object getSession( ::osiris::IdeSystem const & inst, ::osiris::String const & id ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IdeSession> result = inst.getSession(id);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getSkin( ::osiris::IdeSystem const & inst, ::osiris::UniqueID const & id, bool load=true ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IdeSkin> result = inst.getSkin(id, load);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object isAccountLogged( ::osiris::IdeSystem & inst, ::osiris::String const & id ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.isAccountLogged(id);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object isTrustedDomain( ::osiris::IdeSystem & inst, ::std::string const & domain ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.isTrustedDomain(domain);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static void logoutSessions( ::osiris::IdeSystem & inst, ::osiris::String const & id ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.logoutSessions(id);
        __pythreadSaver.restore();
    }

    static void registerSkin( ::osiris::IdeSystem & inst, ::boost::shared_ptr< osiris::IdeSkin > skin ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.registerSkin(skin);
        __pythreadSaver.restore();
    }

    static boost::python::object removeAccount( ::osiris::IdeSystem & inst, ::osiris::String const & id ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.removeAccount(id);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual bool start( bool recovery ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_start = this->get_override( "start" ) )
            return func_start( recovery );
        else{
            __pystate.leave();
            return this->::osiris::IdeSystem::start( recovery );
        }
    }
    
    bool default_start( bool recovery ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::IdeSystem::start( recovery );
    }

    static void trustDomain( ::osiris::IdeSystem & inst, ::osiris::String const & domain ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.trustDomain(domain);
        __pythreadSaver.restore();
    }

    static boost::python::object trustDomainFromUrl( ::osiris::IdeSystem & inst, ::std::string const & url ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.trustDomainFromUrl(url);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static void unregisterSkin( ::osiris::IdeSystem & inst, ::boost::shared_ptr< osiris::IdeSkin > skin ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.unregisterSkin(skin);
        __pythreadSaver.restore();
    }

};

static boost::python::object getHtml_ca05ccdca88182d4cf11afa8957be535( ::osiris::IdeSystem::EventAfterPageRender const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::String const & result = inst.getHtml();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
}

static void setHtml_c8a350b3eca531a137b7c982d258a89f( ::osiris::IdeSystem::EventAfterPageRender & inst, ::osiris::String const & html ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.setHtml(html);
    __pythreadSaver.restore();
}

static boost::python::object getPage_16daec90d6165acf6265e53838aaf6a9( ::osiris::IdeSystem::EventBeforePageRender const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::boost::shared_ptr<osiris::IPage> const & result = inst.getPage();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::boost::shared_ptr<osiris::IPage> const & >( result ) );
}

static void setPage_1535846b1dc4542887a1d28736e9aee3( ::osiris::IdeSystem::EventBeforePageRender & inst, ::boost::shared_ptr< osiris::IPage > const & page ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.setPage(page);
    __pythreadSaver.restore();
}

void register_IdeSystem_class(){

    { //::osiris::IdeSystem
        typedef ::boost::python::class_< IdeSystem_wrapper, ::boost::python::bases< ::osiris::DynamicSingleton< osiris::IdeSystem > >, ::boost::noncopyable > IdeSystem_exposer_t;
        IdeSystem_exposer_t IdeSystem_exposer = IdeSystem_exposer_t( "IdeSystem", ::boost::python::no_init );
        ::boost::python::scope IdeSystem_scope( IdeSystem_exposer );
        { //::osiris::IdeSystem::EventAfterPageRender
            typedef ::boost::python::class_< ::osiris::IdeSystem::EventAfterPageRender, ::boost::python::bases< ::osiris::IEvent >, ::boost::noncopyable > EventAfterPageRender_exposer_t;
            EventAfterPageRender_exposer_t EventAfterPageRender_exposer = EventAfterPageRender_exposer_t( "EventAfterPageRender", ::boost::python::no_init );
            ::boost::python::scope EventAfterPageRender_scope( EventAfterPageRender_exposer );
            { //::osiris::IdeSystem::EventAfterPageRender::getHtml
            
                typedef boost::python::object ( *getHtml_function_type )( ::osiris::IdeSystem::EventAfterPageRender const & );
                
                EventAfterPageRender_exposer.def( 
                    "getHtml"
                    , getHtml_function_type( &getHtml_ca05ccdca88182d4cf11afa8957be535 ) );
            
            }
            { //::osiris::IdeSystem::EventAfterPageRender::setHtml
            
                typedef void ( *setHtml_function_type )( ::osiris::IdeSystem::EventAfterPageRender &,::osiris::String const & );
                
                EventAfterPageRender_exposer.def( 
                    "setHtml"
                    , setHtml_function_type( &setHtml_c8a350b3eca531a137b7c982d258a89f )
                    , ( ::boost::python::arg("inst"), ::boost::python::arg("html") ) );
            
            }
            { //property "html"[fget=::osiris::IdeSystem::EventAfterPageRender::getHtml, fset=::osiris::IdeSystem::EventAfterPageRender::setHtml]
            
                typedef ::osiris::String const & ( ::osiris::IdeSystem::EventAfterPageRender::*fget )(  ) const;
                typedef void ( ::osiris::IdeSystem::EventAfterPageRender::*fset )( ::osiris::String const & ) ;
                
                EventAfterPageRender_exposer.add_property( 
                    "html"
                    , ::boost::python::make_function( 
                          fget( &::osiris::IdeSystem::EventAfterPageRender::getHtml )
                        , bp::return_value_policy< bp::copy_const_reference >() ) 
                    , fset( &::osiris::IdeSystem::EventAfterPageRender::setHtml )
                    , "get\\set property, built on top of \"osiris::String const & osiris::IdeSystem::EventAfterPageRender::getHtml() const [member function]\" and \"void osiris::IdeSystem::EventAfterPageRender::setHtml(osiris::String const & html) [member function]\"" );
            
            }
        }
        { //::osiris::IdeSystem::EventBeforePageRender
            typedef ::boost::python::class_< ::osiris::IdeSystem::EventBeforePageRender, ::boost::python::bases< ::osiris::IEvent > > EventBeforePageRender_exposer_t;
            EventBeforePageRender_exposer_t EventBeforePageRender_exposer = EventBeforePageRender_exposer_t( "EventBeforePageRender" );
            ::boost::python::scope EventBeforePageRender_scope( EventBeforePageRender_exposer );
            { //::osiris::IdeSystem::EventBeforePageRender::getPage
            
                typedef boost::python::object ( *getPage_function_type )( ::osiris::IdeSystem::EventBeforePageRender const & );
                
                EventBeforePageRender_exposer.def( 
                    "getPage"
                    , getPage_function_type( &getPage_16daec90d6165acf6265e53838aaf6a9 ) );
            
            }
            { //::osiris::IdeSystem::EventBeforePageRender::setPage
            
                typedef void ( *setPage_function_type )( ::osiris::IdeSystem::EventBeforePageRender &,::boost::shared_ptr<osiris::IPage> const & );
                
                EventBeforePageRender_exposer.def( 
                    "setPage"
                    , setPage_function_type( &setPage_1535846b1dc4542887a1d28736e9aee3 )
                    , ( ::boost::python::arg("inst"), ::boost::python::arg("page") ) );
            
            }
            { //property "page"[fget=::osiris::IdeSystem::EventBeforePageRender::getPage, fset=::osiris::IdeSystem::EventBeforePageRender::setPage]
            
                typedef ::boost::shared_ptr<osiris::IPage> const & ( ::osiris::IdeSystem::EventBeforePageRender::*fget )(  ) const;
                typedef void ( ::osiris::IdeSystem::EventBeforePageRender::*fset )( ::boost::shared_ptr<osiris::IPage> const & ) ;
                
                EventBeforePageRender_exposer.add_property( 
                    "page"
                    , ::boost::python::make_function( 
                          fget( &::osiris::IdeSystem::EventBeforePageRender::getPage )
                        , bp::return_value_policy< bp::copy_const_reference >() ) 
                    , fset( &::osiris::IdeSystem::EventBeforePageRender::setPage )
                    , "get\\set property, built on top of \"boost::shared_ptr<osiris::IPage> const & osiris::IdeSystem::EventBeforePageRender::getPage() const [member function]\" and \"void osiris::IdeSystem::EventBeforePageRender::setPage(boost::shared_ptr<osiris::IPage> const & page) [member function]\"" );
            
            }
        }
        IdeSystem_exposer.def( ::boost::python::init< >() );
        { //::osiris::IdeSystem::getServer
        
            typedef boost::python::object ( *getServer_function_type )( ::osiris::IdeSystem const & );
            
            IdeSystem_exposer.def( 
                "getServer"
                , getServer_function_type( &IdeSystem_wrapper::getServer ) );
        
        }
        { //::osiris::IdeSystem::getDefaultSkin
        
            typedef boost::python::object ( *getDefaultSkin_function_type )( ::osiris::IdeSystem const & );
            
            IdeSystem_exposer.def( 
                "getDefaultSkin"
                , getDefaultSkin_function_type( &IdeSystem_wrapper::getDefaultSkin ) );
        
        }
        { //::osiris::IdeSystem::getAvailableSkins
        
            typedef boost::python::object ( *getAvailableSkins_function_type )( ::osiris::IdeSystem const & );
            
            IdeSystem_exposer.def( 
                "getAvailableSkins"
                , getAvailableSkins_function_type( &IdeSystem_wrapper::getAvailableSkins ) );
        
        }
        { //::osiris::IdeSystem::getLocalUrl
        
            typedef boost::python::object ( *getLocalUrl_function_type )( ::osiris::IdeSystem const &,::osiris::String const & );
            
            IdeSystem_exposer.def( 
                "getLocalUrl"
                , getLocalUrl_function_type( &IdeSystem_wrapper::getLocalUrl )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("relativeUrl")=(osiris::String::EMPTY) ) );
        
        }
        { //::osiris::IdeSystem::getHtdocsPath
        
            typedef boost::python::object ( *getHtdocsPath_function_type )( ::osiris::IdeSystem const & );
            
            IdeSystem_exposer.def( 
                "getHtdocsPath"
                , getHtdocsPath_function_type( &IdeSystem_wrapper::getHtdocsPath ) );
        
        }
        { //::osiris::IdeSystem::getEventBeforePageRender
        
            typedef boost::python::object ( *getEventBeforePageRender_function_type )( ::osiris::IdeSystem const & );
            
            IdeSystem_exposer.def( 
                "getEventBeforePageRender"
                , getEventBeforePageRender_function_type( &IdeSystem_wrapper::getEventBeforePageRender ) );
        
        }
        { //::osiris::IdeSystem::getEventAfterPageRender
        
            typedef boost::python::object ( *getEventAfterPageRender_function_type )( ::osiris::IdeSystem const & );
            
            IdeSystem_exposer.def( 
                "getEventAfterPageRender"
                , getEventAfterPageRender_function_type( &IdeSystem_wrapper::getEventAfterPageRender ) );
        
        }
        { //::osiris::IdeSystem::createSession
        
            typedef boost::python::object ( *createSession_function_type )( ::osiris::IdeSystem & );
            
            IdeSystem_exposer.def( 
                "createSession"
                , createSession_function_type( &IdeSystem_wrapper::createSession ) );
        
        }
        { //::osiris::IdeSystem::getName
        
            typedef ::osiris::String ( ::osiris::IdeSystem::*getName_function_type )(  ) const;
            typedef ::osiris::String ( IdeSystem_wrapper::*default_getName_function_type )(  ) const;
            
            IdeSystem_exposer.def( 
                "getName"
                , getName_function_type(&::osiris::IdeSystem::getName)
                , default_getName_function_type(&IdeSystem_wrapper::default_getName) );
        
        }
        { //::osiris::IdeSystem::stop
        
            typedef void ( ::osiris::IdeSystem::*stop_function_type )(  ) ;
            typedef void ( IdeSystem_wrapper::*default_stop_function_type )(  ) ;
            
            IdeSystem_exposer.def( 
                "stop"
                , stop_function_type(&::osiris::IdeSystem::stop)
                , default_stop_function_type(&IdeSystem_wrapper::default_stop) );
        
        }
        { //::osiris::IdeSystem::getSession
        
            typedef boost::python::object ( *getSession_function_type )( ::osiris::IdeSystem const &,::osiris::String const & );
            
            IdeSystem_exposer.def( 
                "getSession"
                , getSession_function_type( &IdeSystem_wrapper::getSession )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("id") ) );
        
        }
        { //::osiris::IdeSystem::getSkin
        
            typedef boost::python::object ( *getSkin_function_type )( ::osiris::IdeSystem const &,::osiris::UniqueID const &,bool );
            
            IdeSystem_exposer.def( 
                "getSkin"
                , getSkin_function_type( &IdeSystem_wrapper::getSkin )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("id"), ::boost::python::arg("load")=(bool)(true) ) );
        
        }
        { //::osiris::IdeSystem::isAccountLogged
        
            typedef boost::python::object ( *isAccountLogged_function_type )( ::osiris::IdeSystem &,::osiris::String const & );
            
            IdeSystem_exposer.def( 
                "isAccountLogged"
                , isAccountLogged_function_type( &IdeSystem_wrapper::isAccountLogged )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("id") ) );
        
        }
        { //::osiris::IdeSystem::isTrustedDomain
        
            typedef boost::python::object ( *isTrustedDomain_function_type )( ::osiris::IdeSystem &,::std::string const & );
            
            IdeSystem_exposer.def( 
                "isTrustedDomain"
                , isTrustedDomain_function_type( &IdeSystem_wrapper::isTrustedDomain )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("domain") ) );
        
        }
        { //::osiris::IdeSystem::logoutSessions
        
            typedef void ( *logoutSessions_function_type )( ::osiris::IdeSystem &,::osiris::String const & );
            
            IdeSystem_exposer.def( 
                "logoutSessions"
                , logoutSessions_function_type( &IdeSystem_wrapper::logoutSessions )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("id") ) );
        
        }
        { //::osiris::IdeSystem::registerSkin
        
            typedef void ( *registerSkin_function_type )( ::osiris::IdeSystem &,::boost::shared_ptr<osiris::IdeSkin> );
            
            IdeSystem_exposer.def( 
                "registerSkin"
                , registerSkin_function_type( &IdeSystem_wrapper::registerSkin )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("skin") ) );
        
        }
        { //::osiris::IdeSystem::removeAccount
        
            typedef boost::python::object ( *removeAccount_function_type )( ::osiris::IdeSystem &,::osiris::String const & );
            
            IdeSystem_exposer.def( 
                "removeAccount"
                , removeAccount_function_type( &IdeSystem_wrapper::removeAccount )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("id") ) );
        
        }
        { //::osiris::IdeSystem::start
        
            typedef bool ( ::osiris::IdeSystem::*start_function_type )( bool ) ;
            typedef bool ( IdeSystem_wrapper::*default_start_function_type )( bool ) ;
            
            IdeSystem_exposer.def( 
                "start"
                , start_function_type(&::osiris::IdeSystem::start)
                , default_start_function_type(&IdeSystem_wrapper::default_start)
                , ( ::boost::python::arg("recovery") ) );
        
        }
        { //::osiris::IdeSystem::trustDomain
        
            typedef void ( *trustDomain_function_type )( ::osiris::IdeSystem &,::osiris::String const & );
            
            IdeSystem_exposer.def( 
                "trustDomain"
                , trustDomain_function_type( &IdeSystem_wrapper::trustDomain )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("domain") ) );
        
        }
        { //::osiris::IdeSystem::trustDomainFromUrl
        
            typedef boost::python::object ( *trustDomainFromUrl_function_type )( ::osiris::IdeSystem &,::std::string const & );
            
            IdeSystem_exposer.def( 
                "trustDomainFromUrl"
                , trustDomainFromUrl_function_type( &IdeSystem_wrapper::trustDomainFromUrl )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("url") ) );
        
        }
        { //::osiris::IdeSystem::unregisterSkin
        
            typedef void ( *unregisterSkin_function_type )( ::osiris::IdeSystem &,::boost::shared_ptr<osiris::IdeSkin> );
            
            IdeSystem_exposer.def( 
                "unregisterSkin"
                , unregisterSkin_function_type( &IdeSystem_wrapper::unregisterSkin )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("skin") ) );
        
        }
        { //property "server"[fget=::osiris::IdeSystem::getServer]
        
            typedef ::boost::shared_ptr<osiris::IHttpServer> ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "server"
                , fget( &::osiris::IdeSystem::getServer )
                , "get property, built on top of \"boost::shared_ptr<osiris::IHttpServer> osiris::IdeSystem::getServer() const [member function]\"" );
        
        }
        { //property "defaultSkin"[fget=::osiris::IdeSystem::getDefaultSkin]
        
            typedef ::boost::shared_ptr<osiris::IdeSkin> ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "defaultSkin"
                , fget( &::osiris::IdeSystem::getDefaultSkin )
                , "get property, built on top of \"boost::shared_ptr<osiris::IdeSkin> osiris::IdeSystem::getDefaultSkin() const [member function]\"" );
        
        }
        { //property "availableSkins"[fget=::osiris::IdeSystem::getAvailableSkins]
        
            typedef ::osiris::LockPtr<const osiris::ordered_map<osiris::UniqueID, boost::shared_ptr<osiris::IdeSkin> >,boost::recursive_mutex,boost::unique_lock<boost::recursive_mutex> > ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "availableSkins"
                , fget( &::osiris::IdeSystem::getAvailableSkins )
                , "get property, built on top of \"osiris::LockPtr<const osiris::ordered_map<osiris::UniqueID, boost::shared_ptr<osiris::IdeSkin> >,boost::recursive_mutex,boost::unique_lock<boost::recursive_mutex> > osiris::IdeSystem::getAvailableSkins() const [member function]\"" );
        
        }
        { //property "htdocsPath"[fget=::osiris::IdeSystem::getHtdocsPath]
        
            typedef ::osiris::String ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "htdocsPath"
                , fget( &::osiris::IdeSystem::getHtdocsPath )
                , "get property, built on top of \"osiris::String osiris::IdeSystem::getHtdocsPath() const [member function]\"" );
        
        }
        { //property "eventBeforePageRender"[fget=::osiris::IdeSystem::getEventBeforePageRender]
        
            typedef ::boost::shared_ptr<osiris::EventSource> ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "eventBeforePageRender"
                , fget( &::osiris::IdeSystem::getEventBeforePageRender )
                , "get property, built on top of \"boost::shared_ptr<osiris::EventSource> osiris::IdeSystem::getEventBeforePageRender() const [member function]\"" );
        
        }
        { //property "eventAfterPageRender"[fget=::osiris::IdeSystem::getEventAfterPageRender]
        
            typedef ::boost::shared_ptr<osiris::EventSource> ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "eventAfterPageRender"
                , fget( &::osiris::IdeSystem::getEventAfterPageRender )
                , "get property, built on top of \"boost::shared_ptr<osiris::EventSource> osiris::IdeSystem::getEventAfterPageRender() const [member function]\"" );
        
        }
        { //property "name"[fget=::osiris::IdeSystem::getName]
        
            typedef ::osiris::String ( ::osiris::IdeSystem::*fget )(  ) const;
            
            IdeSystem_exposer.add_property( 
                "name"
                , fget( &::osiris::IdeSystem::getName )
                , "get property, built on top of \"osiris::String osiris::IdeSystem::getName() const [member function]\"" );
        
        }
    }

}
