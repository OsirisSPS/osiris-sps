#include "stdafx.h"
#include "objectsobjectid.h"

#ifndef OS_NOOBJECTID

#include "objectsdescriptor.h"
#include "objectssystem.h"
#include "objectssigner.h"
#include "convert.h"
#include "iportaldatabase.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const ObjectID ObjectID::EMPTY;

//////////////////////////////////////////////////////////////////////

ObjectID::ObjectID()
{

}

ObjectID::ObjectID(const std::string &id) : BaseID(id)
{

}

ObjectID::ObjectID(const ObjectID &second) : BaseID(second)
{

}

ObjectID::~ObjectID()
{

}

ObjectID & ObjectID::operator=(const std::string &id)
{
	ID::operator =(id);
	return *this;
}

ObjectID ObjectID::toObjectID() const
{
	return *this;
}

EntityID ObjectID::toEntityID() const
{
#ifdef OS_ENTITYID
	return EntityID(getString());
#else
	return EntityID(getHash());
#endif
}

// TOCLEAN_OBJECTID_OBJECTTYPE
PortalObjectType ObjectID::getObjectType() const
{
	return Convert::toObjectType(m_header.getFirst());
}

/*
String ObjectID::getObjectTypeName() const
{
	PortalObjectType type = getObjectType();
	if(type != portalObjectTypeUnknown)
	{
		shared_ptr<ObjectsIDescriptor> descriptor = ObjectsSystem::instance()->getDescriptor(type);
		if(descriptor != nullptr)
			return descriptor->getTypeName();
	}

	return OS_PORTAL_OBJECT_UNKNOWN_TYPENAME;
}
*/

/* // TOCLEAN_OBJECTID_PRIMARY_FLAGS
bool ObjectID::isPrimary() const
{
	return (m_header.getSecond() & OS_OBJECTID_FLAGS_REVISION) != OS_OBJECTID_FLAGS_REVISION;
}

bool ObjectID::isRevision() const
{
	return (m_header.getSecond() & OS_OBJECTID_FLAGS_REVISION) == OS_OBJECTID_FLAGS_REVISION;
}
*/

/*
const ObjectID & ObjectID::getRootID()
{
	return EMPTY;
}
*/

bool ObjectID::validateObject(bool allow_null) const
{
	return _validate(allow_null, true);
}

bool ObjectID::validateRevisionable(bool allow_null, bool require_primary) const
{
	return _validate(allow_null, require_primary);
}

ObjectID ObjectID::generate(PortalObjectType type, const std::string &hash)
{
	OS_EXCEPT_IF(IPortalDatabase::isRevisionable(type), "The specified object is revisionable, cannot generate a valid ID");

	// Nota: di default gli oggetti non revisionabili vengono classificati come primarie
	return generate(type, hash, true/*, false*/);
}

ObjectID ObjectID::generate(PortalObjectType type, const std::string &hash, bool isPrimary/*, bool isVirtual*/) // TOCLEAN, prima tolgo isVirtual, poi isPrimary
{
	Header header(idObject);
	header.setFirst(static_cast<byte>(type));

	byte flags = OS_OBJECTID_FLAGS_NONE;
	// Se bisogna generare l'id di una revisione setta il flag di revisione
	if(isPrimary == false)
        flags |= OS_OBJECTID_FLAGS_REVISION;

	bool isVirtual = false; // TOCLEAN
	if(isVirtual)
		flags |= OS_OBJECTID_FLAGS_VIRTUAL;

	header.setSecond(flags);

	return generateID(header, hash);
}

ID::IDType ObjectID::getValidationType() const
{
	return idObject;
}

bool ObjectID::_validate(bool allow_null, bool require_primary) const
{
	if(ID::validate(allow_null) == false)
		return false;

	// Verifica il caso sia richiesta una primaria ma sia ammesso nullptr
	if(allow_null && empty())
		return true;

	// TOCLEAN_OBJECTID_OBJECTTYPE
	/*
	// Verifica se l'id rappresenta una revisione
	if(require_primary && isRevision())
		return false;
	*/

	return true;
}

//////////////////////////////////////////////////////////////////////

#ifdef OS_ENTITYID

const EntityID EntityID::EMPTY;

//////////////////////////////////////////////////////////////////////
fdsfd
EntityID::EntityID()
{

}

EntityID::EntityID(const std::string &id) : BaseID(id)
{

}

EntityID::EntityID(const EntityID &second) : BaseID(second)
{

}

EntityID::~EntityID()
{

}

EntityID & EntityID::operator=(const std::string &id)
{
	ID::operator =(id);
	return *this;
}

ObjectID EntityID::toObjectID() const
{
	return ObjectID(getString());
}

EntityID EntityID::toEntityID() const
{
	return *this;
}

// TOCLEAN_OBJECTID_OBJECTTYPE
PortalObjectType EntityID::getObjectType() const
{
	return Convert::toObjectType(m_header.getFirst());
}

bool EntityID::validateObject(bool allow_null) const
{
	return _validate(allow_null, true);
}

bool EntityID::validateRevisionable(bool allow_null, bool require_primary) const
{
	return _validate(allow_null, require_primary);
}

EntityID EntityID::generate(PortalObjectType type, const std::string &hash)
{
	OS_EXCEPT_IF(IPortalDatabase::isRevisionable(type), "The specified object is revisionable, cannot generate a valid ID");

	// Nota: di default gli oggetti non revisionabili vengono classificati come primarie
	return generate(type, hash, true/*, false*/);
}

EntityID EntityID::generate(PortalObjectType type, const std::string &hash, bool isPrimary/*, bool isVirtual*/) // TOCLEAN, prima tolgo isVirtual, poi isPrimary
{
	Header header(idObject);
	header.setFirst(static_cast<byte>(type));

	byte flags = OS_OBJECTID_FLAGS_NONE;
	// Se bisogna generare l'id di una revisione setta il flag di revisione
	if(isPrimary == false)
        flags |= OS_OBJECTID_FLAGS_REVISION;

	bool isVirtual = false; // TOCLEAN
	if(isVirtual)
		flags |= OS_OBJECTID_FLAGS_VIRTUAL;

	header.setSecond(flags);

	return generateID(header, hash);
}

ID::IDType EntityID::getValidationType() const
{
	return idObject;
}

bool EntityID::_validate(bool allow_null, bool require_primary) const
{
	if(ID::validate(allow_null) == false)
		return false;

	// Verifica il caso sia richiesta una primaria ma sia ammesso nullptr
	if(allow_null && empty())
		return true;

	// TOCLEAN_OBJECTID_OBJECTTYPE
	/*
	// Verifica se l'id rappresenta una revisione
	if(require_primary && isRevision())
		return false;
	*/

	return true;
}

#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif
