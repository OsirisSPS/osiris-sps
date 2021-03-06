// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "htmlattributes.h"
#include "ihtmlcontrol.h"
#include "htmlattributes.pypp.hpp"

namespace bp = boost::python;

static boost::python::object getControl_70383de22d8efa1cd3ab6808921f92a0( ::osiris::HtmlAttributes & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::IHtmlControl & result = inst.getControl();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::reference_existing_object > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::IHtmlControl & >( result ) );
}

static boost::python::object getControl_8ce16ec92ad8c36d56851c5e2923bc41( ::osiris::HtmlAttributes const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::IHtmlControl const & result = inst.getControl();
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::IHtmlControl const & >( result ) );
}

static boost::python::object empty_8f40238f9d0734d20136f7cfddb516d1( ::osiris::HtmlAttributes const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.empty();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object size_f1cbef212b97f188d06562e7b2967770( ::osiris::HtmlAttributes const & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::uint32 result = inst.size();
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static void clear_039df0da8ba6f2c4f7c277bee675fbb3( ::osiris::HtmlAttributes & inst ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.clear();
    __pythreadSaver.restore();
}

static boost::python::object set_0eb8dc978de7e0cc514dce6e961926dc( ::osiris::HtmlAttributes & inst, ::osiris::String const & name, ::osiris::String const & value ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::boost::shared_ptr<osiris::HtmlAttribute> result = inst.set(name, value);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object exists_e913f47b7494988995f89db65f9d815d( ::osiris::HtmlAttributes const & inst, ::osiris::String const & name ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    bool result = inst.exists(name);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object get_2b1cad8c350fa72238f9d1c52c2cd738( ::osiris::HtmlAttributes const & inst, ::osiris::String const & name ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::boost::shared_ptr<osiris::HtmlAttribute> result = inst.get(name);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static boost::python::object get_d34583970577b0ffe701e815a7b19fd4( ::osiris::HtmlAttributes const & inst, ::size_t index ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::boost::shared_ptr<osiris::HtmlAttribute> result = inst.get(index);
    __pythreadSaver.restore();
    return boost::python::object( result );
}

static void remove_787c5a6b35ce0c63a8f5b59ef72f82b0( ::osiris::HtmlAttributes & inst, ::osiris::String const & name ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    inst.remove(name);
    __pythreadSaver.restore();
}

static boost::python::object value_of_35e2b96adf499eed6ade97364f6fe688( ::osiris::HtmlAttributes & inst, ::osiris::String const & name ){
    ::osiris::PythonThreadSaver __pythreadSaver;
    ::osiris::String const & result = inst.value_of(name);
    __pythreadSaver.restore();
    typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
    return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
}

void register_HtmlAttributes_class(){

    { //::osiris::HtmlAttributes
        typedef ::boost::python::class_< ::osiris::HtmlAttributes, ::boost::noncopyable > HtmlAttributes_exposer_t;
        HtmlAttributes_exposer_t HtmlAttributes_exposer = HtmlAttributes_exposer_t( "HtmlAttributes", ::boost::python::init< ::osiris::IHtmlControl & >(( ::boost::python::arg("control") )) );
        ::boost::python::scope HtmlAttributes_scope( HtmlAttributes_exposer );
        ::boost::python::implicitly_convertible< ::osiris::IHtmlControl &, ::osiris::HtmlAttributes >();
        { //::osiris::HtmlAttributes::getControl
        
            typedef boost::python::object ( *getControl_function_type )( ::osiris::HtmlAttributes & );
            
            HtmlAttributes_exposer.def( 
                "getControl"
                , getControl_function_type( &getControl_70383de22d8efa1cd3ab6808921f92a0 ) );
        
        }
        { //::osiris::HtmlAttributes::getControl
        
            typedef boost::python::object ( *getControl_function_type )( ::osiris::HtmlAttributes const & );
            
            HtmlAttributes_exposer.def( 
                "getControl"
                , getControl_function_type( &getControl_8ce16ec92ad8c36d56851c5e2923bc41 ) );
        
        }
        { //::osiris::HtmlAttributes::empty
        
            typedef boost::python::object ( *empty_function_type )( ::osiris::HtmlAttributes const & );
            
            HtmlAttributes_exposer.def( 
                "empty"
                , empty_function_type( &empty_8f40238f9d0734d20136f7cfddb516d1 ) );
        
        }
        { //::osiris::HtmlAttributes::size
        
            typedef boost::python::object ( *size_function_type )( ::osiris::HtmlAttributes const & );
            
            HtmlAttributes_exposer.def( 
                "size"
                , size_function_type( &size_f1cbef212b97f188d06562e7b2967770 ) );
        
        }
        { //::osiris::HtmlAttributes::clear
        
            typedef void ( *clear_function_type )( ::osiris::HtmlAttributes & );
            
            HtmlAttributes_exposer.def( 
                "clear"
                , clear_function_type( &clear_039df0da8ba6f2c4f7c277bee675fbb3 ) );
        
        }
        { //::osiris::HtmlAttributes::set
        
            typedef boost::python::object ( *set_function_type )( ::osiris::HtmlAttributes &,::osiris::String const &,::osiris::String const & );
            
            HtmlAttributes_exposer.def( 
                "set"
                , set_function_type( &set_0eb8dc978de7e0cc514dce6e961926dc )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name"), ::boost::python::arg("value") ) );
        
        }
        { //::osiris::HtmlAttributes::exists
        
            typedef boost::python::object ( *exists_function_type )( ::osiris::HtmlAttributes const &,::osiris::String const & );
            
            HtmlAttributes_exposer.def( 
                "exists"
                , exists_function_type( &exists_e913f47b7494988995f89db65f9d815d )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //::osiris::HtmlAttributes::get
        
            typedef boost::python::object ( *get_function_type )( ::osiris::HtmlAttributes const &,::osiris::String const & );
            
            HtmlAttributes_exposer.def( 
                "get"
                , get_function_type( &get_2b1cad8c350fa72238f9d1c52c2cd738 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //::osiris::HtmlAttributes::get
        
            typedef boost::python::object ( *get_function_type )( ::osiris::HtmlAttributes const &,::size_t );
            
            HtmlAttributes_exposer.def( 
                "get"
                , get_function_type( &get_d34583970577b0ffe701e815a7b19fd4 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("index") ) );
        
        }
        { //::osiris::HtmlAttributes::remove
        
            typedef void ( *remove_function_type )( ::osiris::HtmlAttributes &,::osiris::String const & );
            
            HtmlAttributes_exposer.def( 
                "remove"
                , remove_function_type( &remove_787c5a6b35ce0c63a8f5b59ef72f82b0 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        { //::osiris::HtmlAttributes::value_of
        
            typedef boost::python::object ( *value_of_function_type )( ::osiris::HtmlAttributes &,::osiris::String const & );
            
            HtmlAttributes_exposer.def( 
                "value_of"
                , value_of_function_type( &value_of_35e2b96adf499eed6ade97364f6fe688 )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("name") ) );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::HtmlAttributes > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::HtmlAttributes >, boost::shared_ptr< ::osiris::Object > >();
    }

}
