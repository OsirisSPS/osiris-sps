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
#include "omlmanager.h"

#include "boost/bind.hpp"
#include "conversions.h"
#include "timerperformance.h"
#include "realtimestats.h"
#include "platformmanager.h"
#include "omlcontext.h"
#include "iomlcode.h"
#include "omlitem.h"
#include "idesystem.h"
#include "log.h"
#include "css.h"
#include "htmlparser.h"
#include "omla.h"
#include "omlanchor.h"
#include "omlcode.h"
#include "omldate.h"
#include "omldebug.h"
#include "omlemail.h"
#include "omlemoticon.h"
#include "omlgoto.h"
#include "omlhtml.h"
#include "omlhtmlwrapper.h"
#include "omlif.h"
#include "omlimg.h"
#include "omllang.h"
#include "omlless.h"
#include "omllist.h"
#include "omlmore.h"
#include "omlnobr.h"
#include "omlnoparse.h"
#include "omlobject.h"
#include "omlpopup.h"
#include "omlquote.h"
#include "omlreplace.h"
#include "omlsize.h"
#include "omlsystem.h"
#include "omltab.h"
#include "omltext.h"
#include "omlurl.h"
#include "omluser.h"
#include "omlview.h"

#include "regexmanager.h"

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_BEGIN()

//////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////

OMLManager::OMLManager() :	m_eventBeforeParse(OS_NEW EventSource()),
							m_eventAfterParse(OS_NEW EventSource()),
							m_eventBeforeReverseParse(OS_NEW EventSource()),
							m_eventAfterReverseParse(OS_NEW EventSource())
{
	init();
}

OMLManager::~OMLManager()
{

}

void OMLManager::add(shared_ptr<IOMLCode> code)
{
	// TODO: andrebbe protetta da boost::unique_lock

	m_codes[code->getTag()] = code;
}

void OMLManager::remove(shared_ptr<IOMLCode> code)
{
	// TODO: andrebbe protetta da boost::unique_lock

	if(code == nullptr)
	{
		OS_ASSERTFALSE();
		return;
	}

	m_codes.erase(code->getTag());
}

void OMLManager::addReplace(const String& tag, const int32 minParams, const int32 maxParams, const String &replaceHtml, const String &replacePlain, bool allowedInRowMode, const String &mapDefaultParamTo)
{
	add(shared_ptr<IOMLCode>(OS_NEW OMLReplace(tag, minParams, maxParams, replaceHtml, replacePlain, allowedInRowMode, mapDefaultParamTo)));
}

void OMLManager::addHtmlWrapper(const String& tag, const String& htmlTag, bool allowedInRowMode, const String &parentTag, const String &childsTag, const String &mapDefaultParamTo)
{
	add(shared_ptr<IOMLCode>(OS_NEW OMLHtmlWrapper(tag, htmlTag, allowedInRowMode, parentTag, childsTag, mapDefaultParamTo)));
}

String OMLManager::parse(const String& text, shared_ptr<IPage> page, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl)
{
	return parse(text, page, false, full, secure, row, mode, refID, fullUrl)->getOutput();
}

shared_ptr<OMLContext> OMLManager::parse(const String& text, shared_ptr<IPage> page, bool enableWarning, bool full, bool secure, bool row, OMLRenderMode mode, const String &refID, const String &fullUrl)
{
	RealtimeStatsScopeTimer RSS1(_S("Activity"), _S("Parsering OML"));
	
	EventBeforeParse e;
	e.setOml(text);
	getEventBeforeParse()->fire(&e);

	OS_ASSERT(m_codes.empty() == false);

	String renderedText = e.getOml();

	// General cleaning
	renderedText.replace_all(_S("\r\n"), _S("\n"));

	// Exception: "alfa [[beta" -> "alfa \[[beta".
	/*
	{
		String::size_type curPos = 0;
		for(;;)
		{
			String::size_type newPos = renderedText.find(_S("["), curPos);
			if( (newPos != String::npos) && (newPos>=1) && (newPos<renderedText.length()-1)
				if( (renderedText[newPos-1] != _S("\\")) && (renderedText[newPos+1] == _S("[")) )
					renderedText = renderedText.
		}
		*/
	//renderedText.replace_all(_S("\\[["),_S("[[")); // If i use the correct syntax ("alfa \[[beta"), this will be reverted in the next command.
	//renderedText.replace_all(_S("[["),_S("\\[["));

	shared_ptr<OMLContext> context = shared_ptr<OMLContext>(OS_NEW OMLContext(page, enableWarning, full, secure, row, mode, refID, fullUrl));
	RSS1.stop();

	RealtimeStatsScopeTimer RSS2(_S("Activity"), _S("Rendering OML"));
	context->parseOml(renderedText);
	RSS2.stop();

	{
		EventAfterParse e;
		e.setHtml(context->getOutput());
		getEventAfterParse()->fire(&e);
		context->setOutput(e.getHtml());
	}

	return context;
}

