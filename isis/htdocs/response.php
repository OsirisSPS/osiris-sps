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

// In some environment, UPLOAD_ERR_EXTENSION are not defined.
if (!defined('UPLOAD_ERR_EXTENSION')) 
{
	define('UPLOAD_ERR_EXTENSION', 8);
}

function checkUploadError($name, $errorCode)
{
	$uploadErrors = array(
	    UPLOAD_ERR_INI_SIZE => 'The uploaded file exceeds the upload_max_filesize directive in php.ini.',
	    UPLOAD_ERR_FORM_SIZE => 'The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form.',
	    UPLOAD_ERR_PARTIAL => 'The uploaded file was only partially uploaded.',
	    UPLOAD_ERR_NO_FILE => 'No file was uploaded.',
	    UPLOAD_ERR_NO_TMP_DIR => 'Missing a temporary folder.',
	    UPLOAD_ERR_CANT_WRITE => 'Failed to write file to disk.',
	    UPLOAD_ERR_EXTENSION => 'File upload stopped by extension.',
	);

	if($errorCode !== UPLOAD_ERR_OK)
	{	
		if(isset($uploadErrors[$errorCode]))
			throw new Exception("Param " . $name . ": " . $uploadErrors[$errorCode]);
		else
			throw new Exception("Param " . $name . ": Unknown error uploading file.");
	}
}

function readPostParam($name)
{
	checkUploadError($name, $_FILES[$name]['error']);		
			
	$fileSize = $_FILES[$name]['size'];
	
	$output = "";
		
	if($fileSize != 0)
	{			
		$fileNameTemp = $_FILES[$name]['tmp_name'];
				
		if(is_uploaded_file($fileNameTemp) == false)
			throw new Exception("Param " . $name . ": Invalid upload.");
		
		if($_FILES[$name]['type']  == "application/octet-stream")
		{
			$output = file_get_contents($fileNameTemp);
		}
		else if( ($_FILES[$name]['type']  == "gzip") ||
		          ($_FILES[$name]['type']  == "application/x-gzip") )
		{					
			// V1:
			/*
			if(($fileHandle = fopen($fileNameTemp, "rb")) === FALSE)
				throw new Exception("Param " . $name . ": Internal error.");
			fseek($fileHandle, -4, SEEK_END);						
			if(($buf = fread($fileHandle, 4)) === FALSE)
				throw new Exception("Param " . $name . ": Internal error.");						
			$fileUncompressedSizeX = unpack("V", $buf);
			$fileUncompressedSize = end($fileUncompressedSizeX);			
						
			if($fileUncompressedSize == 0)
				throw new Exception("Param " . $name . ": Internal error.");
			fclose($fileHandle);
			
			if(($fileHandle = gzopen($fileNameTemp, "rb")) === FALSE)
				throw new Exception("Param " . $name . ": Internal error.");
			if(($output = gzread($fileHandle, $fileUncompressedSize)) === FALSE)
				throw new Exception("Param " . $name . ": Internal error.");
			gzclose($fileHandle);
			
			createFile("./data/v1.bin", $output);
			*/
			
			// V2:
			$output = "";
			if(($fileHandle = gzopen($fileNameTemp, "rb")) === FALSE)
				throw new Exception("Param " . $name . ": Internal error.");
			for(;;)
			{
				$read = gzread($fileHandle, 1024);
				if(strlen($read) == 0)
				{
					// EOF reached.
					break;
				}
				else
				{
					$output .= $read;
				}

			}			
			gzclose($fileHandle);			
			//createFile("./data/v2.bin", $output); // VERYURGENT
			
		}
		else
		{			
			throw new Exception("Param " . $name . ": Invalid mime-type.");
		}
	}	

	return $output;
}

function main()
{
	xmlHeader();		
		
	$error_code = 1;
	$error_desc = "";
	try
	{
		if($_SERVER["REQUEST_METHOD"] != "POST")
			throw new Exception("Invalid verb.");
			
		$portal = $_POST["portal"];		
		$user = $_POST["user"];
		$password = $_POST["password"];
		$status = $_POST["status"];	
		$response_id = $_POST["request"];	
				
		logMessage("notice", "Incoming job response ( " . $response_id . " )");
		
		allowedPortal($portal, $user);
		
		$id = composePovId($portal, $user);
		
		commonPeerChecks($id, $password);
		
		$error_code = 2; // From here, an exception will be not a fatal error.
		
		if (checkID($response_id) == false)
			throw new Exception('Invalid response id.');	
					
			
		$withBody = true;
		// Strange thing: "www.altervista.org" return an error code '5' in $_FILES['file_body']['error'] if an answer have a file_body of 0 bytes.
		// Note that 5 isn't any error_code in PHP manual: http://it2.php.net/manual/it/features.file-upload.errors.php
		if($_FILES['file_body']['error'] == 5)
			$withBody = false;				
		
		if($_FILES['file_header']['size'] == 0)
			throw new Exception('Invalid header size.');	
		
		$responseHeader = readPostParam("file_header");			
		
		$responseBody = "";		
		if($withBody)
		{
			$responseBody = readPostParam("file_body");			
		}
		
		$path_base = getOption("data.path");
		$path_portal = $path_base . "/portal_" . $id;
				
		$path_base_accepteds = $path_portal . "/accepteds/";
		$path_base_incoming = $path_portal . "/incomings/";
		$path_base_responses = $path_portal . "/responses/";
		
		$path_accepted = $path_base_accepteds . $response_id;
		
		if(file_exists($path_accepted))
		{
			$path_incoming = $path_base_incoming . $response_id;
			
			// Writing job response
			$file_incoming = fopen($path_incoming, 'w');
			writeFileHeader($file_incoming, "response");
			fwrite($file_incoming, $status . "\n");
			fwrite($file_incoming, $responseHeader . "\n\n");
			fwrite($file_incoming, $responseBody);
			fclose($file_incoming);
			
			// Atomic rename		
			$path_response = $path_base_responses . $response_id;
			if(file_exists($path_response))
				unlink($path_response);
			rename($path_incoming, $path_response);
			unlink($path_accepted);
			
			logMessage("notice", "Job response accepted ( " . $response_id . " )");
			
			$error_code = 0;
		}
		else
		{
			throw new Exception('Unexpected response.');		
		}
	}
	catch(Exception $e)
	{
		$error_desc = htmlencode($e->getMessage());
		if($error_code == 1)
			logMessage("warning", $e->getMessage());
		else
			logMessage("notice", $e->getMessage());
	}
			
	$output = "<isis type=\"response\" error_code=\"$error_code\" error_desc=\"$error_desc\"></isis>\r\n";	
	echo $output;
}

// Calling main code
main();
?>