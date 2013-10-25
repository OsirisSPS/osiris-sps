<?php
/**
* @package Isis
* @copyright Copyright (C) 2008 Kodeware. All rights reserved.
* @license http://www.gnu.org/copyleft/lgpl.html GNU/LGPL, see LICENSE.php
* This program is free software; you can redistribute it and/or modify it under 
* the terms of the GNU Lesser General Public License as published by the Free Software
* Foundation; either version 2 of the License, or (at your option) any later
* version.

* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

* You should have received a copy of the GNU Lesser General Public License along with
* this program; if not, write to the Free Software Foundation, Inc., 59 Temple
* Place - Suite 330, Boston, MA 02111-1307, USA, or go to
* http://www.gnu.org/copyleft/lesser.txt.
*/

// Set flag that this is a parent file
define( '_VALID_ISIS', 1 );

include("lib.php");	
include("common.php");	

function main()
{
	xhtmlHeader(false);
	
	try
	{	
		// Init language system
		languageInit();	

		$url = $_GET["url"];
		$action = $_GET["action"];
		
		
		if( ($url == null) && ($action == null) )
			$url = $_SERVER["QUERY_STRING"];
				
		$url = phpHttpParamDecode($url);
		
		$url = str_replace("<br />","",$url); // Added by Invision Bulletin Board to link too much long.
		$url = str_replace("osiris:? ","osiris:?",$url); // Space added by some bulletin board, for example older IPB.
		
		$data = osirisLinkParser($url);
		
		if( ($url == null) || ($action == _LANG_INVITELINK_SUBMIT) )
		{
			// Form mode
			if($url == null)
			{
				echocr("<div class=\"main\">");
				echocr(_LANG_INVITELINK_WIZARD);				
				echocr("<br />");
				echocr("<br />");
				echocr("<form method=\"get\" action=\"" . getPhpSelf() . "\">");
				echocr("<input type=\"url\" name=\"url\" size=\"100\"/>");
				echocr("<br />");
				echocr("<br />");
				echocr("<input type=\"submit\" name=\"action\" value=\""._LANG_INVITELINK_SUBMIT."\"/>");
				echocr("</form>");
				echocr("</div>");
			}
			else
			{			
				$inviteUrl = getOption("server.virtual_path") . getPhpSelf() . "?url=" . urlencode($url);
				echocr("<div class=\"main\">");
				echocr(_LANG_INVITELINK_SUGGEST);
				echocr("<br/>");
				echocr("<br/>");
				echocr("<code>");				
				echocr("<a href=\"" . $inviteUrl . "\">" . htmlencode($inviteUrl) . "</a>");
				echocr("</code>");
				echocr("</div>");
			}
		}	
		else if( ($data != null) && ($data["type"] == "portal") )
		{
			echocr("<div class=\"main\">");
			echocr(_LANG_INVITELINK_PORTAL_INFO);			
			echocr("<br /><br />");			
			echocr("<a href=\"".htmlencode($url)."\">".htmlencode($url)."</a>");
			echocr("</div>");
		}
		else if( ($data != null) && ($data["type"] == "isis") )
		{
			echocr("<div class=\"main\">");
			echocr(_LANG_INVITELINK_ISIS_INFO);			
			echocr("<br /><br />");			
			echocr("<a href=\"".htmlencode($url)."\">".htmlencode($url)."</a>");
			echocr("</div>");
		}
		else	
		{
			throw new Exception(_LANG_NOTALLOWEDURL);			
		}		
	}
	catch(Exception $e)
	{
		logMessage("error", $e->getMessage());
		
		message("error", $e->getMessage());
	}
	
	xhtmlFooter();
}

main();
?>