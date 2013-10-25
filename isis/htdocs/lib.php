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

defined( '_VALID_ISIS' ) or die( 'Direct Access to this location is not allowed.' );

/* -------------------------------
System Functions
------------------------------- */

function getPhpSelf()
{
	$url = $_SERVER['PHP_SELF'];
	
	// Without path.
	$posS = strrpos($url, '/');
	if($posS !== false)
		$url = substr($url,$posS+1);
	return htmlentities($url, ENT_QUOTES); 	
}

function getPostBackUrl()
{
	return htmlentities($_SERVER["REQUEST_URI"], ENT_QUOTES); 
}

function getCurrentHttpPath($withoutDomain = false)
{	
	$val = $_SERVER["PHP_SELF"];		
	$val = substr($val, 0, strrpos($val, '/')+1);							
	if($withoutDomain == false)
		$val = "http" . iif(getSSL(),"s") . "://" . $_SERVER["HTTP_HOST"] . $val;
	return $val;	
}

function getCurrentPhysicalPath()
{
	$val = $_SERVER["SCRIPT_FILENAME"];
	return substr($val, 0, strrpos($val, "/"));
}

function getSSL()
{
	if(isset($_SERVER["HTTPS"]))
		return ($_SERVER["HTTPS"] == 'on');
	return false;
}

function detectPhpSafeMode()
{
	return ini_get('safe_mode');
}

function getMaxExecutionTime()
{
	return ini_get('max_execution_time');
}	

function randomString($length = 10) 
{ 
	$s = ''; 
	
	for($i = 0 ; $i < $length ; $i++) 
	{ 
		$s .= chr(rand(0,25)+65);
	} 

	return $s; 
}

function randomId()
{
	return time() . randomString(10); 
}

function ensureDir($path, $access = null)
{
	try
	{
		if( ($path == null) || ($path == "") )
			return false;
		if($access == null)
			$access = 0666;
		if(file_exists($path) == false)
			createDir($path, $access);
			
		// Empty "index.html"
		$path2 = $path . "/index.html";
		if(file_exists($path2) == false)
		{
			$file = fopen($path2, 'w');
			fwrite($file, "<html><body bgcolor=\"#FFFFFF\"></body></html>");				
			fclose($file);	
		}
		
		// .htaccess forbidden
		$path2 = $path . "/.htaccess";
		if(file_exists($path2) == false)
		{
			$file = fopen($path2, 'w');
			fwrite($file, "deny from all");				
			fclose($file);	
		}
	}
	catch(Exception $e)
	{
		return false;
	}
	
	return true;
}

function setTimeLimit($sec)
{
	if(getOptionBool("compatibility.use_set_time_limit"))
		set_time_limit($sec);
}

function customizableTimeLimit()
{
	$testValue = 60;
	$original = getMaxExecutionTime();
	set_time_limit($testValue);
	$newValue = getMaxExecutionTime();
	$success = ($newValue == $testValue);
	set_time_limit($original);
	return $success;
}

function createDir($path, $access)
{
	if(mkdir($path, $access) == false)
		throw new Exception("Unable to create a directory: " . $path);
}

function regex_match($regex, $value)
{
	if( ($regex == null) || ($regex == "") )
		return false;
	return preg_match("/".$regex."/i", $value);
}

function regexMatchReplace($val, $regex, $replace)
{
	if(preg_match($regex, $val))
		return preg_replace($regex, $replace, $val);
	else
		return $val;
}

function getDirectorySize($path) 
{ 
	$totalsize = 0; 
	$totalcount = 0; 
	$dircount = 0; 
	if ($handle = opendir ($path)) 
	{ 
		while (false !== ($file = readdir($handle))) 
		{ 
			$nextpath = $path . '/' . $file; 
			if ($file != '.' && $file != '..' && $file != "index.html" && $file != ".htaccess" && !is_link ($nextpath)) 
			{ 
				if (is_dir ($nextpath)) 
				{ 
					$dircount++; 
					$result = getDirectorySize($nextpath); 
					$totalsize += $result['size']; 
					$totalcount += $result['count']; 
					$dircount += $result['dircount']; 
				} 
				elseif (is_file ($nextpath)) 
				{ 
					$totalsize += filesize ($nextpath); 
					$totalcount++; 
				} 
			} 
		} 
	} 
	closedir ($handle); 
	$total['size'] = $totalsize; 
	$total['count'] = $totalcount; 
	$total['dircount'] = $dircount; 
	return $total; 
}

