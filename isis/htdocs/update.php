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
	try
	{
		if(getOptionBool("services.update.enabled") == false)
			throw new Exception("Service not available.");
			
		$redirect = getOption("services.update.redirect"); 
		if($redirect != "")
		{
			$newUrl = $redirect . "?" . $_SERVER["QUERY_STRING"];
			header( 'Location: '. $newUrl);		
		}
		else
		{		
			xmlHeader();
			
			$version = getOption("services.update.version");
			$notes = getOption("services.update.notes");
					
			echo "<osiris version=\"".htmlencode($version)."\" url=\"http://www.osiris-sps.org/repository/osiris".htmlencode($version).".exe\" notes=\"".htmlencode($notes)."\" />";
		}
	}
	catch(Exception $e)
	{
		xmlHeader();
		echo "<error msg=\"".$e->getMessage()."\"/>\r\n";
		logMessage("error", $e->getMessage());
	}
}

// Calling main code
main();
?>
