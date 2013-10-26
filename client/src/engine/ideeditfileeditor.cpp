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
#include "ideeditfileeditor.h"

#include "algorithms.h"
#include "css.h"
#include "iportaldatabase.h"
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
#include "idesession.h"
#include "ideskin.h"
#include "objectsfile.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide {
namespace edit {

//////////////////////////////////////////////////////////////////////

FileEditor::FileEditor(shared_ptr<EntitiesEntity> entity, shared_ptr<EntitiesEntity> parent) :	EditorBase(portalObjectTypeFile, entity, parent),
																					m_title(OS_NEW HtmlTextBox()),
																					m_description(OS_NEW HtmlTextBox()),
																					m_enableBrowser(OS_NEW HtmlCheckBox()),
																					m_browser(OS_NEW HtmlFileBrowser())
{
	m_title->setID(_S("title"));
	m_title->setCss(_S("os_input_full"));
	m_title->setMaxLength(OS_CONTROLS_TITLE_MAXLENGTH);
	m_description->setID(_S("description"));
	m_description->setCss(_S("os_input_full"));
	m_description->setMaxLength(OS_CONTROLS_DESCRIPTION_MAXLENGTH);
	m_browser->setID(_S("browser"));
	m_browser->setCss(_S("os_input_full"));

	getTemplate()->addChildParam(m_title, _S("title"));
	getTemplate()->addChildParam(m_description, _S("description"));
	if(isRevision())
	{
		m_enableBrowser->setID(_S("enableBrowser"));
		m_enableBrowser->setCheck(false);
		m_enableBrowser->setAutoPostBack(true);

		getTemplate()->addChildParam(m_enableBrowser, _S("enable_browser"));
	}

	getTemplate()->addChildParam(m_browser, _S("browser"));
}

FileEditor::~FileEditor()
{

}

String FileEditor::getTemplatePath()
{
	return getSkin()->getTemplatePath(_S("file_editor.xsl"));
}

shared_ptr<ObjectsIRevisionable> FileEditor::createObject()
{
	String file_name;
	String file_type;
	const Buffer *file_data = null;

	if(isRevision() && m_enableBrowser->getCheck() == false)
	{
		shared_ptr<ObjectsFile> currentFile = objects_file_cast(getCurrent());
		if(currentFile != null)
		{
			file_name = currentFile->file_name;
			file_type = currentFile->file_type;
			file_data = currentFile->file_data.get_ptr();
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

	shared_ptr<ObjectsFile> file(OS_NEW ObjectsFile());
	file->title = algorithms::trim_copy(m_title->getValue());
	file->description = algorithms::trim_copy(m_description->getValue());
	file->file_name = algorithms::trim_copy(file_name);
	file->file_data = *file_data;
	file->file_type = file_type;
	file->file_size = file_data->getSize();

	return file;
}

void FileEditor::onPreRender()
{
	EditorBase::onPreRender();

	if(isRevision())
	{
		m_browser->setVisible(m_enableBrowser->getCheck());

		if(getPostBack() == false)
		{
			shared_ptr<ObjectsFile> current = objects_file_cast(getCurrent());
			if(current != null)
			{
				m_title->setValue(current->title);
     			m_description->setValue(current->description);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

} // edit
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
