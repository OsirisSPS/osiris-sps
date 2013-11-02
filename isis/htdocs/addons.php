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

function generateListFromSql($name, $database, &$nodeLists, &$nodeRepository, $sql)
{
	$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	
	$data2 = Array();
	$data2["__tag"] = $name;	
	
	while ($row=mysql_fetch_array($result)) 
	{
		$data3 = Array();
		$data3["__tag"] = "addon";
		$data3["id"] = $row["id"];
		
		addToRepository($row, $nodeRepository);
		
		$data2[] = $data3;
	}
	
	$nodeLists[] = $data2;
}

function addToRepository($row, &$nodeRepository)
{
	$id = $row["id"];
	if(isset($nodeRepository[$id]))
		return;
		
	$nodeAddon = generateAddon($row, "xml");
	
	$nodeRepository[$id] = $nodeAddon;
}

function generateAddon($row, $format)
{	
	$id = $row["id"];
	
	$download = "";	
	$iconHref = "";		
	$logoHref = "";	
	
	$catalogVersion = $row["catalog"];
	
	if($row)
	{
		$download = getOption('server.virtual_path') . "addons/" . $catalogVersion . "/files/" . urlencode($row['file']);			
	
		if(file_exists("./addons/" . $catalogVersion . "/icon/" . $id . ".png"))
			$iconHref = getOption('server.virtual_path') . "addons/" . $catalogVersion . "/icon/" . $id . ".png";		
		
		if(file_exists("./addons/" . $catalogVersion . "/logo/" . $id . ".png"))
			$logoHref = getOption('server.virtual_path') . "addons/" . $catalogVersion . "/logo/" . $id . ".png";
	}
		
	if($format == "html")
	{
		$output = "";
		
		if($row)
		{
			$framed = (getQueryParam("framed","false") == "true");
			
			if($framed == false)
				$output .= ("<div class=\"is_addon\">");
			else
				$output .= ("<div class=\"is_addon_framed\">");
			
			$output .= ("<div class=\"is_addon_detail\" style=\"float:right;\">");
			$output .= ("Author: <b>" . $row['author'] . "</b><br>");
			if($row['category'] != "")
				$output .= ("Category: <b>" . $row['category'] . "</b><br>");
			if($row['tags'] != "")
				$output .= ("Tags: <b>" . $row['tags'] . "</b><br>");
			$output .= ("Version: <b>" . $row['version'] . "</b><br>");			
			if(floatval($row['compatibility']) != 0)
				$output .= ("Compatibility: <b>" . $row['compatibility'] . "</b><br>");
			$output .= ("<div style=\"padding:10px;text-align:center;\">");
			if($row['homepage'])
			{
				$output .= ("<a target=\"_blank\" href=\"" . $row['homepage'] . "\">Home page</a>");
				//$output .= (" | ");
			}
			//$output .= ("<a href=\"" . $download . "\">Download</a>");
			$output .= ("</div>");
			$output .= ("</div>");
			
			$output .= ("<img style=\"float:right;margin:10px;\" src=\"" . htmlencode($logoHref) . "\">");
			
			$output .= ("<img style=\"float:left;margin:10px;\" src=\"" . htmlencode($iconHref) . "\">");
			
			$output .= ("<span style=\"font-size:1.3em\">" . bbencode($row['name']) . "</span><br>");
			$output .= ("<i>" . bbencode($row['description']) . "</i>");

			$output .= ("<div style=\"clear:left;\"></div>");									
			$output .= ("<span>" . bbencode($row['content']) . "</span>");
								
			$output .= ("<div style=\"clear:both;\"></div>");
			$output .= ("</div>");
			
			return $output;
		}
	}
	else if( ($format == "xml") || ($format == "json") )
	{	
		$data = Array();
		$data["__tag"] = "addon";
		$data["id"]=$id;
	
		if($row)
		{
			$data["found"]=true;
			
			$data["type"]=$row["type"];
			$data["active"]=($row["active"] ? "true":"false");
			$data["private"]=($row["private"] ? "true":"false");
			$data["experimental"]=($row["experimental"] ? "true":"false");
			$data["verified"]=($row["verified"] ? "true":"false");
			$data["type"]=$row["type"];
			$data["name"]=$row["name"];
			$data["description"]=$row["description"];
			$data["content"]=$row["content"];
			if($iconHref != "")
				$data["icon_href"]=$iconHref;
			if($logoHref != "")
				$data["logo_href"]=$logoHref;
			$data["category"]=$row["category"];
			$data["tags"]=$row["tags"];
			$data["version"]=$row["version"];
			$data["author"]=$row["author"];
			$data["compatibility"]=$row["compatibility"];
			$data["homepage"]=$row["homepage"];
			$data["trust"]=$row["trust"];
			/*$data["download"]=$download;*/
		}
		else
		{
			$data["found"]=false;
		}
		
		return $data;
	}
}

