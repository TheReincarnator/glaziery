SET "buildlib=%1\build\lib"

mkdir %buildlib% >NUL 2>NUL
copy %1\lib\CxImage\*.lib %buildlib% >NUL

rem Dummy to prevent error code to stop the build
echo. >NUL
