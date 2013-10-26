// <osiris_sps_source_header>
// This file is part of Osiris Serverless Portal System.
// Copyright (C)2005-2012 Osiris Team (info@osiris-sps.org) / http://www.osiris-sps.org )
//
// Osiris Serverless Portal System is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Osiris Serverless Portal System is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Osiris Serverless Portal System.  If not, see <http://www.gnu.org/licenses/>.
// </osiris_sps_source_header>

#ifndef _OS_PYTHON_PYPLUSPLUSCOMMON_H
#define _OS_PYTHON_PYPLUSPLUSCOMMON_H

//////////////////////////////////////////////////////////////////////

#include "pythonoverride.h"
#include "pythonstate.h"
#include "pythonthreadsaver.h"
#include "pythonwrapper.h"

//////////////////////////////////////////////////////////////////////

// TODO: files temporanei di appoggio per le dipendenze degli alias del Py++ (andrebbero generati via script)

// Archives
#include "iarchivesarchiveentry.h"

// Core
#include "ibackgroundjob.h"
#include "log.h"
#include "crypt.h"
#include "thirdpartylibrariesreporter.h"

// Db
#include "idbdriver.h"
#include "idbparameter.h"

// Engine
#include "isearchoptions.h"
#include "entitiessnapshotmanager.h"
#include "ideide.h"
#include "ideaccount.h"
#include "objectsobjectscache.h"
#include "omlemoticon.h"
#include "omlitem.h"
#include "portalsmessenger.h"
#include "portalspeer.h"
#include "portalsportal.h"
#include "searchsearch.h"


// Http
#include "ihttpdirectoryhandler.h"
#include "http.h"
#include "httpcookie.h"

// Lang
#include "languagefolder.h"

// Web
#include "htmlunit.h"



//////////////////////////////////////////////////////////////////////

// TODO: verificare perchè non vengono generati in automatico? in quanto classi virtuali pure?

#include "indexing_suite/deque.hpp"
#include "indexing_suite/list.hpp"

namespace boost { namespace python { namespace indexing {

template<>
struct value_traits< ::boost::shared_ptr< OS_NAMESPACE_NAME::IBackgroundJob > >{

    static bool const equality_comparable = false;
    static bool const less_than_comparable = false;

    template<typename PythonClass, typename Policy>
    static void visit_container_class(PythonClass &, Policy const &){ }
};

} } }

namespace boost { namespace python { namespace indexing {

template<>
struct value_traits< ::boost::shared_ptr< OS_NAMESPACE_NAME::IHttpDirectoryHandler > >{

    static bool const equality_comparable = false;
    static bool const less_than_comparable = false;

    template<typename PythonClass, typename Policy>
    static void visit_container_class(PythonClass &, Policy const &){ }
};

} } }

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYPLUSPLUSCOMMON_H
