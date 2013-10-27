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
#include "modulesxslttransform.h"

#include "modulesxslttransformeditor.h"
#include "modulesxslttransformviewer.h"
#include "modulesmodulesconfig.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace components {
namespace modules {

//////////////////////////////////////////////////////////////////////

XsltTransform::XsltTransform()
{

}

XsltTransform::~XsltTransform()
{

}

ExtensionsModuleID XsltTransform::getID()
{
	return ExtensionsModuleID(OS_MODULES_XSLTTRANSFORM_ID.to_ascii());
}

String XsltTransform::getName()
{
	return _S("XSLT Transform module");
}

shared_ptr<ExtensionsModuleEditor> XsltTransform::createEditor()
{
	return shared_ptr<ExtensionsModuleEditor>(OS_NEW XsltTransformEditor());
}

shared_ptr<ExtensionsModuleViewer> XsltTransform::createViewer()
{
	return shared_ptr<ExtensionsModuleViewer>(OS_NEW XsltTransformViewer());
}

//////////////////////////////////////////////////////////////////////

} // modules
} // components
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
