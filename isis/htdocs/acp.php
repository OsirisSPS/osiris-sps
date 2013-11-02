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

// -----------------------------------------
// Common functions
// -----------------------------------------

function timedReload()
{
	echo "<script type=\"text/javascript\">\n";
	echo "<!--	\n";
	echo "	function timedRedirect()\n";
	echo "	{\n";
	echo "	window.location = \"" . getPhpSelf() . "\"\n";
	echo "	}\n";
	echo "	setTimeout('timedRedirect()', 5000);\n";
	echo "//-->\n";
	echo "</script>\n";			
}

function postValueCheck($key)
{
	if(strlen($key)>=64)
		message("error","Design error: Post key '" . $key. "' too length (" . strlen($key) . " chars), some hosting (ex. Netsons) refuse post key bigger than 64 chars.");	
}

// -----------------------------------------
// Page "home"
// -----------------------------------------

function homeCheckPath($path)
{
	if(getWritable($path))
	{		
	}
	else
	{
		if(file_exists($path . ".dist"))
		{
			message("error","File or directory [b]" . $path . "[/b] not writable. Rename [b]" . $path . ".dist[/b] to [b]" . $path . "[/b] and adjust file permissions (chmod 777).");		
		}
		else
		{
			message("error","File or directory [b]" . $path . "[/b] not writable. Adjust file permissions (chmod 777).");		
		}
	}
}

// General checking
function homeBodyChecking()
{
	if(detectPhpSafeMode() == true)
		message("error","PHP is running in SAFE MODE. Isis can't work with SAFE MODE active. Sorry.");	
	
	ensureDir(getOption("data.path"));	
	ensureDir(getOption("data.path") . "/logs");	
	
	if( (file_exists("configuration.php") == false) && (file_exists("configuration.php.dist") == true) )
		rename("configuration.php.dist","configuration.php");
	if( (file_exists("robots.txt") == false) && (file_exists("robots.txt.dist") == true) )
		rename("robots.txt.dist","robots.txt");
		
	
	homeCheckPath("configuration.php");
	homeCheckPath("robots.txt");
	if(getOptionBool("services.link.seo.enabled"))
		homeCheckPath(getOption("services.link.seo.htaccess") . ".htaccess");
	homeCheckPath(getOption("data.path") . "/");
	//homeCheckPath(getOption("data.path") . "/logs" . "/");	
	
	if(getOption("server.acp.password") == "")
		message("error","ACP (Administration Control Panel) password access not set. Go to 'settings' page and set it.");
		
	// If database is enabled, check it's work.
	if(getOptionBool("server.database.enabled"))
	{	
		$connection = @mysql_connect(getOption("server.database.host"), getOption("server.database.login"), getOption("server.database.password"));
		if(!$connection)
		{
			message("error","Database connection error: " . mysql_error());
		}
		else
		{		
			$database = mysql_select_db(getOption("server.database.database"), $connection);
			if(!$database)
			{
				message("error","Database connection error: " . mysql_error());
			}
		}
			
		if($connection != null)
			mysql_close($connection);		
	}
		
	// Time check
	if(getOptionBool("services.manifest.enabled"))
	{		
		$serverTime = time();
		$internetTime = queryTimeServer(getOption("services.manifest.clock.server"));		
		if($internetTime == 0)
			message("warning","Unable to contact time server: " . getOption("services.manifest.clock.server"));
		else
		{			
			$timeDiff = $serverTime - $internetTime;
			$msgType = "";
			if(abs($timeDiff)>30)
				$msgType = "warning";
			else
				$msgType = "info";
				
			if($timeDiff != 0)
				message($msgType,"Difference from time server (" . getOption("services.manifest.clock.server") . ") of " . $timeDiff . " seconds. Machine time: " . date(DATE_RFC822, $serverTime) . " , Internet time: " . date(DATE_RFC822, $internetTime));			
		}
	}
	
	if( (getOption("data.path") != "./data") && (file_exists("./data")) )	
		message("warning","'data' directory under Isis installation are present but not used. You can remove it.");
		
	// HTAccess Data protection check
	if(getOption("data.path") == "./data")
	{
		if(file_exists(getOption("data.path") . "/index.html"))
		{
			$url = getOption("server.virtual_path") . "data/index.html?r=" . time();
			clearstatcache();
			
			$testData = @file_get_contents($url);
						
			if(strpos($http_response_header[0], "403 Forbidden") === false)
				message("warning","HTAccess protection of 'data' directory don't work. Enable it in your web-server, or move the data directory in another place outside the web requests (by setting 'data.path' for example in /tmp/myisis).Test result: '" . $http_response_header[0] . "' for url '" . $url . "'");
		}
	}
		
	if(getOptionBool("log.enabled") == true)
		message("warning","Log system is enabled. This may slow down performance.");	
		
	if( (getOptionBool("services.bootstrap.enabled") == true) &&
			(getOption("services.bootstrap.redirect") == "") &&
			(getOptionBool("server.database.enabled") != true) )
	{
		message("error","Bootstrap service active, but database is not. Set database settings.");
	}	
}

function homeBody()
{
	mainTitle("Welcome to " . getOption("server.name") . " - Administration Control Panel","Here you can view informations about this installation of Isis, and use tools to configure it.","home");	
	
	clearstatcache();
	
	homeBodyChecking();
		
	mainTitle("How to subscribe Osiris node to this Isis","This is the url to use in Osiris configuration:<code>" . getOption("server.virtual_path") . "</code>","links");
		
	$links = "<a target=\"_blank\" href=\"http://www.osiris-sps.org/permalink.php?id=isis_home\">Home page</a> | Support forum: <a target=\"_blank\" href=\"http://www.osiris-sps.org/permalink.php?id=isis_forum_eng\">English</a> / <a target=\"_blank\" href=\"http://www.osiris-sps.org/permalink.php?id=isis_forum_ita\">Italian</a>";
	mainTitle("Links",$links,"links");	
}

// -----------------------------------------
// Page "settings"
// -----------------------------------------

function settingsControl($key, $notes)
{
	return settingsControlEx($key, "text", $key, $notes);
}

function settingsEncodeId($id)
{
	// This exists because an html tag <input> cannot have a name-attribute with "." (dot).
	return str_replace(".","__",$id);
}

function settingsDecodeId($id)
{
	// This exists because an html tag <input> cannot have a name-attribute with "." (dot).
	return str_replace("__",".",$id);
}

