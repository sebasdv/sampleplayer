cmake_minimum_required(VERSION 3.15)

# Project configuration
project(SamplerPlayer VERSION 1.0.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# JUCE Setup
# ============================================================================

# Option 1: Use system-installed JUCE (uncomment if you have JUCE installed)
# find_package(JUCE CONFIG REQUIRED)

# Option 2: Download JUCE automatically (recommended for easy setup)
include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.9  # You can change this to the latest version
    GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(JUCE)

# ============================================================================
# Application Target
# ============================================================================

juce_add_gui_app(SamplerPlayer
    PRODUCT_NAME "SamplerPlayer"
    COMPANY_NAME "YourCompany"
    BUNDLE_ID "com.yourcompany.samplerplayer"
    ICON_BIG ""
    ICON_SMALL ""
    MICROPHONE_PERMISSION_ENABLED FALSE
    CAMERA_PERMISSION_ENABLED FALSE
)

# Add source files
target_sources(SamplerPlayer PRIVATE
    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MainComponent.h
)

# ============================================================================
# Compiler Definitions
# ============================================================================

target_compile_definitions(SamplerPlayer PRIVATE
    # JUCE Configuration
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    JUCE_USE_CAMERA=0

    # Disable warnings
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_REPORT_APP_USAGE=0

    # Audio settings
    JUCE_ALSA=1
    JUCE_DIRECTSOUND=1
    JUCE_JACK=1
    JUCE_ASIO=0  # Set to 1 if you have ASIO SDK

    # Debug settings (only in debug builds)
    $<$<CONFIG:Debug>:JUCE_DEBUG=1>
    $<$<CONFIG:Debug>:DEBUG=1>
    $<$<CONFIG:Debug>:_DEBUG=1>
)

# ============================================================================
# Link JUCE Modules
# ============================================================================

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
    juce::juce_gui_extra

    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)

# ============================================================================
# Platform-specific settings
# ============================================================================

if(APPLE)
    # macOS specific settings
    target_compile_definitions(SamplerPlayer PRIVATE
        JUCE_COREAUDIO=1
        JUCE_COREMIDI=1
    )
elseif(WIN32)
    # Windows specific settings
    target_compile_definitions(SamplerPlayer PRIVATE
        JUCE_WASAPI=1
        JUCE_DIRECTSOUND=1
    )
elseif(UNIX)
    # Linux specific settings
    target_compile_definitions(SamplerPlayer PRIVATE
        JUCE_ALSA=1
        JUCE_JACK=1
    )
endif()

# ============================================================================
# Installation (optional)
# ============================================================================

install(TARGETS SamplerPlayer
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

# ============================================================================
# Print build information
# ============================================================================

message(STATUS "")
message(STATUS "========================================")
message(STATUS "  SamplerPlayer Build Configuration")
message(STATUS "========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "========================================")
message(STATUS "")
