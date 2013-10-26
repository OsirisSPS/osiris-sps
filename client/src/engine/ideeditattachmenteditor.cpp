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
#include "ideeditattachmenteditor.h"

#include "algorithms.h"
#include "iportaldatabase.h"
#include "css.h"
#include "idesession.h"
#include "ideskin.h"
#include "objectsattachment.h"
#include "htmlcheckbox.h"
#include "htmlcontrols.h"
#include "htmlevent.h"
#include "htmlfilebrowser.h"
#include "htmlliteral.h"
#include "htmltable.h"
#include "htmltablecell.h"
#include "htmltablerow.h"
#include "htmltext.h"
#include "htmltextbox.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

AttachmentEditor::AttachmentEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) : EditorBase(portalObjectTypeAttachment, entity, parent),
																							   m_enableBrowser(OS_NEW HtmlCheckBox()),
																							   m_browser(OS_NEW HtmlFileBrowser())
{
	m_browser->setID(_S("browser"));
	m_browser->setCss(_S("os_input_full"));

	if(isRevision())
	{
		m_enableBrowser->setID(_S("enableBrowser"));
		m_enableBrowser->setCheck(false);
		m_enableBrowser->setAutoPostBack(true);

		getTemplate()->addChildParam(m_enableBrowser, _S("enable_browser"));
	}

	getTemplate()->addChildParam(m_browser, _S("browser"));
}

AttachmentEditor::~AttachmentEditor()
{

}

String AttachmentEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("attachment_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> AttachmentEditor::createObject()
{
	String file_name;
	String file_type;
	const Buffer *file_data = null;

	if(isRevision() && m_enableBrowser->getCheck() == false)
	{
		shared_ptr<ObjectsAttachment> currentAttachment = objects_attachment_cast(getCurrent());
		if(currentAttachment != null)
		{
			file_name = currentAttachment->file_name;
			file_type = currentAttachment->file_type;
			file_data = currentAttachment->file_data.get_ptr();
		}
	}
	else
	{
		file_name = m_browser->getFileName();
		file_type = m_browser->getFileType();
		file_data = m_browser->getFileBuffer();
	}

	if(file_data == null)
		return null;

	shared_ptr<ObjectsAttachment> attachment(OS_NEW ObjectsAttachment());
	attachment->file_name = algorithms::trim_copy(file_name);
	attachment->file_data = *file_data;
	attachment->file_type = file_type;
	attachment->file_size = file_data->getSize();

	return attachment;
}

void AttachmentEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision())
	{
		m_browser->setVisible(m_enableBrowser->getCheck());
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
