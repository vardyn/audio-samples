cmake_minimum_required (VERSION 2.8 FATAL_ERROR)

project (AUDIO_SAMPLES C)
set(CMAKE_MODULE_PATH ${AUDIO_SAMPLES_SOURCE_DIR}/cmake/modules ${CMAKE_MODULE_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AUDIO_SAMPLES_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AUDIO_SAMPLES_BINARY_DIR}/lib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/AudioSamplesCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/AudioSamplesIO)

add_subdirectory(src)
add_subdirectory(test)
