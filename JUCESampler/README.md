# JUCE Sampler Player

Versión standalone del Sampler Player web convertido a JUCE.

## Características

- ✅ **16 Pads** - Grid de 4x4 para reproducir samples
- ✅ **Carga de archivos** - Soporta WAV, MP3, AIFF, OGG, FLAC
- ✅ **Entrada MIDI** - Mapeo automático de notas MIDI a pads
- ✅ **Diseño Neumórfico** - Interfaz visual similar a la versión web
- ✅ **Reproducción múltiple** - Reproduce múltiples samples simultáneamente
- ✅ **Indicadores visuales** - Los pads muestran estado (cargado/reproduciendo)

## 🚀 Inicio Rápido

### Linux/macOS

```bash
# Compila el proyecto
./build.sh

# Ejecuta la aplicación
./run.sh
```

### Windows

```batch
REM Compila el proyecto
build.bat

REM Ejecuta la aplicación
run.bat
```

Los scripts descargarán JUCE automáticamente y compilarán todo por ti.

## Requisitos

### 1. CMake

Necesitas CMake 3.15 o superior. Descarga desde: https://cmake.org/download/

### 2. Compilador C++

- **Windows:** Visual Studio 2022 (Community Edition es suficiente) o MinGW
- **macOS:** Xcode 13 o superior (instala con: `xcode-select --install`)
- **Linux:** GCC 9+ o Clang 10+

### 3. Git

Para descargar JUCE automáticamente. Descarga desde: https://git-scm.com/

### 4. Dependencias Linux (solo Linux)

```bash
sudo apt-get update
sudo apt-get install build-essential git cmake \
    libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev \
    libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev \
    libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev
```

## Compilación

### Método 1: Usando CMake (Recomendado) ⭐

CMake descargará JUCE automáticamente, no necesitas instalarlo manualmente.

#### Windows (Visual Studio)

```bash
# Navega al directorio del proyecto
cd JUCESampler

# Crea el directorio de build
mkdir build
cd build

# Genera el proyecto Visual Studio
cmake .. -G "Visual Studio 17 2022" -A x64

# Compila (Release)
cmake --build . --config Release

# El ejecutable estará en: build/SamplerPlayer_artefacts/Release/SamplerPlayer.exe
```

O abre `build/SamplerPlayer.sln` en Visual Studio y compila desde ahí (Ctrl+Shift+B).

#### macOS (Xcode)

```bash
# Navega al directorio del proyecto
cd JUCESampler

# Crea el directorio de build
mkdir build
cd build

# Genera el proyecto Xcode
cmake .. -G "Xcode"

# Compila (Release)
cmake --build . --config Release

# El .app estará en: build/SamplerPlayer_artefacts/Release/SamplerPlayer.app
```

O abre `build/SamplerPlayer.xcodeproj` en Xcode y compila desde ahí (Cmd+B).

#### Linux (Makefile)

```bash
# Navega al directorio del proyecto
cd JUCESampler

# Crea el directorio de build
mkdir build
cd build

# Genera Makefiles con Release build
cmake .. -DCMAKE_BUILD_TYPE=Release

# Compila (usa -j para compilación paralela)
cmake --build . -j$(nproc)

# O simplemente:
make -j$(nproc)

# El ejecutable estará en: build/SamplerPlayer_artefacts/Release/SamplerPlayer
```

#### Opciones adicionales de CMake

```bash
# Build tipo Debug (con símbolos de debugging)
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Especificar un compilador diferente
cmake .. -DCMAKE_CXX_COMPILER=clang++

# Usar JUCE instalado localmente en vez de descargarlo
# (edita CMakeLists.txt y comenta la sección FetchContent,
#  descomenta find_package(JUCE CONFIG REQUIRED))
```

### Método 2: Usando los scripts de compilación (Más fácil) ⚡

Usa los scripts incluidos para compilar automáticamente:

**Linux/macOS:**
```bash
./build.sh           # Compila en modo Release
./build.sh --debug   # Compila en modo Debug
./build.sh --clean   # Limpia y recompila
```

**Windows:**
```batch
build.bat           REM Compila en modo Release
build.bat --debug   REM Compila en modo Debug
build.bat --clean   REM Limpia y recompila
```

### Método 3: Usando Projucer (Alternativo)

Si prefieres usar Projucer:

