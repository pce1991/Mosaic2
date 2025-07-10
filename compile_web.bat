@echo off
setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Please drag a .c or .cpp file onto this batch file.
    pause
    exit /b
)

:: === CONFIGURE ENVIRONMENT ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "EMSDK_PATH=C:\raylib\emsdk"
set "EMSCRIPTEN_PATH=%EMSDK_PATH%\upstream\emscripten"
set "CLANG_PATH=%EMSDK_PATH%\upstream\bin"
set "PYTHON_PATH=%EMSDK_PATH%\python\3.9.2-nuget_64bit"
set "NODE_PATH=%EMSDK_PATH%\node\20.18.0_64bit\bin"
set "PATH=%EMSDK_PATH%;%EMSCRIPTEN_PATH%;%CLANG_PATH%;%NODE_PATH%;%PYTHON_PATH%;%PATH%"



:: === INPUT AND OUTPUT SETUP ===
set "INPUT_FULL=%~1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"
set "BUILD_DIR=build"
set "DATA_DIR=data"


:: === COMPILER OPTIONS ===
set "CC=%EMSCRIPTEN_PATH%\emcc"
set "CFLAGS=-Wall -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O3 -I. -I%RAYLIB_PATH%\src -I%RAYLIB_PATH%\external -DPLATFORM_WEB --preload-file %DATA_DIR%"
set "LDFLAGS=-L. -L%RAYLIB_PATH%\src -sUSE_GLFW=3 -sEXPORTED_RUNTIME_METHODS=ccall -sASYNCIFY --shell-file data\web\shell.html"
set "LDLIBS=%RAYLIB_PATH%\src\libraylib.web.a"
set "RESOURCES="

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

:: === CLEAN OLD OUTPUTS ===
for %%F in (html wasm js data) do (
    if exist "%BUILD_DIR%\%NAME_PART%.%%F" del /F "%BUILD_DIR%\%NAME_PART%.%%F"
)

echo %PYTHON_PATH%
::exit /b

::=== COMPILE TO BUILD DIR ===
call "%CC%" -o "%BUILD_DIR%\%NAME_PART%.html" "%INPUT_FULL%" %CFLAGS% %LDFLAGS% %LDLIBS% %RESOURCES%

copy "%BUILD_DIR%\%NAME_PART%.html" "%BUILD_DIR%\index.html"

::pause

