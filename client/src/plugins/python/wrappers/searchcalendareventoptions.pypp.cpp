// This file has been generated by Py++.

#include "stdafx.h"
#include "pypluspluscommon.h"
#include "boost/python.hpp"
#include "searchcalendareventoptions.h"
#include "idesearchparam.h"
#include "searchcalendareventoptions.pypp.hpp"

namespace bp = boost::python;

struct SearchCalendarEventOptions_wrapper : ::osiris::SearchCalendarEventOptions, ::osiris::PythonWrapper< ::osiris::SearchCalendarEventOptions > {

    SearchCalendarEventOptions_wrapper(::osiris::SearchCalendarEventOptions const & arg )
    : ::osiris::SearchCalendarEventOptions( arg )
      , ::osiris::PythonWrapper< ::osiris::SearchCalendarEventOptions >(){
        // copy constructor
        
    }

    SearchCalendarEventOptions_wrapper( )
    : ::osiris::SearchCalendarEventOptions( )
      , ::osiris::PythonWrapper< ::osiris::SearchCalendarEventOptions >(){
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

void register_SearchCalendarEventOptions_class(){

    ::boost::python::class_< SearchCalendarEventOptions_wrapper, ::boost::python::bases< ::osiris::ISearchOptions > >( "SearchCalendarEventOptions", ::boost::python::init< >() )    
        .def( 
            "getControl"
            , (::boost::shared_ptr< osiris::IdeSearchParam > ( ::osiris::ISearchOptions::* )(  ) )(&::osiris::ISearchOptions::getControl)
            , (::boost::shared_ptr< osiris::IdeSearchParam > ( SearchCalendarEventOptions_wrapper::* )(  ) )(&SearchCalendarEventOptions_wrapper::default_getControl) );

}
