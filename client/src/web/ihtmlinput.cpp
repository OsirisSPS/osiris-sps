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
#include "ihtmlinput.h"

#include "htmlparser.h"
#include "htmlwriter.h"
#include "httppostparam.h"
#include "httprequest.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

IHtmlInput::IHtmlInput()
{

}

IHtmlInput::~IHtmlInput()
{

}

void IHtmlInput::renderAttributes(HtmlWriter &writer)
{
	ControlBase::renderAttributes(writer);

	if(hasID())
	{
		writer.writeAttribute(_S("id"), getTarget());
		writer.writeAttribute(_S("name"), getTarget());
	}

	if(m_value.empty() == false)
		writer.writeAttribute(_S("value"), m_value);
}

void IHtmlInput::onInit()
{
	ControlBase::onInit();

	if(getPostBack() && hasID())
	{
		// Ottiene il parametro associato al controllo
		shared_ptr<HttpPostParam> param = getRequest()->getPostParam(getTarget().to_wide());
		// Verifica che sia stato postato un parametro relativo al controllo
		if(param != null)
			// Inizializza il valore del controllo
            m_value = param->getString();
		/*
		else // 0.13 RC2. Se un checkbox è a false, il browser non manda il parametro.
			m_value.clear();			
		*/
	}
}

void IHtmlInput::onRender(HtmlWriter &writer)
{
	writer.openTag(_S("input"));
	renderAttributes(writer);
	writer.selfCloseTag();
	writer.writeLine();

	if(m_caption.empty() == false)
	{
		if(hasID())
		{
			writer.openTag(_S("label"));
			writer.writeAttribute(_S("for"), getTarget());
			writer.closeTag();
			writer.add(m_caption);
			writer.closeTag(_S("label"), false);
		}
		else
		{
			writer.write(m_caption);
		}

		writer.writeLine();
	}

	if(hasChilds())
	{
		renderChilds(writer);
		writer.writeLine();
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
