#ifndef API_TEMPLATE
#define API_TEMPLATE
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


#include <zaudio.hpp>
//other includes

namespace zaudio
{
    template<typename sample_t>
    class api_template : public stream_api<sample_t>
    {
    public:

        using base = stream_api<sample_t>; //alias of base class

        //helpful aliases
        using audio_clock = typename base::audio_clock;
        using callback = typename base::callback;
        using error_callback = typename base::error_callback;

        //use the id function from base
        using base::id;

        //return the name of the API
        virtual std::string name() const noexcept
        {
            return "name";
        }
        //return an info paragraph about the api
        virtual std::string info() const noexcept
        {
            return "info";
        }

        //start playback
        virtual stream_error start() noexcept
        {
            //your code here
            return running;//or error
        }

        //pause playback, or stop if pause is not possible
        virtual stream_error pause() noexcept
        {
            //your code here
            return paused;//or error
        }

        //stop playback
        virtual stream_error stop() noexcept
        {
            //your code here
            return stopped;//or error
        }

        //return the current playback state
        virtual stream_error playback_state() noexcept
        {
            //your code here
            return no_error;
        }

        //return a string with a message that corresponds to a certain error code
        virtual std::string get_error_string(const stream_error& err) noexcept
        {
            //your code here
            return {};
        }

        //open a stream
        virtual stream_error open_stream(const stream_params<sample_t>&) noexcept
        {
            //your code here
            return no_error;
        }

        //close a previously opened stream
        virtual stream_error close_stream() noexcept
        {
            //your code here
            return no_error;
        }

        //return the number of devices available for use with the api
        virtual long get_device_count() noexcept
        {
            //your code here
            return 0;
        }

        //return a device_info for the device at "id"
        virtual device_info get_device_info(long id) noexcept
        {
            //your code here
            return {};
        }

        //test a stream_params structure to determine if the api can process with the requested settings
        virtual stream_error is_configuration_supported(const stream_params<sample_t>& params) noexcept
        {
            //your code here
            return no_error;
        }

        //return the id of the defaule input device
        virtual long default_input_device_id() const noexcept
        {
            //your code here
            return 0;
        }
        //return the id of the default output device
        virtual long default_output_device_id() const noexcept
        {
            //your code here
            return 0;
        }
    private:

        //get access to the callbacks stored in the base
        using base::_error_callback;
        using base::_callback;


        //your code here
    };
}


#endif
