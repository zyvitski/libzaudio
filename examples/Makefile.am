bin_PROGRAMS = sine_example device_probing
device_probing_SOURCES = device_probing.cpp
sine_example_SOURCES = sine.cpp
AM_CXXFLAGS = --std=c++11 -g -O3 -pthread -fpic  -MP -Wall -pedantic -I/usr/local/include -I../include
AM_LDFLAGS = -L/usr/local/lib
sine_example_LDFLAGS = -lzaudio -lportaudio
device_probing_LDFLAGS = -lzaudio -lportaudio