// riconverte testo html in BBCode al meglio passibile...
String OMLManager::reverseParse(const String& htmlText, const std::string &format)
{
	/*
	Casi particolari:
	- Caso "img src="alfa.gif">. Se gli chiedo l'url potrei convertire i path in assoluti...
	- Se incontro un [TD] o un [LI]?

	*/

	String input = htmlText;

	String output;

	/* Prima fase, dovrebbe essere pulito:
	- Vengono messi i doppi apici come delimitatori di valori di attributi al posto dei singoli apici o dove non ci sono
	- TODO
	*/
	output = input;

	{
		EventBeforeReverseParse e;
		e.setSource(output);
		e.setFormat(format);
		getEventBeforeReverseParse()->fire(&e);
		output = e.getSource();
	}

	if(format != "oml")
	{
		// Html comments
		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<!--(.*?)-->")), _S("<!>$1</!>"));

		// Html tag 'strong'
		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<strong>(.*?)</strong>")), _S("<b>$1</b>"));

		// Via gli <script>
		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<script(.*?)</script>")), _S(""));

		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<head(.*?)</head>")), _S(""));

		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<title(.*?)</title>")), _S(""));

		// Via i <noscript>
		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<noscript(.*?)</noscript>")), _S(""));

		// Via gli <object>
		//output = regex_replace(output, RegexManager::instance()->getRegex(_S("<object(.*?)</object>")), _S(""));

		// Via gli eventi
		//output = regex_replace(output, _S(" on(.*?)=\"(.*?)\""), _S(" "), true); // DATESTARE, non va

		// Via gli href a javascript
		//output = regex_replace(output, _S(" href=\"javascript:{.*?)\""), _S(" "), true); // DATESTARE, non va

		// Tolgo gli acapo
		output.replace_all(_S("\r"), String::EMPTY);
		output.replace_all(_S("\n"), String::EMPTY);

		// Conversione dei <br> in \n
		output = regex_replace(output, RegexManager::instance()->getRegex(_S("<br(.*?)>")), _S("\n"));

		// Lo converto da html a osml
		output.replace_all(_S("\\<"),_S("\\ <")); // Questo serve per evitare che un "xxx\<span>" diventi "xxx\[span]", sbagliato ( '[' escapizzato ).
		output.replace_all(_S("\\>"),_S("\\ >")); // Item come sopra.
		if(format == "html") // no for "bbc" or "bbc+html".
			output.replace_all(_S("["),_S("\\["));
		output.replace_all(_S("<"),_S("["));
		output.replace_all(_S(">"),_S("]"));
		output.replace_all(_S("'"), _S("\"")); // TEMP

		// Decode html, per convertire ad esempio &amp; in &
		output = HtmlParser::instance()->decode(output);
	}

	{
		EventAfterReverseParse e;
		e.setOml(output);
		getEventAfterReverseParse()->fire(&e);
		output = e.getOml();
	}

	return output;
}

String OMLManager::parseCss(const String& cssText, shared_ptr<IPage> page, bool secure, bool encode)
{
	if(cssText.empty())
		return cssText;

	shared_ptr<OMLContext> context = shared_ptr<OMLContext>(OS_NEW OMLContext(page, false, true, secure, false, omlRenderModeOsiris, String::EMPTY, String::EMPTY));

	context->parseCss(cssText, encode);

	return context->getOutput();
}