function settingsControlEx($key, $type, $title, $notes, $regexSample="", $enumArray = null)
{
	global $OPTIONS;	
	
	$custom = false;	
	
	if(isset($OPTIONS[$key]) == false)
	{
		$custom = false;
	}
	else
	{
		$custom = $OPTIONS[$key]["custom"];
	}
	
	if($custom == false)
	{
		//message("info",$key);
		// Non è stata fatta un'ensure sopra, e non è mai stata valorizzata (custom).
		
		// Se è di un portale, allora il default da mostrare è quello della base.
		if(substr($key,0,8) == "portals.")
		{
			if(substr($key,8,7) != "default")
			{
				$defaultSubKey = "portals.default." . substr($key,106);
				$default = $OPTIONS[$defaultSubKey]["value"];				
				ensureOption($key, $default);
			}
		}
	}
			
	$cur_value = $OPTIONS[$key]["value"];	
	
	//echo $cur_value . gettype($cur_value);
	
	$default = $OPTIONS[$key]["default"];
	
	if(is_null($default) == true)
	{
		$default = "";
	}		
			
	//echo $cur_value . gettype($cur_value);
	
	//if($cur_value != $default)
	echo "<table class=\"is_settings\">";
	if($custom)
		echo "<tr class=\"is_settings_changed\">";
	else
		echo "<tr class=\"is_settings_default\">";
	echo "<td class=\"is_settings_info\">";
	if($title != "")
		echo "<b>" . $title . "</b><br />";
	if($notes != "")
		echo $notes . "<br />";
	if($cur_value != $default)
		echo "Different from default ( <b> " . echoHtmlVar($default) . "</b> )<br />";
	echo "</td>";
	
	// Hack. Is portal specific?
	// If yes, replace ID with 'portal' in post key name, to stay under 64 chars.
	$pos1 = strpos($key, ".");
	$pos2 = strpos($key,".",$pos1+1);
	$str1 = substr($key,0,$pos1);
	$str2 = substr($key,$pos1+1,$pos2-$pos1-1);
	if( ($str1 == "portals") && (validatePovID($str2)) )
	{
		$key = str_replace($str2,"pid",$key);
	}
	
	$controlName = "option_" . settingsEncodeId($key);	
	$controlCustomName = "custom_" . settingsEncodeId($key);	
	$ekey = settingsEncodeId($key);
	
	postValueCheck($controlName);
	postValueCheck($controlCustomName);
	
	echo "<td class=\"is_settings_override\">";
	
		
	echo "<select name=\"" . $controlCustomName . "\" onchange=\"javascript:void(onChangeCustom('" . $ekey . "'));\">";
	echo "<option value=\"0\" " . iif($custom==false, "selected=\"selected\"", "") . ">Default</option>";
	echo "<option value=\"1\" " . iif($custom==true, "selected=\"selected\"", "") . ">Custom</option>";
	echo "</select>";
	
	
	// Maybe optimized?	
	echo "</td>";
	
	echo "<td class=\"is_settings_input\">";
	
	if($type == "text")
	{
		echo "<input style=\"width:100%\" type=\"text\" name=\"" . $controlName . "\" value=\"" . htmlencode($cur_value) . "\" />";
	}
	if($type == "bool")
	{		
		echo "<select name=\"" . $controlName . "\">";
		echo "<option value=\"0\" " . iif($cur_value==false, "selected=\"selected\"", "") . ">No</option>";
		echo "<option value=\"1\" " . iif($cur_value==true, "selected=\"selected\"", "") . ">Yes</option>";
		echo "</select>";
		//echo "<input type=\"checkbox\" name=\"option_" . settingsEncodeId($key) . "\" " . iif($cur_value==1, "checked=\"checked\"", "") . " />";
	}
	else if($type == "textarea")
	{
		echo "<textarea wrap=\"off\" rows=\"5\" style=\"width:100%\" name=\"" . $controlName . "\">" . htmlencode($cur_value) . "</textarea>";
	}
	else if($type == "textareabig")
	{
		echo "<textarea wrap=\"off\" rows=\"20\" style=\"width:100%\" name=\"" . $controlName . "\">" . htmlencode($cur_value) . "</textarea>";
	}
	else if($type == "regex")
	{
		echo "<input style=\"width:100%\" type=\"text\" id=\"" . $controlName . "\" name=\"" . $controlName . "\" value=\"" . htmlencode($cur_value) . "\" />";
		echo "Regex Test: ";
		echo "<input style=\"width:50%\" type=\"text\" id=\"option_" . settingsEncodeId($key) . "_test\" value=\"" . htmlencode($regexSample) . "\" />";
		echo " - <a href=\"javascript:void(regExTest(document.getElementById('option_" . settingsEncodeId($key) . "').value, document.getElementById('option_" . settingsEncodeId($key) . "_test').value))\">Test now</a>";
	}
	else if($type == "enum")
	{
		echo "<select name=\"" . $controlName . "\">";
		foreach($enumArray as $key => $value)
		{
			echo "<option value=\"" . htmlencode($value) . "\" " . iif($cur_value == $value, "selected=\"selected\"", "") . ">" . htmlencode($key) . "</option>";
		}		
		echo "</select>";
	}
		//.match(new Regexp('alfa'))
		/*
		function regExTest(s, r)
		{
			if(s.match(new RegExp(s))
			{
				alert('Match.');
			}
			else
			{
				alert('Doesn\'t match.');
			}
		}
		*/	
	echo "</td>";
	/*
	echo "<td class=\"is_settings_actions\">";
	if($cur_value != $default)
	{
		echo "<a class=\"is_button_100\" href=\"javascript: void(document.getElementByName('option_$name').text = '$default')\">Revert</a>";		
	}
	echo "</td>";
	*/	
	
	//echo "<div style=\"clear:both;\"></div>";
	echo "</tr>";
	
	echo "</table>";
	
	echo "<script type=\"text/javascript\">";
	echo "onChangeCustom('" . $ekey . "');";
	echo "</script>";
}

function settingsControlReplace($i)
{
	$prefixNotes = "Replace n. " . $i . " - ";
	settingsControl("output.replaces.type" . $i, $prefixNotes . "\"direct\" or \"regex\". Direct is simple and faster, Regex is complex but flexible.");
	settingsControl("output.replaces.from" . $i, $prefixNotes . "From html");
	settingsControl("output.replaces.to" . $i, $prefixNotes . "To html");
}



