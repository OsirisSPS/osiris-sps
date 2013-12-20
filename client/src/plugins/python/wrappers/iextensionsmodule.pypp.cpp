// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "__call_policies.pypp.hpp"
#include "iextensionsmodule.h"
#include "ids.h"
#include "extensionsmoduleeditor.h"
#include "extensionsmoduleviewer.h"
#include "iextensionsmodule.pypp.hpp"

namespace bp = boost::python;

struct IExtensionsModule_wrapper : ::osiris::IExtensionsModule, ::osiris::PythonWrapper< ::osiris::IExtensionsModule > {

    IExtensionsModule_wrapper( )
    : ::osiris::IExtensionsModule( )
      , ::osiris::PythonWrapper< ::osiris::IExtensionsModule >(){
        // nullptr constructor
    
    }

    static boost::python::object getCacheContent( ::osiris::IExtensionsModule const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        bool result = inst.getCacheContent();
        __pythreadSaver.restore();
        return boost::python::object( result );
    }

    static boost::python::object getDefaultStyle( ::osiris::IExtensionsModule const & inst ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::String const & result = inst.getDefaultStyle();
        __pythreadSaver.restore();
        typedef bp::return_value_policy< bp::copy_const_reference > call_policies_t;
        return boost::python::object( pyplusplus::call_policies::make_object< call_policies_t, ::osiris::String const & >( result ) );
    }

    virtual void exportXML( ::osiris::UniqueID const & id, ::boost::shared_ptr< osiris::XMLPortalExporter > exporter ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_exportXML = this->get_override( "exportXML" ) )
            func_exportXML( boost::ref(id), exporter );
        else{
            __pystate.leave();
            this->::osiris::IExtensionsModule::exportXML( boost::ref(id), exporter );
        }
    }
    
    void default_exportXML( ::osiris::UniqueID const & id, ::boost::shared_ptr< osiris::XMLPortalExporter > exporter ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IExtensionsModule::exportXML( boost::ref(id), exporter );
    }

    virtual ::osiris::UniqueID getID(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_getID = this->get_override( "getID" );
        return func_getID(  );
    }

    virtual ::osiris::String getName(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_getName = this->get_override( "getName" );
        return func_getName(  );
    }

    virtual ::boost::shared_ptr< osiris::ExtensionsModuleEditor > createEditor(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_createEditor = this->get_override( "createEditor" );
        return func_createEditor(  );
    }

    virtual ::boost::shared_ptr< osiris::ExtensionsModuleViewer > createViewer(  ){
        ::osiris::PythonState __pystate(getPythonThreadState());
        ::osiris::PythonOverride func_createViewer = this->get_override( "createViewer" );
        return func_createViewer(  );
    }

    virtual void initXML( ::boost::shared_ptr< osiris::XMLDocument > document ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_initXML = this->get_override( "initXML" ) )
            func_initXML( document );
        else{
            __pystate.leave();
            this->::osiris::IExtensionsModule::initXML( document );
        }
    }
    
    void default_initXML( ::boost::shared_ptr< osiris::XMLDocument > document ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        ::osiris::IExtensionsModule::initXML( document );
    }

    static void setCacheContent( ::osiris::IExtensionsModule & inst, bool cache ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setCacheContent(cache);
        __pythreadSaver.restore();
    }

    static void setDefaultStyle( ::osiris::IExtensionsModule & inst, ::osiris::String const & style ){
        ::osiris::PythonThreadSaver __pythreadSaver;
        inst.setDefaultStyle(style);
        __pythreadSaver.restore();
    }

};

