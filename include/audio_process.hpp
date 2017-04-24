#ifndef ZAUDIO_AUDIO_PROCESS
#define ZAUDIO_AUDIO_PROCESS

/*
 * This file is part of zaudio.
 *
 *  zaudio is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  zaudio is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with zaudio.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "sample_utility.hpp"
#include "time_utility.hpp"
#include "error_utility.hpp"
#include "stream_params.hpp"
#include "stream_callback.hpp"
#include "buffer_group.hpp"


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
/*!
 *\class audio_process
 *\brief helper class that allows you to work with zaudio in an polymorphic manor
 */
template <typename sample_t>
struct audio_process : public detail::fail_if_type_is_not_sample<sample_t> {
    using audio_clock = stream_time_base::audio_clock;

    using callback = stream_callback<sample_t>;

    using error_callback = stream_error_callback;

    virtual stream_error on_process(buffer_group<sample_t> &, time_point, stream_params<sample_t> &) noexcept;

    virtual void
    on_error(const stream_error& err) noexcept;

    callback
    get_callback() noexcept;

    error_callback
    get_error_callback() noexcept;
};

template <typename sample_t>
stream_error
audio_process<sample_t>::on_process(buffer_group<sample_t>&, time_point, stream_params<sample_t> &) noexcept
{
    return no_error;
}

template <typename sample_t>
void
audio_process<sample_t>::on_error(const stream_error &err) noexcept
{
    default_stream_error_callback()(err);
}

template <typename sample_t>
typename audio_process<sample_t>::callback
audio_process<sample_t>::get_callback() noexcept
{
    using std::placeholders::_1;
    using std::placeholders::_2;
    using std::placeholders::_3;
    return callback(std::bind(&audio_process<sample_t>::on_process, this, _1, _2, _3));
}

template <typename sample_t>
typename audio_process<sample_t>::error_callback
audio_process<sample_t>::get_error_callback() noexcept
{
    using std::placeholders::_1;
    return std::bind(&audio_process<sample_t>::on_error, this, _1);
}
}

#endif // ifndef ZAUDIO_AUDIO_PROCESS
