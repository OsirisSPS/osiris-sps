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

function portalIdToAlias($id)
{
	$alias = getOptionPortal("isis.info.alias", $id);
	if($alias != "")
		return strtolower($alias);
		
	return $id;
}

function seoAdjust($url)
{
	// Do SEO transformations
	if(getOptionBool("services.link.seo.enabled"))
	{
		$currentDomain = "http" . iif(getSSL(),"s") . "://" . getHttpHost();
		
		$seoPath = $currentDomain . getOption("services.link.seo.path");
		
		$list = getOption("services.link.list");
		$lines = explode("\n", $list);
		foreach($lines as $line)
		{
			$values = explode(" ", $line);			
						
			$key = "";
			if(count($values)>=1)
				$key = $values[0];
			
			$dest = "";
			if(count($values)>=2)
				$dest = $values[1];
				
			$flags = "";
			if(count($values)>=3)
				$flags = $values[2];			
						
			if( ($dest != "") && (strpos($flags,"S")) )
			{	
				$match = false;
				if($dest == $url)
					$match = true;
				else				
				{					
					if(strpos($url,$currentDomain) === 0)
					{
						if(substr($url, strlen($currentDomain)) == $dest)
							$match = true;
						if(substr($url, strlen($currentDomain)) == urldecode($dest))
							$match = true;
					}						
				}
				
				if($match)
				{					
					$url = $seoPath . $key . iif($key != "","/","");														
				}
			}
		}
		
		if(strpos($url,getOption("server.virtual_path") . "?") === 0)
		{
			$subUrl = urldecode(substr($url, strlen(getOption("server.virtual_path"))+1));			
			
			$match = false;
						
			// View, home only
			if($match == false)
			{
				$regex = "/^\/portals\/view\?portal=([a-zA-Z0-9]{48,48}?)$/";
				if(preg_match($regex, $subUrl, $matches))
				{	
					if($matches[1] == getOption("services.link.seo.home_portal"))
						$url = $seoPath;
					else			
						$url = $seoPath . "portals/" . portalIdToAlias($matches[1]) . "/";
					$match = true;
				}				
			}
			
			// View, with ID
			if($match == false)
			{
				$regex = "/^\/portals\/view\?id=([a-zA-Z0-9]{48,48}?)&portal=([a-zA-Z0-9]{48,48}?)$/";
				if(preg_match($regex, $subUrl, $matches))
				{	
					if($matches[2] == getOption("services.link.seo.home_portal"))
						$url = $seoPath . $matches[1] . "/";
					else			
						$url = $seoPath . "portals/" . portalIdToAlias($matches[2]) . "/" . $matches[1] . "/";
					$match = true;					
				}				
			}
			
			// Resources			
			if($match == false)
			{
				$regex = "/^\/main\/resources\?file=([a-zA-Z0-9]{48,48}?)&portal=([a-zA-Z0-9]{48,48}?)$/";
				if(preg_match($regex, $subUrl, $matches))
				{	
					// 0.12
					if($matches[2] == getOption("services.link.seo.home_portal"))
						$url = $seoPath . "resources/" . $matches[1] . "/";
					else			
						$url = $seoPath . "portals/" . portalIdToAlias($matches[2]) . "/resources/" . $matches[1] . "/";
					$match = true;					
				}				
			}			
			
			// Users
			if($match == false)
			{
				$regex = "/^\/portals\/user\?id=([a-zA-Z0-9]{48,48}?)&portal=([a-zA-Z0-9]{48,48}?)$/";
				if(preg_match($regex, $subUrl, $matches))
				{	
					// 0.12
					if($matches[2] == getOption("services.link.seo.home_portal"))
						$url = $seoPath . "users/" . $matches[1] . "/";
					else			
						$url = $seoPath . "portals/" . portalIdToAlias($matches[2]) . "/users/" . $matches[1] . "/";
					$match = true;					
				}				
			}
			
			// Other pages
			if($match == false)
			{
				//$pages = "[a-zA-Z0-9_]+?";
				$pages = "users|changes|latest_discussions|search|edit|about";
				$regex = "/^\/portals\/(" . $pages . ")\?portal=([a-zA-Z0-9]{48,48}?)$/";
				if(preg_match($regex, $subUrl, $matches))
				{	
					// 0.12
					if($matches[2] == getOption("services.link.seo.home_portal"))
						$url = $seoPath . $matches[1] . "/";
					else			
						$url = $seoPath . "portals/" . portalIdToAlias($matches[2]) . "/" . $matches[1] . "/";
					$match = true;					
				}				
			}			
			
			// HTDocs
			if($match == false)
			{
				$regex = "/^\/htdocs\/(.*?)\?portal=([A-Z0-9]*?)$/";
				if(preg_match($regex, $subUrl, $matches))
				{	
					if($matches[2] == getOption("services.link.seo.home_portal"))
						$url = $seoPath . "htdocs/" . $matches[1];
					else			
						$url = $seoPath . "portals/" . portalIdToAlias($matches[2]) . "/htdocs/" . $matches[1];
					$match = true;										
				}				
			}		
		}
		
	}
	
	return $url;
}

