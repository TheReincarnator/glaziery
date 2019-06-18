SET "builddir=%1\build\%3"
SET "distdir=%1\%2\%3"

mkdir %distdir%\fonts >NUL 2>NUL
mkdir %distdir%\images >NUL 2>NUL

copy %builddir%\EmptyApplication.exe %distdir% >NUL

copy %1\..\..\resources\common\fonts\*.* %distdir%\fonts >NUL
copy %1\..\..\resources\common\images\*.* %distdir%\images >NUL
copy %1\..\..\resources\simpleskin\fonts\*.* %distdir%\fonts >NUL
copy %1\..\..\resources\simpleskin\images\*.* %distdir%\images >NUL

copy %1\resources\fonts\*.* %distdir%\fonts >NUL
del %distdir%\fonts\.cvsignore >NUL 2>NUL

copy %1\resources\images\*.* %distdir%\images >NUL
del %distdir%\images\.cvsignore >NUL 2>NUL

rem Dummy to prevent error code to stop the build
echo. >NUL
