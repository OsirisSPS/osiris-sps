@echo off 
:main 
IF "%1"==""  GOTO help

:build
md .\plain\isis%1
md .\plain\isis%1\data
md .\plain\isis%1\data\logs
md .\zip
del .\plain\isis%1 /s /q
del .\zip\isis%1.zip
xcopy ..\htdocs .\plain\isis%1 /EXCLUDE:excludes.txt /s
copy ..\htdocs\configuration.php.dist .\plain\isis%1
copy ..\htdocs\robots.txt.dist .\plain\isis%1

7za a -tzip .\zip\isis%1.zip .\plain\isis%1

goto end

:help
echo Isis Package Building Script
echo ----------
echo Syntax: win     -- Show help
echo Syntax: win [version]     - Build version
echo ----------
:end
