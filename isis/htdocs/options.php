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

ensureOption("server.name", "Isis");
ensureOption("server.virtual_path", getCurrentHttpPath());
ensureOption("server.physical_path", getCurrentPhysicalPath());
ensureOption("server.closed_message", "");
ensureOption("server.acp.password", "");
ensureOption("server.acp.only_ssl", false);
ensureOption("server.acp.tabs", "");
ensureOption("server.gzip.enabled", true);
ensureOption("server.database.enabled", false);
ensureOption("server.database.host", "localhost");
ensureOption("server.database.database", "isis");
ensureOption("server.database.login", "root");	
ensureOption("server.database.password", "");
ensureOption("services.redirect.enabled", true);
ensureOption("services.manifest.enabled", true);
ensureOption("services.manifest.error", "");
ensureOption("services.manifest.osiris.version", "Osiris 1.0");
ensureOption("services.manifest.osiris.notes", "");
ensureOption("services.manifest.intervalUpdate", 60);
ensureOption("services.check.enabled", true);
ensureOption("services.check.port.timeout", 5);
ensureOption("services.check.tor.update_url", "https://check.torproject.org/cgi-bin/TorBulkExitList.py");
ensureOption("services.check.tor.update_every", 86400);
ensureOption("services.link.enabled", true);
ensureOption("services.link.list", "");
ensureOption("services.link.redirect", "https://isis.osiris-sps.org/link.php?{id}");
ensureOption("services.link.seo.enabled", false);
ensureOption("services.link.seo.htaccess", "");
ensureOption("services.link.seo.path", getCurrentHttpPath(true));
ensureOption("services.link.seo.home_portal", "");
ensureOption("services.manifest.clock.local", true);
ensureOption("services.manifest.clock.server", "time.ien.it");
ensureOption("services.update.enabled", false);
ensureOption("services.update.redirect", "https://isis.osiris-sps.org/update.php");
ensureOption("services.update.version", 0.1);
ensureOption("services.update.notes", "");
ensureOption("services.bootstrap.enabled", false);
ensureOption("services.bootstrap.redirect", "https://isis.osiris-sps.org/bootstrap.php");
ensureOption("compatibility.use_set_time_limit", true);
ensureOption("ips.allowed", ".*");
ensureOption("ips.blocked", "");
ensureOption("log.enabled", false);
ensureOption("log.prefix", "isis");
ensureOption("log.timestamp", "_Y_m_d");
ensureOption("log.suffix", ".log");
ensureOption("log.filter.php", false);
ensureOption("log.filter.notice", true);
ensureOption("data.path", "./data");
ensureOption("home.redirect", "home.php");
ensureOption("home.header", "");
ensureOption("home.footer", "");
ensureOption("home.portalslist", true);
ensureOption("home.pastelinks", true);
ensureOption("layout.footer", "");
ensureOption("robots.enabled", false);
ensureOption("tuning.index.timeout", (double) 30);
ensureOption("tuning.request.waitstep", (double) 0.1);
ensureOption("tuning.request.timeout", (double) iif(customizableTimeLimit(),600,getMaxExecutionTime()));
ensureOption("tuning.accept.waitstep", (double) 0.1);
ensureOption("tuning.accept.timeout", (double) 20);
ensureOption("tuning.answer.waitstep", (double) 0.1);
ensureOption("tuning.answer.timeout", (double) 20);
ensureOption("jobs.cron", "checking;.*\nsample;.*\ncache_clean;.*");
ensureOption("portals.general.allowed", ".*");
ensureOption("portals.general.blocked", "");
//ensureOption("portals.global", "");
ensureOption("portals.default.isis.info.alias", "");
ensureOption("portals.default.isis.info.name", "");
ensureOption("portals.default.isis.info.description", "");
ensureOption("portals.default.isis.info.acp.notes", "");
ensureOption("portals.default.isis.info.hidden", false);
ensureOption("portals.default.isis.home", "");
ensureOption("portals.default.isis.closed_message", "");
ensureOption("portals.default.isis.redirect", "");
//ensureOption("portals.default.isis.mode", "proxy"); // Not yet implemented.
ensureOption("portals.default.isis.user", "");
ensureOption("portals.default.isis.realtime.enabled", true);
ensureOption("portals.default.isis.url.allowed", ".*");
ensureOption("portals.default.isis.url.blocked", "");
ensureOption("portals.default.isis.guest.password", ".*");
ensureOption("portals.default.isis.guest.message", "Login required.");
ensureOption("portals.default.isis.guest.ips.allowed", ".*");
ensureOption("portals.default.isis.guest.ips.blocked", "");
ensureOption("portals.default.isis.peers.password", ".*");
ensureOption("portals.default.isis.peers.ips.allowed", ".*");
ensureOption("portals.default.isis.peers.ips.blocked", "");
ensureOption("portals.default.isis.peers.version.allowed", ".*");
ensureOption("portals.default.isis.peers.version.blocked", "");
ensureOption("portals.default.isis.cache.enabled", true);
ensureOption("portals.default.isis.cache.url.always", "");
ensureOption("portals.default.isis.cache.url.never", "");
ensureOption("portals.default.isis.cache.store.dynamic", true);
ensureOption("portals.default.isis.cache.realtime.forced", 300);
ensureOption("portals.default.isis.cache.realtime.deltaratio", 1);
ensureOption("portals.default.isis.cache.realtime.deltamax", 604800);
ensureOption("portals.default.isis.cache.background.forced", 60);
ensureOption("portals.default.isis.cache.background.deltaratio", 0.1);
ensureOption("portals.default.isis.cache.background.deltamax", 604800);
ensureOption("portals.default.isis.cache.languages", "it.*,it-it;en.*,en-us;.*,en-us");
ensureOption("portals.default.isis.output.area.systembar", "<div class=\"os_systembar\" style=\"border:0px;margin:0px;padding:0px;background-color:white;background-image: url({@isis_path}images/systembar/bg.png);background-repeat: no-repeat;min-height:66px;\">  \r\n	<div class=\"os_advertising\" style=\"width:468px;height:60px\"><iframe src=\"http://www.osiris-sps.org/adv/isis.php\" width=\"468\" height=\"60\" frameborder=\"0\" scrolling=\"no\" marginheight=\"0\" marginwidth=\"0\" hspace=\"0\" allowtransparency=\"true\"></iframe></div>\r\n	<div class=\"os_systembar_left\">   \r\n		<a class=\"os_nolink\" title=\"Isis Home Page\" href=\"{@isis_path}\">    \r\n			<img src=\"{@isis_path}images/systembar/logo.png\" alt=\"Isis\">   \r\n		</a>   \r\n		<br>   \r\n		<!--<a title=\"Home\" class=\"os_systembar_link\" href=\"{@isis_path}\">Home</a>-->		\r\n	</div>  \r\n	<div class=\"os_clear\"></div> 	\r\n</div>");
ensureOption("portals.default.isis.output.area.footer", "<div style=\"clear:both;padding:10px;text-align:center;\">          \r\n	<a href=\"https://www.osiris-sps.org/isis_docs\" title=\"{@isis_info}\">\r\n		<img src=\"{@isis_path}images/footer_isis.png\">\r\n	</a>\r\n        <a href=\"https://www.osiris-sps.org\" title=\"Generated by Osiris\">\r\n		<img src=\"{@isis_path}images/footer_osiris.png\">\r\n	</a>\r\n	<a href=\"http://validator.w3.org/check/referer\" title=\"HTML5 Valid\">\r\n		<img src=\"{@isis_path}images/footer_valid_html.png\">\r\n	</a>  \r\n	<a href=\"http://jigsaw.w3.org/css-validator/check/referer\" title=\"CSS3 Valid\">\r\n		<img src=\"{@isis_path}images/footer_valid_css.png\">\r\n	</a> \r\n</div>"); 
ensureOption("portals.default.isis.output.replaces", ""); 
ensureOption("portals.default.ide.objects.hot_state_threshold", "20");
ensureOption("portals.default.ide.pager.items", "20");
ensureOption("portals.default.ide.pager.pages", "5");
ensureOption("portals.default.ide.skin.id", "0000000000000000000000000000000000000000");
ensureOption("portals.default.language.time_offset", 0);
ensureOption("portals.default.language.time_dst", 0);
ensureOption("portals.default.url.protocols.find_link", "osiris|http|https|ftp|ftps|gopher|news|nntp|telnet|wais|file|mailto|ed2k|ants2k|magnet|stealthnet");
ensureOption("portals.default.url.protocols.no_confirm", "*"); 
ensureOption("portals.default.url.protocols.no_redirect", "*");
ensureOption("portals.default.url.protocols.collect", "osiris|ed2k|ants2k|magnet|stealthnet|http://localhost:23402/offsystem/.*"); 
ensureOption("portals.default.privacy.allow_external_images", true);
ensureOption("portals.default.privacy.allow_external_urls", true);
ensureOption("portals.default.anonymity.object_mode", 1);
ensureOption("portals.default.anonymity.object_trusted_domains", "");
ensureOption("portals.default.anonymity.object_trusted_types", "");
ensureOption("portals.default.web.redirect_url", "https://isis.osiris-sps.org/redirect.php?#url#");
ensureOption("portals.default.system.isis", "https://isis.osiris-sps.org");
ensureOption("portals.default.options", "");

// System settings
ensureOption("product.name", "isis");
ensureOption("product.title", "Isis - Osiris Gateway");
ensureOption("product.web_title", "Osiris Home Page");
ensureOption("product.web_url", "http://www.osiris-sps.org");
ensureOption("product.version", "1.0");
ensureOption("settings.last_modification", 0);

if(file_exists("configuration.php"))
	include("configuration.php");
?>
