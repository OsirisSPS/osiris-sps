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

#ifndef _OS_UI_EXTENSIONSSWIZARDPAGETEMPLATE_H
#define _OS_UI_EXTENSIONSSWIZARDPAGETEMPLATE_H

#include "base/object.h"
#include "multilanguagewindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class ExtensionsWizardPageTemplate : public wxWizardPageSimple,
									 public MultilanguageWindow
{
	typedef wxWizardPageSimple WizardPageBase;

	DECLARE_EVENT_TABLE()	

public:
	class TemplateDetails : public Object
	{
	public:
		struct FileDetails
		{
			String target;
			String source;
			bool script;
			bool macro;
		};

		typedef list<FileDetails>::type Files;

	// Construction
	public:
		TemplateDetails(const String &folder);
		virtual ~TemplateDetails();

	// Attributes
	public:
		inline const String & getFolder() const;

		inline const String & getName() const;
		inline void setName(const String &name);

		inline const String & getDescription() const;
		inline void setDescription(const String &description);

		inline const String & getAuthor() const;
		inline void setAuthor(const String &author);

		inline const Files & getFiles() const;

	// Operations
	public:
		bool addFile(const String &path, const String &source, bool script, bool macro);

	private:
		String m_folder;
		String m_name;
		String m_description;
		String m_author;
		Files m_files;
	};

private:
	typedef map<int32, shared_ptr<TemplateDetails> >::type TemplatesMap;

// Construction
public:
	ExtensionsWizardPageTemplate(wxWizard *parent);
	virtual ~ExtensionsWizardPageTemplate();

// Attributes
public:
	shared_ptr<TemplateDetails> getTemplate() const;

// Operations
private:
	void createLayout();
	void initWindow();
	void cleanupWindow();

	void loadTemplates();
	shared_ptr<TemplateDetails> loadTemplate(const String &folder, const String &filename);
	int32 addTemplate(const String &name, shared_ptr<TemplateDetails> details);
	void updateCurrentTemplate();

// Events
private:
	void onTemplateSelChanged(wxCommandEvent &e);

private:
	wxStaticBitmap *m_pageIcon;
	wxStaticText *m_pageCaptionTitle;
	wxStaticText *m_pageCaptionDescription;
	wxStaticBoxSizer *m_templateBoxSizer;
	wxStaticText *m_templateTypeLabel;
	wxChoice *m_templateType;
	wxStaticText *m_templateAuthorLabel;
	wxStaticText *m_templateAuthor;
	wxStaticText *m_templateDescriptionLabel;
	wxStaticText *m_templateDescription;
	TemplatesMap m_templatesMap;
};

//////////////////////////////////////////////////////////////////////

inline const String & ExtensionsWizardPageTemplate::TemplateDetails::getFolder() const { return m_folder; }

inline const String & ExtensionsWizardPageTemplate::TemplateDetails::getName() const { return m_name; }
inline void ExtensionsWizardPageTemplate::TemplateDetails::setName(const String &name) { m_name = name; }

inline const String & ExtensionsWizardPageTemplate::TemplateDetails::getDescription() const { return m_description; }
inline void ExtensionsWizardPageTemplate::TemplateDetails::setDescription(const String &description) { m_description = description; }

inline const String & ExtensionsWizardPageTemplate::TemplateDetails::getAuthor() const { return m_author; }
inline void ExtensionsWizardPageTemplate::TemplateDetails::setAuthor(const String &author) { m_author = author; }

inline const ExtensionsWizardPageTemplate::TemplateDetails::Files & ExtensionsWizardPageTemplate::TemplateDetails::getFiles() const { return m_files; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_EXTENSIONSSWIZARDPAGETEMPLATE_H
