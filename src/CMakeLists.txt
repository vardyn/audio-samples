find_package(LibAO REQUIRED)

add_executable(impulse_wav impulse_wav.c)
include_directories(${LIBAO_INCLUDE_DIR})
target_link_libraries(impulse_wav ${LIBAO_LIBRARY})
