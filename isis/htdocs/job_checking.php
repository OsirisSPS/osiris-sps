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

defined( '_VALID_ISIS' ) or die( 'Direct Access to this location is not allowed.' );

function job_checking_run()
{
	$now = time();
	
	// Cleaning nodes zombie.
	// We need these because 'zombie' occur. I need to investigate why.
	foreach(portalsList() as $portal)
	{		
		foreach(portalFileList($portal, "nodes") as $file)
		{
					
			$howOld = $now - filemtime($file);
			
			$maxOld = getOptionDouble("tuning.request.timeout");
						
			if($howOld > $maxOld)
			{
				message("info", "Node '" . $file . "' deleted.");
				unlink($file);
			}					
			
		}
	}
	
	// Cleaning 'accepted' zombie.
	// Zombie occur when a job was accepted by Osiris node, but Osiris never send a response.
	foreach(portalsList() as $portal)
	{		
		foreach(portalFileList($portal, "accepteds") as $file)
		{
					
			$howOld = $now - filemtime($file);
			
			$maxOld = 30*60; // 30 minutes. Need an option?
			if($howOld > $maxOld)
			{
				message("info", "Accept '" . $file . "' deleted.");
				unlink($file);
			}					
			
		}
	}
	
	// Cleaning 'responses' zombie.
	// Zombie occur when a response was created, but nobody read it.
	foreach(portalsList() as $portal)
	{		
		foreach(portalFileList($portal, "responses") as $file)
		{					
			$howOld = $now - filemtime($file);
			
			$maxOld = 30*60; // 30 minutes. Need an option?
			if($howOld > $maxOld)
			{
				message("info", "Response '" . $file . "' deleted.");
				unlink($file);
			}				
		}
	}
	return true;
}

function job_checking_view()
{
	return "General checkings (killing file zombies, integrity checks)";
}

?>