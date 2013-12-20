// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "ioptionvalidator.h"
#include "ioptionvalidator.pypp.hpp"

namespace bp = boost::python;

struct IOptionValidator_wrapper : ::osiris::IOptionValidator, ::osiris::PythonWrapper< ::osiris::IOptionValidator > {

    IOptionValidator_wrapper( )
    : ::osiris::IOptionValidator( )
      , ::osiris::PythonWrapper< ::osiris::IOptionValidator >(){
        // nullptr constructor
    
    }

    virtual bool validate( ::osiris::DataItem const & value ) const {
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_validate = this->get_override( "validate" );
        return func_validate( boost::ref(value) );
    }

};

void register_IOptionValidator_class(){

    { //::osiris::IOptionValidator
        typedef ::boost::python::class_< IOptionValidator_wrapper, ::boost::noncopyable > IOptionValidator_exposer_t;
        IOptionValidator_exposer_t IOptionValidator_exposer = IOptionValidator_exposer_t( "IOptionValidator", ::boost::python::init< >() );
        ::boost::python::scope IOptionValidator_scope( IOptionValidator_exposer );
        { //::osiris::IOptionValidator::validate
        
            typedef bool ( ::osiris::IOptionValidator::*validate_function_type )( ::osiris::DataItem const & ) const;
            
            IOptionValidator_exposer.def( 
                "validate"
                , ( validate_function_type(&::osiris::IOptionValidator::validate) )
                , ( ::boost::python::arg("value") ) );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::IOptionValidator > >();
    }

}
