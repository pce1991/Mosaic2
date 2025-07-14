@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Please drag a .cpp file from src/ onto this batch file or run it with a filename.
    pause
    exit /b
)

echo -------------------------------------
echo Setting up environment for MSVC

:: === FILE INFO ===
set "INPUT_FULL=%~1"
set "INPUT_DIR=%~dp1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"
set "SRC_DIR=src"
set "BUILD_DIR=build"

:: === RAYLIB PATHS ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "RAYLIB_INCLUDE=%RAYLIB_PATH%\src"
set "RAYLIB_LIB=C:\raylib\raylib\build_msvc\raylib"
::set "RAYLIB_LIB=%RAYLIB_PATH%\src"

:: === CREATE BUILD DIRECTORY IF NEEDED ===
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo ------------------------
echo Cleaning old executable
if exist "%BUILD_DIR%\%NAME_PART%.exe" del /F "%BUILD_DIR%\%NAME_PART%.exe"

echo ------------------------
echo Compiling with MSVC

:: Compile with MSVC, include debug symbols (/Zi), and create PDB (/Fd), enable debug runtime (/MDd or /MTd)
rem cl.exe /EHsc /Zi /std:c++20 /MDd /Fe"%BUILD_DIR%\%NAME_PART%.exe" ^
rem     "%INPUT_FULL%" ^
rem     /I"%RAYLIB_INCLUDE%" /I"external" ^
rem     /link /LIBPATH:"%RAYLIB_LIB%" raylib.lib opengl32.lib gdi32.lib winmm.lib /DEBUG

cl.exe /EHsc /Zi /std:c++20 /MDd /Fe"%BUILD_DIR%\%NAME_PART%.exe" /Fd"%BUILD_DIR%\%NAME_PART%.pdb" ^
    "%INPUT_FULL%" ^
    /I"%RAYLIB_INCLUDE%" /I"external" ^
    /link /LIBPATH:"%RAYLIB_LIB%" raylib.lib opengl32.lib gdi32.lib winmm.lib user32.lib shell32.lib setupapi.lib ole32.lib advapi32.lib /DEBUG

echo.
echo Done. Output: %BUILD_DIR%\%NAME_PART%.exe
echo Ready for debugging in Visual Studio!
echo.
pause
