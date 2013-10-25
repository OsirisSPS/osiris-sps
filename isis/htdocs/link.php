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

function resolve($id)
{
	// Check local list
	$list = getOption("services.link.list");
	$lines = explode("\n", $list);
	foreach($lines as $line)
	{
		$values = explode(" ", $line);
		
		$regex = '/\b' . trim($values[0]) . '\b/i';
		$dest = trim($values[1]);
		
		//$xx = "/\\bhelp_(.*?)\\b/i";
		
		//trace("regex:" . $regex . ",dest:" . $dest . ",id:" . $id);
		//trace("regxx:" . $xx . ",dest:" . $dest . ",id:" . $id);
		
		if( ($regex != "") && ($dest != "") )
		{				
			//$regex = "/\\bhelp_(.*?)\\b/i";
			if (preg_match($regex, $id))
			{
				if( (getOptionBool("services.link.seo.enabled")) && (strpos($flags,"S")) )
				{
					return getOption("services.link.seo.path") . $id;
				}
				else
				{
					return preg_replace($regex, $dest, $id);
				}
			}
		}
	}
		
	// Check portals alias & id
	foreach(portalsList() as $portal)
	{
		$alias = getOptionPortal("isis.info.alias", $portal);
		if( ($id == $alias) || ($id == $portal) )
		{
			// Hit
			return getOption("server.virtual_path") . "?%2Fportals%2Fview%3Fportal=" . $portal;			
		}					
	}
	
	$url = getOption("services.link.redirect"); 
	$url = str_replace("{id}",$id,$url);
	return $url;
}

function main()
{
	// Main Code
	try
	{
		if(getOptionBool("services.link.enabled") == false)
			throw new Exception("Service not available.");
			
		$id = validateAlphabetic(getGetText("id"));
		if($id == null)
		{
			$page = validateAlphabetic(getGetText("page"));
			if($page != null)
				$id = $page;
			else
			{
				$help = validateID(getGetText("help"));
				if($help != null)
					$id = "help_" . $help;
				else
					$id = $_SERVER["QUERY_STRING"];
			}
		}
				
		//$id = strtolower($id);
				
		$url = resolve($id);
		
		//trace($url);
		
		if( ($url == null) || ($url == "") )
			throw new Exception("Unknown name");
		
		header( 'Location: '.$url);		
	}
	catch(Exception $e)
	{
		logMessage("error", $e->getMessage());
		
		xhtmlHeader(false);			
		message("error", $e->getMessage());			
		xhtmlFooter();
	}
}

// Calling main code
main();
?>