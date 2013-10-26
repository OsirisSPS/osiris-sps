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

#ifndef _OS_IDE_BLOCK_H
#define _OS_IDE_BLOCK_H

#include "idecontrol.h"
#include "iidexslrenderer.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

class EngineExport IdeBlock : public IXSLRenderer<IPageControl<IHtmlControl> >
{
	typedef IXSLRenderer<IPageControl<IHtmlControl> > ControlBase;

// Construction
public:
	IdeBlock(const String &title = String::EMPTY);
	virtual ~IdeBlock();

// Attributes
public:
	inline const String & getTitle() const;
	inline void setTitle(const String &title);

	inline const String & getHref() const;
	inline void setHref(const String &href);

	inline const BlockType & getType() const;
	inline void setType(const BlockType &type);
	
	inline const String & getInnerStyle() const;
	inline void setInnerStyle(const String &style);

	inline const String & getOuterStyle() const;
	inline void setOuterStyle(const String &style);

	inline bool isLink() const;

	inline shared_ptr<IHtmlControl> getBody() const;

	// Hack, temporary: we use this for a block-page.
	inline const bool & getSpecialPage() const;
	inline void setSpecialPage(const bool &page);

// IXSLRenderer interface
public:
	virtual String getTemplatePath();

// IControl overrides
protected:
	virtual void onInit();

protected:
	String m_title;
	String m_href;
	BlockType m_type;
	String m_innerStyle;
	String m_outerStyle;
	shared_ptr<IHtmlControl> m_body;
	bool m_page;
};

//////////////////////////////////////////////////////////////////////

inline const String & IdeBlock::getTitle() const { return m_title; }
inline void IdeBlock::setTitle(const String &title) { m_title = title; }

inline const String & IdeBlock::getHref() const { return m_href; }
inline void IdeBlock::setHref(const String &href) { m_href = href; }

inline const BlockType & IdeBlock::getType() const { return m_type; }
inline void IdeBlock::setType(const BlockType &type) { m_type = type; }

inline const String & IdeBlock::getInnerStyle() const { return m_innerStyle; }
inline void IdeBlock::setInnerStyle(const String &style) { m_innerStyle = style; }

inline const String & IdeBlock::getOuterStyle() const { return m_outerStyle; }
inline void IdeBlock::setOuterStyle(const String &style) { m_outerStyle = style; }

inline bool IdeBlock::isLink() const { return m_href.empty() == false; }

inline shared_ptr<IHtmlControl> IdeBlock::getBody() const { return m_body; }

inline const bool & IdeBlock::getSpecialPage() const { return m_page; }
inline void IdeBlock::setSpecialPage(const bool &page) { m_page = page; }

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////

#endif // _OS_IDE_BLOCK_H
