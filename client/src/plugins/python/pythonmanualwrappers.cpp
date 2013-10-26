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

#include "stdafx.h"
#include "pythonmanualwrappers.h"

#include "engine.h"
#include "ibackgroundjob.h"
#include "ordered_map.h"
#include "uniqueid.h"
#include "iextensionsextension.h"
#include "isisendpoint.h"
#include "portalsportaloptions.h"
#include "thirdpartylibrariesreporter.h"
#include "thirdpartylibrary.h"
#include "wrappers/indexing_suite/deque.hpp"
#include "wrappers/indexing_suite/list.hpp"
#include "wrappers/indexing_suite/map.hpp"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

template <typename T, boost::python::indexing::method_set_type MethodMask = boost::python::indexing::all_methods>
struct PythonListSuiteExposer
{
	static void expose(bool register_ptr = false)
	{
		// N.B.: se fosse necessario istanziare la lista in python, sarebbe meglio usare un nome non ricavato dal type_id

		boost::python::class_<T>(boost::python::type_id<T>().name())
			.def(boost::python::indexing::list_suite<T, MethodMask>());

		if(register_ptr)
			boost::python::register_ptr_to_python<boost::shared_ptr<T> >();
	}
};

template <typename T, boost::python::indexing::method_set_type MethodMask = boost::python::indexing::all_methods>
struct PythonDequeSuiteExposer
{
	static void expose(bool register_ptr = false)
	{
		// N.B.: se fosse necessario istanziare il deque in python, sarebbe meglio usare un nome non ricavato dal type_id

		boost::python::class_<T>(boost::python::type_id<T>().name())
			.def(boost::python::indexing::deque_suite<T, MethodMask>());

		if(register_ptr)
			boost::python::register_ptr_to_python<boost::shared_ptr<T> >();
	}
};

template <typename T, boost::python::indexing::method_set_type MethodMask = boost::python::indexing::all_methods>
struct PythonMapSuiteExposer
{
	static void expose(bool register_ptr = false)
	{
		// N.B.: se fosse necessario istanziare la mappa in python, sarebbe meglio usare un nome non ricavato dal type_id

		boost::python::class_<T>(boost::python::type_id<T>().name())
			.def(boost::python::indexing::map_suite<T, MethodMask>());

		if(register_ptr)
			boost::python::register_ptr_to_python<boost::shared_ptr<T> >();
	}
};

void initPythonManualWrappers()
{
	PythonMapSuiteExposer<ordered_map<std::string, std::string> >::expose();
	PythonMapSuiteExposer<ordered_map<std::wstring, std::wstring> >::expose();
	PythonMapSuiteExposer<ordered_map<String, String> >::expose();

	// IBackgroundJob non supporta gli operatori < e == quindi vengono disabilitati i metodi di ricerca e di ordinamento in Python (da modifica se in futuro vengono aggiunti)
	PythonDequeSuiteExposer<Engine::BackgroundJobs, boost::python::indexing::all_methods & ~(boost::python::indexing::search_methods | boost::python::indexing::reorder_methods)>::expose(true);
	PythonMapSuiteExposer<ordered_map<UniqueID, shared_ptr<IExtensionsExtension> > >::expose();
	PythonMapSuiteExposer<PortalOptions::IsisEndpoints>::expose();
	// N.B.: vedi note sopra di IBackgroundJob
	PythonListSuiteExposer<ThirdPartyLibrariesReporter::Libraries, boost::python::indexing::all_methods & ~(boost::python::indexing::search_methods | boost::python::indexing::reorder_methods)>::expose();
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
