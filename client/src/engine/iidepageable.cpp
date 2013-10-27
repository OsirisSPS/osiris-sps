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
#include "iidepageable.h"

#include "ideskin.h"
#include "utils.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

const int32 IIdePageable::OFFSET_LAST = -1;

//////////////////////////////////////////////////////////////////////

IIdePageable::IIdePageable()
{

}

IIdePageable::~IIdePageable()
{

}

bool IIdePageable::adjustOffset(const uint32 &total_items, const uint32 &items_to_show, uint32 &offset)
{
	// Controlla se  stato specificato un offset oltre il totale
	if(offset >= total_items)
	{
		// Calcola lo scarto tra il totale e il numero di figli da visualizzare (lo scarto determina il numero
		// di figli da visualizzare nell'ultima pagina)
		uint32 mod = total_items % items_to_show;

		uint32 new_offset = 0;
		// Verifica che ci siano almene "items_to_show" da visualizzare
		if(total_items > items_to_show)
			new_offset = total_items - (mod == 0 ? items_to_show : mod);

		if(new_offset != offset)
		{
			offset = new_offset;
			return true;
		}
	}

	//	NOTA: non bisogna effettuare la correzione ad un intervallo preciso per consentire il link ad un item specifico
	return false;
}

void IIdePageable::createPager(shared_ptr<IdeSkin> skin, shared_ptr<XMLNode> node, uint32 total_items, uint32 items_to_show, uint32 offset)
{
	OS_ASSERT(skin != null);

	if(items_to_show == 0)
		return;

	// Calcola il numero totale di pagine
	uint32 total_pages = total_items / items_to_show;
	if((total_items % items_to_show) != 0)
		total_pages++;

	if(total_pages <= 1)
		return;

	// Assicura che l'offset sia corretto
	adjustOffset(total_items, items_to_show, offset);

	OS_ASSERT(offset % items_to_show == 0);
	// Calcola la pagina corrente
	uint32 current_page = offset % items_to_show == 0 ? offset / items_to_show : 0;

	uint32 pages_to_show = utils::limit<uint32>(skin->getPagerPages(), 1, total_pages);
	uint32 start_index = 0;
	if(current_page > 0)
	{
		int32 count = std::min<int32>(pages_to_show / 2, total_pages - current_page - 1);
		int32 left = std::max<int32>(current_page - (pages_to_show - count - 1), 0);
		start_index = static_cast<uint32>(left);
	}

	uint32 end_index = 0;
	if(pages_to_show > 0)
		end_index = std::min(start_index + pages_to_show, total_pages) - 1;

	shared_ptr<XMLNode> nodePager = node->addChild(_S("pager"));
	nodePager->setAttributeUint32(_S("pages"), total_pages);
	nodePager->setAttributeUint32(_S("current_page"), current_page);
	nodePager->setAttributeUint32(_S("items_to_show"), items_to_show);
	nodePager->setAttributeUint32(_S("items"), total_items);
	nodePager->setAttributeUint32(_S("items_from"), items_to_show * current_page + 1);
	nodePager->setAttributeUint32(_S("items_to"), (current_page == (total_pages - 1)) ? total_items : (items_to_show * (current_page + 1)));

	// TODO, temp, skifezza, ci ho perso tantissimo tempo a trovare una soluzione...
	String gotoHref = getPageHref(999999);
	gotoHref = gotoHref.replace_all(_S("\'"),_S("#"));	// Questo perch altrimenti non posso passarlo come parametro a una funzione js
	nodePager->setAttributeString(_S("goto_href"), gotoHref);

	if(start_index != 0)
	{
		shared_ptr<XMLNode> nodeFirstPage = nodePager->addChild(_S("page"));
		nodeFirstPage->setAttributeUint32(_S("index"), static_cast<uint32>(1));
		nodeFirstPage->setAttributeString(_S("type"), _S("first"));
		nodeFirstPage->setAttributeString(_S("href"), getPageHref(0));
	}

	if(current_page > 0)
	{
		uint32 prev_index = current_page - 1;
		shared_ptr<XMLNode> nodeFirstPrev = nodePager->addChild(_S("page"));
		nodeFirstPrev->setAttributeUint32(_S("index"), prev_index + 1);
		nodeFirstPrev->setAttributeString(_S("type"), _S("prev"));
		nodeFirstPrev->setAttributeString(_S("href"), getPageHref(items_to_show * prev_index));
	}

	for(uint32 page_index = start_index; page_index <= end_index; page_index++)
	{
		shared_ptr<XMLNode> nodePage = nodePager->addChild(_S("page"));
		nodePage->setAttributeUint32(_S("index"), page_index + 1);
		if(page_index == current_page)
			nodePage->setAttributeString(_S("type"), _S("current"));
		nodePage->setAttributeString(_S("href"), getPageHref(items_to_show * page_index));
	}

	if(current_page < (total_pages - 1))
	{
		uint32 next_index = current_page + 1;
		shared_ptr<XMLNode> nodeFirstPrev = nodePager->addChild(_S("page"));
		nodeFirstPrev->setAttributeUint32(_S("index"), next_index + 1);
		nodeFirstPrev->setAttributeString(_S("type"), _S("next"));
		nodeFirstPrev->setAttributeString(_S("href"), getPageHref(items_to_show * next_index));
	}

	if(end_index != (total_pages - 1))
	{
		shared_ptr<XMLNode> nodeLastPage = nodePager->addChild(_S("page"));
		nodeLastPage->setAttributeUint32(_S("index"), total_pages);
		nodeLastPage->setAttributeString(_S("type"), _S("last"));
		nodeLastPage->setAttributeString(_S("href"), getPageHref(items_to_show * (total_pages - 1)));
	}
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
