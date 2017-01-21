# Libzaudio [![](https://travis-ci.org/zyvitski/libzaudio.svg?branch=master)](https://travis-ci.org/zyvitski/libzaudio)

## A Modern C++11 Audio Library

Written By: Alex Zywicki 2016-2017

#### Dependencies:

As of version 0.1.1 Libzaudio depends on [PortAudio.](http://www.portaudio.com/) See the PortAudio website for instructions on installation.

**NOTE:**Libzaudio currently uses PortAudio as a means to help rapidly develop the Libzaudio API. Once Libzaudio's API is feature complete work will begin to replace the use of PortAudio with platform specific audio APIs.

#### License:

Libzaudio is released under the GNU LGPL license. For more information see the file COPYING.lesser  

#### Installation:

To install the latest release: Download the latest .tar archive from the [releases github page](https://github.com/zyvitski/libzaudio/releases). Unpack the archive then run **./configure && make** additionally you may run **sudo make install** to install the binaries and headers.

For development you should clone the latest version of the dev branch. Run **./autogen.sh && ./configure && make** to build.
