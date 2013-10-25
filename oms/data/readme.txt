--------------------------------
Osiris Management System
--------------------------------
A tool for manage the Osiris projects: http://www.osiris-sps.org

More information about this tool: http://www.osiris-sps.org/source_overview/
Informations about developing and building: http://www.osiris-sps.org/developers_area/

Example tools:
- Generation of distribute packages
- Generation of extensions packages
- Deployment
- Misc

Written in C#, can run under Windows, Linux and OSX.
Developer projects under Microsoft Visual Studio 2010.

--------------------------------
Windows requirement
--------------------------------
- WinRar (todo: must be replaced by 7zip)
- 7zip
- NSIS - Nullsoft Installer

--------------------------------
Linux/Debian requirement
--------------------------------
- sudo apt-get install alien p7zip-full mono-runtime mono-utils libmono-winforms2.0-cil 

--------------------------------
Mac OSX requirement
--------------------------------
- Mono



--------------------------------
Server side (Debian) requirement (for updating of the Debian repository)
--------------------------------
- sudo apt-get install dpkg dpkg-dev


--------------------------------
Known bug
--------------------------------
- 10/03/2010 : Debian package don't remove "data" in purge mode.
- 10/03/2010 : Fail to build DMG under OSX.

--------------------------------
Known problem
--------------------------------
- 10/03/2010 : I don't know where /utils/mccoy save the keys...