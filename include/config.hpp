#ifndef ZAUDIO_CONFIG_HPP
#define ZAUDIO_CONFIG_HPP

#ifdef _WIN32
#define ZAUDIO_EXPORT __declspec(dllexport)
#elif _WIN64
#define ZAUDIO_EXPORT __declspec(dllexport)
#else
#define ZAUDIO_EXPORT
#endif

#endif