function getFileInfo($path)
{
	return convertBytesToString(filesize($path)) . ", " . relativeTime(filemtime($path));
}

function deleteDirectory($dir) 
{ 
    if (!file_exists($dir)) return true; 
    if (!is_dir($dir) || is_link($dir)) return unlink($dir); 
    foreach (scandir($dir) as $item) 
    { 
        if ($item == '.' || $item == '..') continue; 
        if (!deleteDirectory($dir . "/" . $item)) 
        { 
            chmod($dir . "/" . $item, 0777); 
            if (!deleteDirectory($dir . "/" . $item)) return false; 
        }; 
    };
    
    return rmdir($dir); 
}

function createFile($path, $value)
{
	$fh = fopen($path, 'w');
	fwrite($fh, $value);
	fclose($fh);
}

function getWritable($path)
{
	$isDir = ($path{strlen($path)-1}=='/');
	if(file_exists($path)) // This because is_writable() return false if the file not exists.
	{		
		if(is_writable($path) == false)
			return false;
			
		if($isDir)
		{
			if(mkdir($path . "writetest") == false)
			{
				return false;			
			}
			rmdir($path . "writetest");		
		}
		
		return true;
	}
	else
	{
		if($isDir == false)
		{
			if (!($f = @fopen($path, 'w+')))
				return false;
			fclose($f);
			unlink($path);
			return true;
		}
		else
		{
			if(mkdir($path) == false)
				return false;
				
			if(mkdir($path . "writetest") == false)
				return false;			
				
			rmdir($path . "writetest");		
			rmdir($path);			
			
			return true;
		}
	}
		
	return false;
}

function renameSave($fromPath, $toPath)
{	
	if(trim($fromPath) == "")
		return false;
		
	if(trim($toPath) == "")
		return false;
		
	if(file_exists($fromPath) == false)
		return false;
		
	if(file_exists($toPath))
	{		
		unlink($toPath);
	}
	
	return rename($fromPath, $toPath);
}

/* -------------------------------
Options Functions
------------------------------- */

$OPTIONS = array();

function existsOption($name)
{
	global $OPTIONS;
	return array_key_exists($name,$OPTIONS);
}

function ensureOption($name, $default)
{
	global $OPTIONS;
	$item = array();
	$item["name"] = $name;
	$item["value"] = $default;
	$item["default"] = $default;
	$item["custom"] = false;
	$item["type"] = gettype($default);
	$OPTIONS[$name] = $item;	
}

function setOption($name, $value)
{
	global $OPTIONS;
	$item = null;
	if(array_key_exists($name,$OPTIONS))
		$item = $OPTIONS[$name];
	if($item == null)
	{
		$item = array();
		$item["name"] = $name;
		$item["value"] = $value;
		$item["default"] = null;
		$item["custom"] = true;
		$item["type"] = gettype($value);
		$OPTIONS[$name] = $item;					
		
	}
	else
	{
		// Update, convert type if necessary		
		$expectType = $item["type"];
		if(gettype($value) != $expectType)
		{
			if($expectType == "boolean")
			{
				$value = (intval($value) != 0);				
			}
			else if($item["type"] == "double")
				$value = (double) $value;
			else if($item["type"] == "integer")
				$value = (int) $value;
			else
				throw new Exception("Unknown expected type " . $expectType);		
		}		
		$OPTIONS[$name]["value"] = $value;
		$OPTIONS[$name]["custom"] = true;	
	}
	
	//trace("setOption :");
	//trace($OPTIONS[$name]);
}

function isOptionCustom($name)
{
	global $OPTIONS;
	return $OPTIONS[$name]["custom"];
}

function resetOption($name)
{
	global $OPTIONS;
	if(isset($OPTIONS[$name]))
	{
		$OPTIONS[$name]["value"] = $OPTIONS[$name]["default"];
		$OPTIONS[$name]["custom"] = false;
	}
}

function getOption($name)
{
	global $OPTIONS;
	if(isset($OPTIONS[$name]))
		return $OPTIONS[$name]["value"];
	return null;
}

function getOptionInt($name)
{
	return (int) getOption($name);
}

function getOptionDouble($name)
{
	return (double) getOption($name);
}

