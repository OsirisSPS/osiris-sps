1.0 - 02/11/2013
- [new] Osiris 1.0 Support. NOTE: This version is compatible only with Osiris 1.0, previous version are rejected.
- [bugfix] Fixed some wrong 304 errors.
- [changes] HTTP error 503 instead of 200 if data isn't available.
- [changes] A lot of minor changes.
----------------------------------------------------------------
0.15 - 
- Improvement of logging system: skip php with @, filter options.
- Fixes some PHP errors.
- Support for "forwarding" http proxy.

0.14 - 06/10/2011
- Various bugfixes.

0.13 - 29/09/2011
- Changed default tuning settings
- Isis Messages are now http-404 response
- Fixed "checking.php"
- Fixed default systembar options
- Fixed related "Osiris Core Extension"
- Fixed a performance bug
- Fixed image url in "home" in SEO mode.
- Log path are now always under data path
- Checking if default "data" path are useful or not
- HTAccess "data" protection & checking 
- SEO rules testing and fixing
- Allow move "data" directory to a path outside webserver

0.12 - 27/09/2011
- [changes] Tor Detection was temporarly disabled by tor-project.org. Detection of this situation implemented.
- [bugfix] Some PHP warnings.
- [changes] Addons improvements.
- [new] Addons catalog support.
- [changes] CSS fix for redirect.
- [new] Custom portal options (for extensions).
- [changes] SEO updates.
- [bugfix] Osiris 0.14 compatibility
- [bugfix] Many misc fixes.
----------------------------------------------------------------
0.11 - 20/07/2011
- [changes] Various improvements
- [new] addons system
- [bugfix] Compatibility fixes for Osiris 0.14
----------------------------------------------------------------
0.10 - 06/04/2010
- [bugfix] Encoding problems on url, especially on RSS Feed.
- [bugfix] Saving of general portal settings
- [new] Option "home.portalslist" & "home.pastelinks"
- [new] Option "layout.footer" for every page, for example to inject Google Analytics.
- [changes] Home page if forced are not more showed, if redirection is enabled.
- [new] New option "server.acp.tabs" for adding custom iframe tab in ACP (to show AWStats, for example).
- [changes] If no nodes are in waiting, cache copy is returned directly if present.
- [bugfix] Encoding problem in invite-link.
- [changes] Bootstrap web-service don't save IP address if Osiris tell it.
----------------------------------------------------------------
0.9 - 27/01/2010
- [new] Paste links feature, inside ACP, to allow quick registrations of portals.
- [bugfix] Compatibility fixes for Osiris 0.13
----------------------------------------------------------------
0.8 - 19/01/2010
- [changes] Support to web-hosting with post-params 64 chars restriction.
- [new] Guest password
- [changes] Compatibility fixes for Osiris 0.13
- [changes] HTML5 output, w3c validations.
- [bugfix] Minor improvements.
----------------------------------------------------------------
0.7 - 18/12/2009
- Compatibility fixes for Osiris 0.13
- Time difference from time-server always showed in ACP (info, warning if more than 30 seconds)
- Service "portcheck" changed in "check?act=port".
- Added TOR detection to "check" service.
- Added HTML & XML output in "check" service.
- Added options "portals.default.url.protocols.no_confirm", "portals.default.url.protocols.no_redirect", "portals.default.url.protocols.collect", "portals.default.web.redirect_url"
- Bugfix: skin issue.
- Minor improvements.
----------------------------------------------------------------
0.6 - 15/09/2009
- Bugfix: saving of default portal settings.
- Bootstrap layout fixed.
- Bootstrap validation request, allowed only from local server.
- Highlight portal status (hidden, closed).
- Added "home.header", "home.footer".
----------------------------------------------------------------
0.5 - 14/09/2009
- General better layout.
- Zombie node checking fixes in status boxes.
- Added "remove" on portals.
- Added "clean cache" at portal level.
- Inclusion of "scriptaculous" & "prototype" libraries.
- Improved service "link", to allow creation of perma-link.
- Porting service "update" from Anubis to Isis.
- Porting service "clock" from Anubis to Isis.
- Porting service "bootstrap" from Anubis to Isis.
- Changed "info.php" to "home.php"
- Link ID are now case-sensitive
- Support of changing portal language
- Adv default to Osiris website, and not to our Google AdSense.
- Minor bugfixed.
----------------------------------------------------------------
0.4 - 27/01/2009
- Added portal tipology inside portal infobox.
- Added option "info.acp.notes".
- Fixed and moved "save settings" button.
- Fixed skin settings for compatibility.
- Automatic directory creation try to chmod to 0666, not anymore to 0777.
- Fixed IPB <br /> problem with invite link.
- Added the support for special web-hosting that invalidate isis xml output. Will work together with Osiris 0.13.
- Fixed automatic subscription from guest.
- Fixed a bug that doesn't show message different from english and italian in some pages.
----------------------------------------------------------------
0.3 - 13/01/2008
Major:
- Bugfix about cache updating.

Minor:
- ACP layout fixes
- Cache simple viewer from status tab
- Added "Retry" and "Back" buttons when an error occur.
- "Safe mode" check converted from "warning" to "error".
- "Environment checking" page before the first access to ACP, to detect PHP4 or PHP Safe-Mode.
----------------------------------------------------------------
0.2 - 31/12/2008
- Cache language options, to aggregate cache.
- Some timeout-management fixes.
- Added a changelogs.
- Conversion of settings from microseconds to seconds.
- Save-Mode detection.
- Javascript-ACP detect.
- Detect some installation step missing.
- Added settings for manage custom web-hosting with PHP restriction.
- Minor fixes.
- Default home-url behiavour changed.
- New "info.php", also a default home.
- New setting "isis.info.hidden".
- "portals.global" deprecated.
- New Osiris & Isis subscription links.
----------------------------------------------------------------
0.1 - 23/12/2008
- First public release