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

#ifndef _IDE_XSLRESOLVER_H
#define _IDE_XSLRESOLVER_H

#include "ixmlresolver.h"
#include "ids.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IdeSkin;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeXSLResolver : public IXMLResolver
{
// Construction
public:
	IdeXSLResolver(shared_ptr<IdeSkin> skin);
	virtual ~IdeXSLResolver();

// Operations
private:
	//bool resolveSkinPath(shared_ptr<IdeSkin> skin, const String &path, String &resolved);
	//bool resolveSkinsPath(const UniqueID &id, const String &path, String &resolved);
	bool resolveHtdocsPath(const String &path, String &resolved);
	bool resolveExtensionsPath(const ExtensionID &id, const String &path, String &resolved);
	bool resolvePluginsPath(const UniqueID &id, const String &path, String &resolved);

// IResolver interface
public:
	virtual bool resolve(const String &uri, String &resolved);

private:
	//static const String SKIN_PATH;
	//static const String SKINS_PATH;
	static const String HTDOCS_PATH;
	static const String EXTENSIONS_PATH;
	static const String PLUGINS_PATH;

private:
	shared_ptr<IdeSkin> m_skin;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _IDE_XSLRESOLVER_H
