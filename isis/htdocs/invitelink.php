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
include("classes/link.php");	

function main()
{
	xhtmlHeader(false);
	
	try
	{	
		// Init language system
		languageInit();	

		$url = null;
		$action = null;
		
		if(isset($_GET["url"]))
			$url = $_GET["url"];
			
		if(isset($_GET["action"]))
			$action = $_GET["action"];
		
		if( ($url == null) && ($action == null) )
		{
			$url = $_SERVER["QUERY_STRING"];
			
			// Strange: sometime PHP don't recognize the "url" param.  For example with this url, RFC valid:
			// https://www.osiris-sps.org/isis/invitelink.php?url=osiris%3A%2F%2F%7Cportal%7C01000001903517ECA4E4611A51A90F3151ACF09933E38BB0%7Cname%3DOsiris%2520Official%2520-%2520Encoding%2520Test%2520%25D0%2590%25D0%25B7%25D3%2599%25D1%2580%25D0%25B1%25D0%25B0%25D1%2598%25D2%25B9%25D0%25B0%25D0%25BD%2520%2528%25D0%2590%25D0%25B7%25D3%2599%25D1%2580%25D0%25B1%25D0%25B0%25D1%2598%25D2%25B9%25D0%25B0%25D0%25BD%2529%2520%2520%25E6%25BE%25B3%25E9%2596%2580%25E7%2589%25B9%25E5%2588%25AB%25E8%25A1%258C%25E6%2594%25BF%25E5%258D%2580%2520%253CBeta%253E%2520Gamma%2520%2527%2520Delta%2520%2522%2520Omega%2520%2521%2522%25C2%25A3%2524%2525%2526%252F%2528%2529%253D%2520-%2520%252521%252522%2525A3%252524%252525%252526%25252F%252528%252529%25253D%7Cdescription%3DOsiris%2520Official%2520-%2520Encoding%2520Test%2520%25D0%2590%25D0%25B7%25D3%2599%25D1%2580%25D0%25B1%25D0%25B0%25D1%2598%25D2%25B9%25D0%25B0%25D0%25BD%2520%2528%25D0%2590%25D0%25B7%25D3%2599%25D1%2580%25D0%25B1%25D0%25B0%25D1%2598%25D2%25B9%25D0%25B0%25D0%25BD%2529%2520%2520%25E6%25BE%25B3%25E9%2596%2580%25E7%2589%25B9%25E5%2588%25AB%25E8%25A1%258C%25E6%2594%25BF%25E5%258D%2580%2520%253CBeta%253E%2520Gamma%2520%2527%2520Delta%2520%2522%2520Omega%2520%2521%2522%25C2%25A3%2524%2525%2526%252F%2528%2529%253D%2520-%2520%252521%252522%2525A3%252524%252525%252526%25252F%252528%252529%25253D%7Cuser%3D%7C
			if(substr($url,0,4) == "url=")
			{
				$url = urldecode(substr($url,4));			
			}
		}
						
		$url = phpHttpParamDecode($url);
		
		$url = str_replace("<br />","",$url); // Added by Invision Bulletin Board to link too much long.
		$url = str_replace("osiris:// ","osiris://",$url); // Space added by some bulletin board, for example older IPB.
		$url = str_replace("osiris:? ","osiris:?",$url); // Space added by some bulletin board, for example older IPB.
		
		$link = new Link($url);
		
		// $url = urldecode($url); // Isis 0.10
		
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
		//else if (substr($url,0,17)=='osiris://|portal|')
		else if($link->getParam("type") == "portal")
		{
			echocr("<div class=\"main\">");
			echocr(_LANG_INVITELINK_PORTAL_INFO);			
			echocr("<br /><br /><br /><br />");			
			//$url = "ed2k://|file|[%E5%8A%A8%E7%94%BB][%E7%81%AB%E5%BD%B1%E5%BF%8D%E8%80%85%E7%96%BE%E9%A3%8E%E4%BC%A0][Naruto][253][%E7%8C%AA%E7%8C%AA%E5%AD%97%E5%B9%95%E7%BB%84][Jp_Cn][Xvid_BF][KPAP].rmvb|113559299|CC37803AF9684B19FB2FC7BDFB256512|/";
			echocr("<a style=\"font-size:1.4em;\" href=\"".htmlencode($link->export())."\">".htmlencode($link->getParam("name"))."</a>");
			//echocr("<a href=\"" . "ed2k://|file|[BDRip][1080p][AAC.Ita-Eng]Monster&co_by_Fle33iX.mkv|8500393358|0557F9A70A871B951B82EEAA114F504B|h=FG7VEIOTVTM6G4B54ZJXCO3NYQI3MWWS|/" . "\">test</a>");
			echocr("</div>");
		}
		//else if (substr($url,0,15)=='osiris://|isis|')
		else if($link->getParam("type") == "isis")
		{
			echocr("<div class=\"main\">");
			echocr(_LANG_INVITELINK_ISIS_INFO);			
			echocr("<br /><br />");			
			echocr("<a href=\"".htmlencode($link->export())."\">".htmlencode($link->getParam("name"))." - " . htmlencode($link->getParam("url")) . "</a>");
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