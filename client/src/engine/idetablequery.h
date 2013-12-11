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

#ifndef _OS_IDE_TABLEQUERY_H
#define _OS_IDE_TABLEQUERY_H

#include "ideportalcontrol.h"
#include "iidexslrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IdeTableQuery : public IXSLRenderer<IPortalPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Enumerated types
public:
	enum ColumnType
	{
		ctString		= 0,
		ctEntityID		= 1,
		ctObjectID		= 2,
		ctUserID		= 3,
		ctScore			= 4,
		ctShortDateTime	= 5,
		ctLongDateTime	= 6,
	};

	typedef map<uint32, String>::type ColumnsTitles;
	typedef map<uint32, ColumnType>::type ColumnsTypes;

// Construction
public:
	IdeTableQuery();
	virtual ~IdeTableQuery();

// Attributes:
public:
	inline const String & getSql() const;
	inline void setSql(const String &sql);

	String getColumnTitle(const uint32 &index) const;
	void setColumnTitle(const uint32 &index,const String &title);	

	ColumnType getColumnType(const uint32 &index) const;
	void setColumnType(const uint32 &index,const ColumnType &type);	

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IControl overrides		
protected:
	virtual void onPreRender();	
	
protected:
	String m_sql;
	ColumnsTitles m_columnsTitles;
	ColumnsTypes m_columnsTypes;
};

//////////////////////////////////////////////////////////////////////

inline const String & IdeTableQuery::getSql() const { return m_sql; }
inline void IdeTableQuery::setSql(const String &sql) { m_sql = sql; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_TABLEQUERY_H
