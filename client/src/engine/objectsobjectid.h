#ifndef _OBJECTS_OBJECTID_H
#define _OBJECTS_OBJECTID_H

#include "ids.h"
#include "objectsobjects.h"

//////////////////////////////////////////////////////////////////////

/*
	Header di un oggetto:
							bytes[0] = PortalObjectType	tipologia di oggetto
							bytes[1] = flags
												bit0		indica se l'oggetto rappresenta una primaria o una revisione (OS_OBJECTID_FLAGS_REVISION)
												bit1		indica se l'oggetto rappresenta un oggetto virtuale (OS_OBJECTID_FLAGS_VIRTUAL)
												bit2
												bit3
												bit4
												bit5
												bit6
												bit7
												bit8
							bytes[2] = 0 riservato (deve essere 0)
							bytes[3] = riservato per l'ID
*/

#ifndef OS_NOOBJECTID

//////////////////////////////////////////////////////////////////////

#include "id.h"
#include "ids.h"
#include "objectsobjects.h"
#include "uniqueid.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

// Clodo, 05/11/2011, Razor
// TOCLEAN:
// Qui ho copiato la classe ObjectID in una EntityID.
// Uso la EntityID ove mi aspetto che l'ID sia di un'entità, la ObjectID di un oggetto generico
// L'unica "pecca" è le statistiche: "reference" può essere un utente o un entità, per cui uso sempre ObjectID con le varie ".toObjectID()" in giro.
// Uso due classi identiche ma non derivate tra loro, perchè voglio errori di compilazione se cerco di passare un EntityID ad un parametro ObjectID o viceversa.
// Forse dovrei creare una classe base ad entrambe?



#ifdef OS_ENTITYID
	class ObjectID;
	class EntityID;
#else
	//#define EntityID UniqueID
#endif

class EngineExport ObjectID : public ID
{
	typedef ID BaseID;

// Construction
public:
	ObjectID();
	ObjectID(const std::string &id);	
	ObjectID(const ObjectID &second);
	virtual ~ObjectID();

// Attributes
public:
	// Restituisce la tipologia di oggetto prevista per l'ID
	PortalObjectType getObjectType() const; // TOCLEAN_OBJECTID_OBJECTTYPE
	// Restituisce il nome della tipologia dell'oggetto
	//String getObjectTypeName() const; // TOCLEAN_OBJECTID_OBJECTTYPE

	// Indica se l'oggetto rappresenta una primaria (solo per gli oggetti revisionabili)
	// bool isPrimary() const; // TOCLEAN_OBJECTID_PRIMARY_FLAGS
	// Indica se l'oggetto rappresenta una revisione (solo per gli oggetti revisionabili)
	// bool isRevision() const; // TOCLEAN_OBJECTID_PRIMARY_FLAGS

	//static const ObjectID & getRootID();

// Operators
public:
	ObjectID & operator=(const std::string &id);

	ObjectID toObjectID() const;
	EntityID toEntityID() const;

// Operations
public:
	// Valida l'ID di un oggetto non revisionabile
	bool validateObject(bool allow_null) const;
	// Valida l'ID di un oggetto revisionabile
	bool validateRevisionable(bool allow_null, bool require_primary) const;

	// Genera un ID di un oggetto non revisionabile
	static ObjectID generate(PortalObjectType type, const std::string &hash);
	// Genera un ID di un oggetto revisionabile
	//static ObjectID generate(PortalObjectType type, const std::string &hash, bool isPrimary);

// ID overrides
protected:
	virtual IDType getValidationType() const;

private:
	bool _validate(bool allow_null, bool require_primary) const;

public:
	static const ObjectID EMPTY;
};

//////////////////////////////////////////////////////////////////////

#ifdef OS_ENTITYID
class EngineExport EntityID : public ID
{
	typedef ID BaseID;

// Construction
public:
	EntityID();
	EntityID(const std::string &id);	
	EntityID(const EntityID &second);
	virtual ~EntityID();

// Attributes
public:
	// Restituisce la tipologia di oggetto prevista per l'ID
	PortalObjectType getObjectType() const; // TOCLEAN_OBJECTID_OBJECTTYPE
	// Restituisce il nome della tipologia dell'oggetto
	String getObjectTypeName() const; // TOCLEAN_OBJECTID_OBJECTTYPE

	// Indica se l'oggetto rappresenta una primaria (solo per gli oggetti revisionabili)
	// bool isPrimary() const; // TOCLEAN_OBJECTID_PRIMARY_FLAGS
	// Indica se l'oggetto rappresenta una revisione (solo per gli oggetti revisionabili)
	// bool isRevision() const; // TOCLEAN_OBJECTID_PRIMARY_FLAGS

	//static const EntityID & getRootID();

// Operators
public:
	EntityID & operator=(const std::string &id);

	ObjectID toObjectID() const;
	EntityID toEntityID() const;

// Operations
public:
	// Valida l'ID di un oggetto non revisionabile
	bool validateObject(bool allow_null) const;
	// Valida l'ID di un oggetto revisionabile
	bool validateRevisionable(bool allow_null, bool require_primary) const;

	// Genera un ID di un oggetto non revisionabile
	static EntityID generate(PortalObjectType type, const std::string &hash);
	// Genera un ID di un oggetto revisionabile
	static EntityID generate(PortalObjectType type, const std::string &hash, bool isPrimary);

// ID overrides
protected:
	virtual IDType getValidationType() const;

private:
	bool _validate(bool allow_null, bool require_primary) const;

public:
	static const EntityID EMPTY;
};
#endif

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // OS_NOOBJECTID

#endif // _OBJECTS_OBJECTID_H
