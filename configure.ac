AC_PREREQ([2.66])
AC_INIT([libzaudio],[0.1.4],[alexander.zywicki@gmail.com])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIRS([m4])
AC_CONFIG_SRCDIR([src/libzaudio.cpp])

AM_INIT_AUTOMAKE([foreign ])
LT_INIT


AC_PROG_CXX
AC_CHECK_HEADER(/usr/local/include/portaudio.h,,[AC_MSG_ERROR([Couldn't find portaudio.h ... try downloading source from www.portaudio.com])])

AC_CONFIG_FILES([Makefile src/Makefile examples/Makefile])
AC_OUTPUT
