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

#ifndef _IDE_PATHWAY_H
#define _IDE_PATHWAY_H

#include "base/object.h"
#include "dbdb.h"
#include "entitiesentities.h"
#include "ideide.h"
#include "objectsobjects.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport IdePathway : public Object
{
public:
	class EngineExport Path : public Object
	{
	// Construction
	public:
		Path(const String &name, const String &href, const String &id, const String &type = String::EMPTY);
		virtual ~Path();

	// Attributes
	public:
		inline const String & getName() const;
		inline void setName(const String &name);

		inline const String & getHref() const;
		inline void setHref(const String &href);

		inline const ObjectID & getID() const;
		inline void setID(const ObjectID &id);

	// Operations
	public:
		void exportXML(shared_ptr<XMLNode> node) const;

	private:
		String m_name;
		String m_href;
		ObjectID m_id;
	};

	typedef shared_ptr<Path> path_ptr;
	typedef list<path_ptr>::type Paths;

// Construction
public:
	IdePathway();
	virtual ~IdePathway();

// Operations
public:
	void add(path_ptr path);
	void add(const String &name, const String &href);
	void add(const String &name, const String &href, const EntityID &id);
	void add(shared_ptr<IPortalDatabase> database, shared_ptr<EntitiesEntity> entity, ObjectsRenderMode mode);

	void exportXML(shared_ptr<XMLNode> node) const;

protected:
	Paths m_paths;
};

//////////////////////////////////////////////////////////////////////

inline const String & IdePathway::Path::getName() const { return m_name; }
inline void IdePathway::Path::setName(const String &name) { m_name = name; }

inline const String & IdePathway::Path::getHref() const { return m_href; }
inline void IdePathway::Path::setHref(const String &href) { m_href = href; }

inline const ObjectID & IdePathway::Path::getID() const { return m_id; }
inline void IdePathway::Path::setID(const ObjectID &id) { m_id = id; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_PATHWAY_H
