<?php
//ò
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


// ---------------------------------------------------------------------------------------
// Osiris request
// ---------------------------------------------------------------------------------------

function actionOsiris($database)
{
	// Reading variables
	$now = time();
	$ip = $_SERVER['REMOTE_ADDR'];
	$portal = validateID($_GET["portal"]);
	$port = validateNumeric($_GET["port"]);
	$peers = validateNumeric($_GET["peers"]);
	$output = "";		
	
	if(!$portal) // Per compatibilità 0.11
		$portal = validateID($_GET["portal_id"]);
	
	// Checking variables
	if (!$ip) {  error ("Unable to detect IP address"); }
	if (!$portal) {  error ("Param 'portal' not found or invalid"); }
	if ($peers == null) { $peers = 10; }
	if ($peers<0) { $peers=0; }
	if ($peers>100) { $peers=100; }	
	
	if($port != null)
	{
		// Save request information
		$sql = "select 1 from isis_bootstrap_nodes where portal_id='$portal' and ip='$ip' and port=$port";
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		$row = mysql_fetch_array($result);
		
		if(!$row)
		{
			$sql = "insert into isis_bootstrap_nodes (portal_id,ip,port,request_date,validation_date,check_date) values ('$portal','$ip',$port,FROM_UNIXTIME('$now'),null,null)";
			$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		}
		else
		{
			$sql = "update isis_bootstrap_nodes set request_date=FROM_UNIXTIME('$now'), check_date=null where portal_id='$portal' and ip='$ip' and port=$port";
			$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		}
		
		// Save portal information
		$sql = "select 1 from isis_bootstrap_portals where portal_id='$portal'";
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		$row = mysql_fetch_array($result);
		
		if (!$row)
		{
			$sql = "insert into isis_bootstrap_portals (portal_id,name,description,first_request,last_request) values ('$portal','','',FROM_UNIXTIME('$now'),FROM_UNIXTIME('$now'))";
			$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		}
		else
		{
			$sql = "update isis_bootstrap_portals set last_request = FROM_UNIXTIME('$now') where portal_id='$portal'";
			$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		}	
	}
	
	$output=$output."<response>\r\n";
	
	if ($peers>0)
	{
		// Extract results
		$where = "";
		if($port != null)
		{
			$where = $where . " and not (ip='$ip' and port=$port) ";
		}			
		$sql = "select ip,port,validation_date from isis_bootstrap_nodes where portal_id='$portal' " . $where . " order by validation_date desc, request_date desc limit $peers";
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		
		$output=$output."	<ips>\r\n";
		
		while ($row=mysql_fetch_array($result)) 
		{
			$output=$output."		<ip address='".$row['ip']."' port='".$row['port']."' validation_date='".$row['validation_date']."' />\r\n";			
		}
		
		$output=$output."	</ips>\r\n";
	}
	
	$output=$output."</response>\r\n";
	
	echo $output;
}

// ---------------------------------------------------------------------------------------
// Validation
// ---------------------------------------------------------------------------------------

function checkPort($portal,$ip, $port, $timeout)
{
	if ($timeout == null) { $timeout=10; }
	if ($timeout<1) { $timeout=1; }
	if ($timeout>20) { $timeout=20; }	

	if ($ip and $port and $timeout) 
	{
		return @fsockopen("$ip", $port, $errno, $errstr, $timeout);
	}
	else
	{
		return false;
	}
}

