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

#ifndef _OS_PYTHON_PYPLUSPLUSMANUALTEMPLATES_H
#define _OS_PYTHON_PYPLUSPLUSMANUALTEMPLATES_H

#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/stringize.hpp"

//////////////////////////////////////////////////////////////////////

#define OS_PYTHON_ENSURE_TEMPLATE(T)	BOOST_PP_CAT(static const size_t __pythonManualTemplate, OS_CURRENT_LINE) = sizeof(T)

//////////////////////////////////////////////////////////////////////

#ifndef OS_GCCXML
	#error pyplusplusmanualtemplates.h must be included only from GCCXML !
#endif // OS_GCCXML

//////////////////////////////////////////////////////////////////////

namespace pyplusplus {
namespace manualtemplates {

//////////////////////////////////////////////////////////////////////

using namespace OS_NAMESPACE_NAME;

//////////////////////////////////////////////////////////////////////

OS_PYTHON_ENSURE_TEMPLATE(HtmlVirtualPage<IMainPage>);
typedef EventsSource<String, IEvent> EventsSourceStringIEvent;
OS_PYTHON_ENSURE_TEMPLATE(EventsSourceStringIEvent);

//////////////////////////////////////////////////////////////////////

} // manualtemplates
} // pyplusplus

//////////////////////////////////////////////////////////////////////

#endif // _OS_PYTHON_PYPLUSPLUSMANUALTEMPLATES_H
