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

#include "stdafx.h"
#include "ideskinsimple.h"

#include "filesystem.h"
#include "idesystem.h"
#include "ihttpserver.h"
#include "osiriscommon.h"
#include "css.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IdeSkinSimple::IdeSkinSimple() : IdeSkin()
{
}

/*
IdeSkinSimple::IdeSkinSimple(shared_ptr<IExtensionsExtension> e) : IdeSkin()
{
	init(e, OS_SKINXML, String::EMPTY);
}

IdeSkinSimple::IdeSkinSimple(shared_ptr<IExtensionsExtension> e, const String& configName, const String& htdocsPath) : IdeSkin()
{	
	init(e, configName, htdocsPath);
}
*/

IdeSkinSimple::~IdeSkinSimple()
{
	//IdeSystem::instance()->unregisterSkin(get_this_ptr());
}

bool IdeSkinSimple::init(shared_ptr<IExtensionsExtension> e, const String& configName, const String& htdocsPath)
{	
	m_extension = e;
	m_configName = configName;
	m_htdocsPath = htdocsPath;

	return loadXml(utils::makeFilePath(e->getPath(), configName));
	/*
	if(loadXml(utils::makeFilePath(e->getPath(), configName)))
		IdeSystem::instance()->registerSkin(get_this_ptr());
	*/
}

String IdeSkinSimple::findResource(String resource, bool httpPath) const
{
	String htdocsExtensionPath = utils::makeFolderPath(m_extension->getPath(), _S("htdocs"));	

	String htdocsSkinPath = utils::makeFolderPath(htdocsExtensionPath, m_htdocsPath);	

	String pathExtension = utils::makeFilePath(htdocsExtensionPath, resource);
	String pathSkin = utils::makeFilePath(htdocsSkinPath, resource);

	// First, we search at Skin level
	if( (m_htdocsPath.empty() == false) && (FileSystem::instance()->exists(pathSkin)) )
	{
		if(httpPath)
			return _S("/") + m_extension->getID().toUTF16() + _S("/") + m_htdocsPath + _S("/") + resource;
		else
			return pathSkin;
	}
	else if(FileSystem::instance()->exists(pathExtension))
	{
		if(httpPath)
			return _S("/") + m_extension->getID().toUTF16() + _S("/") + resource;
		else
			return pathExtension;
	}
	else
		return IdeSkin::findResource(resource, httpPath);
}


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