void OMLManager::init()
{
	/* ----------------------------------------------------------------------
	   Text format
    ---------------------------------------------------------------------- */

	// Grassetto
	//addReplace(_S("b"), 0, 0, _S("<span class=\"") + OS_CSS_OML_B + _S("\">{$body}</span>"), _S("{$body}"), true);

	// Corsivo
	//addReplace(_S("i"), 0, 0, _S("<span class=\"") + OS_CSS_OML_I + _S("\">{$body}</span>"), _S("{$body}"), true);

	// Sottolineato
	addReplace(_S("u"), 0, 0, _S("<span class=\"") + OS_CSS_OML_U + _S("\">{$body}</span>"), _S("{$body}"), true);

	// Barrato
	//addReplace(_S("s"), 0, 0, _S("<span class=\"") + OS_CSS_OML_S + _S("\">{$body}</span>"), _S("{$body}"), true);

	// Sopralineato
	addReplace(_S("o"), 0, 0, _S("<span class=\"") + OS_CSS_OML_O + _S("\">{$body}</span>"), _S("{$body}"), true);

	// Colore
	addReplace(_S("color"), 1, 1, _S("<span class=\"") + OS_CSS_OML_COLOR + _S("\" style=\"color:{$default}\">{$body}</span>"), _S(" {$body} "), true);

	// Dimensione
	//addReplace(_S("size"), 1, 1, _S("<span class=\"") OS_CSS_OML_SIZE _S("\" style=\"font-size:{$default}\">{$body}</span>"), _S(" {$body} "));
	add(shared_ptr<IOMLCode>(OS_NEW OMLSize(_S("size"))));

	// Font
	addReplace(_S("font"), 1, 1, _S("<span class=\"") + OS_CSS_OML_FONT + _S("\" style=\"font-family:{$default}\">{$body}</span>"), _S(" {$body} "));
	addHtmlWrapper(_S("small"), _S("small"), true); // NODOCS


	// Highlight
	addReplace(_S("highlight"), 0, 0, _S("<span class=\"") + OS_CSS_OML_HIGHLIGHT + _S("\">{$body}</span>"), _S(" {$body} "), true);

	addHtmlWrapper(_S("ins"), _S("ins"), true); // NODOCS
	addHtmlWrapper(_S("del"), _S("del"), true); // NODOCS
	addHtmlWrapper(_S("sup"), _S("sup"), true);
	addHtmlWrapper(_S("sub"), _S("sub"), true);

	// Heading 1..6, preformatted
	addHtmlWrapper(_S("h1"), _S("h1"), false);
	addHtmlWrapper(_S("h2"), _S("h2"), false);
	addHtmlWrapper(_S("h3"), _S("h3"), false);
	addHtmlWrapper(_S("h4"), _S("h4"), false);
	addHtmlWrapper(_S("h5"), _S("h5"), false);
	addHtmlWrapper(_S("h6"), _S("h6"), false);
	addReplace(_S("pre"), 0, 0, _S("<span class=\"") + OS_CSS_OML_PREFORMATTED + _S("\">{$body}</span>"), _S("{$body}"));

	addHtmlWrapper(_S("abbr"), _S("abbr"), true, String::EMPTY, String::EMPTY, _S("title"));
	addHtmlWrapper(_S("acronym"), _S("acronym"), true, String::EMPTY, String::EMPTY, _S("title"));
	addHtmlWrapper(_S("address"), _S("address"), false);
	addHtmlWrapper(_S("big"), _S("big"), false); // NODOCS
	addHtmlWrapper(_S("br"), _S("br"), false);
	addHtmlWrapper(_S("tt"), _S("tt"), false);
	addHtmlWrapper(_S("marquee"), _S("marquee"), false); // NODOCS
	addHtmlWrapper(_S("p"), _S("p"), false); // NODOCS

	/* ----------------------------------------------------------------------
	   Layout
    ---------------------------------------------------------------------- */

	// Align-left
	addReplace(_S("left"), 0, 0, _S("<div class=\"") + OS_CSS_OML_LEFT + _S("\">{$body}</div>"), _S(" {$body} "));

	// Align-center
	addReplace(_S("center"), 0, 0, _S("<div class=\"") + OS_CSS_OML_CENTER + _S("\">{$body}</div>"), _S(" {$body} "));

	// Align-right
	addReplace(_S("right"), 0, 0, _S("<div class=\"") + OS_CSS_OML_RIGHT + _S("\">{$body}</div>"), _S(" {$body} "));

	// Align-justify
	addReplace(_S("justify"), 0, 0, _S("<div class=\"") + OS_CSS_OML_JUSTIFY + _S("\">{$body}</div>"), _S(" {$body} "));

	// Indent
	addReplace(_S("indent"), 0, 0, _S("<div class=\"") + OS_CSS_OML_INDENT + _S("\">{$body}</div>"), _S(" {$body} "));

	// Table
	addHtmlWrapper(_S("table"), _S("table"), false, String::EMPTY, _S("thead;tbody;tfoot;tr;caption;col;colgroup"));
	addHtmlWrapper(_S("thead"), _S("thead"), false, _S("table"), _S("tr")); // NODOCS
	addHtmlWrapper(_S("tbody"), _S("tbody"), false, _S("table"), _S("tr")); // NODOCS
	addHtmlWrapper(_S("tfoot"), _S("tfoot"), false, _S("table"), _S("tr")); // NODOCS
	addHtmlWrapper(_S("tr"), _S("tr"), false, _S("table;thead;tbody;tfoot"), _S("td;th"));
	addHtmlWrapper(_S("td"), _S("td"), false, _S("tr"), String::EMPTY);
	addHtmlWrapper(_S("th"), _S("th"), false, _S("tr"), String::EMPTY);

	addHtmlWrapper(_S("caption"), _S("caption"), false, _S("table"), String::EMPTY); // NODOCS
	addHtmlWrapper(_S("col"), _S("col"), false, _S("table"), String::EMPTY); // NODOCS
	addHtmlWrapper(_S("colgroup"), _S("colgroup"), false, _S("table"), String::EMPTY); // NODOCS

	// Phrase Elements
	addHtmlWrapper(_S("strong"), _S("strong"), true);
	addHtmlWrapper(_S("em"), _S("em"), true);
	addHtmlWrapper(_S("mark"), _S("mark"), true);

	addHtmlWrapper(_S("b"), _S("strong"), true);	
	addHtmlWrapper(_S("i"), _S("em"), true);

	addHtmlWrapper(_S("s"), _S("s"), true);
	addHtmlWrapper(_S("strike"), _S("strike"), true);
	addHtmlWrapper(_S("q"), _S("q"), true);
	
	addHtmlWrapper(_S("dfn"), _S("dfn"), false);
	addHtmlWrapper(_S("samp"), _S("samp"), false);
	addHtmlWrapper(_S("kbd"), _S("kbd"), false);
	addHtmlWrapper(_S("var"), _S("var"), false);
	addHtmlWrapper(_S("cite"), _S("cite"), false);

	// Div
	addHtmlWrapper(_S("div"), _S("div"), false);
	addHtmlWrapper(_S("span"), _S("span"), false); // NODOCS

	/* ----------------------------------------------------------------------
	   Other
    ---------------------------------------------------------------------- */

	// List
	add(shared_ptr<IOMLCode>(OS_NEW OMLList(_S("list"))));
	addHtmlWrapper(_S("*"), _S("li"), false, _S("list"), String::EMPTY);
	addHtmlWrapper(_S("ul"), _S("ul"), false, String::EMPTY, _S("li"));
	addHtmlWrapper(_S("ol"), _S("ol"), false, String::EMPTY, _S("li"));
	addHtmlWrapper(_S("li"), _S("li"), false, _S("ul;ol;list"), String::EMPTY);
	addHtmlWrapper(_S("dl"), _S("dl"), false, String::EMPTY, _S("dt;dd"));
	addHtmlWrapper(_S("dt"), _S("dt"), false, _S("dl"), String::EMPTY);
	addHtmlWrapper(_S("dd"), _S("dd"), false, _S("dl"), String::EMPTY);

	// Horizontal rules
	addHtmlWrapper(_S("hr"), _S("hr"), false);

	/* ----------------------------------------------------------------------
	   TextBlock
    ---------------------------------------------------------------------- */

	// Code
	add(shared_ptr<IOMLCode>(OS_NEW OMLCode(_S("code"))));

	// CodeBox
	add(shared_ptr<IOMLCode>(OS_NEW OMLCode(_S("codebox"))));

	// Quote
	add(shared_ptr<IOMLCode>(OS_NEW OMLQuote(_S("quote"))));
	// add(shared_ptr<IOMLCode>(OS_NEW OMLQuote(_S("blockquote"))));
	addHtmlWrapper(_S("blockquote"), _S("blockquote"), false);

	// Box text
	addReplace(_S("box"), 0, 0, _S("<div class=\"") + OS_CSS_OML_BOX + _S("\">{$body}</div>"), _S(" {$body} "));

	// spolier
	addReplace(_S("spoiler"), 0, 1, _S("<div class=\"") + OS_CSS_OML_SPOILER + _S("\"data-os-otype=\"popup\" data-os-popupMode=\"staticclick\"><div data-os-popup=\"header\" class=\"") + OS_CSS_OML_SPOILER_HEADER + _S("\">{$title}</div><div data-os-popup=\"body\" style=\"display:none;\" class=\"") + OS_CSS_OML_SPOILER_BODY + _S("\">{$body}</div></div>"), _S(" {$title} {$body} "), false, _S("title"));

	// whisper
	addReplace(_S("whisper"), 0, 1, _S("<div class=\"") + OS_CSS_OML_WHISPER + _S("\"data-os-otype=\"popup\" data-os-popupMode=\"staticclick\"><div data-os-popup=\"header\" class=\"") + OS_CSS_OML_WHISPER_HEADER + _S("\">{$title}</div><div data-os-popup=\"body\" style=\"display:none;\" class=\"") + OS_CSS_OML_WHISPER_BODY + _S("\">{$body}</div></div>"), _S(" {$title} {$body} "), false, _S("title"));

	// popup
	add(shared_ptr<IOMLCode>(OS_NEW OMLPopup(_S("popup")))); // NODOCS

	// tab
	//add(shared_ptr<IOMLCode>(OS_NEW Tab(_S("tab")))); // NODOCS

	/* ----------------------------------------------------------------------
	   Links
    ---------------------------------------------------------------------- */

	add(shared_ptr<IOMLCode>(OS_NEW OMLImg(_S("img"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLA(_S("a"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLUrl(_S("url"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLUrl(_S("iurl"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLEMail(_S("email"))));

	add(shared_ptr<IOMLCode>(OS_NEW OMLAnchor(_S("anchor")))); // NODOCS
	add(shared_ptr<IOMLCode>(OS_NEW OMLGoto(_S("goto")))); // NODOCS

	/* ----------------------------------------------------------------------
	   Language
    ---------------------------------------------------------------------- */

	add(shared_ptr<IOMLCode>(OS_NEW OMLLang(_S("lang"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLDate(_S("date"))));
	addHtmlWrapper(_S("bdo"), _S("bdo"), false);

	/* ----------------------------------------------------------------------
	   Special
    ---------------------------------------------------------------------- */

	add(shared_ptr<IOMLCode>(OS_NEW OMLNoBR(_S("nobr")))); // NODOCS
	add(shared_ptr<IOMLCode>(OS_NEW OMLNoParse(_S("noparse"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLMore(_S("more"))));
	add(shared_ptr<IOMLCode>(OS_NEW OMLLess(_S("less")))); // NODOCS
	addReplace(_S("!"), 0, 0, _S("<!--{$body}-->"), String::EMPTY, false);
	add(shared_ptr<IOMLCode>(OS_NEW OMLIf(_S("if"))));

	/* ----------------------------------------------------------------------
	   Objects
    ---------------------------------------------------------------------- */


	/* 0.14 removed
	add(shared_ptr<IOMLCode>(OS_NEW OMLHtml(_S("html")))); // NODOCS
	*/

	add(shared_ptr<IOMLCode>(OS_NEW OMLObject(_S("object")))); // NODOCS
	addHtmlWrapper(_S("param"), _S("param"), false, _S("object")); // NODOCS
	addHtmlWrapper(_S("embed"), _S("embed"), false, _S("object")); // NODOCS	

#ifdef OS_DEBUG
	add(shared_ptr<IOMLCode>(OS_NEW OMLSystem(_S("sys")))); // NODOCS
	add(shared_ptr<IOMLCode>(OS_NEW OMLUser(_S("user")))); // NODOCS
	add(shared_ptr<IOMLCode>(OS_NEW OMLView(_S("view")))); // NODOCS
#endif

	/* ----------------------------------------------------------------------
	   Internal
    ---------------------------------------------------------------------- */

	add(shared_ptr<IOMLCode>(OS_NEW OMLText(OMLItem::ITEM_TAG_TEXT)));
#ifdef OS_DEBUG
	add(shared_ptr<IOMLCode>(OS_NEW OMLDebug(_S("debug"))));
#endif

	/* ----------------------------------------------------------------------
	   Emoticons
	---------------------------------------------------------------------- */

	addEmoticon(_S(":)"), _S("smile"));
	addEmoticon(_S(":("), _S("sad"));
	addEmoticon(_S("o:)"), _S("angel"));
	addEmoticon(_S(">:)"), _S("evil"));
	addEmoticon(_S(":|"), _S("indifferent"));
	addEmoticon(_S(";)"), _S("winking"));
	addEmoticon(_S(":p"), _S("silly"));
	addEmoticon(_S("|o"), _S("yawn"));
	addEmoticon(_S(":d"), _S("laugh"));
	addEmoticon(_S(":@"), _S("what"));
	addEmoticon(_S("8d"), _S("awesome"));
	addEmoticon(_S("b)"), _S("cool"));
	addEmoticon(_S(":o"), _S("ohmy"));
	addEmoticon(_S("<_<"), _S("dry"));
	addEmoticon(_S("-_-"), _S("closedeyes"));
	addEmoticon(_S(":asd:"), _S("asd"));
	addEmoticon(_S(":angry:"), _S("angry"));
	addEmoticon(_S(":blink:"), _S("blink"));
	addEmoticon(_S(":huh:"), _S("huh"));
	addEmoticon(_S(":ranting:"), _S("ranting"));
	addEmoticon(_S(":cry:"), _S("cry"));
	addEmoticon(_S(":lol:"), _S("lol"));
	addEmoticon(_S(":superlol:"), _S("superlol"));
	addEmoticon(_S(":megalol:"), _S("megalol"));
	addEmoticon(_S(":mellow:"), _S("mellow"));
	addEmoticon(_S(":rolleyes:"), _S("rolleyes"));
	addEmoticon(_S(":unsure:"), _S("unsure"));
	addEmoticon(_S(":think:"), _S("think"));
	addEmoticon(_S(":confused:"), _S("confused"));
	addEmoticon(_S(":rofl:"), _S("rofl"));
	addEmoticon(_S(":sorry:"), _S("sorry"));
	addEmoticon(_S(":whistle:"), _S("whistle"));
	addEmoticon(_S(":doh:"), _S("doh"));
	addEmoticon(_S(":dunno:"), _S("dunno"));
	addEmoticon(_S(":yes:"), _S("yes"));
	addEmoticon(_S(":no:"), _S("no"));
	addEmoticon(_S(":up:"), _S("up"));
	addEmoticon(_S(":down:"), _S("down"));
	addEmoticon(_S(":ok:"), _S("ok"));
	addEmoticon(_S(":good:"), _S("good"));
	addEmoticon(_S(":bye:"), _S("bye"));
	addEmoticon(_S(":doc:"), _S("doc"));
	addEmoticon(_S(":ph34r:"), _S("ph34r"));
	addEmoticon(_S(":clap:"), _S("clap"));
	addEmoticon(_S(":rules:"), _S("rules"));
	addEmoticon(_S(":the:"), _S("the"));
	addEmoticon(_S(":worthy:"), _S("worthy"));
	addEmoticon(_S(":cheers:"), _S("cheers"));
	addEmoticon(_S(":spank:"), _S("spank"));
	addEmoticon(_S(":wallbash:"), _S("wallbash"));
	addEmoticon(_S(":flowers:"), _S("flowers"));
	addEmoticon(_S(":welcome:"), _S("welcome"));
	addEmoticon(_S(":spam:"), _S("spam"));
	addEmoticon(_S(":closed:"), _S("closed"));
	addEmoticon(_S(":ot:"), _S("offtopic"));
	addEmoticon(_S(":dots:"), _S("dots"));
	addEmoticon(_S(":ban:"), _S("ban"));
	addEmoticon(_S(":flood:"), _S("flood"));

	//m_eventBeforeParse.connect(boost::bind(&OMLManager::onEventTest, this, _1));
}

/*
void OMLManager::onEventTest(IEvent *e)
{
	OMLManager::EventBeforeParse *myEvent = dynamic_cast<OMLManager::EventBeforeParse *>(e);

	String oml = myEvent->getOml();
	oml += _S("-eventTest");
	myEvent->setOml(oml);
}
*/

bool OMLManager::isKnownTag(const String& tag)
{
	if(tag == _S("?")) // Special tag
		return true;
	return (m_codes.find(tag) != m_codes.end());
}

String OMLManager::processItemForHtml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context)
{
	if(i->getSkipped())
		return String::EMPTY;

	Codes::const_iterator i2 = m_codes.find(i->getTagName());
	if(i2 == m_codes.end())
	{
		context->addWarning(String::format(_S("Tag [%S] unknown.").c_str(), i->getTagName().c_str()));
		// Prova almeno a decodificare i figli.
		return i->getHtmlChilds(context);
	}

	if( (context->getRow()) && (!i2->second->allowRowMode()) )
	{
		context->addWarningRowModeNotAllowed(i);
		return String::EMPTY;
	}
	else
		return i2->second->processHtml(i, context);
}

String OMLManager::processItemForOsml(shared_ptr<OMLItem> i, shared_ptr<OMLContext> context)
{
	if(i->getSkipped())
		return String::EMPTY;

	Codes::const_iterator i2 = m_codes.find(i->getTagName());
	if(i2 == m_codes.end())
	{
		context->addWarning(String::format(_S("Tag [%S] unknown.").c_str(), i->getTagName().c_str()));
		// Prova almeno a decodificare i figli.
		return i->getOsmlChilds(context);
	}

	if( (context->getRow()) && (!i2->second->allowRowMode()) )
	{
		context->addWarningRowModeNotAllowed(i);
		return String::EMPTY;
	}
	else
		return i2->second->processOsml(i, context);
}

void OMLManager::addEmoticon(const String &code, const String &name)
{
	// TODO: andrebbe protetta da boost::unique_lock e resa pubblica, la getEmoticons da una shared_lock

	shared_ptr<OMLEmoticon> emo(OS_NEW OMLEmoticon(code, name));
	m_emoticons.push_back(emo);
}

/* *******************************************************
   String Utils
******************************************************* */

/*
void OMLManager::ReplaceBetweenTag(String& text, const String& tagName, const String& from, const String& to)
{
	// Ex. alfa:) [code]alfa:)[/code] beta
	// Rs. alfa:) [code]alfa&#91;&#42343;[/code] beta

	String output;

	String tagStart = _S("[") + tagName + _S("]");
	size_t tagStartLen = tagStart.length();
	String tagEnd = _S("[/") + tagName + _S("]");
	String::size_type posCurrent = 0;

	for(;;)
	{
		String::size_type posTagStart = text.find(tagStart,posCurrent);
		if(posTagStart == String::npos)
			return;
		String::size_type posTagEnd = text.find(tagEnd,posTagStart);
		if(posTagEnd == String::npos)
			return;

		String insideText = text.substr(posTagStart + tagStartLen, posTagEnd - posTagStart - tagStartLen);
		insideText.replace_all(from, to);

		text = text.substr(0, posTagStart + tagStartLen) + insideText + text.substr(posTagEnd);

		posCurrent = posTagStart + tagStartLen + insideText.length();
	}
}
*/
/*
void OMLManager::TransformNoParse(String& text, const String& tagName)
{
	String output;

	String tagStart = _S("[") + tagName;
	size_t tagStartLen = tagStart.length();
	String tagEnd = _S("[/") + tagName + _S("]");
	String::size_type posCurrent = 0;

	for(;;)
	{
		String::size_type posTagStart = Context::findToken(text, tagStart, posCurrent);
		if(posTagStart == String::npos)
			return;
		String::size_type posTagStartEnd = Context::findToken(text, _S("]"), posTagStart);
		if(posTagStartEnd == String::npos)
			return;

		// Qui non uso la findToken, altrimenti con
		// [code]alfa"beta[alfa][/code]
		// non troverebbe il tag di chiusura..
		//String::size_type posTagEnd = Context::findToken(text, tagEnd, posTagStartEnd);
		String::size_type posTagEnd = text.find(tagEnd, posTagStartEnd);
		if(posTagEnd == String::npos)
			return;

		String insideText = text.substr(posTagStartEnd + 1, posTagEnd - posTagStartEnd -1);

		insideText.replace_all(_S("["), _S("\\["));

		text = text.substr(0, posTagStartEnd+1) + insideText + text.substr(posTagEnd);

		posCurrent = posTagStart + tagStartLen + insideText.length() + 1;
	}
}
*/

//////////////////////////////////////////////////////////////////////

OS_NAMESPACE_END()

//////////////////////////////////////////////////////////////////////
