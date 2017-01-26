bindir = $(exec_prefix)/bin/zaudio

bin_PROGRAMS = sine_example device_probing lambda_sine_example playthrough

device_probing_SOURCES = device_probing.cpp
sine_example_SOURCES = sine.cpp
lambda_sine_example_SOURCES = lambda_sine.cpp
playthrough_SOURCES = playthrough.cpp
AM_CXXFLAGS = --std=c++11 -g -O3 -pthread -fpic  -MP -Wall -pedantic -I/usr/local/include -I../include
AM_LDFLAGS = -L/usr/local/lib
sine_example_LDFLAGS = -lzaudio 
device_probing_LDFLAGS = -lzaudio
lambda_sine_example_LDFLAGS= -lzaudio
playthrough_LDFLAGS = -lzaudio
