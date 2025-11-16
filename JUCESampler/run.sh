#!/bin/bash

# SamplerPlayer - Run Script
# Quick run script for Linux/macOS

set -e

# Check if build directory exists
if [ ! -d "build" ]; then
    echo "❌ Error: No se encuentra el directorio 'build'."
    echo "   Compila primero usando: ./build.sh"
    exit 1
fi

cd build

# Determine build type (prefer Release, fallback to Debug)
if [ -f "SamplerPlayer_artefacts/Release/SamplerPlayer" ] || [ -d "SamplerPlayer_artefacts/Release/SamplerPlayer.app" ]; then
    BUILD_TYPE="Release"
elif [ -f "SamplerPlayer_artefacts/Debug/SamplerPlayer" ] || [ -d "SamplerPlayer_artefacts/Debug/SamplerPlayer.app" ]; then
    BUILD_TYPE="Debug"
else
    echo "❌ Error: No se encuentra el ejecutable compilado."
    echo "   Compila primero usando: ./build.sh"
    exit 1
fi

# Run the application
if [ "$(uname)" == "Darwin" ]; then
    # macOS
    echo "🚀 Ejecutando SamplerPlayer ($BUILD_TYPE)..."
    open "SamplerPlayer_artefacts/$BUILD_TYPE/SamplerPlayer.app"
else
    # Linux
    EXECUTABLE="SamplerPlayer_artefacts/$BUILD_TYPE/SamplerPlayer"
    if [ -f "$EXECUTABLE" ]; then
        echo "🚀 Ejecutando SamplerPlayer ($BUILD_TYPE)..."
        "./$EXECUTABLE"
    else
        echo "❌ Error: No se encuentra el ejecutable en $EXECUTABLE"
        exit 1
    fi
fi
