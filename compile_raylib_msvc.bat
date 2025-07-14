@echo off
setlocal enabledelayedexpansion

:: === Check for input ===
if "%~1"=="" (
    echo Please drag a .cpp file from src/ onto this batch file or run it with a filename.
    pause
    exit /b
)

echo -------------------------------------
echo Setting up MSVC raylib build...

:: === File and folder setup ===
set "INPUT_FULL=%~1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"
set "SRC_DIR=src"
set "BUILD_DIR=build"

:: === Raylib paths ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "RAYLIB_SRC=%RAYLIB_PATH%\src"
set "RAYLIB_INCLUDE=%RAYLIB_SRC%"

:: === Create build directory ===
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: === Clean previous outputs ===
echo Cleaning old build artifacts...
del /Q "%BUILD_DIR%\%NAME_PART%.exe" >nul 2>&1
del /Q "%BUILD_DIR%\%NAME_PART%.pdb" >nul 2>&1
del /Q "%BUILD_DIR%\*.obj" >nul 2>&1

:: === Raylib .c source files ===
set RAYLIB_SOURCES=
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rcore.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rshapes.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rtextures.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rtext.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rmodels.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\raudio.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\utils.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rlgl.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rglfw.c"
set RAYLIB_SOURCES=%RAYLIB_SOURCES% "%RAYLIB_SRC%\rgestures.c"

:: === Compilation ===
echo Compiling sources to object files...

cl.exe ^
    /c ^
    /EHsc /Zi /std:c++20 /MDd ^
    "%INPUT_FULL%" %RAYLIB_SOURCES% ^
    /Fo"%BUILD_DIR%\\" ^
    /I"%RAYLIB_INCLUDE%" /I"external"

:: === Linking ===
echo Linking final executable...

cl.exe ^
    "%BUILD_DIR%\*.obj" ^
    /Fe"%BUILD_DIR%\%NAME_PART%.exe" ^
    /DEBUG ^
    /link ^
    opengl32.lib gdi32.lib winmm.lib ^
    user32.lib shell32.lib setupapi.lib ole32.lib advapi32.lib

echo.
echo ✅ Build complete!
echo Output: %BUILD_DIR%\%NAME_PART%.exe
echo PDB:    %BUILD_DIR%\%NAME_PART%.pdb
echo.
pause


:: Check if input file is provided
if "%~1"=="" (
    echo Please drag a .cpp file onto this script or provide a path to your source file.
    pause
    exit /b
)

:: === Setup Paths ===
set "INPUT_FULL=%~1"
set "NAME_PART=%~n1"
set "BUILD_DIR=build"

:: Path to raylib (edit this if your raylib path differs)
set "RAYLIB_PATH=C:\raylib\raylib"
set "RAYLIB_SRC=%RAYLIB_PATH%\src"
set "RAYLIB_INC=%RAYLIB_SRC%"

:: === Create build output directory ===
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: === List of raylib .c source files ===
set SOURCES=
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rcore.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rlgl.h"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rshapes.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rtextures.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rtext.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rmodels.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\utils.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\raudio.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rglfw.c"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rgestures.h"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\raymath.h"
set SOURCES=%SOURCES% "%RAYLIB_SRC%\rcamera.h"



:: === Clean old build ===
if exist "%BUILD_DIR%\%NAME_PART%.exe" del /F "%BUILD_DIR%\%NAME_PART%.exe"
if exist "%BUILD_DIR%\%NAME_PART%.pdb" del /F "%BUILD_DIR%\%NAME_PART%.pdb"

:: === Build command ===
echo.
echo Compiling with MSVC...
echo.

cl.exe /EHsc /Zi /std:c++20 /MDd ^
    "%INPUT_FULL%" %SOURCES% ^
    /I"%RAYLIB_INC%" /I"external" ^
    /Fe"%BUILD_DIR%\%NAME_PART%.exe" ^
    /Fd"%BUILD_DIR%\%NAME_PART%.pdb" ^
    /link ^
    opengl32.lib gdi32.lib winmm.lib ^
    user32.lib shell32.lib setupapi.lib ole32.lib advapi32.lib ^
    /DEBUG

echo.
echo Build complete.
echo Output EXE:  %BUILD_DIR%\%NAME_PART%.exe
echo Output PDB:  %BUILD_DIR%\%NAME_PART%.pdb
echo.
pause
