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

#ifndef _OS_ENGINE_XMLEXPORTER_H
#define _OS_ENGINE_XMLEXPORTER_H

#include "dbdb.h"
#include "ideide.h"
#include "base/object.h"
#include "portalsportals.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

#define OS_IDE_XML_ACTIONS			_S("actions")
#define OS_IDE_XML_ACTION			_S("action")
#define OS_IDE_XML_ACTION_NAME		_S("name")
#define OS_IDE_XML_ACTION_HREF		_S("href")
#define OS_IDE_XML_ACTION_GROUP		_S("group")
#define OS_IDE_XML_ACTION_CONFIRM	_S("confirm")
#define OS_IDE_XML_LINKS			_S("links")
#define OS_IDE_XML_LINK				_S("link")
#define OS_IDE_XML_LINK_NAME		_S("name")
#define OS_IDE_XML_LINK_HREF		_S("href")
#define OS_IDE_XML_STATS			_S("stats")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class XMLAttribute;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport XMLExporter : public Object
{
// Construction
public:
	XMLExporter(shared_ptr<XMLNode> root);
	XMLExporter(shared_ptr<XMLNode> root, const XMLExporter &second);
	virtual ~XMLExporter();

// Attributes
public:
	inline shared_ptr<XMLNode> getRoot() const;

	// Restituisce il nodo delle azioni
	inline shared_ptr<XMLNode> getNodeActions() const;
	// Restituisce il nodo dei links
	inline shared_ptr<XMLNode> getNodeLinks() const;
	// Restituisce il nodo delle statistiche
	inline shared_ptr<XMLNode> getNodeStats() const;

	inline void setAttributeBool(const String &name, bool value);
	inline void setAttributeInt32(const String &name, int32 value);
	inline void setAttributeUint32(const String &name, uint32 value);
	inline void setAttributeDouble(const String &name, double value);
	inline void setAttributeDateTime(const String &name, const DateTime &value);
	inline void setAttributeBuffer(const String &name, const Buffer &value);
	inline void setAttributeString(const String &name, const String &value);

	// Restituisce il sottonodo specificato creandolo se non esiste quando ensure = true
	shared_ptr<XMLNode> getNode(const String &name, bool ensure = true) const;

// Operations
public:
	// Crea un'azione
	shared_ptr<XMLNode> createAction(const String &name, const String &href, const String &group = String::EMPTY, bool requireConfirm = false);
	// Crea un links
	shared_ptr<XMLNode> createLink(const String &name, const String &href, const String &group = String::EMPTY);

	// Crea un sottonodo
	shared_ptr<XMLNode> createNode(const String &name) const;

	// Crea un sottocontesto di esportazione
	template <typename T>
	shared_ptr<T> createChild(shared_ptr<XMLNode> node);

	template <typename T>
	shared_ptr<T> createChild(const String &name);

protected:
	mutable shared_ptr<XMLNode> m_root;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<XMLNode> XMLExporter::getRoot() const { return m_root; }

inline shared_ptr<XMLNode> XMLExporter::getNodeActions() const { return getNode(OS_IDE_XML_ACTIONS); }
inline shared_ptr<XMLNode> XMLExporter::getNodeLinks() const { return getNode(OS_IDE_XML_LINKS); }
inline shared_ptr<XMLNode> XMLExporter::getNodeStats() const { return getNode(OS_IDE_XML_STATS); }

inline void XMLExporter::setAttributeBool(const String &name, bool value) { return getRoot()->setAttributeBool(name, value); }
inline void XMLExporter::setAttributeInt32(const String &name, int32 value) { return getRoot()->setAttributeInt32(name, value); }
inline void XMLExporter::setAttributeUint32(const String &name, uint32 value) { return getRoot()->setAttributeUint32(name, value); }
inline void XMLExporter::setAttributeDouble(const String &name, double value) { return getRoot()->setAttributeDouble(name, value); }
inline void XMLExporter::setAttributeDateTime(const String &name, const DateTime &value) { return getRoot()->setAttributeDateTime(name, value); }
inline void XMLExporter::setAttributeBuffer(const String &name, const Buffer &value) { return getRoot()->setAttributeBuffer(name, value); }
inline void XMLExporter::setAttributeString(const String &name, const String &value) { return getRoot()->setAttributeString(name, value); }

//////////////////////////////////////////////////////////////////////

template <typename T>
shared_ptr<T> XMLExporter::createChild(shared_ptr<XMLNode> node)
{
	OS_ASSERT(dynamic_cast<T *>(this) != nullptr);
	return shared_ptr<T>(OS_NEW T(node, dynamic_cast<const T &>(*this)));
}

template <typename T>
shared_ptr<T> XMLExporter::createChild(const String &name)
{
	return createChild<T>(createNode(name));
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_XMLEXPORTER_H