function settingsBody()
{
	mainTitle("Settings","Here you can change the settings that control how Isis works.","settings");
	
	$group = validateAlphabetic(getGetText("group"));	
	if($group == null)
		$group = "list";
		
	$povId = getGetText("portal");
	
	if( ($povId != "") && (validatePovID($povId) == false) )
		throw new Exception("Invalid portal.");
	
	if($povId == "")
		$povId = "default";
		
	$action = validateAlphabetic(getPostText("action"));
	
	foreach($_POST as $key => $value)
	{
		//trace("post:" . $key . ", value:" . $value);
		//if(substr($key,0,7) == "option_")
		if(substr($key,0,7) == "custom_")
		{			
			$name = substr($key,7);			
			$custom = (getPostText("custom_" . $name) != "0");			
			$value = getPostText("option_" . $name);			
			$key2 = settingsDecodeId($name);
				
			// Some keys are portal-specific?
			// This laps are done to "shortcut" post-key length, that some host require less than 64 chars.		
			if(strpos($key2,".pid.") !== false)
			{
				//trace("from ".$key2);
				$key2 = str_replace(".pid.","." . $povId . ".",$key2);
				//trace("to ".$key2);
			}
			
			//trace("key2:" . $key2 . " custom:" . $custom . " value:" . $value);
			if($custom)
			{				
				$post_value = str_replace("\\\"", "\"", str_replace("\\\\","\\", $value));			
				setOption($key2, $post_value);	
				//echo "setoption:" . $key2 . "=" . $post_value . "  == " . getOption($key2) . "<br/>";				
			}
			else
			{				
				//echo "resetoption" . "<br/>";
				resetOption($key2);
			}
			//echo $key2 . "=" . $post_value . "<br/>";
		}		
	}
	
	if($action == "Save")
	{
		message("info","Settings saved.");
		settingsSave();
	}	
	
	echo "<form name=\"form1\" method=\"post\" action=\"" . getPostBackUrl() . "\">";		
	echo "<input class=\"is_button_200\" name=\"action\" type=\"submit\" value=\"Save\" />";
	echo "<div style=\"clear:both;\"></div>";
	
	if($group == "list")
	{		
		echo linkBox("general","General settings",getPhpSelf() . "?page=settings&group=general");
		echo linkBox("services","Services settings",getPhpSelf() . "?page=settings&group=services");
		echo linkBox("portal_default","Portals default settings",getPhpSelf() . "?page=settings&group=portals");
		
		message("info","For single/specific portal settings, use the link [i]'Settings'[/i] in [b]Status[/b] tab.");
		
		/*
		foreach(portalsList() as $portalId)
		{
			$name = getOptionPortal("isis.info.name", $portalId);
			if($name == "")
				$name = "<noname>";
			
			$title = getOptionPortal("isis.info.name", $portalId) . " (ID: " . $portalId . ")";
			
			$hrefSettings = getPhpSelf() . "?page=settings&group=portal&portal=" . $portalId;
						
			$html = "<a href=\"" . htmlencode($hrefSettings) . "\">" . htmlencode($name) . "</a><br />" . "ID: " . htmlencode($portalId);
			
			echo dataBox("portal",$html);
		}
		*/	
	}	
	else if($group == "general")
	{
		message("info","General settings");
		settingsControl("server.acp.password", "Password to access to this administration page");	
		settingsControl("server.name", "Full name of this server.");
		settingsControl("server.virtual_path", "Virtual path (ex. http://www.alfa.com/isis).<br />Maybe: " . getCurrentHttpPath());
		settingsControl("server.physical_path", "Physical path (ex. /var/www/isis).<br />Maybe: " . getCurrentPhysicalPath());
		settingsControl("data.path", "");
		settingsControl("server.closed_message", "If not empty, this server is closed (for example, write here a maintenance message).");		
		settingsControlEx("server.acp.only_ssl", "bool", "server.acp.only_ssl", "If true, it's allowed to access to this administration area only through SSL.");
		settingsControlEx("server.acp.tabs","textarea","server.acp.tabs", "Custom ACP tabs");
		settingsControlEx("server.gzip.enabled", "bool", "server.gzip.enabled", "");
		settingsControlEx("server.database.enabled", "bool", "server.database.enabled", "Database support enabled (enable it only if the system ask for it)");
		settingsControl("server.database.host", "Database host (ex. <i>localhost</i>)");
		settingsControl("server.database.database", "Database db name");
		settingsControl("server.database.login", "Database user login");	
		settingsControl("server.database.password", "Database user password");		
		settingsControlEx("compatibility.use_set_time_limit", "bool", "compatibility.use_set_time_limit", "");
		settingsControlEx("ips.allowed", "regex", "ips.allowed", "", "192.168.1.1");
		settingsControlEx("ips.blocked", "regex", "ips.blocked", "", "192.168.1.1");
		
		// Home settings	
		message("info","Home Page settings");
		settingsControl("home.redirect", "The default url to be shown if the home-page is requested.");		
		settingsControlEx("home.portalslist", "bool", "home.portalslist", "Show portals list");	
		settingsControlEx("home.pastelinks", "bool", "home.pastelinks", "Show paste links box");	
		settingsControlEx("home.header","textareabig","home.header","This HTML will be showed in the top of home page.");
		settingsControlEx("home.footer","textareabig","home.footer","This HTML will be showed in the bottom of every page. Useful for example for Google Analytics.");
		
		message("info","Layout settings");
		settingsControlEx("layout.footer","textareabig","layout.footer","This HTML will be showed in the bottom of every page.");
		
		message("info","Robots");
		settingsControlEx("robots.enabled", "bool", "robots.enabled", "Allow search-spider to crawl the site. Controls 'robots.txt' generation.");	
				
		// Portals settings	
		message("info","Portals settings");
		settingsControlEx("portals.general.allowed", "regex", "portals.general.allowed", "");
		settingsControlEx("portals.general.blocked", "regex", "portals.general.blocked", "");
		
		message("info","Logging");
		settingsControlEx("log.enabled", "bool", "log.enabled", "");
		settingsControl("log.prefix", "Prefix of the log file name.");
		settingsControl("log.timestamp", "Timestamp of the log file name. Leave empty to have only one log file. Same syntax of PHP function date()");
		settingsControl("log.suffix", "Suffix of the log file name.");
		settingsControlEx("log.filter.notice", "bool", "log.filter.notice", "'Notice' level logged?");
		settingsControlEx("log.filter.php", "bool", "log.filter.php", "PHP messages are logged?");
		
		message("info","Tuning settings");		
		settingsControl("tuning.index.timeout", "Timeout (in second) for the main/guest page.");
		settingsControl("tuning.request.waitstep", "Delay (in second) between checking for a new job for nodes");
		settingsControl("tuning.request.timeout", "Timeout (in second) for node, to receive a job.");
		settingsControl("tuning.accept.waitstep", "Delay (in second) between checking for an accepted job to be done");
		settingsControl("tuning.accept.timeout", "Timeout (in second) for waiting a node that accept the realtime job");
		settingsControl("tuning.answer.waitstep", "Delay (in second) between checking for an answer job to be done");
		settingsControl("tuning.answer.timeout", "Timeout (in second) for waiting an answer for a realtime job");
		
		message("info","Background jobs");
		settingsControlEx("jobs.cron","textareabig","jobs.cron","");		
	}
	else if($group == "services")
	{
		message("info","Link (user-friendly, permalink)");
		settingsControlEx("services.link.enabled", "bool", "services.link.enabled", "Enabled");
		settingsControlEx("services.link.list","textarea","services.link.list", "List of redirections.<br>One line per url.<br>Format: key url [flags].<br>Flags: S=Write in SEO .htaccess<br>With SEO, don't use prefix http:// if you don't want a redirect.");
		settingsControlEx("services.link.redirect","text","services.link.redirect", "Where redirect unknown names.");
		settingsControlEx("services.link.seo.enabled", "bool", "services.link.seo.enabled", "SEO (Search Engine Optimization) enabled. Require Apache mod_rewrite.");	
		settingsControlEx("services.link.seo.htaccess", "text", "services.link.seo.htaccess", "Path (for example '../' where Isis create the .htaccess file for SEO.");	
		settingsControlEx("services.link.seo.path", "text", "services.link.seo.path", "Path (for example '/' where Isis redirect urls to match the .htaccess file for SEO.");	
		settingsControlEx("services.link.seo.home_portal", "text", "services.link.seo.home_portal", "ID of the home portal. For url optimization.");	
		
		message("info","Redirect");
		settingsControlEx("services.redirect.enabled", "bool", "services.redirect.enabled", "Enabled");
		
		message("info","Manifest");
		settingsControlEx("services.manifest.enabled", "bool", "services.manifest.enabled", "Enabled");
		settingsControlEx("services.manifest.error", "text", "services.manifest.error", "If not empty, throw this error on Osiris interface.");
		settingsControl("services.manifest.intervalUpdate", "Delay (in second) between request from Osiris client.");
		settingsControlEx("services.manifest.clock.local", "bool", "services.clock.local", "Use local clock? (Otherwise, use time server)");
		settingsControlEx("services.manifest.clock.server","text","services.clock.server", "Time server.");
		settingsControlEx("services.manifest.osiris.version", "text", "services.manifest.version", "Latest available Osiris version");
		settingsControlEx("services.manifest.osiris.notes", "text", "services.manifest.notes", "Notes about this version");
		
		message("info","Checking");
		settingsControlEx("services.check.enabled", "bool", "services.check.enabled", "Enabled");
		settingsControlEx("services.check.port.timeout", "text", "services.check.port.timeout", "Default timeout for port checking.");
		settingsControlEx("services.check.tor.update_url", "text", "services.check.tor.update_url", "Update url for TOR Exit Nodes.");
		settingsControlEx("services.check.tor.update_every", "text", "services.check.tor.update_every", "When update TOR Exit Nodes List, in seconds.");		
				
		message("info","For Osiris - Update System (require SSL)");
		settingsControlEx("services.update.enabled", "bool", "services.update.enabled", "Enabled");
		settingsControlEx("services.update.redirect","text","services.update.redirect", "Delegate this service to another Isis.");
		settingsControlEx("services.update.version","text","services.update.version", "Client current version known");
		settingsControlEx("services.update.notes","textarea","services.update.notes", "Notes about this version");
				
		message("info","For Osiris - Bootstrap (require MySql, SSL)");
		settingsControlEx("services.bootstrap.enabled", "bool", "services.bootstrap.enabled", "Enabled");
		settingsControlEx("services.bootstrap.redirect","text","services.bootstrap.redirect", "Delegate this service to another Isis.");
	}
	else if( ($group == "portals") || ($group == "portal") )
	{
		$suffix = "";
		if( ($group == "portals") || ($povId == null) )
		{
			message("info","Portals settings defaults");
			$suffix = "default";
		}
		else
		{
			//message("info","Portal ID:" . $portalId);
			portalBox($povId);
			$suffix = $povId;			
		}
		
		if($suffix != "default")
		{
			message("info","Informations");
			settingsControlEx("portals." . $suffix . ".isis.info.alias","text","isis.info.alias", "Alias (short) name");
			settingsControlEx("portals." . $suffix . ".isis.info.name","text","isis.info.name", "Full name");
			settingsControlEx("portals." . $suffix . ".isis.info.description","textarea","info.description", "Description");
			settingsControlEx("portals." . $suffix . ".isis.info.acp.notes","textareabig","info.acp.notes", "Notes, visible only from ACP. Maybe for ex. an internal link to a private portal of managment.");
		}
		
		message("info","General");		
		settingsControlEx("portals." . $suffix . ".isis.info.hidden", "bool", "isis.info.hidden", "If false, the portal are not visible in public list.");
		settingsControlEx("portals." . $suffix . ".isis.home","text","isis.home", "If empty, the 'Home' link redirect to Isis Home. Otherwise, write an url here.");
		settingsControlEx("portals." . $suffix . ".isis.closed_message","text","isis.closed_message", "If not empty, access to this portal is closed (for example, write here a reason).");
		settingsControlEx("portals." . $suffix . ".isis.redirect","text","isis.redirect", "Redirect url");
		//settingsControlEx("portals." . $suffix . ".isis.mode", "text", "isis.mode", "Mode (proxy / redirect / direct)"); // Not yet implemented.
		settingsControlEx("portals." . $suffix . ".isis.realtime.enabled", "bool", "realtime.enabled", "If false, the nodes are contacted for updating only in background.");
		settingsControlEx("portals." . $suffix . ".isis.url.allowed", "regex", "isis.url.allowed", "");
		settingsControlEx("portals." . $suffix . ".isis.url.blocked", "regex", "isis.url.blocked", "");	
		settingsControlEx("portals." . $suffix . ".isis.guest.password", "regex", "isis.guest.password", "");
		settingsControlEx("portals." . $suffix . ".isis.guest.message", "text", "isis.guest.message", "");
		settingsControlEx("portals." . $suffix . ".isis.guest.ips.allowed", "regex", "isis.guest.ips.allowed", "");
		settingsControlEx("portals." . $suffix . ".isis.guest.ips.blocked", "regex", "isis.guest.ips.blocked", "");	
		settingsControlEx("portals." . $suffix . ".isis.peers.password", "regex", "isis.peers.password", "");
		settingsControlEx("portals." . $suffix . ".isis.peers.ips.allowed", "regex", "isis.peers.ips.allowed", "");
		settingsControlEx("portals." . $suffix . ".isis.peers.ips.blocked", "regex", "isis.peers.ips.blocked", "");	
		settingsControlEx("portals." . $suffix . ".isis.peers.version.allowed", "regex", "isis.peers.version.allowed", "");
		settingsControlEx("portals." . $suffix . ".isis.peers.version.blocked", "regex", "isis.peers.version.blocked", "");	

		message("info","Cache settings");
		settingsControlEx("portals." . $suffix . ".isis.cache.enabled", "bool", "isis.cache.enabled", "If caching system is enabled.");	
		settingsControlEx("portals." . $suffix . ".isis.cache.url.always", "regex", "isis.cache.url.always", "If the url match, an already exists cache copy is returned without checking if need to be updated.");
		settingsControlEx("portals." . $suffix . ".isis.cache.url.never", "regex", "isis.cache.url.never", "If the url match, always request to Osiris node.");
		settingsControlEx("portals." . $suffix . ".isis.cache.store.dynamic", "bool", "isis.cache.store.dynamic", "Jobs that normally aren't stored in cache (dynamically), are stored? Normally used with 'cache.realtime.forced'>0");	
		settingsControlEx("portals." . $suffix . ".isis.cache.realtime.forced", "text", "isis.cache.realtime.forced", "If a jobs is requested to Osiris node at minimum X seconds ago, always the cache copy is returned.");
		settingsControlEx("portals." . $suffix . ".isis.cache.realtime.deltaratio", "text", "isis.cache.realtime.deltaratio", "");
		settingsControlEx("portals." . $suffix . ".isis.cache.realtime.deltamax", "text", "isis.cache.realtime.deltamax", "");
		settingsControlEx("portals." . $suffix . ".isis.cache.background.forced", "text", "isis.cache.background.forced", "If >0, jobs that normally aren't stored in cache (dynamically), are stored for X seconds.");
		settingsControlEx("portals." . $suffix . ".isis.cache.background.deltaratio", "text", "isis.cache.background.deltaratio", "");
		settingsControlEx("portals." . $suffix . ".isis.cache.background.deltamax", "text", "isis.cache.background.deltamax", "");
		settingsControlEx("portals." . $suffix . ".isis.cache.languages", "text", "isis.cache.languages", "");
		
		message("info","Output");
		settingsControlEx("portals." . $suffix . ".isis.output.area.systembar","textareabig","output_area_systembar","");
		settingsControlEx("portals." . $suffix . ".isis.output.area.footer","textareabig","output_area_footer","");
		settingsControlEx("portals." . $suffix . ".isis.output.replaces","textareabig","output_replaces","");		
		/*
		settingsControl("output.replaces.count", "Number of replaces options");
		$nro = getOptionInt("output.replaces.count");
		for($ro = 1; $ro<=$nro; $ro++)
		{
			settingsControlReplace($ro);
		}
		*/
	
		message("info","Osiris client");
		settingsControlEx("portals." . $suffix . ".ide.objects.hot_state_threshold", "text", "ide.objects.hot_state_threshold", "");
		settingsControlEx("portals." . $suffix . ".ide.pager.items", "text", "ide.pager.items", "");
		settingsControlEx("portals." . $suffix . ".ide.pager.pages", "text", "ide.pager.pages", "");
		settingsControlEx("portals." . $suffix . ".ide.skin.id", "text", "ide.skin.id", "");
		
		$timeOffsetEnum = array();		
		$timeOffsetEnum["(GMT - 12:00) Enitwetok, Kwajalien"] = -43200;
		$timeOffsetEnum["(GMT - 11:00) Midway Island, Samoa"] = -39600;
		$timeOffsetEnum["(GMT - 10:00) Hawaii"] = -36000;
		$timeOffsetEnum["(GMT - 9:30) French Polynesia"] = -34200;
		$timeOffsetEnum["(GMT - 9:00) Alaska"] = -32400;
		$timeOffsetEnum["(GMT - 8:00) Pacific Time (US &amp; Canada)"] = -28800;
		$timeOffsetEnum["(GMT - 7:00) Mountain Time (US &amp; Canada)"] = -25200;
		$timeOffsetEnum["(GMT - 6:00) Central Time (US &amp; Canada), Mexico City"] = -21600;
		$timeOffsetEnum["(GMT - 5:00) Eastern Time (US &amp; Canada), Bogota, Lima"] = -18000;
		$timeOffsetEnum["(GMT - 4:30) Bolivarian Time"] = -16200;
		$timeOffsetEnum["(GMT - 4:00) Atlantic Time (Canada), Caracas, La Paz"] = -14400;
		$timeOffsetEnum["(GMT - 3:30) Newfoundland"] = -12600;
		$timeOffsetEnum["(GMT - 3:00) Brazil, Buenos Aires, Falkland Is."] = -10800;
		$timeOffsetEnum["(GMT - 2:00) Mid-Atlantic, Ascention Is., St Helena"] = -7200;
		$timeOffsetEnum["(GMT - 1:00) Azores, Cape Verde Islands"] = -3600;
		$timeOffsetEnum["(GMT) Casablanca, Dublin, London, Lisbon, Monrovia"] = 0;
		$timeOffsetEnum["(GMT + 1:00) Brussels, Copenhagen, Madrid, Paris"] = 3600;
		$timeOffsetEnum["(GMT + 2:00) Kaliningrad, South Africa"] = 7200;
		$timeOffsetEnum["(GMT + 3:00) Baghdad, Riyadh, Moscow, Nairobi"] = 10800;
		$timeOffsetEnum["(GMT + 3:30) Tehran"] = 12600;
		$timeOffsetEnum["(GMT + 4:00) Abu Dhabi, Baku, Muscat, Tbilisi"] = 14400;
		$timeOffsetEnum["(GMT + 4:30) Kabul"] = 16200;
		$timeOffsetEnum["(GMT + 5:00) Ekaterinburg, Karachi, Tashkent"] = 18000;
		$timeOffsetEnum["(GMT + 5:30) Bombay, Calcutta, Madras, New Delhi"] = 19800;
		$timeOffsetEnum["(GMT + 5:45) Kathmandu"] = 20700;
		$timeOffsetEnum["(GMT + 6:00) Almaty, Colomba, Dhakra"] = 21600;
		$timeOffsetEnum["(GMT + 6:30) Yangon, Naypyidaw, Bantam"] = 23400;
		$timeOffsetEnum["(GMT + 7:00) Bangkok, Hanoi, Jakarta"] = 25200;
		$timeOffsetEnum["(GMT + 8:00) Hong Kong, Perth, Singapore, Taipei"] = 28800;
		$timeOffsetEnum["(GMT + 8:45) Caiguna, Eucla"] = 31500;
		$timeOffsetEnum["(GMT + 9:00) Osaka, Sapporo, Seoul, Tokyo, Yakutsk"] = 32400;
		$timeOffsetEnum["(GMT + 9:30) Adelaide, Darwin"] = 34200;
		$timeOffsetEnum["(GMT + 10:00) Melbourne, Papua New Guinea, Sydney"] = 36000;
		$timeOffsetEnum["(GMT + 10:30) Lord Howe Island"] = 37800;
		$timeOffsetEnum["(GMT + 11:00) Magadan, New Caledonia, Solomon Is."] = 39600;
		$timeOffsetEnum["(GMT + 11:30) Burnt Pine, Kingston"] = 41400;
		$timeOffsetEnum["(GMT + 12:00) Auckland, Fiji, Marshall Island"] = 43200;
		$timeOffsetEnum["(GMT + 12:45) Chatham Islands"] = 45900;
		$timeOffsetEnum["(GMT + 13:00) Kamchatka, Anadyr"] = 46800;
		$timeOffsetEnum["(GMT + 14:00) Kiritimati"] = 50400;
		settingsControlEx("portals." . $suffix . ".language.time_offset", "enum", "language.time_offset", "Time offset between UTC/GMT. In seconds.","",$timeOffsetEnum);
		
		$timeDSTEnum = array();
		$timeDSTEnum["-1 hour"] = -3600;
		$timeDSTEnum["Disabled"] = 0;
		$timeDSTEnum["+1 hour"] = 3600;
		settingsControlEx("portals." . $suffix . ".language.time_dst", "enum", "language.time_dst", "Time offset if DST (Daylight Saving Time) is active. In seconds.","",$timeDSTEnum);
		
		settingsControlEx("portals." . $suffix . ".url.protocols.find_link", "text", "url.protocols.find_link", "");
		settingsControlEx("portals." . $suffix . ".url.protocols.no_confirm", "text", "url.protocols.no_confirm", ""); 
		settingsControlEx("portals." . $suffix . ".url.protocols.no_redirect", "text", "url.protocols.no_redirect", "");
		settingsControlEx("portals." . $suffix . ".url.protocols.collect", "text", "url.protocols.collect", "");
		settingsControlEx("portals." . $suffix . ".privacy.allow_external_images", "bool", "privacy.allow_external_images", "");
		settingsControlEx("portals." . $suffix . ".privacy.allow_external_urls", "bool", "privacy.allow_external_urls", "");
		settingsControlEx("portals." . $suffix . ".anonymity.object_mode", "text", "anonymity.object_mode", "");
		settingsControlEx("portals." . $suffix . ".anonymity.object_trusted_domains", "text", "anonymity.object_trusted_domains", "");
		settingsControlEx("portals." . $suffix . ".anonymity.object_trusted_types", "text", "anonymity.object_trusted_types", "");
		settingsControlEx("portals." . $suffix . ".web.redirect_url", "text", "web.redirect_url", "");
		settingsControlEx("portals." . $suffix . ".system.isis", "text", "system.isis", "");
		
		settingsControlEx("portals." . $suffix . ".options", "textareabig", "options", "Custom options. Key=Value. One line per option.");
	}	
	
	echo "</form>";	
}

