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

#include "dataitem.h"
#include "ixmlfunction.h"
#include "xmlattribute.h"
#include "xmlattributes.h"
#include "xmldocument.h"
#include "xmlnode.h"
#include "xmlnodes.h"
#include "xmlstylesheet.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////

BOOST_AUTO_TEST_CASE(test_xmldocument)
{
	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	BOOST_CHECK(document->getRoot() == nullptr);

	BOOST_CHECK(document->parseString("<root alfa=\"1\" beta=\"2\"><child1 gamma=\"3\"><child2 delta=\"4\"/><child3/>child_data</child1>root_data</root>"));
	shared_ptr<XMLNode> root = document->getRoot();
	BOOST_CHECK(root != nullptr);
	BOOST_CHECK(root->getName() == "root");
	BOOST_CHECK(root->hasAttributes());
	BOOST_CHECK(root->hasNodes());
	BOOST_CHECK(root->hasData());
	BOOST_CHECK(root->getData() == "root_data");
		
	BOOST_CHECK(root->getAttributeUint32("alfa") == 1);
	BOOST_CHECK(root->getAttributeString("alfa") == "1");
	BOOST_CHECK(root->getAttributeUint32("beta") == 2);
	BOOST_CHECK(root->getAttributeString("beta") == "2");

	shared_ptr<XMLAttributes> rootAttributes = root->getAttributes();
	BOOST_CHECK(rootAttributes != nullptr);
	BOOST_CHECK(rootAttributes->empty() == false);
	BOOST_CHECK(rootAttributes->size() == 2);

	shared_ptr<XMLAttribute> alfa = rootAttributes->find("alfa");
	BOOST_CHECK(alfa != nullptr);
	BOOST_CHECK(alfa->getName() == "alfa");
	BOOST_CHECK(alfa->getValue() == "1");

	shared_ptr<XMLAttribute> beta = rootAttributes->find("beta");
	BOOST_CHECK(beta != nullptr);
	BOOST_CHECK(beta->getName() == "beta");
	BOOST_CHECK(beta->getValue() == "2");

	shared_ptr<XMLNodes> rootNodes = root->getNodes();
	BOOST_CHECK(rootNodes != nullptr);
	BOOST_CHECK(rootNodes->empty() == false);
	BOOST_CHECK(rootNodes->size() == 1);

	shared_ptr<XMLNode> child1 = rootNodes->find("child1");
	BOOST_CHECK(child1 != nullptr);
	BOOST_CHECK(rootNodes->find(0) == child1);
	BOOST_CHECK(rootNodes->find(1) == nullptr);

	BOOST_CHECK(child1->getName() == "child1");
	BOOST_CHECK(root->hasAttributes());
	BOOST_CHECK(root->hasNodes());
	BOOST_CHECK(child1->hasData());
	BOOST_CHECK(child1->getData() == "child_data");

	shared_ptr<XMLNodes> child1Nodes = child1->getNodes();
	BOOST_CHECK(child1Nodes != nullptr);
	BOOST_CHECK(child1Nodes->empty() == false);
	BOOST_CHECK(child1Nodes->size() == 2);

	shared_ptr<XMLNode> child2 = child1Nodes->find("child2");
	BOOST_CHECK(child2 != nullptr);
	BOOST_CHECK(child2->hasAttributes());
	BOOST_CHECK(child2->hasNodes() == false);
	BOOST_CHECK(child2->hasData() == false);
	BOOST_CHECK(child2->getAttributeUint32("delta") == 4);
	BOOST_CHECK(child2->getAttributeString("delta") == "4");

	shared_ptr<XMLNode> child3 = child1Nodes->find("child3");
	BOOST_CHECK(child3 != nullptr);
	BOOST_CHECK(child3->hasAttributes() == false);
	BOOST_CHECK(child3->hasNodes() == false);
	BOOST_CHECK(child3->hasData() == false);
}

class XMLTestFunction : public IXMLFunction
{
// Construction
public:
	XMLTestFunction() : IXMLFunction("test", "http://www.osiris-sps.org/xsl/testapp", 1, 1),
						m_calls(0),
						m_numericData(0)
	{

	}

	virtual ~XMLTestFunction()
	{

	}

// Attributes
public:
	inline uint32 getCalls() const
	{
		return m_calls;
	}

	inline const String & getStringData() const
	{
		return m_stringData;
	}

	inline uint32 getNumericData() const
	{
		return m_numericData;
	}

public:
	virtual bool execute(IContext &context, const Arguments &args, DataItem &result)
	{
		result = static_cast<bool>(args[0]);

		m_calls++;
		if(args.size() > 1)
		{
			const DataItem &optionalParam = args[1];
			if(optionalParam.getType() == DataItem::vt_string)
				m_stringData += static_cast<String>(optionalParam);
			else
				m_numericData += static_cast<uint32>(optionalParam);
		}

		return true;
	}

private:
	uint32 m_calls;
	String m_stringData;
	uint32 m_numericData;
};

BOOST_AUTO_TEST_CASE(test_xmlstylesheet)
{
	shared_ptr<XMLStylesheet> stylesheet(OS_NEW XMLStylesheet());

	String str = "<?xml version='1.0' encoding='utf-8'?>\
	<xsl:stylesheet xmlns:xsl=\"http://www.w3.org/1999/XSL/Transform\"\
					xmlns:testapp=\"http://www.osiris-sps.org/xsl/testapp\"\
					exclude-result-prefixes=\"testapp\"\
					version=\"1.0\">\
		<xsl:output method=\"xml\" indent=\"yes\" omit-xml-declaration=\"yes\"/>\
		<xsl:template match=\"/\">\
			<xsl:if test=\"testapp:test(true())\">\
				<xsl:value-of select=\"root/text()\"/>\
			</xsl:if>\
			<xsl:if test=\"testapp:test(false(), 'alfa')\">\
				<xsl:value-of select=\"root/text()\"/>\
			</xsl:if>\
			<xsl:if test=\"testapp:test(false())\">\
				<xsl:value-of select=\"root/text()\"/>\
			</xsl:if>\
			<xsl:if test=\"testapp:test(false(), 'beta')\">\
				<xsl:value-of select=\"root/text()\"/>\
			</xsl:if>\
			<xsl:if test=\"testapp:test(false(), 999)\">\
				<xsl:value-of select=\"root/text()\"/>\
			</xsl:if>\
			<xsl:if test=\"testapp:test(false(), 1)\">\
				<xsl:value-of select=\"root/text()\"/>\
			</xsl:if>\
		</xsl:template>\
	</xsl:stylesheet>";

	shared_ptr<XMLTestFunction> function(OS_NEW XMLTestFunction());

	BOOST_CHECK(stylesheet->registerFunction(function));
	BOOST_CHECK(stylesheet->parseString(str));

	shared_ptr<XMLDocument> document(OS_NEW XMLDocument());
	BOOST_CHECK(document->parseString("<root>hello</root>"));

	String output;
	BOOST_CHECK(stylesheet->applyToString(document, output));

	BOOST_CHECK(output == "hello");

	BOOST_CHECK(function->getCalls() == 6);
	BOOST_CHECK(function->getStringData() == "alfabeta");
	BOOST_CHECK(function->getNumericData() == 1000);
}

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
