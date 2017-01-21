#ifndef ZAUDIO_STREAM_API
#define ZAUDIO_STREAM_API

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
#include "stream_params.hpp"
#include "error_utility.hpp"
#include "device_info.hpp"
#include "stream_callback.hpp"

#include <memory>
#include <mutex>
#include <tuple>
#include <atomic>

/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{

        /*!
         *\class stream_api
         *\brief represents a platform specific interface for interacting with the audio system
         */

        template<typename sample_t>
        class stream_api : public detail::fail_if_type_is_not_sample<sample_t>
        {
        public:
            using audio_clock = stream_time_base::audio_clock;

            using callback = stream_callback<sample_t>;

            using error_callback = stream_error_callback;

            stream_api() noexcept;

            std::size_t id() const noexcept;

            virtual std::string name() const noexcept = 0;

            virtual std::string info() const noexcept = 0;

            virtual stream_error start() noexcept = 0;

            virtual stream_error pause() noexcept = 0;

            virtual stream_error stop() noexcept = 0;

            virtual stream_error playback_state() noexcept = 0;

            virtual std::string get_error_string(const stream_error& err) noexcept = 0;

            virtual stream_error open_stream(const stream_params<sample_t>&) noexcept = 0;

            virtual stream_error close_stream() noexcept = 0;

            virtual long get_device_count() noexcept = 0;

            virtual device_info get_device_info(long id) noexcept = 0;

            virtual stream_error is_configuration_supported(const stream_params<sample_t>& params) noexcept = 0;

            virtual long default_input_device_id() const noexcept = 0;

            virtual long default_output_device_id() const noexcept = 0;

            virtual double cpu_load() const noexcept = 0;

            void set_callback(callback& cb) noexcept;

            void set_error_callback(error_callback& cb) noexcept;

            std::mutex& callback_mutex() noexcept;
        protected:

            callback* _callback;

            error_callback* _error_callback;

            stream_params<sample_t>* _params;

            std::mutex _callback_mutex;

            stream_error _on_process(const sample_t*,sample_t*) noexcept;

        };

        template<typename sample_t>
        stream_api<sample_t>::stream_api() noexcept: _callback(nullptr),
                                                     _error_callback(nullptr),
                                                     _params(nullptr){}

        //id will be assigned based on std::hash<std::string> of name()
        //aka: unique name = unique id
        template<typename sample_t>
        std::size_t stream_api<sample_t>::id() const noexcept
        {
            return std::hash<std::string>{}(name());
        }

        template<typename sample_t>
        void stream_api<sample_t>::set_callback(callback& cb) noexcept
        {
            _callback = &cb;
        }

        template<typename sample_t>
        void stream_api<sample_t>::set_error_callback(error_callback&cb) noexcept
        {
            _error_callback = &cb;
        }

        template<typename sample_t>
        std::mutex& stream_api<sample_t>::callback_mutex() noexcept
        {
            return _callback_mutex;
        }

        template<typename sample_t>
        stream_error stream_api<sample_t>::_on_process(const sample_t* input, sample_t* output) noexcept
        {
            try
            {
                std::unique_lock<std::mutex> lk{ _callback_mutex, std::defer_lock };
                //the only reason we should not get this lock every time is in the case of a user callback swap
                while(!lk.try_lock()){ continue; }

                auto&& ret = (*_callback)(input,output,audio_clock::now(),*_params);
                if(ret != no_error)
                {
                    (*_error_callback)(ret);
                }
                return ret;
            }
            catch (std::exception& e)
            {
                auto&& err = make_stream_error(stream_status::system_error,e.what());
                (*_error_callback)(err);
                return err;
            }
        }


        /*!
         *\fn default_api
         *\brief constructs an instance of the default_api
         */

        template<typename sample_t>
        std::unique_ptr<stream_api<sample_t>> default_api() noexcept;


        /*!
         *\fn make_stream_api
         *\brief contructs an stream_api objects
         */
        template<typename sample_t,template<typename> class api>
        std::unique_ptr<stream_api<typename std::decay<sample_t>::type>> make_stream_api() noexcept
        {
            return std::unique_ptr<stream_api<typename std::decay<sample_t>::type>>{new api<typename std::decay<sample_t>::type>{}};
        }
}

#endif