// -----------------------------------------
// Page "status"
// -----------------------------------------

function statusJobs($title, $portal, $type, $notes)
{	
	echo "<div class=\"is_status_group\">";
	echo "<div class=\"is_status_title\">" . $title . "</div>";
	echo "<div class=\"is_status_notes\">" . $notes . "</div>";
	$path = getOption("data.path") . "/portal_" . $portal . "/" . $type . "/";
	
	$count = 0;
	
	if ($dh = opendir($path))
	{			
		while (false !== ($file = readdir($dh))) 
		{
			if ($file != "." && $file != ".." && $file != "index.html" && $file != ".htaccess")
			{
				$fullPath = getOption("data.path") . "/portal_" . $portal . "/" . $type . "/" . $file;
				$strInfo = getFileInfo($fullPath);
				echo "<div class=\"is_status_job\"><a href=\"?page=status&amp;portal=" . $portal . "&amp;obj_type=" . $type . "&amp;obj_id=" . $file . "\">" . $file . "</a> (" . $strInfo . ")</div>";
				$count++;
			}
		}
	}
	closedir($dh);
	if($count == 0)
		echo "<div class=\"is_status_job_empty\">None</div>";
	echo "</div>";
}

function statusDetailBox($title, $notes, $body)
{	
	echo "<div class=\"is_status_group\">";
	echo "<div class=\"is_status_title\">" . $title . "</div>";
	echo "<div class=\"is_status_notes\">" . $notes . "</div>";
	echo $body;	
	echo "</div>";
}

