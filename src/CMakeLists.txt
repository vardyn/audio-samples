find_package(LibAO REQUIRED)
include_directories(${LIBAO_INCLUDE_DIR})

find_library(M_LIB m REQUIRED)

add_subdirectory(AudioSamplesCore)
add_subdirectory(AudioSamplesIO)

add_executable(impulse_wav impulse_wav.c)
target_link_libraries(impulse_wav AudioSamplesCore
                                  AudioSamplesIO
                                  ${LIBAO_LIBRARY}
                                  ${M_LIB})

add_executable(simple_kick simple_kick.c)
target_link_libraries(simple_kick AudioSamplesCore
                                  AudioSamplesIO
                                  ${LIBAO_LIBRARY}
                                  ${M_LIB})

add_executable(prbs13 prbs13.c)
target_link_libraries(prbs13 AudioSamplesCore
                             AudioSamplesIO
                             ${LIBAO_LIBRARY}
                             ${M_LIB})
