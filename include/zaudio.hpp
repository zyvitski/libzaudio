
#ifndef zaudio_hpp
#define zaudio_hpp

/*
This file is part of zaudio.

    zaudio is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    zaudio is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with zaudio.  If not, see <http://www.gnu.org/licenses/>.
*/



#include "sample_utility.hpp"
#include "time_utility.hpp"
#include "error_utility.hpp"
#include "stream_params.hpp"
#include "device_info.hpp"
#include "stream_api.hpp"
#include "stream_context.hpp"
#include "audio_stream.hpp"
#include "audio_process.hpp"
#include "pa_stream_api.hpp"

#include "zaudio_defaults.hpp"


/*!
*\note At least one stream api must be present on every platform.
       The provided api is permitted to be a non functional dummy api for platforms where a proper api is impossible
       Preferabbly on any platform that has a filesystem, a file codec api will be provided
       Preferabbly on any platform that has an audio system an hardware api will be provided

       IDEA:
            Network serialization api?
                an api that is designed to work with the std::networking proposal for sending audio over network

       All of the apis will share a uniform interface
       Ideally multiple apis can be used at once
            TODO: come up with endpoint class to make multi api io possible
*/




#endif
