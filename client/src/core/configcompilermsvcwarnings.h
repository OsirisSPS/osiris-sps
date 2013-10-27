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

// N.B.: questo file non necessita ifndef/define/endif

//////////////////////////////////////////////////////////////////////

#pragma warning (disable : 4251)		// '<type> needs to have dll-interface to be used by clients'
#pragma warning (disable : 4275)		// non dll-interface class used as base for dll-interface class
#pragma warning (disable : 4355)		// 'this': used in base member initializer list
#pragma warning (disable : 4661)		// 'no suitable definition provided for explicit template instantiation request'

#if OS_COMPILER_VERSION >= 1400			// Visual Studio 2005
	#pragma warning (disable : 4996)	// '<type> was declared deprecated'
#endif

//////////////////////////////////////////////////////////////////////