function main()
{
	try
	{
		// TODO: Implement also Redirect.		
		/*
		if(getOptionBool("services.addons.enabled") == false)
			throw new Exception("Service not available.");
		*/
			
		$database = connectDatabase();
		
		$format = getQueryParam("format","xml");
		$action = getQueryParam("act","home");
		
		$osirisVersion = "0";	
		$userAgent = "";
		if(isset($_SERVER['HTTP_USER_AGENT']))
			$userAgent = $_SERVER['HTTP_USER_AGENT'];
		if(startsWith($userAgent,"Osiris/"))
			$osirisVersion = str_replace("Osiris/","", $userAgent);
			
		$catalogVersion = $osirisVersion;
		
		$catalogVersion = "1.0"; // Last catalog version
				
		if($format == "xml")
			xmlHeader();
		else if($format == "html")
			xhtmlHeader(false);
			
		$debug = "";	
	
		if( ($action == "home") || ($action == "upgradable") )
		{
			$data = Array();
			$data["__tag"] = $action;
			
			$nodeRepository = Array();
			$nodeRepository["__tag"] = "repository";			
			
			if($action == "home")
			{
				$nodeLists = Array();
				$nodeLists["__tag"] = "lists";
						
				generateListFromSql("recommended", $database, $nodeLists, $nodeRepository, "select * from isis_addons where catalog='" . escapeSql($catalogVersion) . "' and available=1 and active=1 and private=0 and experimental=0 and recommended!=0 order by recommended desc, name");
				generateListFromSql("catalog", $database, $nodeLists, $nodeRepository, "select * from isis_addons where catalog='" . escapeSql($catalogVersion) . "' and available=1 and active=1 and private=0 order by name");
			}
			
			
			if(isset($_POST["currents"]))
			{
				if($action == "home")
				{
					$data2 = Array();
					$data2["__tag"] = "upgradable";
				}
						
				$count = 0;
			
				$currents = array_filter(explode(";",$_POST["currents"]));
				foreach($currents as $item)
				{					
					$fields = explode(":",$item);
					
					$id = $fields[0];
					$version = floatval($fields[1]);
										
					$sql = "select * from isis_addons where catalog='" . escapeSql($catalogVersion) . "' and id='" . escapeSql($id) . "' and active=1";
					$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
					$row=mysql_fetch_array($result);
					if($row)
					{
						addToRepository($row, $nodeRepository);
						
						$repositoryVersion = floatval($row["version"]);
						
						$debug .= $id . " = " . $version . "-" . $repositoryVersion . "\n\r";
						
						if($version < $repositoryVersion)
						{
							$count++;
							
							if($action == "home")
							{
								$data3 = Array();
								$data3["__tag"] = "addon";
								$data3["id"] = $id;
					
								$data2[] = $data3;
							}
						}
					}
				}
				
				$data["upgradable_counter"] = $count;
				if($action == "home")
					$nodeLists[] = $data2;
			}
			
			if($action == "home")			
			{
				$data[] = $nodeLists;
				$data[] = $nodeRepository;			
			}
			
			$output = convertToFormat($data, $format);
			echo $output;
			
			file_put_contents("addons/debug.txt",$debug);
		}	
		
		/*
		if($action == "popular")
		{
			//generateListFromSql("popular", $database, $format, "select id from isis_addons where available=1 and active=1 and private=0 and experimental=0 order by category, name");
		}
		else if($action == "experimental")
		{
			//generateListFromSql("experimental", $database, $format, "select id from isis_addons where available=1 and active=1 and private=0 and experimental=1 order by category, name");
		}
		else if($action == "upgradable")
		{
			$data = Array();
			$data["__tag"] = "upgradable";
						
			$count = 0;
			
			$currents = explode(";",$_POST["currents"]);
			foreach($currents as $item)
			{
				$fields = explode(":",$item);
				
				$id = $fields[0];
				$version = (float) $fields[1];
				
				$sql = "select version from isis_addons where id='" . escapeSql($id) . "'";
				$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
				$row=mysql_fetch_array($result);
				if($row)
				{
					$repositoryVersion = (float) $row["version"];
					
					if($version < $repositoryVersion)
					{
						$count++;
						
						$data2 = Array();
						$data2["__tag"] = "addon";
						$data2["id"] = $id;
				
						$data[] = $data2;
					}
				}
			}
			
			$data["count"] = $count;
			
			echo convertToFormat($data, $format);
		}
		*/
		else if($action == "download")
		{
			$id = getQueryParam("id","");
						
			$download = getOption('server.virtual_path') . "addons/" . $catalogVersion . "/files/" . $id . ".osiris";
			
			header("Location: " . $download);
		}
		else if($action == "single")
		{
			$id = getQueryParam("id","");
			
			$sql = "select * from isis_addons where catalog='" . escapeSql($catalogVersion) . "' and id='" . escapeSql($id) . "'";
			$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
			$row=mysql_fetch_array($result);
			
			$output = generateAddon($row, $format);
			
			if($format == "html")
			{								
				echo $output;
			}
			else if( ($format == "xml") || ($format == "json") )
			{
				echo convertToFormat($output, $format);
			}
		}
		
			
		disconnectDatabase($database);
		
	}
	catch(Exception $e)	
	{
		echo "<error msg=\"".$e->getMessage()."\"/>\r\n";
		logMessage("error", $e->getMessage());
	}
	
	if($format == "html")
	{
		$framed = (getQueryParam("framed","false") == "true");
    xhtmlFooter($framed == false);
	}
}

// Calling main code
main();
?>