function statusBody()
{
	mainTitle("Status","Here you can view informations & statistics.","status");
	
	$pov = getGetText('portal');
	if( ($pov != "") && (validatePovID($pov) == false) )
	{
		throw new Exception("Invalid portal.");
	}
		
	$action = getGetText('act');
	
	if($pov == "")
		$action = "general";
	
	if( ($action == "remove") && ($pov != "") )
	{
		$path = getOption("data.path") . "/portal_" . $pov . "/";		
		
		deleteDirectory($path);
		
		message("info","Portal removed.");				
		
		$action = "general";
	}
	
	if( ($action == "cacheclear") && ($pov != "") )
	{
		$nfiles = 0;
		foreach(portalFileList($pov, "cache_data") as $file)
		{
			message("info", "Cache data '" . $file . "' deleted.");
			$nfiles++;
			unlink($file);								
		}
		
		foreach(portalFileList($pov, "cache_info") as $file)
		{
			message("info", "Cache info '" . $file . "' deleted.");
			$nfiles++;
			unlink($file);								
		}
		
		if($nfiles == 0)
			message("info","Cache already empty.");				
		else
			message("info","Cache cleaned.");				
			
		$action = "general";
	}
	
	
	$pasteLinks = getPostText("paste_links");
	if( ($action == "general") && ($pasteLinks != "") )	
	{
		pasteLinks($pasteLinks);		
	}
		
	if($action == "general")
	{
		// List
		foreach(portalsList() as $pov)
		{
			portalBox($pov);
			flush();
		}
		
		// General statistics
		$totalIsisDirectory = getDirectorySize(getOption("server.physical_path"));
		
		$html = "<span style=\"font-size:1.5em\">Global statistics</span><br/>";
		$html .= "Total Isis installation size: <b>" . convertBytesToString($totalIsisDirectory['size']) . "</b> (<b>" . $totalIsisDirectory['count'] . "</b> files, <b>" . $totalIsisDirectory['dircount'] . "</b> directories)";
		
		echo dataBox("status",$html);
		
		$html = "<span style=\"font-size:1.5em\">Paste links</span><br/>";
		$html .= "<form name=\"form1\" method=\"post\" action=\"" . getPostBackUrl() . "\">";
		$html .= "<textarea name=\"paste_links\" style=\"width:99%\" rows=4>" . htmlencode($pasteLinks) . "</textarea>";
		$html .= "<div style=\"text-align:right\"><input type=\"submit\" value=\"Paste links\"></div>";
		echo dataBox("pastelinks",$html);		
	}
	else
	{
		portalBox($pov);
		
		$objID = validateID(getGetText('obj_id'));
		if($objID != null)
		{
			$objType = validateID(getGetText('obj_type'));
			
			if($objType == "cache")
			{
				$infoPath = getOption("data.path") . "/portal_" . $pov . "/cache_info/" . $objID;
				$dataPath = getOption("data.path") . "/portal_" . $pov . "/cache_data/" . $objID;
				
				codeBox(file_get_contents($infoPath));
				codeBox(file_get_contents($dataPath));
			}
			else
			{
				$job_path = getOption("data.path") . "/portal_" . $pov . "/" . $objType . "/" . $objID;
			
				if(file_exists($job_path))
				{			
					codeBox(file_get_contents($job_path));					
				}
				else
				{
					message("error", "Job not found.");
				}
			}
		}
		
		$path = getOption("data.path") . "/";	
		
		// Nodes
		$output = "";
		foreach(portalFileList($pov, "nodes") as $file)
		{				
			$data = explode("\n",file_get_contents($file));
			
			$output .= "<div class=\"is_status_item\">";
			$output .= "IP: <b>" . $data[1] . "</b>, Client: <b>" . $data[2] . "</b>, waiting from <b>" . relativeTime(filemtime($file)) . "</b>";
			$output .= "</div>";
		}
		statusDetailBox("Nodes", "Nodes waiting for a job.", $output);
		echo statusJobs("Nodes Jobs" , $pov, "nodes", "Jobs waiting, one for each node.");
		
		// Others
		echo statusJobs("Requests - Realtime" , $pov, "requests_realtime", "Jobs requested from a client, realtime, waiting for Osiris node");
		echo statusJobs("Requests - Background" , $pov, "requests_background", "Jobs requested from Isis, background, waiting for Osiris node");		
		echo statusJobs("Accepteds" , $pov, "accepteds", "Jobs accepted by Osiris node, Isis is waiting for reponses.");
		echo statusJobs("Incomings" , $pov, "incomings", "Incoming jobs from Isis");
		echo statusJobs("Responses" , $pov, "responses", "Jobs executed by Osiris node, but not yet sended to client.");		
		
		// Cache
		$output = "<table>";
		$output .= "<tr class=\"is_status_item\">";						
		$output .= "<th>Last check</th>";
		$output .= "<th>Doc date</th>";
		$output .= "<th>Size</th>";
		$output .= "<th>Verb</th>";
		$output .= "<th>Url</th>";
		$output .= "<th>User Agent</th>";
		$output .= "<th>Culture</th>";
		$output .= "<th>Type</th>";
		$output .= "<th>Post</th>";
		$output .= "</tr>\r\n";
		
		$now = time();
			
		foreach(portalFileList($pov, "cache_info") as $file)
		{				
			$fileData = str_replace("cache_info","cache_data",$file);
			$data = explode("\n",file_get_contents($file));
			
			$id = extractStringReverseFind($file,"/");
			
			$link = "?page=status&amp;portal=" . $pov . "&amp;obj_type=cache&amp;obj_id=" . $id;
			
			$output .= "<tr class=\"is_status_item\">";			
			
			$output .= "<td>" . relativeTime($data[1]) . "</td>";
			$output .= "<td>" . relativeTime($data[2]) . "</td>";
			$output .= "<td>" . convertBytesToString(filesize($fileData)) . "</td>";			
			$output .= "<td>" . $data[3] . "</td>";
			$output .= "<td>" . "<a href=\"" . $link . "\">" . $data[4] . "</a></td>";
			$output .= "<td>" . $data[5] . "</td>";
			$output .= "<td>" . $data[6] . "</td>";
			$output .= "<td>" . $data[7] . "</td>";
			$output .= "<td>" . $data[8] . "</td>";
							
			$output .= "</tr>\r\n";
		}
		$output .= "</table>";
		statusDetailBox("Cache", "Document stored as cache.", $output);
	}	
}

