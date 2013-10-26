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

#ifndef _OS_IDE_OMLEDITOR_H
#define _OS_IDE_OMLEDITOR_H

#include "boost/function.hpp"
#include "idecontrol.h"
#include "iidexslrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlCheckBox;
class HtmlComboBox;
class HtmlTextArea;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeOMLEditor : public IXSLRenderer<IPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPageControl<IHtmlControl> > ControlBase;
	typedef boost::function<void (String &)> PreviewCallback;

// Construction
public:
	IdeOMLEditor();
	virtual ~IdeOMLEditor();

// Attributes
public:
	inline bool getExtended() const;
	inline void setExtended(bool extended);

	inline bool getRow() const;
	inline void setRow(bool row);

	inline const PreviewCallback & getPreviewCallback() const;
	inline void setPreviewCallback(const PreviewCallback &previewCallback);

	const String & getValue() const;
	void setValue(const String &value);

// IHtmlControl overrides
protected:
	virtual void onInit();
	virtual void onLoad();
	virtual void onPreRender();

	virtual void onLoadViewState(const DataTree &state);
	virtual void onSaveViewState(DataTree &state);

// Operation
private:
	String optimize(const String &value) const;

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// Events
private:
	void onPreview();
	void onHtml2Oml();
	void onHtmlAndBbcode2Oml();
	void onNormal();
	void onExtended();

private:
	static const String EVENT_PREVIEW;
	static const String EVENT_HTML2OML;
	static const String EVENT_HTMLANDBBCODE2OML;
	static const String EVENT_NORMAL;
	static const String EVENT_EXTENDED;

protected:
	bool m_extended;
	bool m_row;

	shared_ptr<HtmlTextArea> m_editor;
	shared_ptr<HtmlCheckBox> m_modeLite;
	shared_ptr<HtmlComboBox> m_modeMode;
	shared_ptr<HtmlCheckBox> m_modeSecure;
	shared_ptr<HtmlCheckBox> m_modeReport;
	PreviewCallback m_previewCallback;
};

//////////////////////////////////////////////////////////////////////

inline bool IdeOMLEditor::getExtended() const { return m_extended; }
inline void IdeOMLEditor::setExtended(bool extended) { m_extended = extended; }

inline bool IdeOMLEditor::getRow() const { return m_row; }
inline void IdeOMLEditor::setRow(bool row) { m_row = row; }

inline const IdeOMLEditor::PreviewCallback & IdeOMLEditor::getPreviewCallback() const { return m_previewCallback; }
inline void IdeOMLEditor::setPreviewCallback(const PreviewCallback &previewCallback) { m_previewCallback = previewCallback; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_OMLEDITOR_H