function getOptionBool($name)
{
	return (getOption($name) == "true");
}

function matchOption($name, $value)
{
	return regex_match(getOption($name), $value);	
}

/* -------------------------------
Skin Functions
------------------------------- */

function getSkin()
{
	$skin = "";
	if(isset($_GET['skin']))
		$skin = $_GET['skin'];
	// In this case, the skin are also permanently, via cookie.
	if($skin != "")
	{		
		setcookie("isis_skin",$skin);		
		return $skin;
	}
	else if(isset($_COOKIE["isis_skin"]))
	{
		return $_COOKIE["isis_skin"];
	}
	else
	{
		return "";
	}
}

function skinChanger($name)
{
	$params = $_SERVER["QUERY_STRING"];
	if($params != "")
		$params .= "&";
	$params .= "skin=" . urlencode($name);
	
	$url = getPhpSelf() . "?" . $params;
	echocr("<a class=\"os_skin_" . $name . "\" alt=\"" . $name . "\" href=\"" . $url. "\"></a>");
}

/* -------------------------------
Date & Time
------------------------------- */

function queryTimeServer($timeserver, $port=37)
{
	try
	{
		$fp = @fsockopen($timeserver,$port,$err,$errstr,5);
	        # parameters: server, socket, error code, error text, timeout
	  if ($fp) 
	  {
	    fputs($fp,"\n");
	    $timevalue = fread($fp,49);
	    fclose($fp); # close the connection	    
	  }
	  else
	  {
	  	return 0;
	  }
	  
	  $ret = array();
	  $ret[] = $timevalue;
	  $ret[] = $err;     # error code
	  $ret[] = $errstr;  # error text
	  
	  if (!$ret[1]) 
	  {    
	    $timevalue = bin2hex ($ret[0]);
	  	$timevalue = abs (HexDec('7fffffff') - HexDec($timevalue) - HexDec('7fffffff')) ;
	  	$tmestamp = $timevalue - 2208988800; # convert to UNIX epoch time stamp
	  	return $tmestamp;
	  }
	  else
	  {
	  	return 0;
	  }
	}
	catch(Exception $e)
	{
		return 0;
	}
}

function dateIsEmpty($date)
{
	// Questa è sbagliata, è da pulire.
	if(empty($date))
		return true;
	if ($date == "0000-00-00 00:00:00")
		return true;
	return false;
}

function relativeTime($date)
{
	if(dateIsEmpty($date))
		return "";
    
    $periods         = array("second", "minute", "hour", "day", "week", "month", "year", "decade");
    $lengths         = array("60","60","24","7","4.35","12","10");
    
    $now             = time();
    //$unix_date         = strtotime($date);
    $unix_date = $date;
    
       // check validity of date
    if(empty($unix_date)) {    
        return "Bad date";
    }

    // is it future date or past date
    if($now > $unix_date) {    
        $difference     = $now - $unix_date;
        $tense         = "ago";
        
    } else {
        $difference     = $unix_date - $now;
        $tense         = "from now";
    }
    
    for($j = 0; $difference >= $lengths[$j] && $j < count($lengths)-1; $j++) {
        $difference /= $lengths[$j];
    }
    
    $difference = round($difference);
    
    if($difference != 1) {
        $periods[$j].= "s";
    }
    
    return "$difference $periods[$j] {$tense}";
}

/* -------------------------------
Language / Internationals Functions
------------------------------- */

function getLanguageFileName($basePath, $lang)
{
	//$path = $basePath . "languages/".$lang."/".substr(strrchr(getPhpSelf(), "/"), 1);
	$path = $basePath . "languages/".$lang."/".getPhpSelf();	
	return $path;
}

function languageInit($basePath = "")
{	
	$lang = "";
	if(isset($_GET['lang']))
		$lang = $_GET['lang'];
		
	// In this case, the language are also set permanently, via cookie.
	if($lang != "")
	{		
		setcookie("isis_lang",$lang);		
	}
	else if(isset($_COOKIE["isis_lang"]))
		$lang = $_COOKIE["isis_lang"];
	if( ($lang == "") && (isset($_COOKIE["jfcookie"])) )
		$lang = $_COOKIE["jfcookie"]["lang"]; // Joomla / Joomfish
	if( ($lang == "") && isset($_SERVER['HTTP_ACCEPT_LANGUAGE']) )
		$lang = substr($_SERVER['HTTP_ACCEPT_LANGUAGE'],0,2);
			
	$langFileName = getLanguageFileName($basePath, $lang);
	if(!file_exists($langFileName))
		$langFileName = getLanguageFileName($basePath, "en");
	if(file_exists($langFileName))
		include($langFileName);
		
	// Include invariant
	if(file_exists(getLanguageFileName($basePath, "inv")))
		include(getLanguageFileName($basePath, "inv"));	
	
	// Include english
	if(file_exists(getLanguageFileName($basePath, "en")))
		include(getLanguageFileName($basePath, "en"));
}

