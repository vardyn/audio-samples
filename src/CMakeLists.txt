find_package(LibAO REQUIRED)

add_subdirectory(AudioSamplesCore)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/AudioSamplesCore)

add_executable(impulse_wav impulse_wav.c)
include_directories(${LIBAO_INCLUDE_DIR})
target_link_libraries(impulse_wav AudioSamplesCore ${LIBAO_LIBRARY})