1. Descarga JUCE desde https://juce.com/get-juce/download
2. Abre `SamplerPlayer.jucer` en Projucer
3. Configura la ruta de JUCE en Settings → Global Paths
4. Guarda el proyecto (genera archivos en `Builds/`)
5. Abre y compila el proyecto generado

## Uso

### Ejecutar la aplicación

Una vez compilado con CMake, el ejecutable estará en:
- **Windows:** `build/SamplerPlayer_artefacts/Release/SamplerPlayer.exe`
- **macOS:** `build/SamplerPlayer_artefacts/Release/SamplerPlayer.app`
- **Linux:** `build/SamplerPlayer_artefacts/Release/SamplerPlayer`

Si compilaste en modo Debug, reemplaza `Release` con `Debug` en las rutas.

### Cargar samples

1. Click en el botón "Cargar Archivos"
2. Selecciona hasta 16 archivos de audio
3. Los archivos se cargan en orden (primer archivo → Pad 1, etc.)
4. Los pads con samples cargados muestran un indicador verde

### Reproducir samples

**Con mouse/trackpad:**
- Click en cualquier pad para reproducir el sample

**Con teclado MIDI:**
- Conecta un controlador MIDI
- Las notas MIDI se mapean a los pads (nota % 16 + 1)
- Por ejemplo: Nota MIDI 36 (C2) → Pad 5

### Mapeo MIDI recomendado

Para un pad controller típico (Akai MPD, Novation Launchpad, etc.):
- Configura el primer pad en C1 (nota MIDI 24)
- Los siguientes 15 pads mapearán automáticamente

## Estructura del Proyecto

```
JUCESampler/
├── CMakeLists.txt            # Configuración de CMake
├── SamplerPlayer.jucer       # Proyecto JUCE (para Projucer)
├── build.sh                  # Script de compilación (Linux/macOS)
├── build.bat                 # Script de compilación (Windows)
├── run.sh                    # Script de ejecución (Linux/macOS)
├── run.bat                   # Script de ejecución (Windows)
├── Source/
│   ├── Main.cpp              # Punto de entrada de la aplicación
│   ├── MainComponent.h       # Header del componente principal
│   └── MainComponent.cpp     # Implementación del sampler
├── build/                    # Directorio de compilación (generado)
└── README.md                 # Este archivo
```

## Diferencias con la versión Web

### Ventajas de la versión JUCE:

✅ **Mejor rendimiento** - Audio nativo sin latencia del navegador
✅ **Más formatos** - Soporta FLAC y otros formatos que la web puede no soportar
✅ **Standalone** - No necesita navegador web
✅ **Menor latencia MIDI** - Conexión directa con hardware MIDI
✅ **Exportable como plugin** - Se puede convertir en VST/AU con cambios menores

### Limitaciones actuales:

❌ **No incluye visualizador de onda** - Se puede agregar usando `AudioThumbnail`
❌ **No carga samples por defecto desde GitHub** - Solo carga archivos locales

## Próximas mejoras sugeridas

1. **Agregar visualizador de forma de onda** usando `juce::AudioThumbnail`
2. **Controles de volumen** para cada pad
3. **Controles de pitch/speed** para modificar samples
4. **Guardar/cargar presets** - Guardar configuración de samples
5. **Exportar como VST/AU** - Convertir en plugin de audio
6. **Efectos** - Reverb, delay, etc.
7. **Secuenciador** - Grabar y reproducir patrones

## Troubleshooting

### Error: "JUCE modules not found"
- Verifica que la ruta a JUCE esté configurada correctamente en Projucer

### Error de compilación en Linux
- Asegúrate de tener todas las dependencias instaladas (ver sección de requisitos)

### El MIDI no funciona
- Verifica que tu controlador MIDI esté conectado antes de iniciar la app
- En algunas plataformas, necesitas dar permisos de acceso a MIDI

### El audio suena mal o tiene glitches
- Aumenta el buffer size en la configuración de audio del sistema
- Cierra otras aplicaciones de audio que puedan estar usando el dispositivo

## Licencia

Este es un proyecto de ejemplo educativo. Si planeas distribuir la aplicación, asegúrate de cumplir con las licencias de JUCE:
- **GPL** para proyectos open source
- **Comercial** para aplicaciones propietarias (requiere licencia de JUCE)

## Contacto y Contribuciones

Este proyecto es una conversión del Sampler Player web original. Si tienes sugerencias o mejoras, ¡son bienvenidas!

---

**¡Disfruta tu sampler standalone!** 🎵
