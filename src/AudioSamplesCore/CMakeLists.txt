add_library(AudioSamplesCore SHARED ASCFunctions.c
                                    ASCLogging.c
                                    ASCSimpleLogging.c
                                    Envelopes.c
                                    Waveforms.c)
target_link_libraries(AudioSamplesCore ${M_LIB})
