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

#ifndef _OS_HTML_CSS_H
#define _OS_HTML_CSS_H

//////////////////////////////////////////////////////////////////////

#define OS_CSS_ROOT						_S("os_root")

#define OS_CSS_FRAME					_S("osFrame")
#define OS_CSS_NORMALNAME				_S("osNormalName")

#define OS_CSS_EDITFRAME				_S("osEditFrame")
#define OS_CSS_EDITBLOCK				_S("osEditBlock")
#define OS_CSS_EDITTITLEBAR				_S("osEditTitleBar")

#define OS_CSS_LOGO						_S("osLogo")
#define OS_CSS_SUBTITLE					_S("os_subtitle")

#define OS_CSS_TABLE					_S("os_table")

#define OS_CSS_BLOCK					_S("osBlock")
#define OS_CSS_BLOCKTITLE				_S("osBlock_Title")
#define OS_CSS_BROWSERBAR				_S("osBrowserBar")
#define OS_CSS_ERROR					_S("osError")
#define OS_CSS_ERRORTITLE				_S("osError_Title")
#define OS_CSS_MESSAGE					_S("osMessage")
#define OS_CSS_MESSAGETITLE				_S("osMessage_Title")
#define OS_CSS_USERBAR					_S("osUserBar")

#define OS_CSS_FORM_LEFT				_S("os_label")
#define OS_CSS_FORM_RIGHT				_S("os_value")
#define OS_CSS_FORM_COMMANDS			_S("os_commands")

#define OS_CSS_PAGE_STATS_HEADER		_S("os_page_stats_header")
#define OS_CSS_PAGE_STATS_HEADER_TITLE	_S("os_page_stats_header_title")
#define OS_CSS_PAGE_STATS_BODY			_S("os_page_stats_body")

#define OS_CSS_POPUP_HEADER				_S("osPopup_Header")
#define OS_CSS_POPUP_BODY				_S("osPopup_Body")

#define OS_CSS_ROLLER_BODY				_S("osRoller_Body")
#define OS_CSS_ROLLER_TABLE				_S("osRoller_Table")
#define OS_CSS_ROLLER_HEADEROPEN		_S("osRoller_HeaderOpen")
#define OS_CSS_ROLLER_HEADEROPENOVER	_S("osRoller_HeaderOpenHover")
#define OS_CSS_ROLLER_HEADERCLOSE		_S("osRoller_HeaderClose")
#define OS_CSS_ROLLER_HEADERCLOSEHOVER	_S("osRoller_HeaderCloseHover")

#define OS_CSS_TAB_TABLE				_S("osTab_Table")
#define OS_CSS_TAB_LINK					_S("osTab_Link")
#define OS_CSS_TAB_LINKHOVER			_S("osTab_LinkHover")
#define OS_CSS_TAB_PAGE					_S("osTab_Page")

#define OS_CSS_TABCTRL_PAGE				_S("osTabCtrl_Page")
#define OS_CSS_TABCTRL_ACTIVEPAGE		_S("osTabCtrl_ActivePage")

#define OS_CSS_PAGER_LINK				_S("osPager_Link")
#define OS_CSS_PAGER_CURRENT			_S("osPager_Current")
#define OS_CSS_PAGER_PREVNEXT			_S("osPager_PrevNext")
#define OS_CSS_PAGER_FIRSTLAST			_S("osPager_FirstLast")

#define OS_CSS_EMOTICON					_S("osEmoticon")

#define OS_CSS_OML_B					_S("os_oml_b")
#define OS_CSS_OML_I					_S("os_oml_i")
#define OS_CSS_OML_U					_S("os_oml_u")
#define OS_CSS_OML_S					_S("os_oml_s")
#define OS_CSS_OML_O					_S("os_oml_o")
#define OS_CSS_OML_CODETOP			_S("os_oml_code_top")
#define OS_CSS_OML_CODEMAIN			_S("os_oml_code_main")
#define OS_CSS_OML_CODEMAINBOX		_S("os_oml_code_mainbox")
#define OS_CSS_OML_QUOTETOP			_S("os_oml_quote_top")
#define OS_CSS_OML_QUOTEMAIN			_S("os_oml_quote_main")
#define OS_CSS_OML_IMG				_S("os_oml_img")
#define OS_CSS_OML_DIV				_S("os_oml_div")
#define OS_CSS_OML_SPAN				_S("os_oml_span")
#define OS_CSS_OML_EMAIL				_S("os_oml_email")
#define OS_CSS_OML_SIZE				_S("os_oml_size")
#define OS_CSS_OML_COLOR				_S("os_oml_color")
#define OS_CSS_OML_FONT				_S("os_oml_font")
#define OS_CSS_OML_LEFT				_S("os_oml_left")
#define OS_CSS_OML_CENTER			_S("os_oml_center")
#define OS_CSS_OML_RIGHT				_S("os_oml_right")
#define OS_CSS_OML_JUSTIFY			_S("os_oml_justify")
#define OS_CSS_OML_LIST_UL			_S("os_oml_list_ul")
#define OS_CSS_OML_LIST_OL			_S("os_oml_list_ol")
#define OS_CSS_OML_LIST_LI			_S("os_oml_list_li")
#define OS_CSS_OML_USER				_S("os_oml_user")
#define OS_CSS_OML_ANCHOR			_S("os_oml_anchor")
#define OS_CSS_OML_GOTO				_S("os_oml_goto")

