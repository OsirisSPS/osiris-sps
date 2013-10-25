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

// TODO: Fare che scorre la dir "new", e copia il file SOLO SE è una versione più recente.
// Così, non rischio niente se uplodo una versione uguale ma diversa.

defined( '_VALID_ISIS' ) or die( 'Direct Access to this location is not allowed.' );

function detectOsirisType($file, &$type, &$xml)
{
	$tmpPath = 
	$cmd = "7za e \"" . $file . "\" -o/tmp/osiris_extension";
	exec($cmd);
	if(file_exists("/tmp/extension.xml"))
		$type = "extension";
	else if(file_exists("/tmp/plugin.xml"))
		$type = "plugin";
	else if(file_exists("/tmp/skin.xml"))
		$type = "skin";
	else
		return false;	
		
	$xml = file_get_contents("/tmp/" . $type . ".xml");
	return true;
}

function job_addons_check_run()
{
	$checking = false; // If "true", all existing extesions are processed (not files under "new", but files under "files").
	
	message("info","Start addons checking.");	
	logMessage("info","Start addons checking.");	
	
	$database = connectDatabase();
	
	if($checking)
	{
		// Set all addons to "no available". Not scanned ".osiris" will be results unavailable.
		$sql = "update isis_addons set available=0";
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	}
	
	$repositoryPath = "./addons";
	
	$filesNewPath = getCurrentPhysicalPath() . "/addons/new/";
	$filesRepoPath = getCurrentPhysicalPath() . "/addons/files/";
	
	$filesToCheck = $filesNewPath;
	if($checking)                 
		$filesToCheck = $filesRepoPath;	
	
	$dirHandle = @opendir($filesToCheck) or die("Unable to open $filesToCheck");

	while ($file = readdir($dirHandle)) 
	{
		if($file!="." && $file!="..")
		{
			$tmpPath = "/tmp/" . $file;
			
			try
			{
				$currentPath = $filesToCheck . $file;				
				
				$cmd = "rm " . $tmpPath . " -r";
				exec($cmd);
				
				$cmd = "7za x \"" . $currentPath . "\" -o" . $tmpPath;				
				exec($cmd);
				
				$valid = false;
				
				if(file_exists($tmpPath . "/manifest.xml") == false)
					error("manifest.xml missing");
					
				
				$xml = file_get_contents($tmpPath . "/manifest.xml");
				$doc = new SimpleXMLElement($xml);				
				
				$type = $doc->getName();
				$id = str_replace(".osiris","",$file);
				
				$id = $doc['id'];
				$name = $doc['name'];
				$description = $doc['description'];
				$content = $doc['content'];
				$category = $doc['category'];
				$tags = $doc['tags'];
				$trust = $doc['trust'];
				$author = $doc['author'];
				$version = (double) $doc['version'];
				$compatibility = (double) $doc['compatibility'];
				$homepage = $doc['homepage'];
				
				if($compatibility == 0)
					$compatibility = 0.14;
					
				$finalPath = $filesRepoPath . $id . ".osiris";
														
				message("info","Type:[b]" . $type . "[/b], Name: [b]" . $name . "[/b], ID:[b]" . $id . "[/b]");
				
				$sql = "select version from isis_addons where id='" . escapeSql($id) . "'";
				$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
				$row = mysql_fetch_array($result);
				
				if($row)
				{					
					if($checking == false)
					{
						$currentVersion = $row["version"];
												
						if(floatval($version) <= floatval($currentVersion))
						{
							unlink($currentPath);
							error("Version (" . $version . ") same or below the current in the repository (" . $currentVersion . "). Skipped.");
						}
					}
				}
		
				if(!$row)
				{
					$sql = "insert into isis_addons (id) values ('" . escapeSql($id) . "')";
					$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());					
				}
				
				$file = $id . ".osiris";
				
				$sql = "update isis_addons set";
				$sql .= " type='" . escapeSql($type) . "',";
				$sql .= " file='" . escapeSql($file) . "',";
				$sql .= " name='" . escapeSql($name) . "',";
				$sql .= " description='" . escapeSql($description) . "',";
				$sql .= " content='" . escapeSql($content) . "',";
				$sql .= " category='" . escapeSql($category) . "',";
				$sql .= " tags='" . escapeSql($tags) . "',";
				$sql .= " trust='" . escapeSql($trust) . "',";
				$sql .= " author='" . escapeSql($author) . "',";
				$sql .= " version=" . escapeSql($version) . ",";
				$sql .= " compatibility=" . escapeSql($compatibility) . ",";
				$sql .= " homepage='" . escapeSql($homepage) . "',";
				
				$sql .= " filesize=" . filesize($currentPath) . ",";
																
				$sql .= " last_scan=now(), available=1";
				// TODO: Missing "last_update", that must be the lastest modification date of the ".osiris" file.
				$sql .= " where id='" . escapeSql($id) . "'";
				$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());									
				
				$logo = $doc["logo"];
				if($logo != "")
				{
					$logoPath = $tmpPath . "/" . $logo;
					if(file_exists($logoPath))
					{
						copy($logoPath, $repositoryPath . "/logo/" . $id . ".png");
					}				
				}
				
				$icon = $doc["icon"];
				if($icon != "")
				{
					$iconPath = $tmpPath . "/" . $icon;
					if(file_exists($iconPath))
					{
						copy($iconPath, $repositoryPath . "/icon/" . $id . ".png");
					}				
				}
				
				if($finalPath != $currentPath)
				{
					if(file_exists($finalPath))
						unlink($finalPath);
					rename($currentPath, $finalPath);
				}
					
			}
			catch(Exception $e)	
			{
				message("error","File:[b]" . $file . "[/b] - " . $e->getMessage());				
			}
			
			$cmd = "rm " . $tmpPath . " -r";
			exec($cmd);
		}
	}

	closedir($dirHandle);	
	
	disconnectDatabase($database);
	
	return true;
}

function job_addons_check_view()
{
	return "Addons checking.";
}

?>