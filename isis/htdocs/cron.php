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
	// Main Code
	$error = "";
	$open_lock = false;
	
	// Start xhtml output
	xhtmlHeader("","Background jobs");
	
	echocr("<ul>");
		
	try
	{
		$path_base = getOption("data.path");
		
		$path_lock = $path_base . "/background.lock";
		if(file_exists($path_lock))
		{
			throw new Exception("Another request already running.");
		}
		
		// Create lock file
		$file_lock = fopen($path_lock, 'w');
		fwrite($file_lock, "Isis Background Lock");
		fclose($file_lock);
		$open_lock = true;
		
		// Checking IP
		allowedIP();		
		
		$crons = explode("\n", getOption("jobs.cron"));
				
		foreach ($crons as $cron)
		{
			$cron = trim($cron);
			list($name, $when) = explode(";", $cron);
			
			$name = trim($name);
			$when = trim($when);
			
			if( ($name != "") && ($when != "") )
			{			
				echo "<li>Name: " . $name . ", when: " . $when . "</li>";
				
				$jobFilename = "job_" . $name . ".php";
				if(file_exists($jobFilename) == false)
					throw new Exception("Job '" . $name . "' unknown.");
				else
				{
					// Execute job
					try // This 'try' to avoid that a job stop all other jobs.
					{
						include($jobFilename);		
						
						$success = call_user_func("job_" . $name . "_run");
						
						if($success)
							message("info", $name . " done.");
						else
							message("error", $name . " failed.");
					}
					catch(Exception $e)
					{		
						logMessage("error", $e->getMessage());
			
						message("error", $e->getMessage());
					}				
				}
			}
		}
		
	}
	catch(Exception $e)
	{		
		logMessage("error", $e->getMessage());
		
		message("error", $e->getMessage());
	}
	
	// Kill lock file
	if($open_lock == true)
	unlink($path_lock);
	
	echocr("</ul>");
	
	// End xhtml output
	xhtmlFooter();
}


// Calling main code
main();

?>