#define OS_CSS_OML_FLOAT				_S("os_oml_float")
#define OS_CSS_OML_LONG				_S("os_oml_long")
#define OS_CSS_OML_BOX				_S("os_oml_box")

#define OS_CSS_OML_INDENT			_S("os_oml_indent")
#define OS_CSS_OML_SUP				_S("os_oml_sup")
#define OS_CSS_OML_SUB				_S("os_oml_sub")
#define OS_CSS_OML_MARQUEE			_S("os_oml_marquee")
//#define OS_CSS_OML_ABBR				_S("os_oml_abbr")
//#define OS_CSS_OML_ACRONYM			_S("os_oml_acronym")
#define OS_CSS_OML_SPOILER			_S("os_oml_spoiler")
#define OS_CSS_OML_SPOILER_HEADER	_S("os_oml_spoiler_header")
#define OS_CSS_OML_SPOILER_BODY		_S("os_oml_spoiler_body")
#define OS_CSS_OML_WHISPER			_S("os_oml_whisper")
#define OS_CSS_OML_WHISPER_HEADER	_S("os_oml_whisper_header")
#define OS_CSS_OML_WHISPER_BODY		_S("os_oml_whisper_body")

#define OS_CSS_OML_TABLE				_S("os_oml_table")
#define OS_CSS_OML_TR				_S("os_oml_tr")
#define OS_CSS_OML_TH				_S("os_oml_th")
#define OS_CSS_OML_TD				_S("os_oml_td")

#define OS_CSS_OML_HIGHLIGHT			_S("os_oml_highlight")
#define OS_CSS_OML_TELETYPE			_S("os_oml_teletype")

#define OS_CSS_OML_HEADING1			_S("os_oml_heading1")
#define OS_CSS_OML_HEADING2			_S("os_oml_heading2")
#define OS_CSS_OML_HEADING3			_S("os_oml_heading3")
#define OS_CSS_OML_HEADING4			_S("os_oml_heading4")
#define OS_CSS_OML_HEADING5			_S("os_oml_heading5")
#define OS_CSS_OML_HEADING6			_S("os_oml_heading6")
#define OS_CSS_OML_PREFORMATTED		_S("os_oml_preformatted")
#define OS_CSS_OML_URL_INTERNAL		_S("os_oml_url_internal")
#define OS_CSS_OML_URL_EXTERNAL		_S("os_oml_url_external")
#define OS_CSS_OML_URL_OLNK			_S("os_oml_url_olnk")
#define OS_CSS_OML_URL_ED2K_FILE		_S("os_oml_url_ed2k_file")
#define OS_CSS_OML_URL_ED2K_SERVER	_S("os_oml_url_ed2k_server")

#define OS_CSS_FORUM_PANEL				_S("osForum_Panel")
#define OS_CSS_FORUM_COMMANDSLEFT		_S("osForum_CommandsLeft")
#define OS_CSS_FORUM_COMMANDSRIGHT		_S("osForum_CommandsRight")
#define OS_CSS_FORUM_COMMANDREPLY		_S("osForum_CommandReply")
#define OS_CSS_FORUM_COMMANDNEWFORUM	_S("osForum_CommandNewForum")
#define OS_CSS_FORUM_COMMANDNEWTOPIC	_S("osForum_CommandNewTopic")
#define OS_CSS_FORUM_AVATAR				_S("osForum_Avatar")
#define OS_CSS_FORUM_FORUMICON			_S("osForum_ForumIcon")
#define OS_CSS_FORUM_FORUMLINK			_S("osForum_ForumLink")
#define OS_CSS_FORUM_FORUMDESC			_S("osForum_ForumDesc")
#define OS_CSS_FORUM_TOPICICON			_S("osForum_TopicIcon")
#define OS_CSS_FORUM_TOPICDESC			_S("osForum_TopicDesc")
#define OS_CSS_FORUM_POSTHEADER			_S("osForum_PostHeader")
#define OS_CSS_FORUM_POSTDETAILS		_S("osForum_PostDetails")
#define OS_CSS_FORUM_POSTDETAILSICON	_S("osForum_PostDetailsIcon")
#define OS_CSS_FORUM_POSTUSER			_S("osForum_PostUser")
#define OS_CSS_FORUM_POSTCONTENT		_S("osForum_PostContent")
#define OS_CSS_FORUM_POSTCOMMANDS		_S("osForum_PostCommands")
#define OS_CSS_FORUM_POSTFOOTER			_S("osForum_PostFooter")
#define OS_CSS_FORUM_POSTPREVIEW		_S("osForum_PostPreview")

//////////////////////////////////////////////////////////////////////

#endif // _OS_HTML_CSS_H
