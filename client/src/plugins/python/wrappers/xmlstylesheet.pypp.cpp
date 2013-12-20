// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "xmlstylesheet.h"
#include "ixmlresolver.h"
#include "xmlstylesheet.pypp.hpp"

namespace bp = boost::python;

struct XMLStylesheet_wrapper : ::osiris::XMLStylesheet, ::osiris::PythonWrapper< ::osiris::XMLStylesheet > {

    XMLStylesheet_wrapper( )
    : ::osiris::XMLStylesheet( )
      , ::osiris::PythonWrapper< ::osiris::XMLStylesheet >(){
        // nullptr constructor
    
    }

    static boost::python::object getFunctions( ::osiris::XMLStylesheet const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::Collections<osiris::String, osiris::IXMLFunction>::basic_collection<osiris::Collections<osiris::String, osiris::IXMLFunction>::policies::shared> const & result = inst.getFunctions();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::Collections<osiris::String, osiris::IXMLFunction>::basic_collection<osiris::Collections<osiris::String, osiris::IXMLFunction>::policies::shared> const & >( result ) );
    }

    static boost::python::object getResolver( ::osiris::XMLStylesheet const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::boost::shared_ptr<osiris::IXMLResolver> result = inst.getResolver();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getParams( ::osiris::XMLStylesheet const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::StringCollection<osiris::String, osiris::StringCollectionTraits<osiris::String> > const & result = inst.getParams();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::StringCollection<osiris::String, osiris::StringCollectionTraits<osiris::String> > const & >( result ) );
    }

    static void setParam( ::osiris::XMLStylesheet & inst, ::osiris::String const & name, ::osiris::String const & value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setParam(name, value);
        __pythreadSaver.restore();
    }

    static void setBoolParam( ::osiris::XMLStylesheet & inst, ::osiris::String const & name, bool value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setBoolParam(name, value);
        __pythreadSaver.restore();
    }

    static void setStringParam( ::osiris::XMLStylesheet & inst, ::osiris::String const & name, ::osiris::String const & value ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setStringParam(name, value);
        __pythreadSaver.restore();
    }

    static boost::python::object applyToFile( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::XMLDocument > document, ::osiris::String const & filename ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.applyToFile(document, filename);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object applyToString( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::XMLDocument > document, ::osiris::String & str ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.applyToString(document, str);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual void onRegisterFunctions(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onRegisterFunctions = this->get_override( "onRegisterFunctions" ) )
            func_onRegisterFunctions(  );
        else{
            __pystate.leave();
            this->::osiris::XMLStylesheet::onRegisterFunctions(  );
        }
    }
    
    virtual void default_onRegisterFunctions(  ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::XMLStylesheet::onRegisterFunctions( );
    }

    static boost::python::object applyToString2( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::XMLDocument > document ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.applyToString2(document);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getParam( ::osiris::XMLStylesheet const & inst, ::osiris::String const & name ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String const & result = inst.getParam(name);
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
    }

    static boost::python::object hasFunction( ::osiris::XMLStylesheet const & inst, ::osiris::String const & name ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.hasFunction(name);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    virtual void onError( ::osiris::String const & description ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_onError = this->get_override( "onError" ) )
            func_onError( boost::ref(description) );
        else{
            __pystate.leave();
            this->::osiris::XMLStylesheet::onError( boost::ref(description) );
        }
    }
    
    virtual void default_onError( ::osiris::String const & description ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::XMLStylesheet::onError( boost::ref(description) );
    }

    static boost::python::object parseFile( ::osiris::XMLStylesheet & inst, ::osiris::String const & filename ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.parseFile(filename);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object parseString( ::osiris::XMLStylesheet & inst, ::osiris::String const & str ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.parseString(str);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object registerFunction( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::IXMLFunction > function ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.registerFunction(function);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static void setResolver( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::IXMLResolver > resolver ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setResolver(resolver);
        __pythreadSaver.restore();
    }

    static boost::python::object transform( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::XMLDocument > document ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String result = inst.transform(document);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object unregisterFunction( ::osiris::XMLStylesheet & inst, ::boost::shared_ptr< osiris::IXMLFunction > function ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.unregisterFunction(function);
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

};

void register_XMLStylesheet_class(){

    { //::osiris::XMLStylesheet
        typedef ::boost::python::class_< XMLStylesheet_wrapper, ::boost::noncopyable > XMLStylesheet_exposer_t;
        XMLStylesheet_exposer_t XMLStylesheet_exposer = XMLStylesheet_exposer_t( "XMLStylesheet", ::boost::python::no_init );
        ::boost::python::scope XMLStylesheet_scope( XMLStylesheet_exposer );
        XMLStylesheet_exposer.def( ::boost::python::init< >() );
        { //::osiris::XMLStylesheet::getFunctions
        
            typedef boost::python::object ( *getFunctions_function_type )( ::osiris::XMLStylesheet const & );
            
            XMLStylesheet_exposer.def( 
                "getFunctions"
                , getFunctions_function_type( &XMLStylesheet_wrapper::getFunctions ) );
        
        }
        { //::osiris::XMLStylesheet::getResolver
        
            typedef boost::python::object ( *getResolver_function_type )( ::osiris::XMLStylesheet const & );
            
            XMLStylesheet_exposer.def( 
                "getResolver"
                , getResolver_function_type( &XMLStylesheet_wrapper::getResolver ) );
        
        }
        { //::osiris::XMLStylesheet::getParams
        
            typedef boost::python::object ( *getParams_function_type )( ::osiris::XMLStylesheet const & );
            
            XMLStylesheet_exposer.def( 
                "getParams"
                , getParams_function_type( &XMLStylesheet_wrapper::getParams ) );
        
        }
        { //::osiris::XMLStylesheet::setParam
        
            typedef void ( *setParam_function_type )( ::osiris::XMLStylesheet &,::osiris::String const &,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "setParam"
                , setParam_function_type( &XMLStylesheet_wrapper::setParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::XMLStylesheet::setBoolParam
        
            typedef void ( *setBoolParam_function_type )( ::osiris::XMLStylesheet &,::osiris::String const &,bool );
            
            XMLStylesheet_exposer.def( 
                "setBoolParam"
                , setBoolParam_function_type( &XMLStylesheet_wrapper::setBoolParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::XMLStylesheet::setStringParam
        
            typedef void ( *setStringParam_function_type )( ::osiris::XMLStylesheet &,::osiris::String const &,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "setStringParam"
                , setStringParam_function_type( &XMLStylesheet_wrapper::setStringParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::XMLStylesheet::applyToFile
        
            typedef boost::python::object ( *applyToFile_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::XMLDocument>,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "applyToFile"
                , applyToFile_function_type( &XMLStylesheet_wrapper::applyToFile )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("document"), ::boost::python::arg("filename") ) );
        
        }
        { //::osiris::XMLStylesheet::applyToString
        
            typedef boost::python::object ( *applyToString_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::XMLDocument>,::osiris::String & );
            
            XMLStylesheet_exposer.def( 
                "applyToString"
                , applyToString_function_type( &XMLStylesheet_wrapper::applyToString )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("document"), ::boost::python::arg("str") ) );
        
        }
        { //::osiris::XMLStylesheet::onRegisterFunctions
        
            typedef void ( XMLStylesheet_wrapper::*onRegisterFunctions_function_type )(  ) ;
            
            XMLStylesheet_exposer.def( 
                "onRegisterFunctions"
                , onRegisterFunctions_function_type( &XMLStylesheet_wrapper::default_onRegisterFunctions ) );
        
        }
        { //::osiris::XMLStylesheet::applyToString2
        
            typedef boost::python::object ( *applyToString2_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::XMLDocument> );
            
            XMLStylesheet_exposer.def( 
                "applyToString2"
                , applyToString2_function_type( &XMLStylesheet_wrapper::applyToString2 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("document") ) );
        
        }
        { //::osiris::XMLStylesheet::getParam
        
            typedef boost::python::object ( *getParam_function_type )( ::osiris::XMLStylesheet const &,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "getParam"
                , getParam_function_type( &XMLStylesheet_wrapper::getParam )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //::osiris::XMLStylesheet::hasFunction
        
            typedef boost::python::object ( *hasFunction_function_type )( ::osiris::XMLStylesheet const &,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "hasFunction"
                , hasFunction_function_type( &XMLStylesheet_wrapper::hasFunction )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //::osiris::XMLStylesheet::onError
        
            typedef void ( XMLStylesheet_wrapper::*onError_function_type )( ::osiris::String const & ) ;
            
            XMLStylesheet_exposer.def( 
                "onError"
                , onError_function_type( &XMLStylesheet_wrapper::default_onError )
                , ( ::boost::python::arg("description") ) );
        
        }
        { //::osiris::XMLStylesheet::parseFile
        
            typedef boost::python::object ( *parseFile_function_type )( ::osiris::XMLStylesheet &,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "parseFile"
                , parseFile_function_type( &XMLStylesheet_wrapper::parseFile )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("filename") ) );
        
        }
        { //::osiris::XMLStylesheet::parseString
        
            typedef boost::python::object ( *parseString_function_type )( ::osiris::XMLStylesheet &,::osiris::String const & );
            
            XMLStylesheet_exposer.def( 
                "parseString"
                , parseString_function_type( &XMLStylesheet_wrapper::parseString )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("str") ) );
        
        }
        { //::osiris::XMLStylesheet::registerFunction
        
            typedef boost::python::object ( *registerFunction_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::IXMLFunction> );
            
            XMLStylesheet_exposer.def( 
                "registerFunction"
                , registerFunction_function_type( &XMLStylesheet_wrapper::registerFunction )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("function") ) );
        
        }
        { //::osiris::XMLStylesheet::setResolver
        
            typedef void ( *setResolver_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::IXMLResolver> );
            
            XMLStylesheet_exposer.def( 
                "setResolver"
                , setResolver_function_type( &XMLStylesheet_wrapper::setResolver )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("resolver") ) );
        
        }
        { //::osiris::XMLStylesheet::transform
        
            typedef boost::python::object ( *transform_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::XMLDocument> );
            
            XMLStylesheet_exposer.def( 
                "transform"
                , transform_function_type( &XMLStylesheet_wrapper::transform )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("document") ) );
        
        }
        { //::osiris::XMLStylesheet::unregisterFunction
        
            typedef boost::python::object ( *unregisterFunction_function_type )( ::osiris::XMLStylesheet &,::boost::shared_ptr<osiris::IXMLFunction> );
            
            XMLStylesheet_exposer.def( 
                "unregisterFunction"
                , unregisterFunction_function_type( &XMLStylesheet_wrapper::unregisterFunction )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("function") ) );
        
        }
        { //property "resolver"[fget=::osiris::XMLStylesheet::getResolver, fset=::osiris::XMLStylesheet::setResolver]
        
            typedef ::boost::shared_ptr<osiris::IXMLResolver> ( ::osiris::XMLStylesheet::*fget )(  ) const;
            typedef void ( ::osiris::XMLStylesheet::*fset )( ::boost::shared_ptr<osiris::IXMLResolver> ) ;
            
            XMLStylesheet_exposer.add_property( 
                "resolver"
                , fget( &::osiris::XMLStylesheet::getResolver )
                , fset( &::osiris::XMLStylesheet::setResolver )
                , "get\\set property, built on top of \"boost::shared_ptr<osiris::IXMLResolver> osiris::XMLStylesheet::getResolver() const [member function]\" and \"void osiris::XMLStylesheet::setResolver(boost::shared_ptr<osiris::IXMLResolver> resolver) [member function]\"" );
        
        }
        { //property "functions"[fget=::osiris::XMLStylesheet::getFunctions]
        
            typedef ::osiris::Collections<osiris::String, osiris::IXMLFunction>::basic_collection<osiris::Collections<osiris::String, osiris::IXMLFunction>::policies::shared> const & ( ::osiris::XMLStylesheet::*fget )(  ) const;
            
            XMLStylesheet_exposer.add_property( 
                "functions"
                , ::boost::python::make_function( 
                      fget( &::osiris::XMLStylesheet::getFunctions )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"osiris::Collections<osiris::String, osiris::IXMLFunction>::basic_collection<osiris::Collections<osiris::String, osiris::IXMLFunction>::policies::shared> const & osiris::XMLStylesheet::getFunctions() const [member function]\"" );
        
        }
        { //property "params"[fget=::osiris::XMLStylesheet::getParams]
        
            typedef ::osiris::StringCollection<osiris::String, osiris::StringCollectionTraits<osiris::String> > const & ( ::osiris::XMLStylesheet::*fget )(  ) const;
            
            XMLStylesheet_exposer.add_property( 
                "params"
                , ::boost::python::make_function( 
                      fget( &::osiris::XMLStylesheet::getParams )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , "get property, built on top of \"osiris::StringCollection<osiris::String, osiris::StringCollectionTraits<osiris::String> > const & osiris::XMLStylesheet::getParams() const [member function]\"" );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::XMLStylesheet > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::XMLStylesheet >, boost::shared_ptr< ::boost::noncopyable_::noncopyable > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::XMLStylesheet >, boost::shared_ptr< ::osiris::enable_this_ptr< osiris::XMLStylesheet > > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::XMLStylesheet >, boost::shared_ptr< ::osiris::Object > >();
    }

}
