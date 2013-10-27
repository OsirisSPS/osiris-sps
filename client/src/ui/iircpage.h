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

#ifndef _OS_UI_IIRCPAGE_H
#define _OS_UI_IIRCPAGE_H

#include "ircsession.h"
#include "multilanguagewindow.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class IIRCCommand;
class IRCWindow;

//////////////////////////////////////////////////////////////////////

class IIRCPage : public wxPanel,
				 public MultilanguageWindow
{
	typedef wxPanel WindowBase;

// Construction
public:
    IIRCPage(IRCWindow *ircWindow, wxWindow *parent);
	virtual ~IIRCPage();

// Attributes
public:
	inline IRCWindow * getIRCWindow() const;

	shared_ptr<IRCSession> getSession() const;

// Interface
public:
	virtual bool processCommand(const String &command) = 0;
		
// Overridables
public:
	virtual void handlePageOpened();
	virtual void handlePageClosed();

	virtual void updatePage();

protected:
	IRCWindow *m_ircWindow;	
};

//////////////////////////////////////////////////////////////////////

inline IRCWindow * IIRCPage::getIRCWindow() const { return m_ircWindow; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_UI_IIRCPAGE_H
