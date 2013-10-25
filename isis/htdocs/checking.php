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

// This PHP file is separated and custom, because need to be written in a PHP4 compatibility mode.

function echocr($msg)
{
	echo $msg . "\r\n";
}

function message($type, $msg)
{	
	echocr("<div class=\"is_message_" . htmlentities($type, ENT_QUOTES, 'UTF-8') . "\">");
	echocr("<img src=\"images/icons/" . htmlentities($type, ENT_QUOTES, 'UTF-8') . ".png\" alt=\"" . htmlentities($type, ENT_QUOTES, 'UTF-8') . "\" />");
	echocr(htmlentities($msg, ENT_QUOTES, 'UTF-8'));
	echocr("<div style=\"clear:both;\"></div>");
	echocr("</div>");
}

echocr("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
echocr("<html xmlns=\"http://www.w3.org/1999/xhtml\">");
echocr("<head>");	
echocr("<title>Isis Environment Checking</title>");
echocr("<link rel=\"shortcut icon\" href=\"images/favicon.ico\" />");
echocr("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />");
echocr("<link href=\"css/screen.css\" rel=\"stylesheet\" type=\"text/css\" />");	
echocr("</head>");
echocr("<body>");	

$error = 0;

if(version_compare("5", PHP_VERSION, "<") == false)
{
	message("warning","Your version of PHP (" . PHP_VERSION . ") is incompatible with Isis. Isis require PHP version 5 or above.");	
	$error++;
}

if(ini_get('safe_mode') == true)
{
	message("warning","PHP Safe-Mode are enabled. Isis can't work, sorry.");	
	$error++;
}

if($error == 0)
{
	message("info","Environment check completed successfully.");	
	
	if(file_exists("acp.php"))
	{
		message("info","You will redirected to the Administration Control Panel in 5 seconds.");
		
		$newUrl = "acp.php";
		echo "<script type=\"text/javascript\">\n";
		echo "<!--	\n";
		echo "	function timedRedirect()\n";
		echo "	{\n";
		echo "	window.location = \"" . $newUrl . "\"\n";
		echo "	}\n";
		echo "	setTimeout('timedRedirect()', 5000);\n";
		echo "//-->\n";
		echo "</script>\n";	
	}
	else
	{
		message("info","You can download and install Isis on this web-server.");
	}
}
else
{
	message("error","Fatal error occur. Isis can't work in this environment. Sorry.");	
}

echocr("</body>");	

?>