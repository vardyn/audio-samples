add_executable(test_riff_read test_riff_read.c)
target_link_libraries(test_riff_read AudioSamplesCore
                                     AudioSamplesIO)

add_executable(test_riff_read_write test_riff_read_write.c)
target_link_libraries(test_riff_read_write AudioSamplesCore
                                           AudioSamplesIO)

add_executable(test_wav_read test_wav_read.c)
target_link_libraries(test_wav_read AudioSamplesCore
                                    AudioSamplesIO)

add_executable(test_wav_read_write test_wav_read_write.c)
target_link_libraries(test_wav_read_write AudioSamplesCore
                                          AudioSamplesIO)
