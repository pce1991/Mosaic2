@echo off
if "%~1"=="" (
    echo Please drag a .c file onto this batch file or run it with a filename.
    pause
    exit /b
)

echo > Setup required Environment
echo -------------------------------------

rem === CONFIGURE YOUR ENVIRONMENT HERE ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "COMPILER_PATH=C:\raylib\w64devkit\bin"
set "CC=g++"
set "CFLAGS=%RAYLIB_PATH%\src\raylib.rc.data -s -static -O2 -Wall -std=c++17 -I%RAYLIB_PATH%\src -Iexternal -DPLATFORM_DESKTOP"
set "LDFLAGS=-lraylib -lopengl32 -lgdi32 -lwinmm"
set "PATH=%COMPILER_PATH%;%PATH%"

rem === EXTRACT FILE INFO FROM ARGUMENT ===
set "FILE_FULL=%~1"
set "CURRENT_DIRECTORY=%~dp1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"

cd /d "%CURRENT_DIRECTORY%"

echo.
echo > Clean latest build
echo ------------------------
if exist "%NAME_PART%.exe" del /F "%NAME_PART%.exe"

echo.
echo > Compile program
echo -----------------------
%CC% --version
%CC% -o "%NAME_PART%.exe" "%FILE_NAME%" %CFLAGS% %LDFLAGS%


echo.