// -----------------------------------------
// Page "jobs"
// -----------------------------------------

function jobsBody()
{	
	mainTitle("Jobs","","log");
	
	$jobToRun = validateAlphabeticEx(getGetText("run"));
	
	// List
	$path = ".";
	$listHtml = "";
	if ($dh = opendir($path))
	{				
		while (false !== ($file = readdir($dh))) 
		{		
			if ($file != "." && $file != "..")
			{		
				$match = preg_replace("/job_(.*?)\.php/i","$1", $file);
				
				if($match != $file)
				{
					$jobFilename = $path . "/" . $file;
				
					$name = $match;
				
					include($jobFilename);
					
					if($jobToRun == "")
					{
						$autoRunPath = getOption("data.path") . "/" . $name . ".jobneedrun";
						if(file_exists($autoRunPath))
						{
							unlink($autoRunPath);
							$jobToRun = $name;
							
							message("info", "Auto Executing job '" . $name . "'...");
						}
					}	
					
					if($jobToRun == $name)
					{
						message("info", "Executing job '" . $name . "'...");
						$success = call_user_func("job_" . $name . "_run");
					
						if($success)
							message("info", "Executing job '" . $name . "' successfully.");
						else
							message("error", "Executing job '" . $name . "' failed.");
					}
					
					$boxHtml = call_user_func("job_" . $name . "_view");
								
					if($boxHtml == "")
						$boxHtml = $name;
						
					$hrefRun = getPhpSelf() . "?page=jobs&run=" . htmlencode($name);
		
					$actions = "<a class=\"nolinkfx\" href=\"" . htmlencode($hrefRun) . "\">";
					$actions .= "<img src=\"images/icons/execute.png\" style=\"width:16px;height:16px;\" alt=\"" . htmlencode("Run now") . "\" />";
					$actions .= "</a>";				
		
					$html = $boxHtml;
		
					$html .= "<div style=\"padding:5px;margin:5px;border:1px solid #CCCCCC;float:right;\">";
					$html .= "Name: <b>" . htmlencode($name) . "</b><br />";
					//$html .= "Last execution: <b>" . "unknown" . "</b><br />";				
					$html .= "</div>";
					
					$html .= "<div style=\"padding:5px;margin:5px;border:1px solid #CCCCCC;float:right;\">" . $actions . "</div>";
						
					$listHtml .= dataBox("job",$html);		
				}
			}
		}
	}
	closedir($dh);
	
	echo $listHtml;
}

