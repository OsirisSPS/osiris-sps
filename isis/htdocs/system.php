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
	xmlHeader();
		
	$error_code = 1; // 0 = No error, 1 = Fatal
	$error_desc = "";
	try
	{
		logMessage("notice","Requesting system information");
		
		if($_SERVER["REQUEST_METHOD"] != "POST")
			throw new Exception("Invalid verb.");
		
		$portal = $_POST["portal"];
		$pov = $_POST["pov"];
		$password = $_POST["password"];
		
		allowedPortal($portal,$pov);
		
		$id = composePovId($portal, $pov);
		
		commonPeerChecks($id, $password);
				
		$settings_last_modification = getOption("settings.last_modification");
		if($settings_last_modification == "")
			throw new Exception("Settings empty.");
		
		$error_code = 0;
	}
	catch(Exception $e)
	{
		$error_desc = $e->getMessage();
		logMessage("error", $e->getMessage());
	}
	
	$attributes = "";
	$attributes .= " error_code=\"" . $error_code . "\"";
	if($error_code != 0)
	{		
		$attributes .= " error_desc=\"" . htmlencode($error_desc) . "\"";		
	}		
	
	$output = "<isis type=\"system\"" . $attributes . ">\r\n";	
	
	if($error_code == 0)
	{
		$output .= "\t<options>\r\n";
		$output .= outputOptionSpecial("isis.last_modification", $settings_last_modification);
		$output .= outputOptionSpecial("isis.version", getOption("product.version"));
		$output .= outputOptionSpecial("tuning.request.timeout", getOptionDouble("tuning.request.timeout") * 1000000); // Osiris expect a microseconds value
		
		$output .= outputOption($id, "isis.redirect");
		//$output .= outputOption($id, "isis.output.area.systembar");
		//$output .= outputOption($id, "isis.output.area.footer");
		$output .= outputOption($id, "objects.hot_state_threshold");
		$output .= outputOption($id, "ide.pager.items");
		$output .= outputOption($id, "ide.pager.pages");
		$output .= outputOption($id, "ide.skin.id");
		$output .= outputOption($id, "language.time_offset");
		$output .= outputOption($id, "language.time_dst");
		$output .= outputOption($id, "url.protocols.find_link");
		$output .= outputOption($id, "url.protocols.no_confirm");
		$output .= outputOption($id, "url.protocols.no_redirect");
		$output .= outputOption($id, "privacy.allow_external_images");
		$output .= outputOption($id, "privacy.allow_external_urls");
		$output .= outputOption($id, "anonymity.html_mode");
		$output .= outputOption($id, "anonymity.object_mode");
		$output .= outputOption($id, "anonymity.object_trusted_domains");
		$output .= outputOption($id, "anonymity.object_trusted_types");
		$output .= outputOption($id, "web.redirect_url");
		$output .= outputOption($id, "system.isis");
		
		$list = getOptionPortal("options", $portal);
		$lines = array_filter(explode("\n", $list));
		foreach($lines as $line)
		{
			$values = explode("=", $line);
			
			$key = trim($values[0]);
			$value = trim($values[1]);
			
			$output .= outputOptionSpecial($key, $value);
		}
		
		$output .= "\t</options>\r\n";
	}
	
	$output .= "</isis>";
	
	echo $output;
	
	logMessage("notice","Sending system information");
}

function encodeOptionToXml($value)
{
	if(gettype($value) == "boolean")
	{
		return $value ? "true":"false";
	}
	else
	{
		return htmlencode($value);
	}
}

function outputOption($id, $name)
{
	return "\t\t<option name=\"" . htmlencode($name) . "\" value=\"" . encodeOptionToXml(getOptionPortal($name, $id)) . "\" />\r\n";	
}

function outputOptionSpecial($name, $value)
{
	return "\t\t<option name=\"" . htmlencode($name) . "\" value=\"" . encodeOptionToXml($value) . "\" />\r\n";	
}

// Calling main code
main();
?>