void register_IExtensionsModule_class(){

    { //::osiris::IExtensionsModule
        typedef ::boost::python::class_< IExtensionsModule_wrapper, ::boost::noncopyable > IExtensionsModule_exposer_t;
        IExtensionsModule_exposer_t IExtensionsModule_exposer = IExtensionsModule_exposer_t( "IExtensionsModule", ::boost::python::no_init );
        ::boost::python::scope IExtensionsModule_scope( IExtensionsModule_exposer );
        IExtensionsModule_exposer.def( ::boost::python::init< >() );
        { //::osiris::IExtensionsModule::getCacheContent
        
            typedef boost::python::object ( *getCacheContent_function_type )( ::osiris::IExtensionsModule const & );
            
            IExtensionsModule_exposer.def( 
                "getCacheContent"
                , getCacheContent_function_type( &IExtensionsModule_wrapper::getCacheContent ) );
        
        }
        { //::osiris::IExtensionsModule::getDefaultStyle
        
            typedef boost::python::object ( *getDefaultStyle_function_type )( ::osiris::IExtensionsModule const & );
            
            IExtensionsModule_exposer.def( 
                "getDefaultStyle"
                , getDefaultStyle_function_type( &IExtensionsModule_wrapper::getDefaultStyle ) );
        
        }
        { //::osiris::IExtensionsModule::exportXML
        
            typedef void ( ::osiris::IExtensionsModule::*exportXML_function_type )( ::osiris::UniqueID const &,::boost::shared_ptr< osiris::XMLPortalExporter > ) ;
            typedef void ( IExtensionsModule_wrapper::*default_exportXML_function_type )( ::osiris::UniqueID const &,::boost::shared_ptr< osiris::XMLPortalExporter > ) ;
            
            IExtensionsModule_exposer.def( 
                "exportXML"
                , exportXML_function_type(&::osiris::IExtensionsModule::exportXML)
                , default_exportXML_function_type(&IExtensionsModule_wrapper::default_exportXML)
                , ( ::boost::python::arg("id"), ::boost::python::arg("exporter") ) );
        
        }
        { //::osiris::IExtensionsModule::getID
        
            typedef ::osiris::UniqueID ( ::osiris::IExtensionsModule::*getID_function_type )(  ) ;
            
            IExtensionsModule_exposer.def( 
                "getID"
                , ( getID_function_type(&::osiris::IExtensionsModule::getID) ) );
        
        }
        { //::osiris::IExtensionsModule::getName
        
            typedef ::osiris::String ( ::osiris::IExtensionsModule::*getName_function_type )(  ) ;
            
            IExtensionsModule_exposer.def( 
                "getName"
                , ( getName_function_type(&::osiris::IExtensionsModule::getName) ) );
        
        }
        { //::osiris::IExtensionsModule::createEditor
        
            typedef ::boost::shared_ptr<osiris::ExtensionsModuleEditor> ( ::osiris::IExtensionsModule::*createEditor_function_type )(  ) ;
            
            IExtensionsModule_exposer.def( 
                "createEditor"
                , ( createEditor_function_type(&::osiris::IExtensionsModule::createEditor) ) );
        
        }
        { //::osiris::IExtensionsModule::createViewer
        
            typedef ::boost::shared_ptr<osiris::ExtensionsModuleViewer> ( ::osiris::IExtensionsModule::*createViewer_function_type )(  ) ;
            
            IExtensionsModule_exposer.def( 
                "createViewer"
                , ( createViewer_function_type(&::osiris::IExtensionsModule::createViewer) ) );
        
        }
        { //::osiris::IExtensionsModule::initXML
        
            typedef void ( ::osiris::IExtensionsModule::*initXML_function_type )( ::boost::shared_ptr< osiris::XMLDocument > ) ;
            typedef void ( IExtensionsModule_wrapper::*default_initXML_function_type )( ::boost::shared_ptr< osiris::XMLDocument > ) ;
            
            IExtensionsModule_exposer.def( 
                "initXML"
                , initXML_function_type(&::osiris::IExtensionsModule::initXML)
                , default_initXML_function_type(&IExtensionsModule_wrapper::default_initXML)
                , ( ::boost::python::arg("document") ) );
        
        }
        { //::osiris::IExtensionsModule::setCacheContent
        
            typedef void ( *setCacheContent_function_type )( ::osiris::IExtensionsModule &,bool );
            
            IExtensionsModule_exposer.def( 
                "setCacheContent"
                , setCacheContent_function_type( &IExtensionsModule_wrapper::setCacheContent )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("cache") ) );
        
        }
        { //::osiris::IExtensionsModule::setDefaultStyle
        
            typedef void ( *setDefaultStyle_function_type )( ::osiris::IExtensionsModule &,::osiris::String const & );
            
            IExtensionsModule_exposer.def( 
                "setDefaultStyle"
                , setDefaultStyle_function_type( &IExtensionsModule_wrapper::setDefaultStyle )
                , ( ::boost::python::arg("inst"), ::boost::python::arg("style") ) );
        
        }
        { //property "cacheContent"[fget=::osiris::IExtensionsModule::getCacheContent, fset=::osiris::IExtensionsModule::setCacheContent]
        
            typedef bool ( ::osiris::IExtensionsModule::*fget )(  ) const;
            typedef void ( ::osiris::IExtensionsModule::*fset )( bool ) ;
            
            IExtensionsModule_exposer.add_property( 
                "cacheContent"
                , fget( &::osiris::IExtensionsModule::getCacheContent )
                , fset( &::osiris::IExtensionsModule::setCacheContent )
                , "get\\set property, built on top of \"bool osiris::IExtensionsModule::getCacheContent() const [member function]\" and \"void osiris::IExtensionsModule::setCacheContent(bool cache) [member function]\"" );
        
        }
        { //property "defaultStyle"[fget=::osiris::IExtensionsModule::getDefaultStyle, fset=::osiris::IExtensionsModule::setDefaultStyle]
        
            typedef ::osiris::String const & ( ::osiris::IExtensionsModule::*fget )(  ) const;
            typedef void ( ::osiris::IExtensionsModule::*fset )( ::osiris::String const & ) ;
            
            IExtensionsModule_exposer.add_property( 
                "defaultStyle"
                , ::boost::python::make_function( 
                      fget( &::osiris::IExtensionsModule::getDefaultStyle )
                    , bp::return_value_policy< bp::copy_const_reference >() ) 
                , fset( &::osiris::IExtensionsModule::setDefaultStyle )
                , "get\\set property, built on top of \"osiris::String const & osiris::IExtensionsModule::getDefaultStyle() const [member function]\" and \"void osiris::IExtensionsModule::setDefaultStyle(osiris::String const & style) [member function]\"" );
        
        }
        ::boost::python::register_ptr_to_python< boost::shared_ptr< ::osiris::IExtensionsModule > >();
        ::boost::python::implicitly_convertible< boost::shared_ptr< ::osiris::IExtensionsModule >, boost::shared_ptr< ::osiris::Object > >();
    }

}
