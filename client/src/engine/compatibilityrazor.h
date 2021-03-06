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

#ifndef _COMPATIBILITYRAZOR_H
#define _COMPATIBILITYRAZOR_H

//////////////////////////////////////////////////////////////////////

#define OS_NOOBJECTID

//#define OS_ENTITYID
#define OS_TODOCIP // If defined, CIP logic are used.
#define OS_NEWSIGNATURE

// If OS_DEBUGID is defined, ids.h generate different class for each ID, to have compilation error if ID mismatch.
// Define only for compilation test, actually it's failed in execution (i don't understand why python don't see EntityID::EMPTY.
//#define OS_DEBUGID

//////////////////////////////////////////////////////////////////////

#endif // _COMPATIBILITY_MANAGER_H