// Useful to convert language string to html
function langToHtml($text)
{
	$out = str_replace("\r\n","<br />",$text);
	return $out;
}


/* -------------------------------
Logging Functions
------------------------------- */

function logMessage($msgType, $msgDesc)
{
	if(getOption("log.enabled") == "true")
	{
		if($msgDesc == null)
			$msgDesc = "";
			
		try
		{
			$logPath = getOption("data.path") . "/logs";
			$logTimeStamp = getOption("log.timestamp");
			
			$logName = $logPath . "/" . getOption("log.prefix");
			if($logTimeStamp != "")
				$logName .= gmdate($logTimeStamp);
			$logName .= getOption("log.suffix");
			if( ($logName) && ($logName != "") )
			{
				$fh = fopen($logName, 'a');
				if($fh)
				{
					$msgRow = gmdate('F d Y h:i:s A') . " - " . $_SERVER['REMOTE_ADDR'] . " - " . $msgType . " - " . $msgDesc . "\r\n";
					fwrite($fh, $msgRow);
					fclose($fh);
				}
			}
		}
		catch(Exception $e)
		{
			// Last resort: dump in page
			echo $e->getMessage();
		}
	}
}

function isisErrorHandler($errno, $errstr, $errfile, $errline)
{
	logMessage("php " . $errno, $errstr . " in " . $errfile . " (line " . $errline . ")");
}

/* -------------------------------
Checking Functions
------------------------------- */

function checkID($id)
{
	if( ($id == null) || ($id == "") )
		return false;
	return (preg_match('/^[a-z0-9]*$/i', $id) == true);
}

function checkAlphabetic($str)
{
	if( ($str == null) || ($str == "") )
		return false;
	return (preg_match('/^[a-z0-9]*$/i', $str) == true);
}

function checkAlphabeticEx($str)
{
	if( ($str == null) || ($str == "") )
		return false;
	return (preg_match('/^[a-z0-9_\.\-]*$/i', $str) == true);
}

function checkNumeric($str)
{
	if( ($str == null) || ($str == "") )
		return false;
	return (preg_match('/^[	0-9]*$/i', $str) == true);
}

function validateID($id)
{
	if(checkID($id) == true)
		return $id;
	else
		return null;
}

function validateAlphabetic($str)
{
	if(checkAlphabetic($str) == true)
		return $str;
	else
		return null;
}

function validateAlphabeticEx($str)
{
	if(checkAlphabeticEx($str) == true)
		return $str;
	else
		return null;
}

function validateNumeric($str)
{
	if(checkNumeric($str) == true)
		return $str;
	else
		return null;
}

// Controlla se un valore æŸ¡ccettato da due opzioni di white & black list.
function allowedValueList($value, $optionRegexWhite, $optionRegexBlack, $desc)
{
	$regex = getOption($optionRegexWhite);
	if( ($regex != null) && ($regex != "") )
	{
		$result = preg_match("/".$regex."/", $value);
		if($result === false)
			throw new Exception("Syntax error in '" . $optionRegexWhite . "' option.");		
		if($result == 0)
			throw new Exception($desc . " not in whitelist.");		
	}
	
	$regex = getOption($optionRegexBlack);
	if( ($regex != null) && ($regex != "") )
	{
		$result = preg_match("/".$regex."/", $value);
		if($result === false)
			throw new Exception("Syntax error in '" . $optionRegexBlack . "' option.");		
		if($result != 0)
			throw new Exception($desc . " in blacklist.");		
	}
}