// -----------------------------------------
// Page "log"
// -----------------------------------------

function logBody()
{	
	mainTitle("Log","Here you can see the logs. Click a log to view it.<br />Remember to enable the log in the <b>settings</b> page if you need it.","log");
	
	// Avoid caching of files status.
	clearstatcache();
	
	$path_log = getOption("data.path") . "/logs";
	if(validateAlphabetic(getGetText("action")) == "clear")
	{		
		if ($dh = opendir($path_log))
		{			
			while (false !== ($file = readdir($dh))) 
			{
				if ($file != "." && $file != ".." && $file != "index.html" && $file != ".htaccess")
				{
					$path_logfile = $path_log . "/" . $file;
					unlink($path_logfile);
				}
			}
		}
		message("info","Logs cleared.");				
	}
	
	if(getOptionBool("log.enabled") == false)
		message("info","Log system is disabled.");	
		
	echo "<a class=\"is_button_200\" href=\"?page=log&amp;action=view\">Refresh</a>";
	echo "<a class=\"is_button_200\" href=\"?page=log&amp;action=clear\">Clear logs</a>";
	echo "<div style=\"clear:both;\"></div>";	
	
	if ($dh = opendir($path_log))
	{			
		$body = "";
				
		$body .= "<ul>";
		while (false !== ($file = readdir($dh))) 
		{
			if ($file != "." && $file != ".." && $file != "index.html" && $file != ".htaccess")
			{
				$strSize = convertBytesToString(filesize($path_log . "/" . $file));
				$body .= "<li><a href=\"?page=log&amp;name=" . $file . "\">" . $file . "</a> (" . $strSize . ")</li>";
			}
		}
		$body .= "</ul>";		
		
		echo frameBox("Logs files",$body);
	}
	closedir($dh);	
	
	$name_log = getGetText("name");
	if($name_log != null)
	{
		$path_logfile = $path_log . "/" . $name_log;
		
		if(file_exists($path_logfile))
		{
			echocr("<fieldset><legend>Log file: <b>" . $name_log . "</b></legend>");
			echo "<div class=\"is_log\">";
			$log = file_get_contents($path_logfile);
			$log = htmlencode($log);
			$log = str_replace("&#13;&#10;" , "<br />", $log);
			echo $log;
			echo "</div>";
			echocr("</fieldset>");
		}		
	}
}

// -----------------------------------------
// Page "preview"
// -----------------------------------------

function previewBody()
{
	$url = "index.php";
	
	mainTitle("Preview", "Here you can see the preview.<br />Click <a href=\"" . $url . "\" target=\"_blank\">here</a> to open in a new tab.","preview");		
	echo "<iframe class=\"is_preview\" src=\"". $url . "\" />";
}

// -----------------------------------------
// Page "about"
// -----------------------------------------

function aboutBody()
{
	mainTitle("About", "Here you can view informations about this version of Isis, and the team who create this application","about");
	
	echo "<div class=\"is_about\">";	
	echo "<div class=\"is_about_title\">Isis - Version " . getOption("product.version") . "</div>";
	echo "<div class=\"is_about_subtitle\">Developers:</div>";
	echo "<div class=\"is_about_people\">Berserker</div>";
	echo "<div class=\"is_about_people\">Clodo</div>";
	echo "<div class=\"is_about_subtitle\">Testers:</div>";
	echo "<div class=\"is_about_people\">HostFat</div>";	
	echo "<div class=\"is_about_people\">Megaborg</div>";	
	echo "<div class=\"is_about_people\">Sylfaen</div>";		
	
	echo "<div class=\"is_about_footer\">..and all users who are using and supporting Osiris, Isis, and our community!</div>";
	echo "</div>";
	
	
	
	$body = "";	
	$body .= "PHP safe-mode: <b>" . iif(detectPhpSafeMode(),"true","false") . "</b><br />";
	$body .= "PHP current max_execution_time: <b>" . getMaxExecutionTime() . "</b>, customizable: <b>" . iif(customizableTimeLimit(),"true","false") . "</b><br />";
	$body .= "PHP constant UPLOAD_ERR_EXTENSION: <b>" . iif(UPLOAD_ERR_EXTENSION==8,"true","false") . "</b><br />";
	echo frameBox("System informations",$body);
	
	echocr("<br />");
	echocr("<a class=\"is_button_200\" target=\"_blank\" href=\"?page=phpinfo\">phpinfo()</a>");	
	echocr("<div style=\"clear:both;\"></div>");	
	
	$body = "";
	$path_license = "LICENSE.php";
	$body .= "<div class=\"is_license\">";
	if(file_exists($path_license))
	{
		$license = file_get_contents($path_license);
		$license = htmlencode($license);
		$license = str_replace("&#10;" , "<br />", $license);
		$body .= $license;
	}
	else
	{
		$body .= "LICENSE.php is missing.";
	}	
	echo frameBox("License ( <a target=\"_blank\" href=\"http://www.gnu.org/copyleft/lgpl.html\">link</a> )",$body);
	
	echocr("<div style=\"clear:both;\"></div>");
	
	$body = "";
	$path_license = "CHANGELOG.php";
	$body .= "<div class=\"is_changelogs\">";
	if(file_exists($path_license))
	{
		$license = file_get_contents($path_license);
		$license = htmlencode($license);
		$license = str_replace("&#10;" , "<br />", $license);
		$body .= $license;
	}
	else
	{
		$body .= "CHANGELOGS.php is missing.";
	}
	echo frameBox("Change logs",$body);
	
	echocr("<div style=\"clear:both;\"></div>");
	
	echocr("</div>");
}