function newLinkAdjust($povId, $url, $urlSuffix, $format)
{	
	//echo $url . "<br/>";
	
	// Don't transform anchors.
	if(substr($url,0,1) == "#")
		return $url;	
		
	// TODO: do a regex match to check if is a valid url.

	// Hack: scriptaculus library don't want extra-parameters. So i redirect the osiris-library to isis-library.		
	// Maybe compatibility 0.13, if i remove Scriptaculus and use only JQuery.
	if(strpos($url,"scriptaculous.js") !== false)
		return "js/scriptaculous/scriptaculous.js";
	
	// Compatibility 0.13
	if(substr($url,0,16) == "http://127.0.0.1")
	{
		$pos = strpos($url,"/",16);
		$url = substr($url,$pos);
	}
		
	// If a ":" is found, it's an absolute url (: mean that a protocol are specified).
	// This because all Osiris url are relative.
	if(strpos($url,":") != 0)
		return $url;		
		
	// Add the suffix.
	$url = $urlSuffix . $url;	
	
	// Cleaning: remove /./
	$url = str_replace("/./","/",$url);
	
	// Cleaning: remove /../
	for(;;)
	{
		$pos = strpos($url,"/../");
		if($pos === false)
			break;
		$pos2 = strrpos(substr($url,0,$pos), '/');
		if($pos2 === false)
			break;
		
		$url = substr($url,0,$pos2) . substr($url,$pos+3);		
	}
		
	// All Osiris urls are relative, and start with a "/".	
	//if( (substr($url,0,1) != "/") && (substr($url,0,3) != "../") )
	if(substr($url,0,1) != "/")
		return $url;
		
	// From here, we know that URL follow Osiris standard.
		
	$url = htmldecode($url); // Osiris dump in html-encoded.	
	
	if($format == "rss/xml")
		$url = htmldecode($url);
	
	// Temporary, Osiris don't return portal and user reference in all url. Must be fixed from Osiris.	
	$portalIdPos = strpos($url,"portal=");
	if($portalIdPos === false)
	{	
		$questionPos = strpos($url,"?");
		if($questionPos === false)
			$url .= "?";
		else
			$url .= "&";
		$url .= "portal=" . $povId;			
	}
		
	// TODO: ..
	
	$url = rawurlencode($url);	
	
	$url = htmlencode($url);	
	
	if($format == "rss/xml")
		$url = htmlencode($url);
		
	$url = getOption("server.virtual_path") . "?" . $url;	
	
	$url = seoAdjust($url);
	
	return $url;
}

function newLinksAdjust($povId, $body, $before, $after, $urlSuffix = "", $format = "html")
{	
	$offsetPos = 0;
	for(;;)
	{
		$posStart = strpos($body, $before, $offsetPos);
		if($posStart === false)
			break;
		
		$posEnd = strpos($body,$after,$posStart+strlen($before));
		if($posEnd === false)
			break;
			
		//trace("offset:" . $offsetPos . ",start:" . $posStart . ",end:" . $posEnd);
			
		$url = substr($body, $posStart + strlen($before), $posEnd - $posStart - strlen($before));
		
		//trace("url: " . $url);
		
		$url = newLinkAdjust($povId, $url, $urlSuffix, $format);
		
		$body = substr($body, 0, $posStart + strlen($before)) . $url . substr($body, $posEnd);
		$offsetPos = $posStart + strlen($before) + strlen($url) + strlen($after);					
	}
	
	return $body;
}

function readResponse($path, &$headers, &$body)
{
	$file_size = filesize($path);
	$file_response = fopen($path, 'r');
	$document_content = fread($file_response, $file_size);
	fclose($file_response);
	return parseDocument($document_content, $headers, $body);											
}

function parseDocument($content, &$headers, &$body)
{	
	$pos_job_header = strpos($content,"\n");	
	if($pos_job_header === false)
		return 400;
		
	$pos_status = strpos($content,"\n", $pos_job_header + 1);
	if($pos_status === false)
		return 400;
		
	$pos_body = strpos($content,"\n\n", $pos_status + 1);
	if($pos_body === false)	
		return 400;
	
	$job_header = substr($content, 0, $pos_job_header);
	$http_result = substr($content, $pos_job_header + 1, $pos_status - ($pos_job_header+1));
	$header = substr($content, $pos_status + 1, $pos_body - ($pos_status+1));
	$body = substr($content,$pos_body+2);
		
	$headers_rows = explode("\n",$header);
	foreach($headers_rows as $key => $value)
	{
		$sepPos = strpos($value,":",0);
		if($sepPos === false)
		{
		}
		else
		{
			$headers[substr($value,0,$sepPos)] = trim(substr($value,$sepPos+1));
		}
	}
		
	if($http_result == "301")
		return 301;
	else if($http_result == "304")
		return 304;
	else if($http_result == "404")
		return 404;
	else if($http_result == "200")
		return 200;
	else 
	{
		return 400;
	}
}

