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

#ifndef _PORTALSOPTIONSHARED_H
#define _PORTALSOPTIONSHARED_H

#include "base/object.h"
#include "boost/thread/recursive_mutex.hpp"
#include "collections.h"
#include "dbdb.h"
#include "locked.h"
#include "portalsportal.h"
#include "version.h"
#include "xmloptions.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ConnectionsManager;
class DateTime;
class IDbOptions;
class IPortalDatabase;
class IsisEndpoint;
class Portal;
class XMLDocument;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport PortalsOptionsShared : public Object
{
// Construction
public:
	PortalsOptionsShared(const shared_ptr<Portal> portal);
	virtual ~PortalsOptionsShared();

// Attributes
public:
	
	inline const shared_ptr<Portal> getPortal() const;

	String getPortalName() const;
	void setPortalName(const String &name);

	String getPortalDescription() const;
	void setPortalDescription(const String &description);

// POV Rules (change stabilization):

	ObjectsReputationThreshold getAuthorsReputationThreshold() const;
	void setAuthorsReputationThreshold(const ObjectsReputationThreshold &threshold);

	ObjectsReputationThreshold getEditorsReputationThreshold() const;
	void setEditorsReputationThreshold(const ObjectsReputationThreshold &threshold);

	String getPovWhiteList() const;
	void setPovWhiteList(const String &value);

	String getPovBlackList() const;
	void setPovBlackList(const String &value);

	uint32 getAvatarMaxWidth() const;
	void setAvatarMaxWidth(const uint32 &value);

	uint32 getAvatarMaxHeight() const;
	void setAvatarMaxHeight(const uint32 &value);

	bool getObjectsPhysicalRemove() const;
	void setObjectsPhysicalRemove(const bool &value);

	int32 getObjectsPhysicalRemoveDays() const;
	void setObjectsPhysicalRemoveDays(const int32 &value);

	uint32 getObjectsMaxSize() const;
	void setObjectsMaxSize(const uint32 &value);

	bool getAllowObjectInFuture() const;
	void setAllowObjectInFuture(const bool &value);

	bool getAllowObjectUnsigned() const;
	void setAllowObjectUnsigned(const bool &value);

	String getBadWords() const;
	void setBadWords(const String &value);

// Layout

	String getRegisterTerms() const;
	void setRegisterTerms(const String &value);

	ObjectID getLayoutTileImage() const;
	void setLayoutTileImage(const ObjectID &value);

	String getLayoutTileColorBackground() const;
	void setLayoutTileColorBackground(const String &value);

	String getLayoutTileColorForeground() const;
	void setLayoutTileColorForeground(const String &value);

	String getLayoutCss() const;
	void setLayoutCss(const String &value);

	String getLayoutComponent() const;
	void setLayoutComponent(const String &value);

	String getLayoutSkin() const;
	void setLayoutSkin(const String &value);

	String getLayoutSkinParams() const;
	void setLayoutSkinParams(const String &value);
	
	bool getLayoutHeader() const;
	void setLayoutHeader(const bool &value);



	std::string getAcceptableHash() const;
	

// Operations
public:
	void clear();
	
	bool validate() const;

	void reload(const shared_ptr<IPortalDatabase> &database);

	bool writeString(String &data);
	
	void unload();

private:
	//bool _readXML(shared_ptr<XMLDocument> document);
	//bool _writeXML(shared_ptr<XMLDocument> document) const;

	void addToMap(std::string &raw, const std::string &name) const;
	
public:
	//static const String NAME;
	//static const String DESCRIPTION;	

	/*
	struct EngineExport options
	{
		static const String portal_name;
		static const String portal_description;		
		static const int threshold_author;
		static const int threshold_editor;
		static const String objects_size_limit;		
		static const String register_terms;		
	};
	*/

private:
	weak_ptr<Portal> m_portal;
	shared_ptr<XMLOptions> m_options;	

	std::string m_latestAcceptableHash;
};

//////////////////////////////////////////////////////////////////////

inline const shared_ptr<Portal> PortalsOptionsShared::getPortal() const { return m_portal.lock();; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _PORTALSOPTIONSHARED_H
