// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "cachesystem.h"
#include "cachesystemsingleton.pypp.hpp"

namespace bp = boost::python;

static boost::python::object instance_14106c20da3d17624b8651041e839598(  ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::CacheSystem * result = ::osiris::DynamicSingleton<osiris::CacheSystem>::instance();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::reference_existing_object > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::CacheSystem * >( result ) );
}

static boost::python::object exists_338c1c01c7361623217b1590542859ba(  ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = ::osiris::DynamicSingleton<osiris::CacheSystem>::exists();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object create_5aa20f89bf6ccecb46ebd4469f2734e2(  ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::CacheSystem * result = ::osiris::DynamicSingleton<osiris::CacheSystem>::create();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::reference_existing_object > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::CacheSystem * >( result ) );
}

static void destroy_81adb372b4c473ccef02df70ec516a8a(  ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::DynamicSingleton<osiris::CacheSystem>::destroy();
    __pythreadSaver.restore();
}

void register_CacheSystemSingleton_class(){

    ::boost::python::class_< ::osiris::DynamicSingleton< osiris::CacheSystem >, ::boost::noncopyable >( "CacheSystemSingleton", ::boost::python::init< >() )    
        .def( 
            "instance"
            , (boost::python::object (*)(  ))( &instance_14106c20da3d17624b8651041e839598 ) )    
        .def( 
            "exists"
            , (boost::python::object (*)(  ))( &exists_338c1c01c7361623217b1590542859ba ) )    
        .def( 
            "create"
            , (boost::python::object (*)(  ))( &create_5aa20f89bf6ccecb46ebd4469f2734e2 ) )    
        .def( 
            "destroy"
            , (void (*)(  ))( &destroy_81adb372b4c473ccef02df70ec516a8a ) )    
        .staticmethod( "instance" )    
        .staticmethod( "exists" )    
        .staticmethod( "create" )    
        .staticmethod( "destroy" );

}