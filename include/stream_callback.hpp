#ifndef ZAUDIO_STREAM_CALLBACK
#define ZAUDIO_STREAM_CALLBACK
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
#include <functional>
#include "stream_params.hpp"
#include "time_utility.hpp"
namespace zaudio
{
    /*!
     *\typedef stream_callback
     *\brief a function object type that represents an audio stream callback function
     */
    template<typename sample_t>
    using stream_callback  = std::function<stream_error (const sample_t*, sample_t*, time_point, stream_params<sample_t>&) noexcept>;
}

#endif
