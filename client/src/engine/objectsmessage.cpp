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
#include "objectsmessage.h"

#include "cryptmanager.h"
#include "cryptkey.h"
#include "idesystem.h"
#include "objectsobjects.h"
#include "xmldocument.h"
#include "xmlnode.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

ObjectsMessage::ObjectsMessage()
{

}

ObjectsMessage::ObjectsMessage(const ObjectsMessage &second) : BaseClass(second)
{
	*this = second;
}

ObjectsMessage::~ObjectsMessage()
{

}

Buffer ObjectsMessage::generateSession()
{
	return CryptManager::instance()->randomSHA();
}

bool ObjectsMessage::encode(const Buffer &public_key, Buffer &encrypted_session, Buffer &encrypted_content) const
{
	if(subject.empty() || body.empty())
		return false;

	// Genera una nuova chiave di sessione
	Buffer plain_session = generateSession();
	// Cripta la chiave di sessione con la chiave pubblica specificata
	if(CryptManager::instance()->rsaEncrypt(public_key, plain_session.getData(), plain_session.getSize(), encrypted_session) == false)
		return false;

	String plain_content;
	// Esporta il messaggio
	if(_toString(plain_content) == false)
		return false;

	// Crypta il messaggio con la chiave di sessione
	CryptKey key(plain_session.getData(), plain_session.getSize());
	return CryptManager::instance()->aesEncrypt(key, plain_content.buffer(), plain_content.buffer_size(), encrypted_content);
}

bool ObjectsMessage::decode(const Buffer &private_key, const Buffer &encrypted_session, const Buffer &encrypted_content)
{
	Buffer plain_session;
	// Decripta la chiave di sessione
	if(CryptManager::instance()->rsaDecrypt(private_key, encrypted_session.getData(), encrypted_session.getSize(), plain_session) == false)
		return false;

	Buffer plain_content;
	// Decripta il messaggio con la chiave originale
	CryptKey key(plain_session.getData(), plain_session.getSize());
	if(CryptManager::instance()->aesDecrypt(key, encrypted_content.getData(), encrypted_content.getSize(), plain_content) == false)
		return false;

	// Carica il messaggio
	String plain_xml;
	if(plain_content.empty() == false)
		plain_xml.assign(reinterpret_cast<const uchar *>(plain_content.getData()), plain_content.getSize() / sizeof(uchar));

	if(_fromString(plain_xml) == false)
		return false;

	return true;
}

bool ObjectsMessage::_toString(String &str) const
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());

	shared_ptr<XMLNode> root = document->create(OS_MESSAGE_XML_ROOT);
	root->addChild(OS_MESSAGE_XML_SUBJECT)->setData(subject);
	root->addChild(OS_MESSAGE_XML_BODY)->setData(body);

	return document->writeString(str);
}

bool ObjectsMessage::_fromString(const String &str)
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	if(document->parseString(str) == false)
		return false;

	shared_ptr<XMLNode> root = document->getRoot();
	shared_ptr<XMLNode> subject_node = root->getNode(OS_MESSAGE_XML_SUBJECT);
	if(subject_node != nullptr)
		subject = subject_node->getData();

	shared_ptr<XMLNode> body_node = root->getNode(OS_MESSAGE_XML_BODY);
	if(body_node != nullptr)
		body = body_node->getData();

	return true;
}

ObjectsMessage & ObjectsMessage::operator =(const ObjectsMessage &second)
{
	subject = second.subject;
	body = second.body;

	return *this;
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
