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

class Link
{
	var $raw;
	var $data;
	var $valid;
	
	function __construct($link = "")
	{		
		$this->parse($link);
	}
	
	function get($key)
	{
		if(isset($this->data[$key]))
			return $this->data[$key];
		else
			return "";
	}
	
	function set($key, $value)
	{
		$this->data[$key] = $value;
	}
	
	function parse($link)
	{
		// If $link is empty, we suppose manual creation, already checked. So it's valid.
		if($link == "")
		{
			$this->valid = true;
			return;
		}
		
		$this->raw = $link;
		$this->valid = true;
		
		$result = array();
		if(substr($link,0,9) == "osiris://")
		{
			// Old format.
			$array1 = explode("|",$link);
			
			$this->data["type"] = $array1[1];
			
			if( ($this->data["type"] == "portal") || ($this->data["type"] == "isis") )
			{
				$this->data["portal"] = $array1[2];			
				for($i=3;$i<count($array1);$i++)
				{
					$keyval = $array1[$i];
					$array2 = explode("=",$keyval);
					$key = trim($array2[0]);
					$value = "";
					if(isset($array2[1]))
						$value = urldecode(trim($array2[1]));
						
					if($key != "")
						$this->data[$key] = $value;
				}				
			}
			else if ($this->data["type"] == "file")
			{
				// Done, but not used by Isis.
				$this->data['id'] = $array1[2];
			}
			else if ($this->data["type"] == "url")
			{
				// Partially done, but not used by Isis. Need fix.
				$url = urldecode($array1[2]);				
				$params = urldecode($array1[3]);
				
				if($params != "")
					$url = $url . "?" . $params;
				$this->data['url'] = $url;			
			}
			else if ($this->data["type"] == "skin")
			{
				// Done, but not used by Isis.
				$this->data['path'] = urldecode($array1[2]);
			}
			else
			{
				$this->valid = false;				
			}
		}
		else if(substr($link,0,8) == "osiris:?")
		{
			$params = substr($link,8);
			$array1 = explode("&",$params);
			for($i=0;$i<count($array1);$i++)
			{
				$array2 = explode("=",$array1[$i]);
				$key = trim($array2[0]);
				$value = "";
				if(isset($array2[1]))
					$value = urldecode($array2[1]);
				$this->data[$key] = $value;
			}
		}
		else
		{
			$this->valid = false;			
		}
	}
		
	function export($format = "old")
	{
		if($this->valid == false)
			return "{invalid}";
			
		if($format == "new")
		{
			$out = "";
			foreach($this->data as $key => $value)
			{
				if($out != "")
					$out .= "&";
				$out .= $key . "=" . rawurlencode($value);
			}			
			return "osiris:?" . $out;
		}
		else
		{
			// Old format
			if($this->get("type") == "portal")
			{
				return "osiris://|portal|" . rawurlencode($this->get("portal")) . "|name=" . rawurlencode($this->get("name")) . "|description=" . rawurlencode($this->get("description")) . "|user=" . rawurlencode($this->get("user")) . "|";
			}
			else if($this->get("type") == "isis")
			{
				return "osiris://|isis|" . rawurlencode($this->get("portal")) . "|name=" . rawurlencode($this->get("name")) . "|url=" . rawurlencode($this->get("url")) . "|";
			}
			else if($this->get("type") == "file")
			{
				// Done, but not used by Isis.
				return "osiris://|file|" . rawurlencode($this->get("id")) . "|";
			}
			else if($this->get("type") == "url")
			{
				// Partially done, but not used by Isis. Need fix.
				$x = explode("?",$this->get("url"));
				$url = $x[0];
				$params = "";
				if(isset($x[1]))
					$params = $x[1];
				return "osiris://|url|" . rawurlencode($url) . "|" . rawurlencode($params) . "|";
			}
			else if($this->get("type") == "skin")
			{
				// Done, but not used by Isis.
				return "osiris://|skin|" . rawurlencode($this->get("path")) . "|";
			}
		}
	}	
}


?>