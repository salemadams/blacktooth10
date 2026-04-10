cmake_minimum_required(VERSION 3.22)

project(TenBand VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(JUCE_PATH "/Users/salemadams/Desktop/JUCE" CACHE PATH "Path to the JUCE framework")
add_subdirectory(${JUCE_PATH} ${CMAKE_BINARY_DIR}/JUCE)

juce_add_plugin(TenBand
    PRODUCT_NAME                "10band"
    COMPANY_NAME                "SalemAdams"
    BUNDLE_ID                   com.salemadams.tenband
    PLUGIN_MANUFACTURER_CODE    Sadm
    PLUGIN_CODE                 Tnbd
    FORMATS                     AU VST3 Standalone
    IS_SYNTH                    FALSE
    NEEDS_MIDI_INPUT            FALSE
    NEEDS_MIDI_OUTPUT           FALSE
    IS_MIDI_EFFECT              FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD     TRUE)

target_sources(TenBand PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginEditor.cpp)

target_compile_definitions(TenBand PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_STRICT_REFCOUNTEDPOINTER=1)

target_link_libraries(TenBand
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
