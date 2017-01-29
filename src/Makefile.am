ACLOCAL_AMFLAGS= -I m4

lib_LTLIBRARIES = libzaudio.la
libzaudio_la_SOURCES = libzaudio.cpp
libzaudiodir = $(includedir)/libzaudio
libzaudio_HEADERS =../include/config.hpp ../include/zaudio.hpp ../include/pa_stream_api.hpp ../include/audio_process.hpp ../include/audio_stream.hpp ../include/device_info.hpp ../include/error_utility.hpp ../include/sample_utility.hpp ../include/stream_api.hpp ../include/stream_callback.hpp ../include/stream_context.hpp ../include/stream_params.hpp ../include/time_utility.hpp ../include/zaudio_defaults.hpp ../include/buffer_view.hpp ../include/buffer_group.hpp ../include/constants.hpp
libzaudio_la_LDFLAGS = -version-info 1:2:1 -lportaudio
AM_CXXFLAGS = --std=c++11 -pthread -fpic  -MP -Wall -pedantic -I/usr/local/include -I../include -L/usr/local/lib
AM_CFLAGS = -O3
