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

#ifndef _ENGINE_OBJECTSSIGNER_H
#define _ENGINE_OBJECTSSIGNER_H

#include "base/object.h"
#include "buffer.h"
#include "dbvalue.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport ObjectsSigner : public Object
{
// Construction
public:
	ObjectsSigner();
	template <typename T>
	ObjectsSigner(const T &value);
	~ObjectsSigner();

// Operation
public:
	// Aggiunge un valore al blocco
	template <typename T>
	void add(const T &value);

	template <typename T>
	void add(const DbValue<T> &value);

	// Usa la chiave privata per generare la firma sul blocco corrente
	bool sign(const Buffer &private_key, Buffer &signature);
	// Indica se la firma  valida per il blocco
	bool validate(const Buffer &public_key, const Buffer &signature) const;

private:
	void add(const void *data, uint32 size);

private:
	Buffer m_block;
	std::string m_debug; // CLODOURGENT

protected:
	static const std::string FIELD_DELIMITER;
};

//////////////////////////////////////////////////////////////////////

template <typename T>
ObjectsSigner::ObjectsSigner(const T &value)
{
	add(value);
}

template <typename T>
void ObjectsSigner::add(const DbValue<T> &value)
{
	// Aggiunge il valore solo nel caso non sia nullptr (facilita l'aggiunta di nuovi campi in futuro se di default a nullptr)
	if(value.isNull() == false)
		add(value.get());
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _ENGINE_OBJECTSSIGNER_H
