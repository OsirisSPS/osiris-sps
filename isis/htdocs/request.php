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
	$now = time();
	$pageId = randomId();
	
	// Set the number of seconds a script is allowed to run. 5 seconds of tollerance.
	setTimeLimit(getOptionDouble("tuning.request.timeout") + 5);
	
	logMessage("notice", "Incoming request from Osiris node ( PID:" . $pageId . " )");
	
	$path_node = "";
	
	$error_code = 1; // 0 = No error; 1 = Fatal error; 2 = Error, retry later; 3 = Retry now (ex. timeout) 4 = System update required.
	$error_desc = "";
	
	$request_header = null;
	$request_id = null;
	$request_priority = null;
	$request_verb = null;
	$request_url = null;
	$request_user_agent = null;
	$request_culture = null;
	$request_http_if_modified_since = null;
	
	xmlHeader();
	
	try
	{		
		if($_SERVER["REQUEST_METHOD"] != "POST")
			throw new Exception("Invalid verb.");
		
		// Reading variables		
		$ip = $_SERVER['REMOTE_ADDR'];
		$userAgent = $_SERVER['HTTP_USER_AGENT'];
		$portal = $_POST["portal"];
		$user = $_POST["user"];
		$password = $_POST["password"];
		$system = $_POST["system"];		
					
		allowedPortal($portal, $user);
				
		$id = composePovId($portal, $user);
						
		commonPeerChecks($id, $password);
				
		$error_code = 2; // From here, an exception will be not a fatal error.
		
		// Checking system
		$settings_last_modification = getOption("settings.last_modification");
		
		if($system != $settings_last_modification)
		{
			$error_code = 4; // System update required.
			throw new Exception("System update required.");
		}
				
		$path_base = getOption("data.path");
		$path_portal = $path_base . "/portal_" . $id;
		
		$path_base_requests_realtime = $path_portal . "/requests_realtime/";
		$path_base_requests_background = $path_portal . "/requests_background/";
		$path_base_accepteds = $path_portal . "/accepteds/";
		
		$path_node = $path_portal . "/nodes/" . $pageId;
		register_shutdown_function('ending', $path_node);
		
		$file_node = fopen($path_node, 'w');
		writeFileHeader($file_node, "node");
		fwrite($file_node, $ip . "\n");
		fwrite($file_node, $userAgent . "\n");
		fwrite($file_node, $system . "\n");
		fclose($file_node);	
			
		$request_id = null;
		$path_accepted = null;
		
		// Waiting for a "request" file.		
		$wait_total = 0;
		$wait_step = getOptionDouble("tuning.request.waitstep");
		$wait_timeout = getOptionDouble("tuning.request.timeout");
					
		for(;;)
		{
			if ($dh = opendir($path_base_requests_realtime))
			{			
				$file = null;
				while (false !== ($file = readdir($dh))) 
				{
					if ($file != "." && $file != ".." && $file != "index.html" && $file != ".htaccess") 
					{
						break;
					}
				}
				closedir($dh);				
								
				if($file != null)
				{	
					// Atomic rename
					$request_id = $file;
					$path_request = $path_base_requests_realtime . $file;
					$path_accepted = $path_base_accepteds . $file;	
					if(file_exists($path_accepted))
						unlink($path_accepted);
					rename($path_request, $path_accepted);					
					break;
				}
									
				usleep(secToMicrosec($wait_step));
				$wait_total += $wait_step;
				if($wait_total > $wait_timeout) 
				{						
					break;			
				}
				
				// If connection was closed from Osiris, exit now.
				if(connection_aborted())
				{
					logMessage("notice","Connection aborted. ( PID:" . $pageId . " )");
					break;					
				}
			}
		}

		if($path_accepted == null)
		{
			$error_code = 3; // Timeout
			throw new Exception("Incoming request timeout, no jobs found. ( PID:" . $pageId . " )");
		}
		else
		{
			logMessage("notice","Sending job ( Job:" . $request_id . " , PID:" . $pageId . " )");
			$data_accepted = file_get_contents($path_accepted);	
			list($request_header, $request_priority, $request_verb, $request_url, $request_user_agent, $request_culture, $request_http_if_modified_since, $request_content_type, $request_post_data) = explode("\n", $data_accepted);			
							
			$error_code = 0; // No error
		}		
	}
	catch(Exception $e)
	{
		$error_desc = htmlencode($e->getMessage());
		if($error_code != 4)
		{
			if($error_code == 1)
				logMessage("warning", $e->getMessage());
			else
				logMessage("notice", $e->getMessage());
		}
	}
			
	$output = null;
	if($error_code != 0)
	{
		$output = "<isis type=\"request\" error_code=\"$error_code\" error_desc=\"" . htmlencode($error_desc) . "\"></isis>\r\n";	
	}
	else
	{
		$output = "<isis type=\"request\" error_code=\"$error_code\" portal=\"" . htmlencode($portal) ."\" user=\"" . htmlencode($user) ."\" isis_version=\"" . getOption("product.version") . "\" request_id=\"" . htmlencode($request_id) . "\" priority=\"" . htmlencode($request_priority) . "\" verb=\"" . htmlencode($request_verb) . "\" url=\"" . htmlencode($request_url) . "\" user_agent=\"" . htmlencode($request_user_agent) . "\" culture=\"" . htmlencode($request_culture) . "\" if_modified_since=\"" . htmlencode($request_http_if_modified_since) . "\" content_type=\"" . htmlencode($request_content_type) . "\" post_data=\"" . htmlencode($request_post_data) . "\"></isis>\r\n";		
	}

	echo $output;		
	
	ending($path_node); // register_shutdown_function() in some cases, don't work.
}

function ending($path_node)
{
	// This is our shutdown function, in here we can do any last operations before the script is complete.
	
	$path_node_last = substr($path_node,0,strpos($path_node,"/nodes/")) . "/last_node";
	
	renameSave($path_node, $path_node_last);	
}

// Calling main code
main();
?>