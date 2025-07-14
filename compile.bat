@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Please drag a .cpp file from src/ onto this batch file or run it with a filename.
    pause
    exit /b
)

::echo > Setup required Environment
echo -------------------------------------

:: === CONFIGURE ENVIRONMENT ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "COMPILER_PATH=C:\raylib\w64devkit\bin"
set "CC=g++"
set "CFLAGS=%RAYLIB_PATH%\src\raylib.rc.data -s -static -O2 -std=c++20 -I%RAYLIB_PATH%\src -Iexternal -DPLATFORM_DESKTOP"
set "LDFLAGS=-lraylib -lopengl32 -lgdi32 -lwinmm"
set "PATH=%COMPILER_PATH%;%PATH%"

:: === FILE INFO ===
set "INPUT_FULL=%~1"
set "INPUT_DIR=%~dp1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"
set "SRC_DIR=src"
set "BUILD_DIR=build"

:: === CREATE BUILD DIRECTORY IF NEEDED ===
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo.
::echo > Clean latest build
echo ------------------------
if exist "%BUILD_DIR%\%NAME_PART%.exe" del /F "%BUILD_DIR%\%NAME_PART%.exe"

echo.
::echo > Compile program
echo -----------------------
%CC% --version

:: === COMPILE ===
%CC% -o "%BUILD_DIR%\%NAME_PART%.exe" "%INPUT_FULL%" %CFLAGS% %LDFLAGS%

echo.
echo Done. Output executable: %BUILD_DIR%\%NAME_PART%.exe
echo.

