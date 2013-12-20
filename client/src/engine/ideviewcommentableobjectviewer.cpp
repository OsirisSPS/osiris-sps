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
#include "ideviewcommentableobjectviewer.h"

#include "conversions.h"
#include "convert.h"
#include "dbtables.h"
#include "entitiesentity.h"
#include "entitiesentitychilds.h"
#include "httpparser.h"
#include "httprequest.h"
#include "httpsession.h"
#include "ideeditposteditor.h"
#include "idesystem.h"
#include "ideportaledit.h"
#include "idesession.h"
#include "ideskin.h"
#include "ideblock.h"
#include "ideviewpostviewer.h"
#include "objectsirevisionable.h"
#include "objectspost.h"
#include "omlquote.h"
#include "portalsportal.h"
#include "xmlportalexporter.h"
#include "xmlnode.h"


//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()
namespace ide { 
namespace view {
 
//////////////////////////////////////////////////////////////////////

CommentableObjectViewer::CommentableObjectViewer(shared_ptr<EntitiesEntity> entity) : ControlBase(entity)
{

}

CommentableObjectViewer::~CommentableObjectViewer()
{

}

CommentableObjectViewer::Action CommentableObjectViewer::getAction() const
{
	String action = getRequest()->getUrlParam(OS_URL_PARAM_ACTION);
	if(action.empty())
		return actNone;

	if(action == conversions::to_utf16<uint32>(actCreatePost))
		return actCreatePost;

	return actNone;
}

String CommentableObjectViewer::getCreatePostLink(const EntityID &quoted_object)
{
	ordered_map<std::wstring, std::wstring> params;
	params.set(OS_URL_PARAM_ACTION, conversions::to_wstring<uint32>(actCreatePost));

	if(quoted_object.empty() == false)
		params.set(OS_URL_PARAM_QUOTE, quoted_object.toWide());

	return getPortal()->getViewObjectLink(getEntityID(), params);
}

String CommentableObjectViewer::getEditObjectLink(shared_ptr<ObjectsIRevisionable> object) const
{
	if(object == nullptr)
	{
		OS_ASSERTFALSE();
		return String::EMPTY;
	}

	return portal::Edit::getUrl(getPortal(), object->getEntityID(), portal::Edit::acAddRevision);
}

String CommentableObjectViewer::localizePost(const EntityID &id)
{
	String href;

	shared_ptr<EntitiesEntity> entity = getPortal()->getEntity(getDatabase(), id);
	if(entity != nullptr)
	{
		shared_ptr<ObjectsPost> post = objects_post_cast(entity->getCurrent());
		if(post != nullptr)
		{
			shared_ptr<EntitiesEntity> parent_entity = getPortal()->getEntity(getDatabase(), post->getParent());
			if(parent_entity != nullptr)
			{
				shared_ptr<DbSqlSelect> select;
				// Ottiene la select per l'estrazione dei posts
				parent_entity->getChildsStatement(getDatabase(), portalObjectTypePost, RangeUint32::EMPTY, true, false, false, select);
				// Aggiunge come limite la data del post specificato per localizzarne l'indice
				select->where.add(DbSqlField(DBTABLES::SNAPSHOT_OBJECTS::SUBMIT_DATE, DBTABLES::SNAPSHOT_OBJECTS_TABLE), Convert::toSQL(entity->getEntitySubmitDate()), DbSqlCondition::cfMinor | DbSqlCondition::cfEqual | DbSqlCondition::cfAnd);

				ordered_map<std::wstring, std::wstring> params;
				// Calcola il numero di posts fino a quello specificato
				uint32 count = parent_entity->getChildsCount(getDatabase(), portalObjectTypePost, select);
				if(count > 0)
				{
					// Ottiene il numero
					uint32 pager_items = getSkin()->getPagerItems();
					// Calcola l'offset per la visualizzazione del post
					uint32 offset = static_cast<uint32>(count/pager_items)*pager_items;
					if(offset > 0)
						params.set(OS_URL_PARAM_OFFSET, conversions::to_wstring(offset));
				}

				// Genera un link al padre con ancora l'id del post
				href = HttpParser::createAnchor(parent_entity->getViewLink(getPortal(), params), entity->getEntityID().getString());
			}
		}
	}

	return href;
}

void CommentableObjectViewer::onViewObject()
{
	shared_ptr<ObjectsIRevisionable> object = getObject();
	if(object == nullptr)
		return;

	shared_ptr<XMLPortalExporter> exporter(OS_NEW XMLPortalExporter(getDocument()->create(object->getDescriptor()->getTypeName()), getPage(), XMLPortalExporter::emFull));
	// Renderizza l'oggetto
	renderObject(exporter, object);
	// Renderizza i commenti sull'oggetto
	renderPosts(exporter);
	// Renderizza i tags dell'oggetto
	renderTags(exporter);
}

void CommentableObjectViewer::onCreatePost()
{
	String quoted_text;
	shared_ptr<EntitiesEntity> quoted_entity;

	EntityID quoted_id = static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_QUOTE)).to_ascii();
	if(quoted_id.empty() == false)
	{
		quoted_entity = getPortal()->getEntity(getDatabase(), quoted_id);
		if(quoted_entity != nullptr)
		{
			shared_ptr<ObjectsIObject> quoted_object = quoted_entity->getCurrent();
			if(quoted_object != nullptr)
			{
				quoted_text = OMLQuote::quoteObject(quoted_entity->getEntityID(), quoted_object->getContent());
				quoted_text += _S("\r\n");
			}
		}
	}

