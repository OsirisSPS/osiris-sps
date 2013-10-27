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

#ifndef _OS_IDE_ASSISTANT_H
#define _OS_IDE_ASSISTANT_H

#include "iidexslrenderer.h"
#include "ideportalcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class HtmlTextArea;
class XMLPortalExporter;

//////////////////////////////////////////////////////////////////////

class EngineExport IdeAssistant : public IXSLRenderer<IPortalPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeAssistant();
	virtual ~IdeAssistant();

// Operations
private:
	void _exportTarget();
	void _exportSelectedObjects();

// IControl overrides
protected:
	virtual void onLoad();
	virtual void onPreRender();

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// Events:
private:
	void onViewMode(IEvent *e);
	void onObjectAdd(IEvent *e);
	void onObjectRemove(IEvent *e);
	void onObjectsClear(IEvent *e);
	void onObjectView(IEvent *e);
	void onObjectEdit(IEvent *e);

private:
	static const String EVENT_ONVIEWMODE;
	static const String EVENT_ONOBJECTADD;
	static const String EVENT_ONOBJECTREMOVE;
	static const String EVENT_ONOBJECTSCLEAR;
	static const String EVENT_ONOBJECTVIEW;
	static const String EVENT_ONOBJECTEDIT;

protected:
	shared_ptr<XMLPortalExporter> m_exporter;
	shared_ptr<HtmlTextArea> m_notes;
};

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_ASSISTANT_H
