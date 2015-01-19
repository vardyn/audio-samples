add_executable(test_riff_read test_riff_read.c)
target_link_libraries(test_riff_read AudioSamplesCore
                                     AudioSamplesIO)