function allowedValueListEx($value, $regexWhite, $regexBlack, $desc)
{
	$regex = $regexWhite;
	if( ($regex != null) && ($regex != "") )
	{
		$result = preg_match("/".$regex."/", $value);
		if($result === false)
			throw new Exception("Syntax error in '" . $regexWhite . "' regex.");		
		if($result == 0)
			throw new Exception($desc . " not in whitelist.");		
	}
	
	$regex = $regexBlack;
	if( ($regex != null) && ($regex != "") )
	{
		$result = preg_match("/".$regex."/", $value);
		if($result === false)
			throw new Exception("Syntax error in '" . $regexBlack . "' regex.");		
		if($result != 0)
			throw new Exception($desc . " in blacklist.");		
	}
}

function allowedIP()
{
	$ip = $_SERVER['REMOTE_ADDR'];
	
	allowedValueList($ip, "ips.allowed", "ips.blocked", "IP");	
}

/* -------------------------------
Database Functions
------------------------------- */

function connectDatabase()
{	
	if(getOption("server.database.enabled") == "true")
	{
		$connection = @mysql_connect(getOption("server.database.host"), getOption("server.database.login"), getOption("server.database.password")) or error("Unable to connect to database: ".mysql_error());				
		$database = mysql_select_db(getOption("server.database.database"), $connection) or error("Unable to select the database: ".mysql_error());
		
		$sql = "SET NAMES 'utf8'";
		mysql_query($sql, $connection);
		
		return $connection;
	}
	else
		return null;
}

function disconnectDatabase($db)
{
	if($db != null)
		mysql_close($db);
}

function escapeSql($text)
{
	return mysql_real_escape_string($text);
}

/* -------------------------------
General Helper Functions
------------------------------- */

function phpHttpParamDecode($str)
{
	// $_POST, for example, doesn't return the exact value. To-check in http standard.
	$str = str_replace("\\'","'",$str);
	$str = str_replace("\\\"","\"",$str);
	return $str;
}

function htmlencode($str)
{
	$str = htmlentities($str, ENT_QUOTES, 'UTF-8'); 	
	$str = str_replace("\n","&#10;",$str);	
	$str = str_replace("\r","&#13;",$str);	
	return $str;
}

function htmldecode($str)
{
	$str = html_entity_decode($str, ENT_QUOTES, 'UTF-8');
	return $str;
}

function bbencode($str)
{	
	$output = $str;
	
	$output = htmlentities($output, ENT_QUOTES, 'UTF-8');
	
	$output = str_replace("\n","<br />", $output);
	$output = str_replace("[h1]","<h1>", $output);
	$output = str_replace("[/h1]","</h1>", $output);
	$output = str_replace("[h2]","<h2>", $output);
	$output = str_replace("[/h2]","</h2>", $output);
	$output = str_replace("[h3]","<h3>", $output);
	$output = str_replace("[/h3]","</h3>", $output);
	$output = str_replace("[h4]","<h4>", $output);
	$output = str_replace("[/h4]","</h4>", $output);
	$output = str_replace("[h5]","<h5>", $output);
	$output = str_replace("[/h5]","</h5>", $output);
	$output = str_replace("[h6]","<h6>", $output);
	$output = str_replace("[/h6]","</h6>", $output);
	$output = str_replace("[b]","<strong>", $output);
	$output = str_replace("[/b]","</strong>", $output);
	$output = str_replace("[i]","<i>", $output);
	$output = str_replace("[/i]","</i>", $output);
	$output = str_replace("[code]","<code>", $output);
	$output = str_replace("[code language=&quot;oml&quot;]","<code>", $output);
	$output = str_replace("[/code]","</code>", $output);	
	
	//$output = preg_replace('/\[url=([^\[]+)\]([^\[]+)\[/url\]/i',"<a href=\"${1}\">${2}</a>",$output);
	//$output = preg_replace('/\[url\]([^\[]+)\[/url\]/i',"<a href=\"\\1\">\\1</a>",$output);
	$output = preg_replace('/(\[url=)(.*)(\])(.*)(\[\/url\])/', '<a href="${2}">${4}</a>', $output);
	
	return $output;
}

function echocr($str)
{
	echo $str . "\r\n";
}

function echojs($code)
{
	echocr("<script type=\"text/javascript\">");
	echocr($code);
	echocr("</script>");
}

function echoHtmlVar($var)
{ 
	if(gettype($var) == "boolean")
		return iif($var, "True", "False");
	else
		return htmlencode($var);
}

function error($desc) 
{ 
	throw new Exception($desc);
}

