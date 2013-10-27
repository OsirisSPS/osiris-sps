rd /s /q __tmp__
mkdir __tmp__
xcopy package __tmp__ /e
xcopy iosiris.xpt "__tmp__\components"
xcopy "..\..\bin\win32\release\firefox.dll" "__tmp__\components"
cd __tmp__
xcopy "..\..\..\bin\win32\release\*.dll" osiris
xcopy "..\..\..\bin\win32\release\*.pyd" osiris
xcopy "..\..\..\bin\win32\release\*.manifest" osiris
cd osiris
del firefox.dll
mkdir share
xcopy ..\..\..\..\share share /e
cd ..
..\..\..\..\utils\7zip\7za.exe  a -tzip ..\osiris.xpi *
cd ..
rd /s /q __tmp__
