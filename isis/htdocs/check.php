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

function detectTOR()
{
	$serverIP = $_SERVER['SERVER_ADDR'];
	$requestIP = $_SERVER['REMOTE_ADDR'];
	
	// Check if is a TOR Exit Node		
	$torListPath = getOption("data.path") . "/torbulkexitlist.txt";	
	
	$needUpdate = false;
	if(file_exists($torListPath) == false)
		$needUpdate = true;
	else
	{
		$howOld = time() - filemtime($torListPath);
			
		$maxOld = getOptionDouble("services.check.tor.update_every");
						
		if($howOld > $maxOld)
			$needUpdate = true;
	}
	
	$torData = "";
	if($needUpdate == true)
	{
		$updateUrl = getOption("services.check.tor.update_url");
		
		$fullUpdateUrl = $updateUrl . "?ip=" . urlencode($serverIP);
		
		$valid = true;
		if( false == ($torData=file_get_contents($fullUpdateUrl)))
			$valid = false;		
		
		if(strlen($torData) == 0)
			$valid = false;
			
		if($valid)
			file_put_contents($torListPath, $torData);
		else
			$torData = "";
	}
	else
	{
		$torData = file_get_contents($torListPath);
	}
		
	//if($torData == "")
	//	throw new Exception("Unable to retrieve TOR Exit List.");
	if($torData == "")
		return "unknown";
		
	// Not the better approach, but works.
	if(strpos($torData,$requestIP))
		return "true";
	else
		return "false";
}

function dumpResult($action, $server, $port, $timeout, $msg, $resultTor, $testPort, $result, $msgResult, $mode)
{
	if($mode == "html")
	{
		echocr("<div id=\"is_waitbar\" style=\"display:none;\" class=\"is_waitbar\">");
		echocr($msg);
		echocr("<br /><br /><br />");
		
		if($result)
		{
		  echocr("<img src=\"images/icons/success.png\" alt=\"" . $msgResult . "\" title=\"" . $msgResult . "\" />");
		  echocr("<br /><br />");
		  echocr("<span style=\"font-size:1.3em; color:#008800\">" . $msgResult . "</span>");
		}
		else 
		{
			echocr("<img src=\"images/icons/failed.png\" alt=\"" . $msgResult . "\" title=\"" . $msgResult . "\" />");
			echocr("<br /><br />");
			echocr("<span style=\"font-size:1.3em; color:#FF0000\">" . $msgResult . "</span>");
		}
		
		echo "<div style=\"border:1px solid gray;margin:10px;height:1px;\"></div>";
		echocr(_LANG_TOR);
		echocr("<br /><br /><br />");
		
		if($resultTor == "true")
		{
			$msgResultTor = _LANG_TOR_SUCCESS;
		}
		else if($resultTor == "false")
		{
			$msgResultTor = _LANG_TOR_FAILED;
		}
		else
		{
			$msgResultTor = _LANG_TOR_UNKNOWN;
		}
		
		if($resultTor == "true")
		{
		  echocr("<img src=\"images/icons/success.png\" alt=\"" . $msgResultTor . "\" title=\"" . $msgResultTor . "\" />");
		  echocr("<br /><br />");
		  echocr("<span style=\"font-size:1.3em; color:#008800\">" . $msgResultTor . "</span>");
		}
		else if($resultTor == "false")
		{
			echocr("<img src=\"images/icons/failed.png\" alt=\"" . $msgResultTor . "\" title=\"" . $msgResultTor . "\" />");
			echocr("<br /><br />");
			echocr("<span style=\"font-size:1.3em; color:#FF0000\">" . $msgResultTor . "</span>");
		}
		else
		{
			echocr("<img src=\"images/icons/unknown.png\" alt=\"" . $msgResultTor . "\" title=\"" . $msgResultTor . "\" />");
			echocr("<br /><br />");
			echocr("<span style=\"font-size:1.3em; color:#FF0000\">" . $msgResultTor . "</span>");
		}
		
		echo "</div>";
		
		echojs("$(\"#is_waitbar\").fadeIn(1000);");
	}
	else
	{
		echo "<check action=\"" . htmlencode($action) . "\" ip=\"" . htmlencode($server) . "\" port=\"" . htmlencode($port) . "\" timeout=\"" . htmlencode($timeout) . "\" tor=\"" . htmlencode($resultTor) . "\" test=\"" . iif($testPort,"true","false") . "\" result=\"" . iif($result,"true","false") . "\" message=\"" . htmlencode($msgResult) . "\" />";
	}
}

function main()
{	
	$action = "check";
	$output = "html";
	if(isset($_GET["output"]))
		$output = validateAlphabetic($_GET["output"]);
			
	if($output == "html")
		xhtmlHeader(false);
	else
		xmlHeader(false);
	
	try
	{
		// Init language system
		languageInit();	
				
		if(getOptionBool("services.check.enabled") == false)
			throw new Exception("Service not available.");

		$server = $_SERVER['REMOTE_ADDR'];
				
		$testPort = !(isset($_GET["notest"]));
			
		$port = null;
		if(isset($_GET["port"]))
			$port = validateNumeric($_GET["port"]);
			
		$timeout = null;		
		if(isset($_GET["timeout"]))
			$timeout = validateNumeric($_GET["timeout"]);
			
		if ($port == null) { $port=6060; }
		if ($timeout == null) { $timeout=getOptionInt("services.check.port.timeout"); }
		if ($timeout<1) { $timeout=1; }
		if ($timeout>20) { $timeout=20; }	
		
		$msg = langToHtml(_LANG_PORT);
		$msg = str_replace("{ip}",htmlencode($server),$msg);
		$msg = str_replace("{port}",htmlencode($port),$msg);
		$msg = str_replace("{timeout}",htmlencode($timeout), $msg);
					
		$result = false;
		
		if ($server and $port and $timeout and $testPort) 
		{
			$result =  @fsockopen("$server", $port, $errno, $errstr, $timeout);
		}
		else
			$result = null;
			
		$resultTor = detectTOR();
					
		if($testPort)
		{
			if($result)
			{
				$msgResult = _LANG_PORT_SUCCESS;
			}
			else
			{
				$msgResult = _LANG_PORT_FAILED;
				$msgResult = str_replace("{timeout}",htmlencode($timeout), $msgResult);
			}
		}
		else
		{
			$msgResult = "Test skipped.";
		}
		
		dumpResult($action, $server, $port, $timeout, $msg, $resultTor, $testPort, $result, $msgResult, $output);
	}
	catch(Exception $e)
	{
		logMessage("error", $e->getMessage());
		
		if($output == "html")
			message("error", $e->getMessage());
		else
			echo "<check error=\"" . htmlencode($e->getMessage()) . "\" />";
	}
	
	if($output == "html")
		xhtmlFooter();
}

main();
?>