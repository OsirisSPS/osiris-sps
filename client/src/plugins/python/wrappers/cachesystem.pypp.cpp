// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "cachesystem.h"
#include "cachesystem.pypp.hpp"

namespace bp = boost::python;

struct CacheSystem_wrapper : ::osiris::CacheSystem, ::osiris::PythonWrapper< ::osiris::CacheSystem > {

    CacheSystem_wrapper( )
    : ::osiris::CacheSystem( )
      , ::osiris::PythonWrapper< ::osiris::CacheSystem >(){
        // null constructor
    
    }

    static void set( ::osiris::CacheSystem & inst, ::osiris::String const & key, ::osiris::DataItem const & value, ::osiris::uint32 expiration=0 ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.set(key, value, expiration);
        __pythreadSaver.restore();
    }

    virtual ::osiris::String getName(  ) const  {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getName = this->get_override( "getName" ) )
            return func_getName(  );
        else{
            __pystate.leave();
            return this->::osiris::CacheSystem::getName(  );
        }
    }
    
    ::osiris::String default_getName(  ) const  {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::CacheSystem::getName( );
    }

    virtual void stop(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_stop = this->get_override( "stop" ) )
            func_stop(  );
        else{
            __pystate.leave();
            this->::osiris::CacheSystem::stop(  );
        }
    }
    
    void default_stop(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::CacheSystem::stop( );
    }

    static boost::python::object exists( ::osiris::CacheSystem & inst, ::osiris::String const & key ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.exists(key);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object get( ::osiris::CacheSystem & inst, ::osiris::String const & key ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::DataItem result = inst.get(key);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object remove( ::osiris::CacheSystem & inst, ::osiris::String const & key ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.remove(key);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual bool start( bool recovery ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_start = this->get_override( "start" ) )
            return func_start( recovery );
        else{
            __pystate.leave();
            return this->::osiris::CacheSystem::start( recovery );
        }
    }
    
    bool default_start( bool recovery ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::CacheSystem::start( recovery );
    }

};

void register_CacheSystem_class(){

    { //::osiris::CacheSystem
        typedef ::boost::python::class_< CacheSystem_wrapper, ::boost::python::bases< ::osiris::DynamicSingleton< osiris::CacheSystem > >, ::boost::noncopyable > CacheSystem_exposer_t;
        CacheSystem_exposer_t CacheSystem_exposer = CacheSystem_exposer_t( "CacheSystem", ::boost::python::no_init );
        ::boost::python::scope CacheSystem_scope( CacheSystem_exposer );
        CacheSystem_exposer.def( ::boost::python::init< >() );
        { //::osiris::CacheSystem::set
        
            typedef void ( *set_function_type )( ::osiris::CacheSystem &,::osiris::String const &,::osiris::DataItem const &,::osiris::uint32 );
            
            CacheSystem_exposer.def( 
                "set"
                , set_function_type( &CacheSystem_wrapper::set )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("key"), ::boost::python::arg("value"), ::boost::python::arg("expiration")=(::osiris::uint32)(0) ) );
        
        }
        { //::osiris::CacheSystem::getName
        
            typedef ::osiris::String ( ::osiris::CacheSystem::*getName_function_type )(  ) const;
            typedef ::osiris::String ( CacheSystem_wrapper::*default_getName_function_type )(  ) const;
            
            CacheSystem_exposer.def( 
                "getName"
                , getName_function_type(&::osiris::CacheSystem::getName)
                , default_getName_function_type(&CacheSystem_wrapper::default_getName) );
        
        }
        { //::osiris::CacheSystem::stop
        
            typedef void ( ::osiris::CacheSystem::*stop_function_type )(  ) ;
            typedef void ( CacheSystem_wrapper::*default_stop_function_type )(  ) ;
            
            CacheSystem_exposer.def( 
                "stop"
                , stop_function_type(&::osiris::CacheSystem::stop)
                , default_stop_function_type(&CacheSystem_wrapper::default_stop) );
        
        }
        { //::osiris::CacheSystem::exists
        
            typedef boost::python::object ( *exists_function_type )( ::osiris::CacheSystem &,::osiris::String const & );
            
            CacheSystem_exposer.def( 
                "exists"
                , exists_function_type( &CacheSystem_wrapper::exists )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("key") ) );
        
        }
        { //::osiris::CacheSystem::get
        
            typedef boost::python::object ( *get_function_type )( ::osiris::CacheSystem &,::osiris::String const & );
            
            CacheSystem_exposer.def( 
                "get"
                , get_function_type( &CacheSystem_wrapper::get )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("key") ) );
        
        }
        { //::osiris::CacheSystem::remove
        
            typedef boost::python::object ( *remove_function_type )( ::osiris::CacheSystem &,::osiris::String const & );
            
            CacheSystem_exposer.def( 
                "remove"
                , remove_function_type( &CacheSystem_wrapper::remove )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("key") ) );
        
        }
        { //::osiris::CacheSystem::start
        
            typedef bool ( ::osiris::CacheSystem::*start_function_type )( bool ) ;
            typedef bool ( CacheSystem_wrapper::*default_start_function_type )( bool ) ;
            
            CacheSystem_exposer.def( 
                "start"
                , start_function_type(&::osiris::CacheSystem::start)
                , default_start_function_type(&CacheSystem_wrapper::default_start)
                , ( ::boost::python::arg("recovery") ) );
        
        }
        { //property "name"[fget=::osiris::CacheSystem::getName]
        
            typedef ::osiris::String ( ::osiris::CacheSystem::*fget )(  ) const;
            
            CacheSystem_exposer.add_property( 
                "name"
                , fget( &::osiris::CacheSystem::getName )
                , "get property, built on top of \"osiris::String osiris::CacheSystem::getName() const [member function]\"" );
        
        }
    }

}