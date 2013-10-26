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

#ifndef _ENGINE_IDS_H
#define _ENGINE_IDS_H

#include "uniqueid.h"

#ifdef OS_DEBUGID
#include "objectsobjects.h" // TOCLEAN, when i clean the "Generate".
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

#define ObjectOrEntityID String // TOCLEAN: usato probabilmente temporaneamente, ad esempio per "reference" di IStatistics.

#ifndef OS_DEBUGID
	#define EntityID UniqueID
	#ifdef OS_NOOBJECTID
		#define ObjectID UniqueID
	#endif
	#define PortalID UniqueID
	#define PovID UniqueID
	#define ExtensionID UniqueID
	#define ExtensionsModuleID UniqueID
	#define ExtensionsComponentID UniqueID
	#define SkinID UniqueID

#else
// Maybe exists a better approach: We use different class, to avoid copy between them without compilation errors.
class EngineExport EntityID : public UniqueID 
{ 
public:
	EntityID() { };
	EntityID(const std::string &id) : UniqueID(id) { };
	EntityID(const EntityID &second) : UniqueID(second) { };
	EntityID(const UniqueID &second) : UniqueID(second) { };
public:
	static const EntityID EMPTY;
};

#ifdef OS_NOOBJECTID
class EngineExport ObjectID : public UniqueID 
{ 
public:
	ObjectID() { };
	ObjectID(const std::string &id) : UniqueID(id) { };
	ObjectID(const ObjectID &second) : UniqueID(second) { };
	ObjectID(const UniqueID &second) : UniqueID(second) { };

	static ObjectID generate() { return UniqueID::generate(); }

	std::string getHash() const { return getString(); }; // TOCLEAN
	//static ObjectID generate(PortalObjectType type, const std::string &hash) { return ObjectID(hash); } // TOCLEAN
public:
	static const ObjectID EMPTY;
};
#endif

class EngineExport PortalID : public UniqueID 
{ 
public:
	PortalID() { };
	PortalID(const std::string &id) : UniqueID(id) { };
	PortalID(const PortalID &second) : UniqueID(second) { };
	PortalID(const UniqueID &second) : UniqueID(second) { };
public:
	static const PortalID EMPTY;
};

class EngineExport PovID : public UniqueID 
{ 
public:
	PovID() { };
	PovID(const std::string &id) : UniqueID(id) { };
	PovID(const PovID &second) : UniqueID(second) { };
	PovID(const UniqueID &second) : UniqueID(second) { };
public:
	static const PovID EMPTY;
};

class EngineExport ExtensionID : public UniqueID 
{ 
public:
	ExtensionID() { };
	ExtensionID(const std::string &id) : UniqueID(id) { };
	ExtensionID(const ExtensionID &second) : UniqueID(second) { };
	ExtensionID(const UniqueID &second) : UniqueID(second) { };
};

class EngineExport ExtensionsModuleID : public UniqueID 
{ 
public:
	ExtensionsModuleID() { };
	ExtensionsModuleID(const std::string &id) : UniqueID(id) { };
	ExtensionsModuleID(const ExtensionsModuleID &second) : UniqueID(second) { };
	ExtensionsModuleID(const UniqueID &second) : UniqueID(second) { };
public:
	static const ExtensionsModuleID EMPTY;
};

class EngineExport ExtensionsComponentID : public UniqueID 
{ 
public:
	ExtensionsComponentID() { };
	ExtensionsComponentID(const std::string &id) : UniqueID(id) { };
	ExtensionsComponentID(const ExtensionsComponentID &second) : UniqueID(second) { };
	ExtensionsComponentID(const UniqueID &second) : UniqueID(second) { };
public:
	static const ExtensionsComponentID EMPTY;
};

class EngineExport SkinID : public UniqueID 
{ 
public:
	SkinID() { };
	SkinID(const std::string &id) : UniqueID(id) { };
	SkinID(const SkinID &second) : UniqueID(second) { };
	SkinID(const UniqueID &second) : UniqueID(second) { };
};

#endif


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_IDS_H