function iif($expression, $returntrue, $returnfalse = '') 
{ 
    return ($expression ? $returntrue : $returnfalse); 
}

function secToMicrosec($sec)
{
	return ($sec * 1000000);
}

function isPostBack()
{
	return ($_SERVER["REQUEST_METHOD"] == "POST");
}

function getPostInt($id, $min, $max)
{
	$val = $_POST[$id];
	if( ($min != null) && ($val<$min) )
		$val = $min;
	if( ($max != null) && ($val>$max) )
		$val = $max;
	return $val;
}

function getPostText($id)
{
	if(isset($_POST[$id]))
		return phpHttpParamDecode($_POST[$id]);
	return "";
}

function getPostBool($id)
{
	return ($_POST[$id] == 'on');
}

function getGetText($id)
{
	if(isset($_GET[$id]))
		return $_GET[$id];
	return "";
}

function getQueryParam($id, $def = "")
{
	if(isset($_GET[$id]))
		$str = $_GET[$id];
	else if(isset($_POST[$id]))
		$str = $_POST[$id];
	else
		return $def;
		
	$str = str_replace("\\'","'",$str);
	$str = str_replace("\\\"","\"",$str);
	$str = str_replace("\\\\","\\",$str);  // 08/06/2009...
	
	return $str;
}

function convertDateFromHtml($date)
{
	if($date == null)
		return null;
	return strtotime($date);
}

function convertDateToHtml($date)
{
	return gmdate('D, d M Y H:i:s', $date) . ' GMT';				
}

function extractStringUntil($str, $delimiters)
{
	$result = strcspn($str, $delimiters);
	return substr($str, 0, $result);
	/*if ($result != strlen($str)) 
	{
		return substr($str, 0, $result);
	}
        return "";*/
}

function extractStringReverseFind($str, $delimeter)
{
	$pos = strpos(strrev($str), strrev($delimeter));
	if ($pos === false)
		return "";
	else
	{
		$pos = strlen($str)-$pos;
		return substr($str, $pos);
	}
}

function convertBytesToString($bytes)
{
	if($bytes == 0)
		return "empty.";
		
	$s = array('bytes', 'KB', 'MB', 'GB', 'TB', 'PB');
	$e = floor(log($bytes)/log(1024));
	$v = $bytes/pow(1024, floor($e));	
	
	return sprintf('%.0f '.$s[$e], $v);
}

function str_replace_once($needle , $replace , $haystack)
{
    // Looks for the first occurence of $needle in $haystack
    // and replaces it with $replace.
    $pos = strpos($haystack, $needle);
    if ($pos === false) {
        // Nothing found
    return $haystack;
    }
    return substr_replace($haystack, $replace, $pos, strlen($needle));
} 

/* -------------------------------
Trace & Debugging
------------------------------- */

function getVarDump($var_value, $var_name = null)
{ 
	$output = "";
	if (is_array($var_value)) 
	{ 		
		$index = 0;
		$output .= "array(<ol>";
		foreach ($var_value as $key => $value) 
		{ 
			/*if($index != 0)
				$output .= ", ";*/
			//$output .= $index . "- ";			
			$output .= "<li>" . getVarDump($value, $key) . "</li>";			
			$index++;
		} 		
		$output .= "</ol>)";
	} 
	else 
	{
		//if($var_name != null)
		$output .= htmlencode($var_name) . "=";
		$output .= "\"" . htmlencode(strval($var_value)) . "\"<sup>" . gettype($var_value) . "</sup>";
	} 
	return $output;
}

function trace($var_value)
{
	echocr("Trace: " . getVarDump($var_value) . "<br />");
	flush();
}

function getArrayTableDump($VARIABLE_NAME, $VARIABLE_ARRAY)
{ 
	if (is_array($VARIABLE_ARRAY)) 
	{ 
		$output = "<table width=\"100%\">"; 
		$output .= "<tr><th>" . $VARIABLE_NAME . "</th><th>Value</th></tr>"; 
		foreach ($VARIABLE_ARRAY as $key => $value) 
		{ 
			$value = arrayDump($key, $value); 
			$output .= "<tr><td>$key</td><td>$value</td></tr>"; 
		} 
		$output .= "</table>"; 
		return $output; 
	} 
	else 
	{
		return strval($VARIABLE_ARRAY);
	} 
} 

