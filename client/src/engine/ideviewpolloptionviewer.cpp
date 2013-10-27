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
#include "ideviewpolloptionviewer.h"

#include "entitiesentity.h"
#include "httpsession.h"
#include "ideskin.h"
#include "objectspolloption.h"
#include "portalsportal.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace view {

//////////////////////////////////////////////////////////////////////

PollOptionViewer::PollOptionViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{

}

PollOptionViewer::~PollOptionViewer()
{

}

String PollOptionViewer::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("poll_option_viewer.xsl"));
}

void PollOptionViewer::onInit()
{
	ControlBase::onInit();

	shared_ptr<ObjectsPollOption> poll_option = objects_poll_option_cast(getObject());
	if(poll_option != null)
	{
		getSession()->redirect(getPortal()->getViewObjectLink(poll_option->getParent()));
		return;
	}
}

void PollOptionViewer::onLoad()
{
	ControlBase::onLoad();

	// TODO: deve mostrare gli utenti che hanno votato l'opzione e un link al poll di riferimento (non effettuare il redirect in automatico)	
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