// -----------------------------------------
// Page "debug"
// -----------------------------------------

function debugBody()
{
	mainTitle("Debug", "Debug informations, for debugging only.","debug");
	
	echo getArrayTableDump('GLOBAL GET', $_GET); 
	echo getArrayTableDump('GLOBAL POST', $_POST); 
	echo getArrayTableDump('GLOBAL COOKIE', $_COOKIE); 
	echo getArrayTableDump('GLOBAL SERVER', $_SERVER); 
	echo getArrayTableDump('GLOBAL ENV', $_ENV); 
	echo getArrayTableDump('GLOBAL REQUEST', $_REQUEST); 
}

// -----------------------------------------
// Page "Custom Tab"
// -----------------------------------------

function customBody($name, $url)
{
	mainTitle($name, "Click <a href=\"" . $url . "\" target=\"_blank\">here</a> to open in a new tab.","customtab");		
	echo "<iframe class=\"is_customtab\" src=\"". $url . "\" />";
}

// -----------------------------------------
// Core page
// -----------------------------------------

function loginLogged()
{
	if(isset($_SESSION["username"]))
		if($_SESSION["username"] == "admin")
			return true;
			
	$pwd = getGetText("acp_password");
	if($pwd == getOption('server.acp.password'))
	{
		$_SESSION["username"] = "admin";
		return true;
	}
			
	return false;
}

function loginBody()
{
	mainTitle("Welcome to Isis " . getOption("product.version") . " - Administration Control Panel","","home");	
	
	mainTitle("Login", _LANG_LOGIN_WELCOME,"login");
		
	$pwd = getPostText("password");
	if($pwd == getOption('server.acp.password'))
	{
		$_SESSION["username"] = "admin";
		// Redirection
		message('info',_LANG_LOGIN_SUCCESS);		
		
		timedReload();
	}
	else
	{
		if($pwd != null)
			message('error',_LANG_LOGIN_FAILED);		
			
		echo "<form class=\"is_login_form\" action=\"" . getPhpSelf() . "\" method=\"post\">";
		echo "<input type=\"password\" name=\"password\" size=\"20\"/>";
		echo "<input type=\"submit\" value=\""._LANG_LOGIN_SUBMIT."\"/>";
		echo "</form>";
	}
}

function logoutBody()
{
	$_SESSION["username"] = null;
	
	message('info',_LANG_LOGOUT_SUCCESS);		
	
	timedReload();
}

function mainBody()
{	
	$page = validateAlphabetic(getGetText("page"));
	
	if($page == "")
		$page = "home";
	
	/*
	if( ($page != "status") && ($page != "settings") && ($page != "jobs") &&($page != "log") && ($page != "preview") && ($page != "about") && ($page != "logout") && ($page != "debug") )
		$page = "home";
	*/
		
	echo "<div class=\"is_acp_body\">";
	
	// Common layout	
	echo "<div class=\"is_tab_control\">";
	tabButton($page, "home", "Home");	
	tabButton($page, "status", "Status");
	tabButton($page, "settings", "Settings");
	tabButton($page, "jobs", "Jobs");
	tabButton($page, "log", "Log");
	tabButton($page, "preview", "Preview");
	tabButton($page, "about", "About");
	tabButton($page, "logout", "Logout");
	//tabButton($page, "debug", "Debug");
	
	// Custom Tabs
	$list = getOption("server.acp.tabs");
	$lines = explode("\n", $list);
	foreach($lines as $line)
	{
		$line = trim($line);
		if($line != "")
		{
			$values = explode(" ", $line);
			tabButton($page, $values[0], $values[0], "customtab");
		}
	}
	
	echo "</div>";	
	
	flush();
	echo "<div class=\"is_body\">";
	
	if($page == "home")
	{
		homeBody();
	}
	else if($page == "status")
	{
		statusBody();
	}
	else if($page == "settings")
	{
		settingsBody();
	}	
	else if($page == "jobs")
	{
		jobsBody();
	}	
	else if($page == "debug")
	{
		debugBody();
	}
	else if($page == "log")
	{
		logBody();
	}
	else if($page == "preview")
	{
		previewBody();
	}
	else if($page == "about")
	{
		aboutBody();	
	}	
	else if($page == "logout")
	{
		logoutBody();	
	}
	else
	{
		// Custom Tabs
		foreach($lines as $line)
		{
			$line = trim($line);
			if($line != "")
			{
				$values = explode(" ", $line);
				if($page == $values[0])
					customBody($values[0],$values[1]);
			}
		}
	}
	echo "<div style=\"clear:both;\"></div>";
	echo "</div>";
	
	echo "</div>";	
}

function generatePage()
{
	// Checking IP
	//allowedIP();
	
	// Init php session
	session_start();
	
	if(validateAlphabetic(getGetText("page")) == "phpinfo")
	{
		phpinfo();
		arrayDump("_SERVER",$_SERVER);
		die();
	}
	
	// Start xhtml output
	xhtmlHeaderEx("<link href=\"" . getOption("server.virtual_path") . "css/acp.css\" rel=\"stylesheet\" type=\"text/css\" /><script type=\"text/javascript\" src=\"js/acp.js\"></script>", "ACP");
	
	try
	{
	
		// Init language system
		languageInit();	
		
		echo "<noscript>";
		message('warning','You need to activate JavaScript to use this ACP.');
		echo "</noscript>";
		
		// SSL Check
		if( (getOptionBool("server.acp.only_ssl") == true) && (getSSL() == false) )
		{
			message('error',_LANG_NOSSL);		
		}
		else
		{		
			// Login check
			if (loginLogged() == false)
				loginBody();
			else	
				mainBody();		
		}
	}
	catch(Exception $e)
	{
		$error_desc = htmlencode($e->getMessage());
		message("error", $error_desc);
	}
	
	// End xhtml output
	xhtmlFooter();
}



// Calling page generation
generatePage();

?>