/* -------------------------------
Data conversion to string
------------------------------- */
// Xml conversion wrong, works only on simple array without childs.
function convertToFormat($data, $format)
{
	//return trace($data);
	$output = "";
	if($format == "json")
	{
		$output = json_encode($data);
	}
	else if($format == "xml")
	{
		if(is_array($data)) 
		{
			$tag = "";
			if(isset($data["__tag"]))
				$tag = $data["__tag"];
			
			$attr = "";
			$body = "";
									
			foreach ($data as $key => $value) 
			{ 
				$key = strval($key); // ? Otherwise the if below is wrong with $key == 0 ...
				if($key != "__tag")
				{					
					if(is_array($value))
					{						
						$body .= convertToFormat($value, $format);						
					}
					else
					{
						if($attr != "")
							$attr .= " ";
						$attr .= htmlencode($key) . "=\"" . convertToFormat($value, $format) . "\"";					
					}
				}
			}
			
			$output = "<" . htmlencode($tag);
			if($attr != "")
				$output .= " " . $attr;
			if($body != "")
			{
					$output .= ">";
					$output .= $body;
					$output .= "</" . htmlencode($tag) . ">";
			}
			else
				$output .= " />";
		}
		else if(gettype($data) == "boolean")
		{
			if($data)
				$output .= "true";
			else
				$output .= "false";
		}
		else
			$output .= htmlencode(strval($data));
	}
	
	return $output;
}


/* -------------------------------
Document output Functions
------------------------------- */

$initHeader = false;
function commonHeader()
{
	global $init;
	if($init == false)
	{
		// Set the default timezone to use. Available since PHP 5.1.
		date_default_timezone_set('UTC');
	
		// Set custom error handler
		$old_error_handler = set_error_handler("isisErrorHandler");
	
		if(getOptionBool("server.gzip.enabled") == true)
			ob_start("ob_gzhandler");
		
		$init = true;
	}
}

