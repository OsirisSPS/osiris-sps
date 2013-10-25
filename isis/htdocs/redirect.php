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

function main()
{	
	// Main Code
	try
	{
		// Init language system
		languageInit();	
		
		if(getOptionBool("services.redirect.enabled") == false)
			throw new Exception("Service not available.");
		
		$url = null;
		if(isset($_GET["url"]))
			$url = $_GET["url"];
		if( ($url == null) && (isset($_SERVER["QUERY_STRING"])) )
			$url = $_SERVER["QUERY_STRING"];
			
		$mode = null;
		if(isset($_GET["mode"]))
			$mode = $_GET["mode"];
			
		if($mode == null)
			$mode = "meta";
			
		if($mode == "direct")
		{
			header( 'Location: '.$url);
		}
		else if($mode == "meta")
		{
			xhtmlHeaderEx("<META http-equiv=\"refresh\" content=\"3;URL=" . htmlencode($url) . "\">", "Redirection...");
			
			//$msg = _LANG_META;
			//$msg = str_replace("{url}",htmlencode($url),$msg);
			echocr("<div id=\"is_waitbar_container\">");
			echocr("<div id=\"is_waitbar\" style=\"display:none;\">");
			echocr("<img id=\"is_waitbar_progress\" src=\"images/waitbar.gif\" title=\"Wait\" />");
			echocr("<div id=\"is_waitbar_message\">" . _LANG_META . "</div>");
			echocr("<div id=\"is_waitbar_url\"><a href=\"" . htmlencode($url) . "\">" . htmlencode($url) . "</a></div>");
			echocr("</div>");
			echocr("</div>");
			
			$js = "";
			//$js .= "$(\"#is_waitbar\").css(\"position\",\"absolute\");";
    	//$js .= "$(\"#is_waitbar\").css(\"top\", ( $(window).height() - $(\"#is_waitbar\").height() ) / 2+$(window).scrollTop() + \"px\");";
    	//$js .= "$(\"#is_waitbar\").css(\"left\", ( $(window).width() - $(\"#is_waitbar\").width() ) / 2+$(window).scrollLeft() + \"px\");";    
    	
    	$js .= "$(\"#is_waitbar\").fadeIn(1000);";
			
			echojs($js);		
			
			xhtmlFooter(false);
		}
	}
	catch(Exception $e)
	{
		xhtmlHeader();
		
		logMessage("error", $e->getMessage());
		
		message("error", $e->getMessage());
		
		xhtmlFooter();
	}
}

// Calling main code
main();
?>