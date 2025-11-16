# JUCE Sampler Player

Versión standalone del Sampler Player web convertido a JUCE.

## Características

- ✅ **16 Pads** - Grid de 4x4 para reproducir samples
- ✅ **Carga de archivos** - Soporta WAV, MP3, AIFF, OGG, FLAC
- ✅ **Entrada MIDI** - Mapeo automático de notas MIDI a pads
- ✅ **Diseño Neumórfico** - Interfaz visual similar a la versión web
- ✅ **Reproducción múltiple** - Reproduce múltiples samples simultáneamente
- ✅ **Indicadores visuales** - Los pads muestran estado (cargado/reproduciendo)

## Requisitos

### 1. JUCE Framework

Descarga e instala JUCE desde: https://juce.com/get-juce/download

**Versión recomendada:** JUCE 7.0 o superior

### 2. Compilador C++

- **Windows:** Visual Studio 2022 (Community Edition es suficiente)
- **macOS:** Xcode 13 o superior
- **Linux:** GCC 9+ o Clang 10+

### 3. Dependencias Linux (solo Linux)

```bash
sudo apt-get install libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev  \
    libfreetype6-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxcursor-dev \
    libxext-dev libxinerama-dev libxrandr-dev libxrender-dev \
    libwebkit2gtk-4.0-dev \
    libglu1-mesa-dev mesa-common-dev
```

## Compilación

### Método 1: Usando Projucer (Recomendado)

1. **Abre Projucer** (viene con JUCE)

2. **Abre el proyecto:**
   - File → Open → Selecciona `SamplerPlayer.jucer`

3. **Configura las rutas de JUCE:**
   - En Projucer, ve a Settings (ícono de engranaje)
   - En "Global Paths", configura "Path to JUCE" apuntando a tu instalación de JUCE

4. **Exporta el proyecto:**
   - En Projucer, guarda el proyecto (Ctrl/Cmd+S)
   - Esto generará los archivos de proyecto nativos en `Builds/`

5. **Compila:**
   - **Windows:** Abre `Builds/VisualStudio2022/SamplerPlayer.sln` y compila (F7)
   - **macOS:** Abre `Builds/MacOSX/SamplerPlayer.xcodeproj` y compila (Cmd+B)
   - **Linux:**
     ```bash
     cd Builds/LinuxMakefile
     make CONFIG=Release
     ```

### Método 2: CMake (Alternativo)

Si prefieres usar CMake, puedes crear un `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.15)
project(SamplerPlayer VERSION 1.0.0)

add_subdirectory(JUCE)  # Apunta a tu instalación de JUCE

juce_add_gui_app(SamplerPlayer
    PRODUCT_NAME "SamplerPlayer")

target_sources(SamplerPlayer PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MainComponent.h)

target_compile_definitions(SamplerPlayer PRIVATE
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0)

target_link_libraries(SamplerPlayer PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra)
```

Luego compila con:
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Uso

### Ejecutar la aplicación

Una vez compilado, el ejecutable estará en:
- **Windows:** `Builds/VisualStudio2022/x64/Release/SamplerPlayer.exe`
- **macOS:** `Builds/MacOSX/build/Release/SamplerPlayer.app`
- **Linux:** `Builds/LinuxMakefile/build/SamplerPlayer`

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
├── SamplerPlayer.jucer       # Proyecto JUCE (ábrelo en Projucer)
├── Source/
│   ├── Main.cpp              # Punto de entrada de la aplicación
│   ├── MainComponent.h       # Header del componente principal
│   └── MainComponent.cpp     # Implementación del sampler
├── Builds/                   # Carpetas de compilación (generadas por Projucer)
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
