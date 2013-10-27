for /f %%a IN ('dir /b ..\48x48\*.png') do ..\..\..\..\..\..\..\..\utils\imagemagick\convert ..\48x48\%%a -resize 24x24^ ..\24x24\%%a
for /f %%a IN ('dir /b ..\48x48\*.png') do ..\..\..\..\..\..\..\..\utils\imagemagick\convert ..\48x48\%%a -resize 32x32^ ..\32x32\%%a
for /f %%a IN ('dir /b ..\48x48\*.png') do ..\..\..\..\..\..\..\..\utils\imagemagick\convert ..\48x48\%%a -resize 16x16^ ..\16x16\%%a

pause