function normalizeUserAgent($povId, $user_agent)
{
	if($user_agent == "")
		return "";
	// Cache files maybe user-agent specific, based on this selection to optimize resources.
	
	if(preg_match("/.*Windows CE.*/i",$user_agent))
		return "wince";		
	else if(preg_match("/.*Nintendo Wii.*/i",$user_agent))
		return "wii";		
	else if(preg_match("/.*PlayStation Portable.*/i",$user_agent))
		return "psp";
	else if(preg_match("/.*iPhone.*/i",$user_agent))
		return "iphone";
	else if(preg_match("/.*PLAYSTATION 3.*/i",$user_agent))
		return "ps3";
	else
		return ""; // Default
}

function normalizeLanguage($povID, $lang)
{
	// Cache files maybe language/culture specific, based on this selection to optimize resources.
	
	$opt = strtolower(getOptionPortal("isis.cache.languages", $povID));
	//$opt = strtolower("it.*,it-it;en.*,en-us;.*,it-it");
	$val = strtolower($lang);
	
	$items = explode(";",$opt);
	foreach($items as $item)
	{
		$pair = explode(",",$item);
		
		if(preg_match("/^" . $pair[0] . "$/", $val))
			return $pair[1];				
	}	
	
	return "en-us";
}

function main()
{
	/*
	$body = "<a href=\"/alfa/beta/../ciccio/gatto/../lupo/tito/./rito\"><a href=\"gatto\">";
	
	$newBody = newLinksAdjust("123123123",$body,"href=\"","\"");
	
	trace($body);
	trace($newBody);
		
	die();
	*/
	
	// Main Code
	try
	{
		commonHeader();
		
		// Set the number of seconds a script is allowed to run. 5 seconds of tollerance.		
		setTimeLimit(getOptionDouble("tuning.index.timeout") + 5);
		
		// Isis is not active until an administration password is not specified.
		if(getOption("server.acp.password") == "")
		{
			header( 'Location: checking.php' );
			die();
		}
		
		// Checking IP
		allowedIP();
		
		if(getOption("server.closed_message") != "")
			throw new Exception(getOption("server.closed_message"));
		
		// Collect request informations
		$ip = $_SERVER['REMOTE_ADDR'];
		$verb = $_SERVER["REQUEST_METHOD"];
		
		$force = false; // If true, always a peer request it's done.
		
		$url = "";
		if(isset($_GET["url"]))
		{
			$url = $_GET["url"];		
			if(isset($_GET["force"]))
				$force = true;
		}
		else
			$url = phpHttpParamDecode($_SERVER["QUERY_STRING"]);			
		$url = rawurldecode($url);		
				
		$client_user_agent = "";
		if(isset($_SERVER['HTTP_USER_AGENT']))
		$client_user_agent = phpHttpParamDecode($_SERVER['HTTP_USER_AGENT']);
		
		$client_culture = "en-US";
		// Culture. If exists cookie "os_culture" use, otherwise use HTTP_ACCEPT_LANGUAGE.
		if(isset($_COOKIE["os_culture"]))
		{
			$client_culture = $_COOKIE["os_culture"];
			//trace("cookie! " . $client_culture);
		}
		else
		{
			// Only the first culture in HTTP_ACCEPT_LANGUAGE are processed and sended to Osiris. This because Osiris manage the alternative cultures in a different way.
			if(isset($_SERVER['HTTP_ACCEPT_LANGUAGE']))
				$client_culture = extractStringUntil($_SERVER['HTTP_ACCEPT_LANGUAGE'],";,");		
			// trace("accept! " . $client_culture);
		}
		
		$client_if_modified_since = null;
		if(isset($_SERVER["HTTP_IF_MODIFIED_SINCE"]))
			$client_if_modified_since = convertDateFromHtml($_SERVER["HTTP_IF_MODIFIED_SINCE"]);
				
		$client_content_type = null;
		if(isset($_SERVER["CONTENT_TYPE"]))
			$client_content_type = $_SERVER["CONTENT_TYPE"];
				
		$client_post_data = "";
		if (is_array($_POST)) 
		{
			foreach ($_POST as $key => $value) 
			{ 
				if($client_post_data != "")
					$client_post_data .= "&";
				$client_post_data .= urlencode(phpHttpParamDecode($key)) . "=" . urlencode(phpHttpParamDecode($value));							
			}
		}
		
		$mode = "proxy";
		
		$http_result = 500;
		$processMessage = "";
		$error = "";		
		$document_headers = null;
		$document_body = null;
		
		if ($url == "")
		{
			$url = getOption("home.redirect");
			if($url == "")
				throw new Exception("You need to specify a default url with the option 'home.redirect'");
				
			header( 'Location: '.$url);
			die();
		}
			
		{
			$msg = "Realtime, guest requesting url: " . $url;
			if($client_if_modified_since != null)
				$msg .= ", already have the copy of " . date(DATE_RFC822, $client_if_modified_since);
			logMessage("notice",$msg);
		}
		
		
		$now = time();
		$path_base = getOption("data.path");
				
		// Extract PortalID
		$povId = "";
		$povIdPos = strpos($url,"portal=");
		if($povIdPos != 0)
		{
			$povId = substr($url,$povIdPos+7,40+40+1);			
		}
		
		allowedPov($povId);		
										
		$portal_path = $path_base . "/portal_" . $povId;	
		
		if(file_exists($portal_path) == false)
			throw new Exception("The url can't be reached.");
		
		if(getOptionPortal("isis.closed_message", $povId) != "")
			throw new Exception(getOptionPortal("isis.closed_message", $povId));
			
		// Guest password required?
		$guestPassword = getOptionPortal("isis.guest.password", $povId);
		if( ($guestPassword != ".*") && ($guestPassword != "*") )
		{			
			$guestCurrentPassword = "";
			$cookie = "isis_pwd_" . sha1($povId);
			if(isset($_COOKIE[$cookie]))
			{
				$guestCurrentPassword = $_COOKIE[$cookie];
			}
			
			if(matchOptionPortal("isis.guest.password", $guestCurrentPassword, $povId) == false)
				throw new Exception("Login required.");			
		}
				
		
			
		allowedValueListEx($url, getOptionPortal("isis.url.allowed", $povId), getOptionPortal("isis.url.blocked", $povId), "Url");
			
		allowedValueListEx($ip, getOptionPortal("isis.guest.ips.allowed", $povId), getOptionPortal("isis.guest.ips.blocked", $povId), "IP");
				
		$portal_redirect = getOptionPortal("isis.redirect", $povId); 
		if($portal_redirect != "")
		{
			header( 'Location: '.$portal_redirect);
			die();
		}			
		else
		{				
			$cache_enabled = getOptionPortal("isis.cache.enabled", $povId);
			$cache_last_modification = null;
						
			// Normalize language
			if($cache_enabled == true)
				$client_culture = normalizeLanguage($povId, $client_culture);
			
			$cache_user_agent = normalizeUserAgent($povId, $client_user_agent);			
			$cache_key = $verb . "_" . $url . "_" . $cache_user_agent . "_" . $client_culture . "_" . $client_content_type . "_" . $client_post_data;
			$cache_id = sha1($cache_key);
			$cache_exists = false;
		
			$http_result = 500;	// Internal Error
			$ask_to_peers = getOptionPortalBool("isis.realtime.enabled", $povId);
			
			if($cache_enabled)
			{							
				$path_cache_info = $portal_path . "/cache_info/" . $cache_id;
				$path_cache_data = $portal_path . "/cache_data/" . $cache_id;
								
				if(file_exists($path_cache_info))
				{
					$cache_exists = true;
					
					$cache_header = "";
					$data_cache_info = file_get_contents($path_cache_info);			
					list($cache_header, $cache_last_check, $cache_last_modification) = explode("\n", $data_cache_info);			
					
					if(regexMatch(getOptionPortal("isis.cache.url.always", $povId), $url))
					{
						$ask_to_peers = false;
						$processMessage = "Cache, Always used";
					}
					else if(regexMatch(getOptionPortal("isis.cache.url.never", $povId), $url))
					{
						$ask_to_peers = true;
						$processMessage = "Never use cache";
					}					
					else
					{
						$ask_to_peers = needAskToNode($povId, $cache_last_modification, $cache_last_check);
						
						if($ask_to_peers == false)
						{
							$processMessage = "Cache, Reccomended"; // Cache reccomended by various params.
						}
					}
				}
			}
			
			if($force)
			{
				$ask_to_peers = true;
				$processMessage = "Forced request";				
			}
			
			$nOsirisNodes = -1;
			if($ask_to_peers == true)
			{
				$nOsirisNodes = getPortalNodesCount($povId);
								
				if($nOsirisNodes == 0)
				{
					if(file_exists($portal_path . "/last_node"))
					{
						$delay = time() - filemtime($portal_path . "/last_node");
						
						if($delay > getOptionDouble("tuning.request.timeout"))
						{
							$ask_to_peers = false;		
							$processMessage = "Cache, No peers and latest is too old.";
						}		
					}
					else
						$ask_to_peers = false;
				}			
			}
						
			if($ask_to_peers == true)
			{		
				$path_base_incomings = $portal_path . "/incomings/";
				$path_base_requests_realtime = $portal_path . "/requests_realtime/";
				$path_base_responses = $portal_path . "/responses/";
						
				// We can't simply use "cache_id" as "request_id", because the response maybe a http 304 for a client, or an http 200 for another, for example.
				//$request_id = sha1(time() . $cache_id);
				$request_id = randomId();
				
				logMessage("notice","Realtime, waiting for job response ( " . $request_id . " )");
				
				$path_incoming = $path_base_incomings . $request_id;		
				
				$request_if_modified_since = "";
				if($cache_enabled == false)
				{
					// Se la cache è disabilitata, giro le info del client
					if(isset($_SERVER["HTTP_IF_MODIFIED_SINCE"]))
						$request_if_modified_since = $_SERVER["HTTP_IF_MODIFIED_SINCE"];			
					//trace($request_if_modified_since);
				}
				else
				{
					// Altrimenti invio le informazioni della cache di Isis
					if($cache_last_modification)
					{						
						$request_if_modified_since = convertDateToHtml($cache_last_modification);						
					}
				}
				
				// Create "request" file				
				$priority = "realtime";
				
				$file_request = fopen($path_incoming, 'w');
				writeFileHeader($file_request, "request");
				fwrite($file_request, $priority . "\n");
				fwrite($file_request, $verb . "\n");
				fwrite($file_request, $url . "\n");
				fwrite($file_request, $client_user_agent. "\n");
				fwrite($file_request, $client_culture . "\n");
				fwrite($file_request, $request_if_modified_since . "\n");
				fwrite($file_request, $client_content_type . "\n");
				fwrite($file_request, $client_post_data . "\n");
				fclose($file_request);	
				
				// Atomic rename
				$path_request = $path_base_requests_realtime . $request_id;
				rename($path_incoming, $path_request);
				
				$path_response = $path_base_responses . $request_id;
				
				// Wait a job is assigned.
				$wait_step = getOptionDouble("tuning.accept.waitstep");
				$wait_timeout = getOptionDouble("tuning.accept.timeout");
				$wait_total = 0;
				$waitForAnswer = false;
				for(;;)
				{				
					if(file_exists($path_request) == false)				
					{					
						$waitForAnswer = true;
						break;
					}
					else
					{
						usleep(secToMicrosec($wait_step));
						$wait_total += $wait_step;
						$wait_step *= 1.5; // URGENT TEMP
						if($wait_total > $wait_timeout) 
						{
							logMessage("notice","Timeout during waiting for a realtime job accept ( " . $request_id . " )");
							break;					
						}
						
						// If connection was closed from guest, exit now.
						if(connection_aborted())
						{
							logMessage("notice","Connection aborted during waiting for a realtime job accept ( " . $request_id . " )");
							break;					
						}
					}
				}	
				
				if($waitForAnswer == true)
				{
					// Wait an answer for the job.
					$wait_step = getOptionDouble("tuning.answer.waitstep");
					$wait_timeout = getOptionDouble("tuning.answer.timeout");
					$wait_total = 0;
					for(;;)
					{				
						if(file_exists($path_response))				
						{					
							$http_result = readResponse($path_response, $document_headers, $document_body);							
							break;
						}
						else
						{
							usleep(secToMicrosec($wait_step));
							$wait_total += $wait_step;
							$wait_step *= 1.5; // URGENT TEMP
							if($wait_total > $wait_timeout) 
							{
								logMessage("notice","Timeout during waiting for a realtime job answer ( " . $request_id . " )");
								break;					
							}
							
							// Here we don't test if connection is aborted.
							// Because, at least, cache will be updated.
						}
					}	
				}				
				
				// In every case, if still exists, delete the request.
				if(file_exists($path_request))
					unlink($path_request);
								
				if($http_result != 500)
				{	
					if( ($http_result == 304) && ($cache_last_modification != null) )
						$document_last_modification = $cache_last_modification;
					else if(isset($document_headers['Last-Modified']))					
						$document_last_modification = convertDateFromHtml($document_headers['Last-Modified']);
					else
						$document_last_modification = null;
					
					
					// If "Last-Modified" header is missing, normally document doesn't need to be stored in cache.
					// But if "cache_realtime_force" are enabled, cache file are builded.
					if( ($document_last_modification == null) && (getOptionPortalBool("isis.cache.store.dynamic", $povId)) )
					{
						$document_last_modification = $now;						
					}
					
					if($cache_enabled == true)
					{						
						
						// The browser have a old cache copy. Isis have a newest cache copy. Isis do the request. Osiris return a 304 (because request is based on Isis cache data).
						// Without this, the browser receive the 304 and maintain they old cache copy.
						if( ($http_result == 304) && ($cache_exists) && ($client_if_modified_since != null) && ($cache_last_modification != null) && ($client_if_modified_since != $cache_last_modification) )
						{
							$http_result = 500;
						}
						
						// Like above, the browser don't have a cache copy. Isis have. Osiris return a 304.
						// Without this, the browser receive the 304 and nothing are showed.
						if( ($http_result == 304) && ($client_if_modified_since == null) )
						{
							$http_result = 500;
						}
						
						if($http_result != 500)
						{						
							$cacheable = ($document_last_modification != null);
						
							if($cacheable)
							{
								$last_check = time();
								$file_cache_info = fopen($path_cache_info, 'w');
								writeFileHeader($file_cache_info, "cache info");
								
								fwrite($file_cache_info, $last_check . "\n");
								fwrite($file_cache_info, $document_last_modification . "\n");							
								
								fwrite($file_cache_info, $verb . "\n");
								fwrite($file_cache_info, $url . "\n");
								fwrite($file_cache_info, $cache_user_agent . "\n");
								fwrite($file_cache_info, $client_culture . "\n");
								fwrite($file_cache_info, $client_content_type . "\n");
								fwrite($file_cache_info, $client_post_data . "\n");							
								
								fclose($file_cache_info);	
								
								// Move "response" to "data"
								if ($http_result != 304) // 0.3
								{
									if(file_exists($path_cache_data))
										unlink($path_cache_data);
									rename($path_response,$path_cache_data);							
								}
								else
								{
									// We update only the info, not the data.
									unlink($path_response);
								}
							}
							else
							{
								// Maybe a url that are previously cached, but not from now.
								if(file_exists($path_cache_info))
									unlink($path_cache_info);
								if(file_exists($path_cache_data))
									unlink($path_cache_data);							
							}
						}
					}
										
					if( ($http_result == 200) && 
					    ($client_if_modified_since != null) && 
					    ($document_last_modification != null) &&
					    ($client_if_modified_since == $document_last_modification) 
					    )
					{
						// A request to peers are made, but client version still up-to-date.
						$http_result = 304;												
					}
				}

				// In every case, if still exists, delete the response.
				if(file_exists($path_response))
					unlink($path_response);
			}			
						
			if($http_result == 500)
			{	
				if($cache_exists == true)
				{
					// No peers need to be contacted, or no peers give a valid response (cached version is better than nothing)
					//if( ($client_if_modified_since != null) && ($client_if_modified_since<=$cache_last_modification) )
					if( ($client_if_modified_since != null) && ($client_if_modified_since == $cache_last_modification) )
					{
						// Valid cache available, and client version of document is up-to-date.						
						$http_result = 304;
					}
					else
					{
						$http_result = readResponse($path_cache_data, $document_headers, $document_body);
								
						if( ($http_result != 200) && ($http_result != 301) )
						{
							$http_result = 500; // Unexpected result, only http-200 or 301 need to be stored in cache.
						}
						else
						{
							//$infoMessage = "Cache: " . relativeTime($cache_last_modification);
						}
					}
				}
				else
				{				
					if($ask_to_peers == true)
					{
						$processMessage .= ", No cache available and no peers are available";
						throw new Exception("No data available and no peers are available. Please retry later.");
					}
					else
					{
						$processMessage .= ", No cache available. Unknown reason.";
						throw new Exception("No data available. Please retry later.");
					}
				}
			}
			else
			{
				$processMessage .= ", Realtime";				
			}
		}
				
		logMessage("notice","Processing: " . $processMessage . "; HTTP Answer: " . $http_result . "; Url: " . $url);
	}
	catch(Exception $e)
	{
		$http_result=500;
		$error = $e->getMessage();
	}
	
	// disconnectDatabase($database);
	
	if($mode == "redirect")
	{		
		$redirectUrl = "http://31.174.42.159:8181" . $url;
		// or, better
		//$redirectUrl = "http://31.174.42.159:8181" . $request_id;		
		
		$redirect_document_mimetype = "html";
		if($redirect_document_mimetype == "html")
		{
			// Here, if a document is an HTML page, we render a "frameset" layout, 
			echocr("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Frameset//EN\" \"http://www.w3.org/TR/html4/frameset.dtd\">");
			echocr("<HTML>");
			echocr("<HEAD>");
			echocr("<TITLE>Isis</TITLE>");
			echocr("</HEAD>");
			/*
			echocr("<script type=\"text/javascript\">");
			echocr("function test()");
			echocr("{");
			echocr("alert('test');");
			echocr("}");
			echocr("</script>");
			*/
			echocr("<FRAMESET rows=\"100%, 10%\">");
			echocr("	<FRAME id=\"osiris\" src=\"redirect_wait.php?url=" . urlencode($redirectUrl) . "\">");
			//echocr("	<FRAME src=\"redirect_control.php\">");
			echocr("</FRAMESET>");
			echocr("<NOFRAMES>");
			echocr("Browser with frame support is required.");
			echocr("</NOFRAMES>");
			echocr("</FRAMESET>");
			echocr("</HTML>");
		}
		else
		{
			// Otherwise it's a direct redirect.
			header("Location: " . $redirectUrl);
		}
	}
	else
	{			
		// Client response					
		if($http_result == 304) // Not Modified
		{
			header("HTTP/1.0 304 Not Modified");
		}
		else if ( ($http_result == 200) || ($http_result == 301) )
		{
			// Here we do different transformations. Are done here every time to allow dynamic content, for that this transformations are not saved as cache.
			// TODO: All this transformations must be done on Osiris, to implement "redirect" mode.
			
			$processType = "none";
						
			foreach ($document_headers as $key => $value) 
			{	
				if($key == "Date")
				{
				}
				else if($key == "Server")
				{
				}
				else if($key == "Allow")
				{
				}
				else if($key == "Last-Modified")
				{
				}
				else if($key == "Content-Disposition")
				{
				}	
				else if($key == "Content-Length")
				{
					// TODO, Content-length must be adjusted.
					$value = "";
				}
				else if($key == "Content-Type")
				{					
					logMessage("notice", "CT:" . $url . " , " . $value);
					if(strpos($value,"text/html") !== false)
						$processType = "html";
					else if(strpos($value,"text/css") !== false)
						$processType = "css";
					else if(strpos($value,"text/javascript") !== false)
						$processType = "js";
					else if(strpos($value,"application/rss+xml") !== false)
						$processType = "rss/xml";
				}
				else if($key == "Connection")
				{
				}
				else if($key == "Location")
				{
					// Redirect (301) must be adjusted.
					$value = seoAdjust(getOption("server.virtual_path") . "?" . $value);
				}		
				else if( ($key == "Set-Cookie") && (substr($value,0,11) == "os_culture=") )
				{					
					// Cookie Culture are the only accepted cookie.
				}
				else
				{
					$value = "";
				}
				
				if($value != "")
					header($key . ": " . $value);
			}
			
			$body = $document_body;
			
			if( ($processType == "html") || ($processType == "rss/xml") )
			{
				// Transformations
				if(getOptionPortal("isis.home", $povId) == "")
					$body = str_replace("href=\"/main/home\"","href=\"" . getOption("server.virtual_path") . "\"",$body);
				else
					$body = str_replace("href=\"/main/home\"","href=\"" . getOptionPortal("isis.home", $povId) . "\"",$body); 
				
				// TODO: Here, hacks... Maybe Osiris return urls in form {url:..} ?
				$body = newLinksAdjust($povId, $body, "src=\"", "\"", "", $processType);
				$body = newLinksAdjust($povId, $body, "href=\"", "\"", "", $processType);
				$body = newLinksAdjust($povId, $body, "action=\"", "\"", "", $processType);
				$body = newLinksAdjust($povId, $body, "url(\"", "\")", "", $processType);					
				
				$body = newLinksAdjust($povId, $body, "value=\"", "\"", "", $processType);
				
				// Replaces								
				$customAreaSystem = strpos($body, "<!--{__Isis_Area_Custom_System__}-->");
				if($customAreaSystem !== false)
				{
					$body = str_replace_once("<!--{__Isis_Area_Custom_System__}-->", getOptionPortal("isis.output.area.systembar", $povId), $body);
					$body = str_replace("<!--{__Isis_Area_Custom_System__}-->", "", $body);
				}
				else
				{
					$body = str_replace_once("<!--{__Isis_Area_System__}-->", getOptionPortal("isis.output.area.systembar", $povId), $body);
					$body = str_replace("<!--{__Isis_Area_System__}-->", "", $body);
				}
				$customAreaFooter = strpos($body, "<!--{__Isis_Area_Custom_Footer__}-->");
				if($customAreaFooter !== false)
				{
					$body = str_replace_once("<!--{__Isis_Area_Custom_Footer__}-->", getOptionPortal("isis.output.area.footer", $povId), $body);
					$body = str_replace("<!--{__Isis_Area_Custom_Footer__}-->", "", $body);
				}
				else
				{
					$body = str_replace_once("<!--{__Isis_Area_Footer__}-->", getOptionPortal("isis.output.area.footer", $povId), $body);
					$body = str_replace("<!--{__Isis_Area_Footer__}-->", "", $body);
				}				
				
				// Isis ACP variables
				$body = str_replace("{@isis_info}", $processMessage, $body);
				$body = str_replace("{@isis_path}", getOption("server.virtual_path"), $body);
				
				$list = getOptionPortal("isis.output.replaces",$povId);
				$lines = explode("\n", $list);
				foreach($lines as $line)
				{
					$line2 = trim($line);
					if( ($line2 != "") && (substr($line2,0,1) != "#") && (substr($line2,0,2) != "//") )
					{
						//$values = explode("|", $line);
						$values = explode("|", $line2);
							
						//$regex = '/\b' . trim($values[0]) . '\b/i';
						$regex = '/' . trim($values[0]) . '/';
						
						$replace = "";
						if(count($values)>=2)
							$replace = trim($values[1]);
							
						if (preg_match($regex, $body))
						{
							$body = preg_replace($regex, $replace, $body);
						}
						else
						{							
						}
					}
				}
				
				// Se è sotto SSL, converte i link assoluti dello stesso dominio di Isis da http:// a https://.
				// Necessario per permettere nel portale ufficiale di Osiris, di mostrare correttamente Anubis come <iframe> (senza errori di cross-domain).
				if(getSSL())
					$body = str_replace("http://" . getHttpHost(), "https://" . getHttpHost(), $body);
				else
					$body = str_replace("https://" . getHttpHost(), "http://" . getHttpHost(), $body);
			}
			
			if ($processType == "html")
			{
				$body = str_replace("</body>", getOption("layout.footer") . "</body>",$body);
			}
			
			if ($processType == "rss/xml")
			{
				$body = newLinksAdjust($povId, $body, "<link>", "</link>", "", $processType);
			}
						
			if( ($processType == "css") || ($processType == "html") )
			{	
				// Transformations
				// Hack: Osiris must be return, in CSS, the already-absolute urls.
				
				$pos = strrpos($url, "/");
				if($pos !== false)
				{
						$body = newLinksAdjust($povId, $body, "url(\"", "\")", "", $processType);				
						$body = newLinksAdjust($povId, $body, "url('", "')", "", $processType);				
												
						// jQuery Skin don't use the " in url()
						$body = newLinksAdjust($povId, $body, "url(", ")", "", $processType);									
				}									
			}
			
			if ($processType == "js")
			{
				$body = newLinksAdjust($povId, $body, "Osiris.adjustStaticUrl(\"", "\")", "", $processType);
				if(strpos($url,"osiris.js"))
				{
					$body = str_replace("// {FullUrl Isis Adjustment}","fullUrl=\"" . getOption("server.virtual_path") . "?\" + encodeURIComponent(url);",$body);
				}					
			}

			
			echo $body;
		}	
		else
		{
			if($http_result == 404)
				$error = "Document not found. (404)";
				
			if($error == "")
				$error = "Error code: " . $http_result;
				
			// Start xhtml output
			//xhtmlHeader();
			header('HTTP/1.0 404 Not Found');
			xhtmlHeaderEx("<script type=\"text/javascript\" src=\"" . getOption("server.virtual_path") . "js/main.js\"></script>", "");
			
			/*
			echocr("<div style=\"text-align:center;\">");
			echocr("	<div style=\"width:auto;padding:10px;margin:10px;background-color:#FFFFFF;\">");
			echocr("		<div style=\"padding:10px;background-color:#F0F0F0; border:1px solid #BBBBBB;font-size:3em;\">" . getOption("server.name") . "</div>");		
			echocr("		<div style=\"margin:30px;font-size:1.5em;\">" . bbencode($error) . "</div>");
			echocr("	</div>");
			echocr("<br />");
			echocr("<br />");
			
			echocr("</div>");
			*/
			echocr("<div id=\"is_waitbar_container\">");
			echocr("<div id=\"is_waitbar\" style=\"display:none;\" class=\"is_waitbar\">");
			echocr("		<div style=\"padding:20px;border-bottom:1px dotted #DDDDDD;font-size:3em;\">" . getOption("server.name") . "</div>");					
			
			if($error == "Login required.")
			{
				echocr("		<div style=\"margin:30px;font-size:1.5em;\">" . bbencode(getOptionPortal("isis.guest.message", $povId)) . "</div>");
				echocr("<br />");
				echocr("<br />");
				echocr("<input id='password' type='password'/>");
				echocr("<input style=\"width:120px\" type=button value=\"Login\" onClick=\"javascript:doGuestPassword('" . htmlencode(sha1($povId)) . "');\">");
			}
			else
			{
				echocr("		<div style=\"margin:30px;font-size:1.5em;\">" . bbencode($error) . "</div>");
				echocr("<br />");
				echocr("<br />");
				echocr("<input style=\"width:120px\" TYPE=\"button\" VALUE=\"Retry\" onClick='parent.location=\"javascript:location.reload()\"'>");
				echocr("<input style=\"width:120px\" type=button value=\"Back\" onClick=\"history.go(-1)\">");
			}
			echocr("</div>");
			echocr("</div>");
			
			echojs("$(\"#is_waitbar\").fadeIn(1000);");
			
			
			echo getOption("layout.footer");
					
			// End xhtml output
			xhtmlFooter();
		}	
	}
}


// Calling main code
main();

?>