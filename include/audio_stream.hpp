#ifndef ZAUDIO_AUDIO_STREAM
#define ZAUDIO_AUDIO_STREAM

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

#include "error_utility.hpp"
#include "stream_params.hpp"
#include "stream_context.hpp"

#include <type_traits>


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
    template<typename sample_t>
    struct audio_process;


    /*!
    *\class audio_stream
    *\brief represents a program level connection to the audio context
    */

    template<typename sample_t>
    class audio_stream : public detail::fail_if_type_is_not_sample<sample_t>
    {
    public:
      using callback = stream_callback<sample_t>;

      using context_type = stream_context<sample_t>;

      using stream_params_type = stream_params<sample_t>;

      audio_stream();

      audio_stream(const stream_params_type& params,
                   const callback& cb,
                   const stream_error_callback& error_callback = default_stream_error_callback());

      audio_stream(const stream_params_type& params,
                   context_type& ctx,
                   const callback& cb,
                   const stream_error_callback& error_callback = default_stream_error_callback());

      audio_stream(const stream_params_type& params,
                   audio_process<sample_t>& proc);

      audio_stream(const stream_params_type& params,
                   context_type& ctx, audio_process<sample_t>& proc);

      ~audio_stream();

      stream_error start() noexcept;

      stream_error pasue() noexcept;

      stream_error stop() noexcept;

      stream_error playback_state() noexcept;

      callback exchange_callback(callback&& cb);

      stream_error_callback exchange_error_callback(stream_error_callback&& cb);

      const stream_params_type& params() noexcept;

      void params(const stream_params_type& p) noexcept;


    private:
      void init();

      void destroy() noexcept;

      stream_params_type _params;

      callback _callback;

      stream_error_callback _error_callback;

      std::reference_wrapper<context_type> _context;

    };


    template<typename sample_t>
    audio_stream<sample_t>::audio_stream() : _params(),
                                             _context(default_stream_context<sample_t>()),
                                             _error_callback(default_stream_error_callback())
    {
        init();
    }

    template<typename sample_t>
    audio_stream<sample_t>::audio_stream(const stream_params_type& params,
                                         const callback& cb,
                                         const stream_error_callback& error_callback) : _params(params),
                                                                                        _context(default_stream_context<sample_t>()),
                                                                                        _callback(cb),
                                                                                        _error_callback(error_callback)
    {
        init();
    }

    template<typename sample_t>
    audio_stream<sample_t>::audio_stream(const stream_params_type& params,
                                         context_type& ctx,
                                         const callback& cb,
                                         const stream_error_callback& error_callback) : _params(params),
                                                                                        _callback(cb),
                                                                                        _error_callback(error_callback),
                                                                                        _context(ctx)
    {
        init();
    }

    template<typename sample_t>
    audio_stream<sample_t>::audio_stream(const stream_params_type& params,
                                         audio_process<sample_t>& proc) : _params(params),
                                                                          _callback(proc.get_callback()),
                                                                          _error_callback(proc.get_error_callback()),
                                                                          _context(default_stream_context<sample_t>())
    {
        init();
    }

    template<typename sample_t>
    audio_stream<sample_t>::audio_stream(const stream_params_type& params,
                                         context_type& ctx,
                                         audio_process<sample_t>& proc) : _params(params),
                                                                          _callback(proc.get_callback()),
                                                                          _error_callback(proc.get_error_callback()),
                                                                          _context(ctx)
    {
        init();
    }

    template<typename sample_t>
    audio_stream<sample_t>::~audio_stream()
    {
        destroy();
    }

    template<typename sample_t>
    stream_error audio_stream<sample_t>::start() noexcept
    {
        return _context.get().api()->start();
    }

    template<typename sample_t>
    stream_error audio_stream<sample_t>::pasue() noexcept
    {
        return _context.get().api()->pause();
    }

    template<typename sample_t>
    stream_error audio_stream<sample_t>::stop() noexcept
    {
        return _context.get().api()->stop();
    }

    template<typename sample_t>
    stream_error audio_stream<sample_t>::playback_state() noexcept
    {
        return _context.get().api()->playback_state();
    }

    template<typename sample_t>
    typename audio_stream<sample_t>::callback audio_stream<sample_t>::exchange_callback(callback&& cb)
    {
        auto&& out = _callback;
        auto& mtx = _context.get().api()->callback_mutex();
        std::unique_lock<std::mutex> lk{mtx,std::defer_lock};
        if(lk.try_lock_for(std::chrono::seconds(1)))
        {
            _callback = cb;
            return out;
        }
        else
        {
            throw stream_exception(make_stream_error(stream_status::system_error, "Unable to aquire lock to swap callback."));
        }
    }

    template<typename sample_t>
    stream_error_callback audio_stream<sample_t>::exchange_error_callback(stream_error_callback&& cb)
    {
        auto&& out = _error_callback;
        auto& mtx = _context.get().api()->callback_mutex();
        std::unique_lock<std::mutex> lk{mtx,std::defer_lock};
        if(lk.try_lock_for(std::chrono::seconds(1)))
        {
            _error_callback = cb;
            return out;
        }
        else
        {
            throw stream_exception(make_stream_error(stream_status::system_error, "Unable to aquire lock to swap error callback."));
        }
    }

    template<typename sample_t>
    const typename audio_stream<sample_t>::stream_params_type& audio_stream<sample_t>::params() noexcept
    {
        return _params;
    }

    template<typename sample_t>
    void audio_stream<sample_t>::params(const stream_params_type& p) noexcept
    {
        _params = p;
    }


    template<typename sample_t>
    void audio_stream<sample_t>::init()
    {
        _context.get().api()->set_callback(_callback);
        _context.get().api()->set_error_callback(_error_callback);
        auto&& is_compat = _context.get().is_configuration_supported(params());
        if(is_compat != no_error)
        {
            throw stream_exception(is_compat);
        }
        _context.get().api()->open_stream(params());
    }

    template<typename sample_t>
    void audio_stream<sample_t>::destroy() noexcept
    {
        _context.get().api()->close_stream();

    }


    /*!
    *\fn start_stream
    *\brief helper function for interacting with the audio stream
    */
    template<typename stream>
    stream_error start_stream(stream&& s) noexcept
    {
      return s.start();
    }

    /*!
    *\fn pause_stream
    *\brief helper function for interacting with the audio stream
    */
    template<typename stream>
    stream_error pause_stream(stream&& s) noexcept
    {
      return s.pause();
    }

    /*!
    *\fn stop_stream
    *\brief helper function for interacting with the audio stream
    */
    template<typename stream>
    stream_error stop_stream(stream&& s) noexcept
    {
      return s.stop();
    }

    /*!
    *\fn make_audio_stream
    *\brief helper function that constructs an audio_stream object
    */
    template<typename sample_t,typename... args_t>
    audio_stream<typename std::decay<sample_t>::type> make_audio_stream(args_t&&... args)
    {
      return audio_stream<typename std::decay<sample_t>::type>(args...);
    }

}

#endif
