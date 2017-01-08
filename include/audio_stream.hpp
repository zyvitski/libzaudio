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
  class audio_stream : protected stream_params<sample_t>
  {
  public:
      using callback = stream_callback<sample_t>;
      using context_type = stream_context<sample_t>;
      using stream_params_type = stream_params<sample_t>;
      audio_stream() : stream_params_type(),
                       _context(default_stream_context<sample_t>()),
                       _error_callback(default_stream_error_callback())
      {
          init();
      }
      audio_stream(const stream_params_type& params,
                   const callback& cb,
                   const stream_error_callback& error_callback = default_stream_error_callback()) : stream_params_type(params),
                                                                                                    _context(default_stream_context<sample_t>()),
                                                                                                    _callback(cb),
                                                                                                    _error_callback(error_callback)
      {
          init();
      }
      audio_stream(const stream_params_type& params,
                   context_type& ctx,
                   const callback& cb,
                   const stream_error_callback& error_callback = default_stream_error_callback()) : stream_params_type(params),
                                                                                                    _callback(cb),
                                                                                                    _error_callback(error_callback),
                                                                                                    _context(ctx)
      {
          init();
      }
      audio_stream(const stream_params_type& params,
                   audio_process<sample_t>& proc) : stream_params_type(params),
                                                    _callback(proc.get_callback()),
                                                    _error_callback(proc.get_error_callback()),
                                                    _context(default_stream_context<sample_t>())
      {
          init();
      }
      audio_stream(const stream_params_type& params,
                   context_type& ctx, audio_process<sample_t>& proc) : stream_params_type(params),
                                                                       _callback(proc.get_callback()),
                                                                       _error_callback(proc.get_error_callback()),
                                                                       _context(ctx)
      {
          init();
      }
      ~audio_stream()
      {
          destroy();
      }
      stream_error start() noexcept
      {
          return _context.get().api()->start();
      }
      stream_error pasue() noexcept
      {
          return _context.get().api()->pause();
      }
      stream_error stop() noexcept
      {
          return _context.get().api()->stop();
      }
      stream_error playback_state() noexcept
      {
          return _context.get().api()->playback_state();
      }
      callback exchange_callback(callback&& cb,std::chrono::milliseconds duration = std::chrono::milliseconds(1000))
      {
          std::unique_lock<std::mutex> lk{_context.get().api()->callback_mutex(),std::defer_lock};
          if(lk.try_lock_for(duration))
          {
              std::swap(_callback,cb);
              return cb;
          }
          else
          {
              throw std::runtime_error("Unable to aquire lock to exchange user callback!");
          }
      }
      stream_error_callback exchange_error_callback(stream_error_callback&& cb,
                                                    std::chrono::milliseconds duration = std::chrono::milliseconds(1000))
      {
          std::unique_lock<std::mutex> lk{_context.get().api()->callback_mutex(),std::defer_lock};
          if(lk.try_lock_for(duration))
          {
              std::swap(_error_callback,cb);
              return cb;
          }
          else
          {
              throw std::runtime_error("Unable to aquire lock to exchange user error callback!");
          }
      }

      const stream_params_type& params() noexcept
      {
          return *(stream_params_type*)this;

      }
      void params(const stream_params_type& p) noexcept
      {
          *(stream_params_type*)this = p;
      }

  private:
      void init()
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
      void destroy() noexcept
      {
          _context.get().api()->close_stream();

      }
      callback _callback;
      stream_error_callback _error_callback;
      std::reference_wrapper<context_type> _context;
  };

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
  audio_stream<sample_t> make_audio_stream(args_t&&... args)
  {
      return audio_stream<sample_t>(args...);
  }

}

#endif
