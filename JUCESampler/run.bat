@echo off
REM SamplerPlayer - Run Script for Windows
REM Quick run script

setlocal

REM Check if build directory exists
if not exist build (
    echo Error: No se encuentra el directorio 'build'.
    echo    Compila primero usando: build.bat
    pause
    exit /b 1
)

cd build

REM Determine build type (prefer Release, fallback to Debug)
set BUILD_TYPE=Release
if exist SamplerPlayer_artefacts\Release\SamplerPlayer.exe (
    set BUILD_TYPE=Release
) else if exist SamplerPlayer_artefacts\Debug\SamplerPlayer.exe (
    set BUILD_TYPE=Debug
) else (
    echo Error: No se encuentra el ejecutable compilado.
    echo    Compila primero usando: build.bat
    pause
    exit /b 1
)

REM Run the application
set EXECUTABLE=SamplerPlayer_artefacts\%BUILD_TYPE%\SamplerPlayer.exe
if exist %EXECUTABLE% (
    echo Ejecutando SamplerPlayer (%BUILD_TYPE%)...
    start "" "%EXECUTABLE%"
) else (
    echo Error: No se encuentra el ejecutable en %EXECUTABLE%
    pause
    exit /b 1
)
