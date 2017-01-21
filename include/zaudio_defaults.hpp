#ifndef ZAUDIO_DEFAULTS
#define ZAUDIO_DEFAULTS

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

#include <memory>
#include "stream_api.hpp"
#include "stream_context.hpp"
#include "pa_stream_api.hpp"

/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
    /*!
     *\fn default_api
     *\brief constructs an instance of the default_api
     */
    template<typename sample_t>
    std::unique_ptr<stream_api<sample_t>> default_api() noexcept
    {
        return make_stream_api<sample_t,pa_stream_api>();
    }

    /*!
     *\fn default_stream_context
     *\brief constructs an instance of the default stream context
     */
    template<typename sample_t>
    stream_context<typename std::decay<sample_t>::type> default_stream_context() noexcept
    {
        return stream_context<typename std::decay<sample_t>::type>{};
    }

}
#endif
