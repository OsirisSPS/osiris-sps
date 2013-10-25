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

/*
Todo:
- Check if HTTrack is available.
- Problemi con la lingua...

HTTrack parameters:
v : verbose
c1: 1 connection
R1: 1 retry
b0: Do not accept cookie
s0: Do not follow robots.txt
C0: Do not use cache
I0: Do not make index
-F "" : Do not specify any user-agent
%l "it-it" : Language
*/

function job_scan_run()
{
	$options = "-v -c1 -R1 -b0 -s0 -C0 -I0 -F \"\" -%l \"it-it\" -p0";
	$portal = "01000001660B91DF9808905C18DC4B85B3261FCAD1ED8B0A";
	$cmd = "httrack \"http://isis.kodeware.net/link.php?01000001660B91DF9808905C18DC4B85B3261FCAD1ED8B0A\" -O /home/clodo/testhttrack \"+*portal=01000001660B91DF9808905C18DC4B85B3261FCAD1ED8B0A*\" " . $options;
	
	//$cmd = "echo ciao;echo eccoloxx";
	$out = array();
	//exec($cmd, $out);
	
	message("info", $cmd);
	flush();
	
	echo "<code>";
	foreach($out as $row)
	{
		echo $row . "<br />";
	}
	echo "</code>";
	
	return true;
}

function job_scan_view()
{
	return "Scan job. DON'T USE IT, EXPERIMENTAL";
}

?>