function xmlHeader()
{
	commonHeader();
	header("Content-type: text/xml; charset=UTF-8");
	echocr("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
}

function xmlFooter()
{
}

function xhtmlHeader($acp = null, $basePath = "")
{	
	if($acp != null)
		xhtmlHeaderEx("<link href=\"" . getOption("server.virtual_path") . "css/acp.css\" rel=\"stylesheet\" type=\"text/css\" /><script type=\"text/javascript\" src=\"js/acp.js\"></script>", "ACP");
	else
		xhtmlHeaderEx("", "");
}

function xhtmlHeaderEx($extraHead, $extraTitle, $basePath = "")
{
	commonHeader();
		
	$skin = "css/skin_" . getSkin() . ".css";
	
	header("Content-type: text/html; charset=UTF-8");
	//echocr("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
	echocr("<!DOCTYPE html>");
	//echocr("<html xmlns=\"http://www.w3.org/1999/xhtml\">");
	echocr("<html>");
	echocr("<head>");	
	echo "<title>" . getOption("product.title");
	if( ($extraTitle != null) && ($extraTitle != "") )
		echo " - " . $extraTitle;
	echo "</title>";		
		
	echocr("<meta name=\"description\" content=\"Osiris - Serverless Portal System\" />");
	echocr("<meta name=\"keywords\" content=\"Osiris, Serverless, Portal, Forum, P2P, CMS, Content Management System, Anarchist, Monarchist, Free, Freeware, Censorship, Portali, Anarchico, Monarchico, Libero, Gratuito, Censura\" />");
	echocr("<meta name=\"Generator\" content=\"" . getOption("product.name") . "\" />");
	echocr("<meta name=\"robots\" content=\"index, follow\" />");
	echocr("<link rel=\"shortcut icon\" href=\"images/favicon.ico\" />");
	echocr("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />");
	echocr("<link href=\"" . getOption("server.virtual_path") . $basePath . "css/screen.css\" rel=\"stylesheet\" type=\"text/css\" />");	
	if(file_exists($skin))
		echocr("<link href=\"" . getOption("server.virtual_path") . $basePath . $skin . "\" rel=\"stylesheet\" type=\"text/css\" />");
	//echocr("<script src=\"" . getOption("server.virtual_path") . "js/prototype.js\" type=\"text/javascript\"></script>");
	//echocr("<script src=\"" . getOption("server.virtual_path") . "js/scriptaculous/scriptaculous.js\" type=\"text/javascript\"></script>");
	echocr("<script src=\"" . getOption("server.virtual_path") . "js/jquery.js\" type=\"text/javascript\"></script>");
	echocr($extraHead);	
	echocr("</head>");
	
	echocr("<body>");	
}

function xhtmlFooter($withFooter = true)
{
	if($withFooter)
	{
		echocr("<div id=\"isis_footer\" class=\"footer\">");
		echocr("<a href=\"" . getOption("product.web_url") . "\" target=\"_blank\"><img src=\"" . getOption("server.virtual_path") . "images/footer_isis.png\" alt=\"" . getOption("product.web_title") . "\" style=\"margin: 5px 16px;\" /></a>");
		echocr("<a href=\"http://validator.w3.org/check/referer\" target=\"_blank\"><img src=\"" . getOption("server.virtual_path") . "images/footer_valid_html.png\" alt=\"Valid HTML!\" style=\"margin: 5px 16px;\" /></a>");
		echocr("<a href=\"http://jigsaw.w3.org/css-validator/check/referer\" target=\"_blank\"><img src=\"" . getOption("server.virtual_path") . "images/footer_valid_css.png\" alt=\"Valid CSS!\" style=\"margin: 5px 16px;\" /></a>");
		if(getSSL())
			echocr("<a href=\"http://wp.netscape.com/eng/ssl3/\" target=\"_blank\"><img src=\"" . getOption("server.virtual_path") . "images/footer_ssl.png\" alt=\"SSL encryption\" style=\"margin: 5px 16px;\" /></a>");	
		echocr("</div>");
	}
			
	echocr("</body>");
	echocr("</html>");
}

/* -------------------------------
IDE Functions
------------------------------- */

function tabButton($page, $name, $title, $icon = null, $action = null)
{
	if($icon == null)
		$icon = $name;
	echo "<a class=\"" . iif($page==$name,"is_tab_focus","is_tab_normal") . "\" href=\"?page=" . urlencode($name);
	if($action != null)
		echo "&action=" . urlencode($action);
	echo "\">";
	echo "<img src=\"images/icons/" . urlencode($icon) . ".png\" style=\"width:16px;height:16px;\" alt=\"" . htmlencode($title) . "\" />";
	echo htmlencode($title);
	echo "</a>";
}

function mainTitle($title, $notes, $icon)
{
	echo "<div class=\"is_main_titlebox\">";
	echo "<img style=\"float:left;\" src=\"images/icons/" . htmlencode($icon) . ".png\" alt=\"" . htmlencode($icon) . "\" />";
	echo "<div class=\"is_main_title\">" . $title . "</div>";
	echo "<div class=\"is_main_notes\">" . $notes . "</div>";
	echo "<div style=\"clear:both\"></div>";
	echo "</div>";
}

function message($type, $msg)
{	
	echo "<div class=\"is_message_" . htmlencode($type) . "\">";
	echo "<img src=\"images/icons/" . htmlencode($type) . ".png\" alt=\"" . htmlencode($type) . "\" />";
	echo bbencode($msg);
	echo "<div style=\"clear:both;\"></div>";
	echo "</div>";
}

function linkBox($icon, $msg, $href)
{
	echo "<div class=\"is_link\">";
	echo "<img src=\"images/icons/" . htmlencode($icon) . ".png\" alt=\"" . htmlencode($icon) . "\" />";
	echo "<a href=\"" . $href . "\">" . bbencode($msg) . "</a>";
	echo "</div>";
}

function dataBox($icon, $html, $classCss = "")
{
	$output = "";
	$output .= "<div class=\"is_link " . $classCss . "\">";
	$output .= "<img style=\"float:left\" src=\"" . getOption("server.virtual_path") . "images/icons/" . htmlencode($icon) . ".png\" alt=\"" . htmlencode($icon) . "\" />";
	$output .= $html;	
	$output .= "<div style=\"clear:both;\"></div>";
	$output .= "</div>";
	return $output;	
}

function codeBox($data)
{
	echo "<div class=\"is_log\">";		
	$data = htmlencode($data);
	$data = str_replace("&#13;&#10;","<br />", $data);	
	$data = str_replace("&#10;","<br />", $data);	
	$data = str_replace("&#13;","<br />", $data);	
	echo $data;
	echo "</div>";
}

include("options.php");
?>