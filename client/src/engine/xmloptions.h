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

#ifndef _OS_CORE_XMLOPTIONS_H
#define _OS_CORE_XMLOPTIONS_H

#include "base/object.h"
#include "boost/thread/shared_mutex.hpp"
#include "dbsqlvalues.h"
#include "dataitem.h"
#include "locked.h"

//////////////////////////////////////////////////////////////////////

#define OS_XMLOPTIONS_NODE_ROOT						_S("options")
#define OS_XMLOPTIONS_NODE_OPTION					_S("option")
#define OS_XMLOPTIONS_ATTRIBUTE_NAME				_S("name")
#define OS_XMLOPTIONS_ATTRIBUTE_VALUE				_S("value")

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class DataTableRow;
class IOptionValidator;
class Option;
class XMLDocument;
class XMLNode;

//////////////////////////////////////////////////////////////////////

class EngineExport XMLOptions : public Object
{
public:
	typedef map<String, shared_ptr<Option> >::type Options;		// mappa standard per tenere le opzioni in ordine alfabetico

// Construction
public:
	XMLOptions();
	virtual ~XMLOptions();

// Attributes
public:
	Locked<const Options, boost::shared_mutex>::shared getOptions() const;

	DataItem getOption(const String &name) const;
	bool getOption(const String &name, DataItem &value) const;
	bool setOption(const String &name, const DataItem &value);
	
    bool exists(const String &name) const;

// Operations
public:
	bool ensureOption(const String &name, const DataItem &value, bool publicOption, shared_ptr<IOptionValidator> validator = nullptr);
	bool ensureOption(const String &name, const DataItem &value, const DataItem &defaultValue, bool publicOption, shared_ptr<IOptionValidator> validator = nullptr);

	void parseString(const String &str);
	void parseXML(shared_ptr<XMLDocument> document);
	void parseXMLNode(shared_ptr<XMLNode> node);	

	String toString(bool allowNull = true) const;
	shared_ptr<XMLDocument> toXML(bool allowNull = true) const;
	shared_ptr<XMLNode> toXMLNode(const String &nodeName = OS_XMLOPTIONS_NODE_ROOT, bool allowNull = true) const;

	XMLOptions & toDefault();

	void read(const DataTableRow &row, const String &field);
	void write(DbSqlValues &values, const String &field) const;

	// Rimuove l'opzione specificata
	bool removeOption(const String &name);
	// Rimuove tutte le opzioni che iniziano con il pattern specificato
	uint32 removePattern(const String &name, bool icase = true);

private:
	bool doEnsureOption(const String &name, const DataItem &value, const DataItem &defaultValue, bool publicOption, shared_ptr<IOptionValidator> validator);

private:
	mutable boost::shared_mutex m_cs;
	Options m_options;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_CORE_XMLOPTIONS_H
