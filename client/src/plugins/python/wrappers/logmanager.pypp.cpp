// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "logmanager.h"
#include "logmanager.pypp.hpp"

namespace bp = boost::python;

struct LogManager_wrapper : ::osiris::LogManager, ::osiris::PythonWrapper< ::osiris::LogManager > {

    LogManager_wrapper( )
    : ::osiris::LogManager( )
      , ::osiris::PythonWrapper< ::osiris::LogManager >(){
        // nullptr constructor
    
    }

    static boost::python::object getEnabled( ::osiris::LogManager const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.getEnabled();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getLevel( ::osiris::LogManager const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::LogLevel result = inst.getLevel();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static void init( ::osiris::LogManager & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.init();
        __pythreadSaver.restore();
    }

    static void cleanup( ::osiris::LogManager & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.cleanup();
        __pythreadSaver.restore();
    }

    static void log( ::osiris::LogManager & inst, ::osiris::String const & message, ::osiris::LogLevel level=(::osiris::logLevelInfo), ::std::string const & fileName=(osiris::constants::empty_string), ::osiris::uint32 fileLine=0, ::std::string const & functionName=(osiris::constants::empty_string) ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.log(message, level, fileName, fileLine, functionName);
        __pythreadSaver.restore();
    }

    static void setEnabled( ::osiris::LogManager & inst, bool enabled ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setEnabled(enabled);
        __pythreadSaver.restore();
    }

    static void setLevel( ::osiris::LogManager & inst, ::osiris::LogLevel level ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setLevel(level);
        __pythreadSaver.restore();
    }

};

void register_LogManager_class(){

    { //::osiris::LogManager
        typedef ::boost::python::class_< LogManager_wrapper, ::boost::python::bases< ::osiris::StaticSingleton< osiris::LogManager, true > >, ::boost::noncopyable > LogManager_exposer_t;
        LogManager_exposer_t LogManager_exposer = LogManager_exposer_t( "LogManager", ::boost::python::no_init );
        ::boost::python::scope LogManager_scope( LogManager_exposer );
        LogManager_exposer.def( ::boost::python::init< >() );
        { //::osiris::LogManager::getEnabled
        
            typedef boost::python::object ( *getEnabled_function_type )( ::osiris::LogManager const & );
            
            LogManager_exposer.def( 
                "getEnabled"
                , getEnabled_function_type( &LogManager_wrapper::getEnabled ) );
        
        }
        { //::osiris::LogManager::getLevel
        
            typedef boost::python::object ( *getLevel_function_type )( ::osiris::LogManager const & );
            
            LogManager_exposer.def( 
                "getLevel"
                , getLevel_function_type( &LogManager_wrapper::getLevel ) );
        
        }
        { //::osiris::LogManager::init
        
            typedef void ( *init_function_type )( ::osiris::LogManager & );
            
            LogManager_exposer.def( 
                "init"
                , init_function_type( &LogManager_wrapper::init ) );
        
        }
        { //::osiris::LogManager::cleanup
        
            typedef void ( *cleanup_function_type )( ::osiris::LogManager & );
            
            LogManager_exposer.def( 
                "cleanup"
                , cleanup_function_type( &LogManager_wrapper::cleanup ) );
        
        }
        { //::osiris::LogManager::log
        
            typedef void ( *log_function_type )( ::osiris::LogManager &,::osiris::String const &,::osiris::LogLevel,::std::string const &,::osiris::uint32,::std::string const & );
            
            LogManager_exposer.def( 
                "log"
                , log_function_type( &LogManager_wrapper::log )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("message"), ::boost::python::arg("level")=(::osiris::logLevelInfo), ::boost::python::arg("fileName")=(osiris::constants::empty_string), ::boost::python::arg("fileLine")=(::osiris::uint32)(0), ::boost::python::arg("functionName")=(osiris::constants::empty_string) ) );
        
        }
        { //::osiris::LogManager::setEnabled
        
            typedef void ( *setEnabled_function_type )( ::osiris::LogManager &,bool );
            
            LogManager_exposer.def( 
                "setEnabled"
                , setEnabled_function_type( &LogManager_wrapper::setEnabled )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("enabled") ) );
        
        }
        { //::osiris::LogManager::setLevel
        
            typedef void ( *setLevel_function_type )( ::osiris::LogManager &,::osiris::LogLevel );
            
            LogManager_exposer.def( 
                "setLevel"
                , setLevel_function_type( &LogManager_wrapper::setLevel )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("level") ) );
        
        }
        { //property "enabled"[fget=::osiris::LogManager::getEnabled, fset=::osiris::LogManager::setEnabled]
        
            typedef bool ( ::osiris::LogManager::*fget )(  ) const;
            typedef void ( ::osiris::LogManager::*fset )( bool ) ;
            
            LogManager_exposer.add_property( 
                "enabled"
                , fget( &::osiris::LogManager::getEnabled )
                , fset( &::osiris::LogManager::setEnabled )
                , "get\\set property, built on top of \"bool osiris::LogManager::getEnabled() const [member function]\" and \"void osiris::LogManager::setEnabled(bool enabled) [member function]\"" );
        
        }
        { //property "level"[fget=::osiris::LogManager::getLevel, fset=::osiris::LogManager::setLevel]
        
            typedef ::osiris::LogLevel ( ::osiris::LogManager::*fget )(  ) const;
            typedef void ( ::osiris::LogManager::*fset )( ::osiris::LogLevel ) ;
            
            LogManager_exposer.add_property( 
                "level"
                , fget( &::osiris::LogManager::getLevel )
                , fset( &::osiris::LogManager::setLevel )
                , "get\\set property, built on top of \"osiris::LogLevel osiris::LogManager::getLevel() const [member function]\" and \"void osiris::LogManager::setLevel(osiris::LogLevel level) [member function]\"" );
        
        }
    }

}
