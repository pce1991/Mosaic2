@echo off
if "%~1"=="" (
    echo Please drag a .c file onto this batch file or run it with a filename.
    pause
    exit /b
)

echo > Setup required Environment
echo -------------------------------------

:: === SET ENVIRONMENT PATHS ===
set "RAYLIB_PATH=C:\raylib\raylib"
set "EMSDK_PATH=C:\raylib\emsdk"
set "EMSCRIPTEN_PATH=%EMSDK_PATH%\upstream\emscripten"
set "CLANG_PATH=%EMSDK_PATH%\upstream\bin"
set "PYTHON_PATH=%EMSDK_PATH%\python\3.9.2-nuget_64bit"
set "NODE_PATH=%EMSDK_PATH%\node\20.18.0_64bit\bin"

set "PATH=%EMSDK_PATH%;%EMSCRIPTEN_PATH%;%CLANG_PATH%;%NODE_PATH%;%PYTHON_PATH%;%PATH%"

:: === SET COMPILER OPTIONS ===
set "CC=%EMSCRIPTEN_PATH%\emcc"
set "CFLAGS=-Wall -std=c99 -D_DEFAULT_SOURCE -Wno-missing-braces -Wunused-result -O3 -I. -I%RAYLIB_PATH%\src -I%RAYLIB_PATH%\external -DPLATFORM_WEB"
set "LDFLAGS=-L. -L%RAYLIB_PATH%\src -sUSE_GLFW=3 -sEXPORTED_RUNTIME_METHODS=ccall -sASYNCIFY --shell-file %RAYLIB_PATH%\src\shell.html"
set "LDLIBS=%RAYLIB_PATH%\src\libraylib.web.a"

:: === OPTIONAL RESOURCE EMBEDDING ===
set "RESOURCES="
:: set "RESOURCES=--preload-file path_to_resources"

:: === FILE INFO ===
set "FILE_FULL=%~1"
set "CURRENT_DIRECTORY=%~dp1"
set "FILE_NAME=%~nx1"
set "NAME_PART=%~n1"

cd /d "%CURRENT_DIRECTORY%"

echo.
echo > Clean latest build
echo ------------------------
for %%F in (html wasm js data) do (
    if exist "%NAME_PART%.%%F" del /F "%NAME_PART%.%%F"
)

echo.
echo > Compile program
echo -----------------------
"%CC%" --version
"%CC%" -o "%NAME_PART%.html" "%FILE_NAME%" %CFLAGS% %LDFLAGS% %LDLIBS% %RESOURCES%

echo.
echo > Start local server and open browser
echo ----------------------------------------------
:: Launch local server
start "" "%PYTHON_PATH%\python.exe" -m http.server 8080

:: Wait briefly for server to start
timeout /t 2 >nul

:: Open browser to the generated HTML file
start "" "http://localhost:8080/%NAME_PART%.html"

echo.
pause