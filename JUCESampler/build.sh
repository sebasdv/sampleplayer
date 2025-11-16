#!/bin/bash

# SamplerPlayer - Build Script
# Quick build script for Linux/macOS

set -e  # Exit on error

echo "========================================="
echo "  SamplerPlayer - Build Script"
echo "========================================="
echo ""

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "❌ Error: CMake no está instalado."
    echo "   Instala CMake desde: https://cmake.org/download/"
    exit 1
fi

# Check CMake version
CMAKE_VERSION=$(cmake --version | head -n1 | cut -d' ' -f3)
echo "✓ CMake version: $CMAKE_VERSION"

# Parse arguments
BUILD_TYPE="Release"
CLEAN_BUILD=false

while [[ $# -gt 0 ]]; do
    case $1 in
        --debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        --clean)
            CLEAN_BUILD=true
            shift
            ;;
        --help)
            echo "Uso: ./build.sh [opciones]"
            echo ""
            echo "Opciones:"
            echo "  --debug    Compila en modo Debug"
            echo "  --clean    Limpia el build anterior antes de compilar"
            echo "  --help     Muestra esta ayuda"
            echo ""
            exit 0
            ;;
        *)
            echo "Opción desconocida: $1"
            echo "Usa --help para ver las opciones disponibles"
            exit 1
            ;;
    esac
done

echo "✓ Build type: $BUILD_TYPE"
echo ""

# Clean build if requested
if [ "$CLEAN_BUILD" = true ]; then
    echo "🧹 Limpiando build anterior..."
    rm -rf build
    echo "✓ Build limpiado"
    echo ""
fi

# Create build directory
if [ ! -d "build" ]; then
    echo "📁 Creando directorio build..."
    mkdir build
fi

# Navigate to build directory
cd build

# Run CMake
echo "⚙️  Configurando proyecto con CMake..."
cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE

# Build
echo ""
echo "🔨 Compilando..."
cmake --build . --config $BUILD_TYPE -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo ""
echo "========================================="
echo "  ✅ Compilación exitosa!"
echo "========================================="
echo ""

# Find executable
if [ "$(uname)" == "Darwin" ]; then
    # macOS
    EXECUTABLE="SamplerPlayer_artefacts/$BUILD_TYPE/SamplerPlayer.app/Contents/MacOS/SamplerPlayer"
else
    # Linux
    EXECUTABLE="SamplerPlayer_artefacts/$BUILD_TYPE/SamplerPlayer"
fi

if [ -f "$EXECUTABLE" ]; then
    echo "📦 Ejecutable: $EXECUTABLE"
    echo ""
    echo "Para ejecutar:"
    echo "  cd build && ./$EXECUTABLE"
    echo ""
    echo "O simplemente:"
    echo "  ./run.sh"
else
    echo "⚠️  No se pudo encontrar el ejecutable en la ubicación esperada."
fi
