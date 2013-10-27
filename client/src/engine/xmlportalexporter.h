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

#ifndef _OS_ENGINE_XMLPORTALEXPORTER_H
#define _OS_ENGINE_XMLPORTALEXPORTER_H

#include "xmlexporter.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IPortalDatabase;
class IPortalPage;
class IdeSession;
class Portal;

//////////////////////////////////////////////////////////////////////

class EngineExport XMLPortalExporter : public XMLExporter
{
	typedef XMLExporter ExporterBase;

// Enumerated types
public:
	enum ExportMode
	{
		emLite		= 0,
		emFull		= 1,
		emData		= 2,
	};

// Construction
public:
	XMLPortalExporter(shared_ptr<XMLNode> root, shared_ptr<IPortalPage> page, ExportMode mode = emFull, bool withStats = true);
	XMLPortalExporter(shared_ptr<XMLNode> root, const XMLPortalExporter &second);
	virtual ~XMLPortalExporter();

// Attributes
public:
	inline shared_ptr<IPortalPage> getPage() const;

	inline ExportMode getMode() const;
	inline void setMode(ExportMode ExportMode);

	inline bool getWithStats() const;
	inline void setWithStats(bool withStats);

	// Restituisce il portale di riferimento
	shared_ptr<Portal> getPortal() const;
	// Restituisce il database di riferimento
	shared_ptr<IPortalDatabase> getDatabase() const;
	// Restituisce il logged user di riferimento
	shared_ptr<IdeSession> getSessionAccount();

private:
	weak_ptr<IPortalPage> m_page;
	ExportMode m_mode;
	bool m_withStats;
};

//////////////////////////////////////////////////////////////////////

inline shared_ptr<IPortalPage> XMLPortalExporter::getPage() const { return m_page.lock(); }

inline XMLPortalExporter::ExportMode XMLPortalExporter::getMode() const { return m_mode; }
inline void XMLPortalExporter::setMode(XMLPortalExporter::ExportMode mode) { m_mode = mode; }

inline bool XMLPortalExporter::getWithStats() const { return m_withStats; }
inline void XMLPortalExporter::setWithStats(bool withStats) { m_withStats = withStats; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_ENGINE_XMLPORTALEXPORTER_H
