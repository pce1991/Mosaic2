@echo off
setlocal enabledelayedexpansion

:: === CONFIGURE ENVIRONMENT ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "EMSDK_PATH=C:\raylib\emsdk"
set "EMSCRIPTEN_PATH=%EMSDK_PATH%\upstream\emscripten"
set "CLANG_PATH=%EMSDK_PATH%\upstream\bin"
set "PYTHON_PATH=%EMSDK_PATH%\python\3.9.2-nuget_64bit"
set "NODE_PATH=%EMSDK_PATH%\node\20.18.0_64bit\bin"
set "PATH=%EMSDK_PATH%;%EMSCRIPTEN_PATH%;%CLANG_PATH%;%NODE_PATH%;%PYTHON_PATH%;%PATH%"

:: === COMPILER OPTIONS ===
set "CC=%EMSCRIPTEN_PATH%\emcc"
set "CFLAGS=-Wall -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O3 -I. -I%RAYLIB_PATH%\src -I%RAYLIB_PATH%\external -DPLATFORM_WEB"
set "LDFLAGS=-L. -L%RAYLIB_PATH%\src -sUSE_GLFW=3 -sEXPORTED_RUNTIME_METHODS=ccall -sASYNCIFY --shell-file %RAYLIB_PATH%\src\shell.html"
set "LDLIBS=%RAYLIB_PATH%\src\libraylib.web.a"
set "RESOURCES="

:: === INPUT AND OUTPUT SETUP ===
set "INPUT_FULL=%~1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"
set "BUILD_DIR=build"

:: === START LOCAL SERVER ===
start "" "%PYTHON_PATH%\python.exe" -m http.server 8080

echo "started server"

:: WAIT FOR SERVER TO START
timeout /t 2 >nul

start "" "http://localhost:8080/build/my_game.html"

echo "opened file"

pause