	shared_ptr<ObjectsIRevisionable> object = getObject();

	shared_ptr<edit::PostEditor> post_editor(OS_NEW edit::PostEditor(nullptr, getEntity()));
	post_editor->setID(_S("postEditor"));
	post_editor->setTitle(_S("re: ") + object->getTitle());
	post_editor->setContent(quoted_text);
	if(quoted_entity != nullptr)
		post_editor->setReference(quoted_entity->getEntityID());

	post_editor->setRedirectionType(edit::IObjectEditor::rtViewParent);
	post_editor->setRedirectionParam(OS_URL_PARAM_OFFSET, conversions::to_wstring(IIdePageable::OFFSET_LAST));

	getControls()->add(post_editor);

	_createLastPosts();
}

void CommentableObjectViewer::_createLastPosts()
{
	// Calcola il numero di posts totali
	uint32 total_posts = getEntity()->getChildsCount(getDatabase(), portalObjectTypePost);
	// Calcola il numero di risposte da visualizzare
	uint32 posts_count = utils::min<uint32>(total_posts, getSkin()->getPagerItems());
	// Carica i posts
	shared_ptr<EntitiesEntities> posts = getEntity()->getChilds(getDatabase(), portalObjectTypePost, RangeUint32(total_posts - posts_count, posts_count));
	if(posts->empty() == false)
	{
		shared_ptr<PostViewer> posts_viewer(OS_NEW PostViewer());
		getControls()->add(posts_viewer);
		// Scorre gli ultimi posts dal pi recente
		for(EntitiesEntities::reverse_iterator i = posts->rbegin(); i != posts->rend(); ++i)
		{
			posts_viewer->addPost(posts->get(getDatabase(), *i));
		}
	}
}

void CommentableObjectViewer::renderObject(shared_ptr<XMLPortalExporter> exporter, shared_ptr<ObjectsIRevisionable> object)
{
	object->exportXML(exporter);

	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		exporter->createAction(_S("reply"), getCreatePostLink());
		exporter->createAction(_S("edit"), getEditObjectLink(object));
	}
}

void CommentableObjectViewer::renderPosts(shared_ptr<XMLPortalExporter> exporter)
{
	if(getObject() == nullptr || getObject()->allowChild(portalObjectTypePost) == false)
		return;

	// Calcola il numero totale di posts
	uint32 total_posts = getEntity()->getChildsCount(getDatabase(), portalObjectTypePost);
	if(total_posts == 0)
		return;

	// Numero di posts da visualizzare per pagina
	uint32 posts_to_show = getSkin()->getPagerItems();
	// Inizializza l'offset di estrazione
	uint32 offset = conversions::from_utf16<uint32>(getRequest()->getUrlParam(OS_URL_PARAM_OFFSET));

	// Corregge l'offset se necessario
	adjustOffset(total_posts, posts_to_show, offset);

	objects_posts_list posts;
	// Estrae le risposte nell'intervallo richiesto
	shared_ptr<EntitiesEntities> post_childs = getEntity()->getChilds(getDatabase(), portalObjectTypePost, RangeUint32(offset, posts_to_show));
	for(EntitiesEntities::iterator i = post_childs->begin(); i != post_childs->end(); ++i)
	{
		// Carica la riposta corrente
		shared_ptr<EntitiesEntity> post_entity = post_childs->get(getDatabase(), *i);
		if(post_entity != nullptr)
		{
			// Calcola la revisione corrente della risposta
			shared_ptr<ObjectsPost> current_post = objects_post_cast(post_entity->getCurrent());
			if(current_post != nullptr)
				posts.push_back(current_post);
		}
	}

	if(posts.empty() == false)
	{
		shared_ptr<XMLNode> posts_node = exporter->getNode(OS_PORTAL_OBJECT_POST_GROUPNAME);
		for(objects_posts_list::const_iterator i = posts.begin(); i != posts.end(); ++i)
		{
			shared_ptr<XMLPortalExporter> postExporter(OS_NEW XMLPortalExporter(posts_node->addChild(OS_PORTAL_OBJECT_POST_TYPENAME), exporter->getPage(), exporter->getMode()));
			renderPost(postExporter, *i);
		}
	}

	createPager(getSkin(), exporter->getRoot(), total_posts, posts_to_show, offset);
}

void CommentableObjectViewer::renderPost(shared_ptr<XMLPortalExporter> exporter, shared_ptr<ObjectsPost> post)
{
	post->exportXML(exporter);

	if(getSessionAccount()->isPortalGuest(getDatabase()) == false)
	{
		exporter->createAction(_S("quote"), getCreatePostLink(post->getEntityID()));
		exporter->createAction(_S("edit"), getEditObjectLink(post));
	}
}

void CommentableObjectViewer::onInit()
{
	ControlBase::onInit();

	EntityID target = static_cast<String>(getRequest()->getUrlParam(OS_URL_PARAM_TARGET)).to_ascii();
	if(target.validate(false))
	{
		String href = localizePost(target);
		if(href.empty() == false)
		{
			getSession()->redirect(href.to_ascii());
			return;
		}
	}
}

void CommentableObjectViewer::onLoad()
{
	ControlBase::onLoad();

	switch(getAction())
	{
	case actNone:			onViewObject();
							break;

	case actCreatePost:		onCreatePost();
							break;

	default:				OS_ASSERTFALSE();
							break;
	}
}

//////////////////////////////////////////////////////////////////////

} // view
} // ide
OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
