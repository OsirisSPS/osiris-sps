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
		
		$url = getOption("home.redirect");
		if($url != "home.php")
		{
			header( 'Location: '.$url);
			die();
		}
		
		if(getOptionBool("home.pastelinks"))
		{
			$pasteLinks = getPostText("paste_links");
			if($pasteLinks != "")
			{
				pasteLinks($pasteLinks);		
				echocr("<br>");
			}
		}

		$portal = getGetText("portal");
		
		echo getOption("home.header");
		
		if($portal == "")
		{	
			if(getOptionBool("home.portalslist"))
			{				
				echocr("<h1>List of available portals:</h1>");
				// List
				foreach(portalsList() as $portal)
				{
					if(getOptionPortalBool("isis.info.hidden", $portal) == false)
					{
						portalBox($portal, false);
						flush();
					}
				}
			}
			
			if(getOptionBool("home.pastelinks"))
			{
				echocr("<br>");
				echocr("<br>");
			
				$html = "<span style=\"font-size:1.5em\">Paste links</span><br/>";
				$html .= "<form name=\"form1\" method=\"post\" action=\"" . getPostBackUrl() . "\">";
				$html .= "<textarea name=\"paste_links\" style=\"width:99%\" rows=4>" . htmlencode($pasteLinks) . "</textarea>";
				$html .= "<div style=\"text-align:right\"><input type=\"submit\" value=\"Paste links\"></div>";
				echo dataBox("pastelinks",$html);		
			}
		}
		else
		{
			if(getOptionPortalBool("isis.info.hidden", $portal) == false)
			{
				allowedPortal($portal);
			
				portalBox($portal, false);
			}
		}		
	}
	catch(Exception $e)
	{
		logMessage("error", $e->getMessage());
		
		message("error", $e->getMessage());
	}
	
	echo getOption("home.footer");
	
	echo getOption("layout.footer");
	
	xhtmlFooter();
}

main();
?>