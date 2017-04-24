#ifndef ZAUDIO_STREAM_CONTEXT
#define ZAUDIO_STREAM_CONTEXT

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

#include <vector>

#include "sample_utility.hpp"
#include "stream_params.hpp"
#include "stream_api.hpp"
#include "device_info.hpp"


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
/*!
 *\class stream_context
 *\brief
 * //NOTE: DOES NOT ACCOUNT FOR MULTIPLE STREAMS YET
 * //TODO: Account for multiple streams / APIs
 */


template <typename sample_t>
class stream_context : public detail::fail_if_type_is_not_sample<sample_t>
{
public:
    using api_type = stream_api<sample_t>;

    using stream_params_type = stream_params<sample_t>;

    using callback = stream_callback<sample_t>;

    explicit
    stream_context(std::unique_ptr<api_type> api = default_api<sample_t>()) noexcept;

    api_type *
    api() const noexcept;

    long
    get_device_count() noexcept;

    device_info
    get_device_info(long id) noexcept;

    std::vector<device_info>
    get_device_info_list() noexcept;

    stream_error
    is_configuration_supported(const stream_params_type& params) noexcept;

    long
    default_input_device_id() const noexcept;

    long
    default_output_device_id() const noexcept;

private:
    std::unique_ptr<api_type> _api;
};

template <typename sample_t>
stream_context<sample_t>::stream_context(std::unique_ptr<typename stream_context<sample_t>::api_type> api) noexcept :
    _api(std::move(api))
{ }

template <typename sample_t>
typename stream_context<sample_t>::api_type * stream_context<sample_t>::api() const noexcept
{
    return _api.get();
}    // get the api

template <typename sample_t>
long
stream_context<sample_t>::get_device_count() noexcept
{
    return _api.get()->get_device_count();
}

template <typename sample_t>
device_info
stream_context<sample_t>::get_device_info(long id) noexcept
{
    return _api.get()->get_device_info(id);
}

template <typename sample_t>
std::vector<device_info>
stream_context<sample_t>::get_device_info_list() noexcept
{
    std::vector<device_info> out;
    for (long i = 0; i < get_device_count(); ++i) {
        out.push_back(get_device_info(i));
    }
    return out;
}

template <typename sample_t>
stream_error
stream_context<sample_t>::is_configuration_supported(const typename stream_context<sample_t>::stream_params_type& params)
noexcept
{
    return _api.get()->is_configuration_supported(params);
}

template <typename sample_t>
long
stream_context<sample_t>::default_input_device_id() const noexcept
{
    return _api.get()->default_input_device_id();
}

template <typename sample_t>
long
stream_context<sample_t>::default_output_device_id() const noexcept
{
    return _api.get()->default_output_device_id();
}

/*!
 *\fn make_stream_context
 *\brief helper function that creates a stream_context object
 */
template <typename sample_t, typename ... args_t>
stream_context<typename std::decay<sample_t>::type>
make_stream_context(args_t&& ... args) noexcept
{
    return stream_context<typename std::decay<sample_t>::type> { std::forward<args_t&&>(args) ... };
}

/*!
 *\fn default_stream_context
 *\brief helper function that creates the a stream_context object with default values
 */
template <typename sample_t>
stream_context<typename std::decay<sample_t>::type>
default_stream_context() noexcept;
}

#endif // ifndef ZAUDIO_STREAM_CONTEXT
