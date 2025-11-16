@echo off
REM SamplerPlayer - Build Script for Windows
REM Quick build script using CMake

setlocal

echo =========================================
echo   SamplerPlayer - Build Script
echo =========================================
echo.

REM Check if CMake is installed
where cmake >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo Error: CMake no esta instalado.
    echo    Instala CMake desde: https://cmake.org/download/
    pause
    exit /b 1
)

REM Get CMake version
for /f "tokens=3" %%i in ('cmake --version ^| findstr /R "^cmake"') do set CMAKE_VERSION=%%i
echo CMake version: %CMAKE_VERSION%

REM Parse arguments
set BUILD_TYPE=Release
set CLEAN_BUILD=0

:parse_args
if "%~1"=="" goto end_parse
if /i "%~1"=="--debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse_args
)
if /i "%~1"=="--clean" (
    set CLEAN_BUILD=1
    shift
    goto parse_args
)
if /i "%~1"=="--help" (
    echo Uso: build.bat [opciones]
    echo.
    echo Opciones:
    echo   --debug    Compila en modo Debug
    echo   --clean    Limpia el build anterior antes de compilar
    echo   --help     Muestra esta ayuda
    echo.
    pause
    exit /b 0
)
echo Opcion desconocida: %~1
echo Usa --help para ver las opciones disponibles
pause
exit /b 1

:end_parse

echo Build type: %BUILD_TYPE%
echo.

REM Clean build if requested
if %CLEAN_BUILD% EQU 1 (
    echo Limpiando build anterior...
    if exist build rmdir /s /q build
    echo Build limpiado
    echo.
)

REM Create build directory
if not exist build (
    echo Creando directorio build...
    mkdir build
)

REM Navigate to build directory
cd build

REM Detect Visual Studio version
set VS_GENERATOR="Visual Studio 17 2022"
where devenv 2022 >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    set VS_GENERATOR="Visual Studio 16 2019"
)

REM Run CMake
echo Configurando proyecto con CMake...
cmake .. -G %VS_GENERATOR% -A x64
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error al configurar el proyecto.
    pause
    exit /b 1
)

REM Build
echo.
echo Compilando...
cmake --build . --config %BUILD_TYPE% -j
if %ERRORLEVEL% NEQ 0 (
    echo.
    echo Error al compilar.
    pause
    exit /b 1
)

echo.
echo =========================================
echo   Compilacion exitosa!
echo =========================================
echo.

REM Find executable
set EXECUTABLE=SamplerPlayer_artefacts\%BUILD_TYPE%\SamplerPlayer.exe
if exist %EXECUTABLE% (
    echo Ejecutable: %EXECUTABLE%
    echo.
    echo Para ejecutar:
    echo   cd build ^&^& %EXECUTABLE%
    echo.
    echo O simplemente:
    echo   run.bat
) else (
    echo No se pudo encontrar el ejecutable en la ubicacion esperada.
)

echo.
pause
