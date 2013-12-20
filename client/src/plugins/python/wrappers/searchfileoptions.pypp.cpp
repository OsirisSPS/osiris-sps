// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "searchfileoptions.h"
#include "idesearchparam.h"
#include "searchfileoptions.pypp.hpp"

namespace bp = boost::python;

struct SearchFileOptions_wrapper : ::osiris::SearchFileOptions, ::osiris::PythonWrapper< ::osiris::SearchFileOptions > {

    SearchFileOptions_wrapper(::osiris::SearchFileOptions const & arg )
    : ::osiris::SearchFileOptions( arg )
      , ::osiris::PythonWrapper< ::osiris::SearchFileOptions >(){
        // copy constructor
        
    }

    SearchFileOptions_wrapper( )
    : ::osiris::SearchFileOptions( )
      , ::osiris::PythonWrapper< ::osiris::SearchFileOptions >(){
        // nullptr constructor
    
    }

    virtual ::boost::shared_ptr< osiris::IdeSearchParam > getControl(  ) {
        ::osiris::PythonState __pystate(getPythonThreadState());
        if( ::osiris::PythonOverride func_getControl = this->get_override( "getControl" ) )
            return func_getControl(  );
        else{
            __pystate.leave();
            return this->::osiris::ISearchOptions::getControl(  );
        }
    }
    
    ::boost::shared_ptr< osiris::IdeSearchParam > default_getControl(  ) {
        ::osiris::PythonThreadSaver __pythreadSaver;
        return ::osiris::ISearchOptions::getControl( );
    }

};

void register_SearchFileOptions_class(){

    ::boost::python::class_< SearchFileOptions_wrapper, ::boost::python::bases< ::osiris::ISearchOptions > >( "SearchFileOptions", ::boost::python::init< >() )    
        .def( 
            "getControl"
            , (::boost::shared_ptr< osiris::IdeSearchParam > ( ::osiris::ISearchOptions::* )(  ) )(&::osiris::ISearchOptions::getControl)
            , (::boost::shared_ptr< osiris::IdeSearchParam > ( SearchFileOptions_wrapper::* )(  ) )(&SearchFileOptions_wrapper::default_getControl) );

}
