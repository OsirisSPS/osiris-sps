// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "idboptions.h"
#include "idboptions.pypp.hpp"

namespace bp = boost::python;

struct IDbOptions_wrapper : ::osiris::IDbOptions, ::osiris::PythonWrapper< ::osiris::IDbOptions > {

    IDbOptions_wrapper( )
    : ::osiris::IDbOptions( )
      , ::osiris::PythonWrapper< ::osiris::IDbOptions >(){
        // nullptr constructor
    
    }

    static boost::python::object size( ::osiris::IDbOptions const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::size_t result = inst.size();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static void setParam( ::osiris::IDbOptions & inst, ::osiris::String const & name, ::osiris::String const & value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setParam(name, value);
        __pythreadSaver.restore();
    }

    virtual ::osiris::ordered_map< osiris::String, osiris::String > & getParams(  ) const {
        throw std::logic_error("warning W1049: This method could not be overriden in Python - method returns reference to local variable!");
    }

    static boost::python::object getParam( ::osiris::IDbOptions const & inst, ::osiris::String const & name ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String const & result = inst.getParam(name);
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
    }

    static boost::python::object hasParam( ::osiris::IDbOptions const & inst, ::osiris::String const & name ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.hasParam(name);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

};

void register_IDbOptions_class(){

    { //::osiris::IDbOptions
        typedef ::boost::python::class_< IDbOptions_wrapper, ::boost::noncopyable > IDbOptions_exposer_t;
        IDbOptions_exposer_t IDbOptions_exposer = IDbOptions_exposer_t( "IDbOptions", ::boost::python::init< >() );
        ::boost::python::scope IDbOptions_scope( IDbOptions_exposer );
        { //::osiris::IDbOptions::size
        
            typedef boost::python::object ( *size_function_type )( ::osiris::IDbOptions const & );
            
            IDbOptions_exposer.def( 
                "size"
                , size_function_type( &IDbOptions_wrapper::size ) );
        
        }
        { //::osiris::IDbOptions::setParam
        
            typedef void ( *setParam_function_type )( ::osiris::IDbOptions &,::osiris::String const &,::osiris::String const & );
            
            IDbOptions_exposer.def( 
                "setParam"
                , setParam_function_type( &IDbOptions_wrapper::setParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::IDbOptions::getParams
        
            typedef ::osiris::ordered_map<osiris::String, osiris::String> & ( ::osiris::IDbOptions::*getParams_function_type )(  ) const;
            
            IDbOptions_exposer.def( 
                "getParams"
                , ( getParams_function_type(&::osiris::IDbOptions::getParams) )
                , bp::return_value_policy< bp::reference_existing_object >() );
        
        }
        { //::osiris::IDbOptions::getParam
        
            typedef boost::python::object ( *getParam_function_type )( ::osiris::IDbOptions const &,::osiris::String const & );
            
            IDbOptions_exposer.def( 
                "getParam"
                , getParam_function_type( &IDbOptions_wrapper::getParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //::osiris::IDbOptions::hasParam
        
            typedef boost::python::object ( *hasParam_function_type )( ::osiris::IDbOptions const &,::osiris::String const & );
            
            IDbOptions_exposer.def( 
                "hasParam"
                , hasParam_function_type( &IDbOptions_wrapper::hasParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //property "params"[fget=::osiris::IDbOptions::getParams]
        
            typedef ::osiris::ordered_map<osiris::String, osiris::String> & ( ::osiris::IDbOptions::*fget )(  ) const;
            
            IDbOptions_exposer.add_property( 
                "params"
                , ::boost::python::make_function( 
                      fget( &::osiris::IDbOptions::getParams )
                    , bp::return_value_policy< bp::reference_existing_object >() ) 
                , "get property, built on top of \"osiris::ordered_map<osiris::String, osiris::String> & osiris::IDbOptions::getParams() const [member function]\"" );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::IDbOptions > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::IDbOptions >, boost::shared_ptr< ::osiris::Object > >();
    }

}
