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

#ifndef _OS_IDE_MAILBOX_H
#define _OS_IDE_MAILBOX_H

#include "datadata.h"
#include "iidepageable.h"
#include "iidexslrenderer.h"
#include "ideportalcontrol.h"
#include "ihtmlcontrol.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IdeMailBox : public IXSLRenderer<IPortalPageControl<IHtmlControl> >,
								public IIdePageable
{
	typedef IXSLRenderer<IPortalPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeMailBox(MessageFolder folder = messageFolderInbox);
	IdeMailBox(const ObjectID &message);
	virtual ~IdeMailBox();

private:
	void _construct(MessageFolder folder, const ObjectID &message);

// Attributes
public:
	inline MessageFolder getFolder() const;
	inline void setFolder(MessageFolder folder);

	inline const ObjectID & getMessage() const;
	inline void setMessage(const ObjectID &id);

	inline uint32 getOffset() const;
	inline void setOffset(uint32 offset);

	inline bool getSecure() const;
	inline void setSecure(bool secure);

// Operations
protected:
	virtual void renderFolder(MessageFolder folder, shared_ptr<XMLNode> node);
	virtual void renderMessage(const ObjectID &id, shared_ptr<XMLNode> node);

private:
	void _renderFolder(MessageFolder folder);

// Events
private:
	void onDeleteMessage(IEvent *e);
	void onRestoreMessage(IEvent *e);
	void onReadAllMessages(IEvent *e);
	void onDeleteAllMessages(IEvent *e);
	void onChangeOffset(IEvent *e);

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IControl overrides
public:
	virtual void onPreRender();

// Pager interface
public:
	virtual String getPageHref(uint32 offset) const;

protected:
	MessageFolder m_folder;
	ObjectID m_message;
	uint32 m_offset;
	bool m_secure;

private:
	static const String PARAM_SECURE;
	static const String EVENT_ONDELETEMESSAGE;
	static const String EVENT_ONRESTOREMESSAGE;
	static const String EVENT_ONREADALLMESSAGES;
	static const String EVENT_ONDELETEALLMESSAGES;
	static const String EVENT_ONCHANGEOFFSET;
};

//////////////////////////////////////////////////////////////////////

inline MessageFolder IdeMailBox::getFolder() const { return m_folder; }
inline void IdeMailBox::setFolder(MessageFolder folder) { m_folder = folder; }

inline const ObjectID & IdeMailBox::getMessage() const { return m_message; }
inline void IdeMailBox::setMessage(const ObjectID &message) { m_message = message; }

inline uint32 IdeMailBox::getOffset() const { return m_offset; }
inline void IdeMailBox::setOffset(uint32 offset) { m_offset = offset; }

inline bool IdeMailBox::getSecure() const { return m_secure; }
inline void IdeMailBox::setSecure(bool secure) { m_secure = secure; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_MAILBOX_H