function actionValidation($database)
{
	$timeout = validateNumeric($_GET["timeout"]);
	if ($timeout == null) { $timeout=10; }
	if ($timeout<1) { $timeout=1; }
	if ($timeout>20) { $timeout=20; }	
	
	$ntest = validateNumeric($_GET["ntest"]);
	if ($ntest == null) { $ntest=10; }
	if ($ntest<1) { $ntest=1; }
	// if ($ntest>20) { $ntest=20; }	
	
	$portal = $_GET["portal_id"];
	$ip = $_GET["ip"];
	$now = time();
	
	$serverIP = $_SERVER['SERVER_ADDR'];
	$requestIP = $_SERVER['REMOTE_ADDR'];
	
	//echo "<div class=\"main\">";
	message("info","Validation of [b]".htmlencode($ntest)."[/b] ip (max) with [b]".htmlencode($timeout)."[/b] seconds timeout.");
	
	if($serverIP != $requestIP)
	{
		message("error","Requested from IP: [b]" . htmlencode($requestIP) . "[/b]; only request from the server ([b]" . htmlencode($serverIP) . "[/b]) are accepted.");
	}
	else
	{	
		flush();
		
		echo "<table class=\"os_table\">";
		echo "<tr>";
		echo "<th>"."Portal"."</th>";
		echo "<th>"."Ip"."</th>";
		echo "<th>"."Port"."</th>";
		echo "<th>"."Last validation"."</th>";
		echo "<th>"."Last check"."</th>";
		echo "<th>"."Result"."</th>";
		echo "</tr>";
		
		flush();
		
		$sql = "select portal_id,ip,port,last_validation_date,check_date from isis_bootstrap_nodes ";
		if( ($portal) && ($ip) ) $sql = $sql." where portal_id = '".$portal."' and ip = '".$ip."' ";
		if( (!$portal) && ($ip) ) $sql = $sql." where ip = '".$ip."' ";
		if( ($portal) && (!$ip) ) $sql = $sql." where portal_id = '".$portal."' ";
		
		$sql = $sql." order by validation_date desc, check_date asc limit ".$ntest;
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
				
		while ($row=mysql_fetch_array($result)) 
		{
			$portal = $row['portal_id'];
			$ip = $row['ip'];
			$port = $row['port'];
			$validation_date = $row['last_validation_date'];
			$check_date = $row['check_date'];
			if ($port == null) { $port=6060; }
			
			echo "<tr>";
			echo "<td>".$portal."</td>";
			echo "<td>".$ip."</td>";
			echo "<td>".$port."</td>";
			echo "<td>".$validation_date."</td>";
			echo "<td>".$check_date."</td>";	
			
			$check = checkPort($portal,$ip, $port, $timeout);
			$action = "";
			
			if($check)
			{
				$action = "<img style=\"width:16px;height:16px;\" src=\"images/icons/success.png\" alt=\"Success!\" title=\"Success!\">";
				$sql = "update isis_bootstrap_nodes set last_validation_date=FROM_UNIXTIME('$now'), validation_date=FROM_UNIXTIME('$now') where portal_id='$portal' and ip='$ip' and port=$port";
				$result2 = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
			}
			else
			{
				$action = "<img style=\"width:16px;height:16px;\" src=\"images/icons/failed.png\" alt=\"Failed.\" title=\"Failed.!\">";
				$sql = "update isis_bootstrap_nodes set validation_date=null where portal_id='$portal' and ip='$ip' and port=$port";
				$result2 = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
			}
			$now = time();
			$sql = "update isis_bootstrap_nodes set check_date=FROM_UNIXTIME('$now') where portal_id='$portal' and ip='$ip' and port=$port";
			$result3 = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
				
			echo "<td>".$action."</td>";
			echo "</tr>";
			flush();	
		}
		
		echo "</table>";
		
		message("info","Checking completed.");
		
		flush();
		
		//echo "</div>";
		
		$cleanRequestLimit = getOptionInt("clean_request_limit");
		message("info","Clean all nodes that have request_date older by " . $cleanRequestLimit . " days...");
		flush();
		//$sql = "delete from isis_bootstrap_nodes where request_date <= DATE_SUB( CURDATE( ) , INTERVAL " . $cleanRequestLimit . " DAY ) ";
		//$result4 = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		
		$cleanPortalsLimit = getOptionInt("clean_portals_limit");
		message("info","Clean all portals that doesn't have a name, and isn't alive by " . $cleanPortalsLimit . " days...");
		flush();
		//$sql = "delete from isis_bootstrap_portals where name = '' and last_request <= DATE_SUB(CURDATE(), INTERVAL " . $cleanPortalsLimit . " DAY ) ";
		//$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
		
		message("info","Clean all nodes that aren't in a known portal...");
		flush();
		//$sql = "delete from isis_bootstrap_nodes where portal_id not in (select portal_id from isis_bootstrap_portals)";
		//$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	}
}

/*
// ---------------------------------------------------------------------------------------
// Naming
// ---------------------------------------------------------------------------------------

function actionNaming($database)
{
	// Reading variables
	$now = time();
	$ip = $_SERVER['REMOTE_ADDR'];
	$portal_id = $_GET["portal_id"];
	$name = $_GET["name"];
	$description = $_GET["description"];
	$output = "";
	
	// Checking variables
	if (!$portal_id) {  error ("Param 'portal_id' not found or invalid"); }
	if (!$name) {  error ("Param 'name' not found or invalid"); }
	
	// Save request information
	$sql = "select count(*) from isis_bootstrap_portals where portal_id='$portal_id'";
	$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	$row = mysql_fetch_array($result);
	
	if ($row['count(*)']==0)
	{
		$sql = "insert into isis_bootstrap_portals (portal_id,name,description) values ('$portal_id','$name','$description')";
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	}
	else
	{
		$sql = "update isis_bootstrap_portals set name='$name', description='$description' where portal_id='$portal_id'";
		$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	}
	
	$output=$output."<response>\r\n";
	
	$output=$output."</response>\r\n";
	
	echo $output;
}
*/
// ---------------------------------------------------------------------------------------
// Stats
// ---------------------------------------------------------------------------------------

// -----------------------------------------
// Common Functions
// -----------------------------------------

function value_from_sql($conn,$sql)
{
	$result = mysql_query($sql,$conn) or error("Sql error: ".mysql_error());
	$row = mysql_fetch_array($result);
	
	if($row)
		return $row[0];
	else
		return null;
}

function statsHeaderTitle($str)
{
	echocr("<tr><th colspan=\"2\">" . htmlencode($str) . "</th></tr>");
}

function statsHeaderNotes($str)
{
	echocr("<tr><td colspan=\"2\">" . htmlencode($str) . "</td></tr>");
}

function statsSingleQuery($conn,$label, $sql, $valueSuffix = "")
{	
	$value = value_from_sql($conn,$sql);
	echocr("<tr><td class=\"os_label\">".htmlencode($label)."</td><td class=\"os_value\">".htmlencode($value . $valueSuffix)."</td></tr>");
}

function statsPortalControl($row)
{
	$monarchist = ereg('.1..............................................',$row['portal_id']);
	$public = ereg('0...............................................',$row['portal_id']);
	
	$output = "<div>";	
	$portaltype = "";
	$icontype = "";
	if($monarchist)
	{
		$portaltype = $portaltype."monarchist";
		$icontype = $icontype."monarchist";
	}
	else
	{
		$portaltype = $portaltype."anarchist";
		$icontype = $icontype."anarchist";
	}
	if($public)
	{
		$portaltype = $portaltype." , public";
		$icontype = $icontype."_public";
	}
	else
	{
		$portaltype = $portaltype." , private";
		$icontype = $icontype."_private";
	}
	$icontype = "images/reports_portals_".$icontype.".png";
	$name = $row['portal_name'];
	$description = $row['portal_description'];
	$viewname = $name;
	if($viewname == null)
		$viewname = "<unknown name>";
	if($description == null)
		$description = "";
	
	$output=$output."<img style=\"float:left;\" title=\"".$portaltype."\" src=\"".$icontype."\" />";
	
	// Actions
	$invitelink = "";
	$output=$output."<a style=\"float:right;\" target=\"_new\" title=\"Search with Google\" href=\"http://www.google.com/search?q=".$row['portal_id']."\"><img src=\"images/reports_google.png\"/></a>";
	if($monarchist)
	{
		$invitelink = "invitelink.php?url=osiris://|portal|".urlencode($row['portal_id'])."|name=".urlencode($name)."|description=".urlencode($description)."|";
		$output=$output."<a style=\"float:right;\" target=\"_new\" title=\"Monarchist invite Link\" href=\"".$invitelink."\"><img src=\"images/reports_invitelink.png\"/></a>";
	}	
	
	if($invitelink != "")
		$output .= "<a href=\"" . $invitelink . "\">";
	$output=$output."<b>".htmlencode($viewname)."</b><br/><span style=\"font-size:0.9em\">";
	if($invitelink != "")
		$output .= "</a>";
	if($description != "")
		$output = $output . htmlencode($description) . "<br/>";
	$output = $output . "<i>ID: " . htmlencode($row['portal_id']) . "</i></span>";
	
	$output=$output."</div>";
	
	return $output;
}

// General page
function statsGeneralBody($database)
{
	mainTitle("General statistics",bbencode("Here you can view some general statistics.\nUn nodo è inteso come un client Osiris disponibile per un portale. Un singolo client Osiris registrato a 20 portali sono considerati 20 nodi.\nUn client è inteso come un'installazione Osiris, indipendentemente da quanti portali quel client condivide."),"stats_general");
	
	echocr("<table style=\"width:100%\">");
		
	statsHeaderTitle("Statistiche sui portali");
	
	statsSingleQuery($database,"N.portali conosciuti","select count(*) from isis_bootstrap_portals");
	statsSingleQuery($database,"N.portali con almeno un nodo","select count(distinct portal_id) from isis_bootstrap_nodes");
	statsSingleQuery($database,"N.portali con almeno un client raggiungibile","select count(distinct portal_id) from isis_bootstrap_nodes where validation_date is not null");
		
	statsHeaderTitle("Statistiche sui nodi");
	
	statsSingleQuery($database,"N.nodi","select count(ip) from isis_bootstrap_nodes");
	statsSingleQuery($database,"N.nodi raggiungibili","select count(ip) from isis_bootstrap_nodes where validation_date is not null");
	statsSingleQuery($database,"Percentuale di nodi raggiungibili","select 100 * ( select count(ip) from isis_bootstrap_nodes where validation_date is not null ) / ( select count(ip) from isis_bootstrap_nodes )", " %");
	
	statsSingleQuery($database,"N.nodi mai controllati","select count(ip) from isis_bootstrap_nodes where check_date is null");
	statsSingleQuery($database,"N.nodi mai validati come raggiungibili","select count(ip) from isis_bootstrap_nodes where last_validation_date is null");
			
	statsHeaderTitle("Statistiche sui client");
	
	statsSingleQuery($database,"N.client","select count(distinct ip,port) from isis_bootstrap_nodes");
	statsSingleQuery($database,"N.client raggiungibili","select count(distinct ip,port) from isis_bootstrap_nodes where validation_date is not null");
	statsSingleQuery($database,"Percentuale di client raggiungibili","select 100 * ( select count(distinct ip,port) from isis_bootstrap_nodes where validation_date is not null ) / ( select count(distinct ip,port) from isis_bootstrap_nodes )", " %");
	
	statsHeaderTitle("Statistiche sui controlli");
	statsSingleQuery($database,"Data di notifica di presenza più recente","select max(request_date) from isis_bootstrap_nodes");		
	statsSingleQuery($database,"Data di notifica di presenza più vecchia","select min(request_date) from isis_bootstrap_nodes");		
	statsSingleQuery($database,"Data di validità più recente","select max(last_validation_date) from isis_bootstrap_nodes");		
	statsSingleQuery($database,"Data di validità più vecchia","select min(last_validation_date) from isis_bootstrap_nodes where last_validation_date is not null");		
	statsSingleQuery($database,"Data controllo più recente","select max(check_date) from isis_bootstrap_nodes");
	statsSingleQuery($database,"Data controllo più vecchio","select min(check_date) from isis_bootstrap_nodes");		
	
	echocr("</table>");
}

// Portals page
function statsPortalsBody($database)
{
	// Reading variables
	$now = time();
	$output = "";	
	
	$validationRangeYear = time() - 60*60*24*365;
	$validationRangeMonth = time() - 60*60*24*30;
	$validationRangeDay = time() - 60*60*24*1;
	
	mainTitle("Known portals",bbencode("[b]Peers[/b] indica il numero totale di nodi conosciuti.\n[b]Peers now[/b] indica il numero di nodi raggiungibili ora.\n[b]Peers last day[/b] indica il numero di nodi raggiungibili/validati nelle ultime 24 ore.\n[b]Peers last month[/b] indica il numero di nodi raggiungibili/validati negli ultimi 30 giorni.\n[b]Peers last year[/b] indica il numero di nodi raggiungibili/validati negli ultimi 365 giorni.\n[b]Born date[/b] indica la prima richiesta di indirizzi di bootstrap su quel portale.\n[b]Alive date[/b] indica l'ultima richiesta di indirizzi di bootstrap su quel portale."),"stats_portals");

	// Defaults
	$filterOnlyKnown = true;
	$filterText = "";
	$orderMode = "peers_desc";
	if(isPostBack() == true)
	{
		$filterOnlyKnown = getPostBool("chkFilterOnlyKnown");
		$filterText = getPostText("txtFilterText");
		$orderMode = getPostText("cboOrdering");
	}
	
	$output = $output . "<div class=\"os_filters\">";
	$output = $output . "<form method=\"post\" action=\"" . $PHP_SELF . "\">";
	$output = $output . "Search: <input type=\"input\" name=\"txtFilterText\" value=\"" . $filterText . "\" />";
	$output = $output . " | Only known portals: <input type=\"checkbox\" name=\"chkFilterOnlyKnown\" " . iif($filterOnlyKnown,"checked=\"checked\"") . "/>";
	$output = $output . " | Ordering: <select name=\"cboOrdering\">";
	$output = $output . "<option value=\"peers_desc\" ". iif($orderMode=="peers_desc","selected=\"selected\"") . ">Peers - Descending</option>";
	$output = $output . "<option value=\"born_desc\" ". iif($orderMode=="born_desc","selected=\"selected\"") . ">Born date - Descending</option>";
	$output = $output . "<option value=\"alive_desc\" ". iif($orderMode=="alive_desc","selected=\"selected\"") . ">Alive date - Descending</option>";
	$output = $output . "</select>";
	$output = $output . " | <input type=\"submit\" name=\"cmdSearch\" value=\"Search\"/>";
	$output = $output . "</form>";
	$output = $output . "</div>";		
	
	$output=$output."<table cell-padding=\"5px\"; style=\"font-size:0.8em;width:100%;\">\r\n";	
	
	// Extract results
	/*
	$sql = 
	"SELECT " . 
	"t1.portal_id, " .
	"isis_bootstrap_portals.name,  " .
	"isis_bootstrap_portals.description,  " .
	"count( * ) peers,  " .
	"(select count(*) from isis_bootstrap_nodes t2 where t2.portal_id = t1.portal_id and t2.validation_date>=FROM_UNIXTIME('$validationRange')) peers_24h, " .
	"min( creation_date ) born_date,  " .
	"max( request_date ) alive_date " . // Dovrebbe essere max(validation_date)....
	"FROM isis_bootstrap_nodes t1 left join isis_bootstrap_portals on t1.portal_id=isis_bootstrap_portals.portal_id ".	
	"GROUP BY portal_id ".
	"ORDER BY peers DESC";
	*/
	
	$sql = 
	"select " .
	"portal_id, " .
	"name portal_name, " .
	"description portal_description, " .
	"(select count(*) from isis_bootstrap_nodes t2 where t1.portal_id=t2.portal_id) peers, " .
	"(select count(*) from isis_bootstrap_nodes t2 where t1.portal_id=t2.portal_id and t2.validation_date is not null) peers_now, " .
	"(select count(*) from isis_bootstrap_nodes t2 where t1.portal_id=t2.portal_id and t2.last_validation_date>=FROM_UNIXTIME('$validationRangeDay')) peers_day, " .
	"(select count(*) from isis_bootstrap_nodes t2 where t1.portal_id=t2.portal_id and t2.last_validation_date>=FROM_UNIXTIME('$validationRangeMonth')) peers_month, " .
	"(select count(*) from isis_bootstrap_nodes t2 where t1.portal_id=t2.portal_id and t2.last_validation_date>=FROM_UNIXTIME('$validationRangeYear')) peers_year, " .
	"first_request born_date, " .
	"last_request alive_date " .
	"from isis_bootstrap_portals t1 ";
	
	$where = "";
	if($filterText != "")
	{
		$filterText = escapeSql($filterText);
		if($where != "") $where = $where . " and ";
		$where = $where . " (t1.name like '%" . $filterText . "%' or t1.description like '%" . $filterText . "%') ";
	}	
	if($filterOnlyKnown == true)
	{
		if($where != "") $where = $where . " and ";
		$where = $where . " t1.name != '' ";
	}
	
	if($where != "") 
		$sql = $sql . " where " . $where;
		
	if($orderMode == "peers_desc")
		$sql = $sql . "order by peers desc";
	else if($orderMode == "born_desc")
		$sql = $sql . "order by first_request desc";
	else if($orderMode == "alive_desc")
		$sql = $sql . "order by last_request desc";		
	
	$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	
	$output=$output."<tr>";
	$output=$output."<th>"."Portal"."</th>";
	$output=$output."<th>"."Peers"."</th>";
	$output=$output."<th>"."Peers now"."</th>";
	$output=$output."<th>"."Peers last day"."</th>";
	$output=$output."<th>"."Peers last month"."</th>";
	$output=$output."<th>"."Peers last year"."</th>";
	$output=$output."<th>"."Born date"."</th>";
	$output=$output."<th>"."Alive date"."</th>";
	$output=$output."</tr>";
	
	while ($row=mysql_fetch_array($result)) 
	{
		$monarchist = ereg('.1..............................................',$row['portal_id']);
		$public = ereg('0...............................................',$row['portal_id']);
		
		$output=$output."<tr>";
		$output=$output."<td>" . statsPortalControl($row) . "</td>";	
		$output=$output."<td>".$row['peers']."</td>";
		$output=$output."<td>".$row['peers_now']."</td>";
		$output=$output."<td>".$row['peers_day']."</td>";
		$output=$output."<td>".$row['peers_month']."</td>";
		$output=$output."<td>".$row['peers_year']."</td>";
		$output=$output."<td>".$row['born_date']."</td>";
		$output=$output."<td>".$row['alive_date']."</td>";
		$output=$output."</tr>";
		
		flush();
	}
	
	$output=$output."</table>\r\n";
	
	echo $output;
}

// Nodes page
function statsNodesBody($database)
{
	mainTitle(_LANG_NODES_TITLE,"","stats_nodes");
		
	// Defaults
	$filterOnlyKnown = true;
	$filterText = "";
	$orderMode = "validation_desc";
	if(isPostBack() == true)
	{
		$filterOnlyKnown = getPostBool("chkFilterOnlyKnown");
		$filterText = getPostText("txtFilterText");
		$orderMode = getPostText("cboOrdering");
	}
	
	echocr("<div class=\"os_filters\">");
	echocr("<form method=\"post\" action=\"" . $PHP_SELF . "\">");
	echocr(_LANG_NODES_FILTER_SEARCH . " <input type=\"input\" name=\"txtFilterText\" value=\"" . $filterText . "\" />");
	echocr(" | " . _LANG_NODES_FILTER_ONLYKNOWN . " <input type=\"checkbox\" name=\"chkFilterOnlyKnown\" " . iif($filterOnlyKnown,"checked=\"checked\"") . "/>");
	echocr(" | " . _LANG_NODES_FILTER_ORDERING . " <select name=\"cboOrdering\">");
	echocr("<option value=\"portalname_asc\" ". iif($orderMode=="portalname_asc","selected=\"selected\"") . ">" . _LANG_NODES_FILTER_ORDERING_PORTALNAME_ASC . "</option>");
	echocr("<option value=\"request_desc\" ". iif($orderMode=="request_desc","selected=\"selected\"") . ">" . _LANG_NODES_FILTER_ORDERING_REQUEST_DESC . "</option>");
	echocr("<option value=\"validation_desc\" ". iif($orderMode=="validation_desc","selected=\"selected\"") . ">" . _LANG_NODES_FILTER_ORDERING_VALIDATION_DESC . "</option>");
	echocr("</select>");
	echocr(" | <input type=\"submit\" name=\"cmdSearch\" value=\"" . _LANG_NODES_FILTER_DOSEARCH . "\"/>");
	echocr("</form>");
	echocr("</div>");
	
	$sql = 
	"select tn.*, tp.name portal_name, tp.description portal_description " .
	"from isis_bootstrap_nodes tn left join isis_bootstrap_portals tp on tn.portal_id=tp.portal_id ";
	
	$where = "";
	if($filterText != "")
	{
		$filterText = escapeSql($filterText);
		if($where != "") $where = $where . " and ";
		$where = $where . " (tp.name like '%" . $filterText . "%' or tp.description like '%" . $filterText . "%' or tn.ip like '%" . $filterText . "%' or tn.port like '%" . $filterText . "%') ";
	}	
	if($filterOnlyKnown == true)
	{
		if($where != "") $where = $where . " and ";
		$where = $where . " tp.name != '' ";
	}
	
	if($where != "") 
		$sql = $sql . " where " . $where;
		
	if($orderMode == "portalname_asc")
		$sql = $sql . " order by tp.name asc";
	else if($orderMode == "request_desc")
		$sql = $sql . " order by request_date desc";
	else if($orderMode == "validation_desc")
		$sql = $sql . " order by last_validation_date desc";
	
	$result = mysql_query($sql,$database) or error("Sql error: ".mysql_error());
	
	echocr("<table>");
	echocr("<tr>");
	echocr("<th>" . _LANG_NODES_TABLE_PORTAL . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_IP . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_PORT . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_CREATION . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_LASTREQUEST . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_LASTVALIDATION . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_VALIDNOW . "</th>");
	echocr("<th>" . _LANG_NODES_TABLE_LASTCHECK . "</th>");
	echocr("</tr>");
	
	while ($row=mysql_fetch_array($result)) 
	{
		$portal_id = $row['portal_id'];
		$portal_name = $row['portal_name'];
		$portal_description = $row['portal_description'];
		$ip = $row['ip'];
		$port = $row['port'];
		$creation_date = $row['creation_date'];
		$request_date = $row['request_date'];
		$last_validation_date = $row['last_validation_date'];
		$validation_date = $row['validation_date'];
		$check_date = $row['check_date'];
		
		echocr("<tr>");
		echocr("<td>" . statsPortalControl($row) . "</td>");
		echocr("<td>" . $ip . "</td>");
		echocr("<td>" . $port . "</td>");
		echocr("<td>" . $creation_date . "</td>");
		echocr("<td>" . $request_date . "</td>");
		echocr("<td>" . $last_validation_date . "</td>");		
		echocr("<td>");
		if($validation_date != null)
		{
			echocr("<img style=\"width:16px;height:16px;\" src=\"images/icons/success.png\" alt=\"Yes\" title=\"Yes\">");
		}
		else
		{
			echocr("<img style=\"width:16px;height:16px;\" src=\"images/icons/failed.png\" alt=\"Failed.\" title=\"Failed.!\">");
		}
		echocr("</td>");
		echocr("<td>" . $check_date . "</td>");
		echocr("</tr>");
	}
	
	echocr("</table>");
}

function actionStats($database)
{
	$page = validateAlphabetic($_GET["page"]);
		
	if( ($page != "general") && ($page != "portals") && ($page != "nodes") )
		$page = "general";
		
	// Common layout
	echo "<div class=\"is_tab_control\">";
	skinChanger("default");
	skinChanger("white");
	tabButton($page, "general", _LANG_GENERAL, "stats_general", "stats");
	tabButton($page, "portals", _LANG_PORTALS, "stats_portals", "stats");
	tabButton($page, "nodes", _LANG_NODES, "stats_nodes", "stats");		
	echo "</div>";
	echo "<div class=\"is_logo\"></div>";
	flush();
	echo "<div class=\"is_body\">";
	
	if($page == "general")
	{
		statsGeneralBody($database);
	}
	else if($page == "portals")
	{
		statsPortalsBody($database);
	}
	else if($page == "nodes")
	{
		statsNodesBody($database);
	}
	echo "<div style=\"clear:both;\"></div>";
	echo "</div>";		
}

// ---------------------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------------------


function main()
{
	$database = null;
	
	$action = validateAlphabetic(getQueryParam("action", "osiris"));
		
	if($action == "osiris")
	{
		$outputMode = "xml";
	}	
	else if($action == "validate")
	{
		$outputMode = "xhtml";
	}
	/*else if($action == "naming")
	{
		$outputMode = "xhtml";
	}*/
	else if($action == "stats")
	{
		$outputMode = "xhtml";
	}
	else
	{
		// For report an error.
		$outputMode = "xhtml";		
	}
	
	if($outputMode == "xhtml")
	{
		// Init language system
		languageInit();	
		
		xhtmlHeader();
	}
	else if($outputMode == "xml")
	{
		xmlHeader();
	}
	else	
		die();
		
	try
	{
		$database = connectDatabase();
		
		if($action == "osiris")
			actionOsiris($database);
		else if($action == "validate")
			actionValidation($database);
		/*else if($action == "naming")
			actionNaming($database);*/
		else if($action == "stats")
			actionStats($database);
		else
			error("Invalid action.");
	}
	catch(Exception $e)
	{
		logMessage("error", $e->getMessage());
		
		if($outputMode == "xhtml")
		{
			message("error", $e->getMessage());
		}
		else if($outputMode == "xml")
		{
			echo "<error msg=\"".$e->getMessage()."\"/>\r\n";			
		}
	}
	
	disconnectDatabase($database);
	
	if($outputMode == "xhtml")
	{
		xhtmlFooter();
	}	
}

// Calling main code
main();
?>
