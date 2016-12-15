lib_LTLIBRARIES = libzaudio.la
libzaudio_la_SOURCES = libzaudio.cpp
libzaudiodir = $(includedir)/libzaudio
libzaudio_HEADERS = ../include/zaudio.hpp ../include/pa_stream_api.hpp
libzaudio_la_LDFLAGS = -version-info 0:1:0
AM_CXXFLAGS = --std=c++11 -g -O3 -pthread -fpic  -MP -Wall -pedantic -I/usr/local/include -I../include -L/usr/local/lib
