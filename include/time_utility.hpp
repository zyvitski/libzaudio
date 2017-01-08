#ifndef ZAUDIO_TIME_UTILITY
#define ZAUDIO_TIME_UTILITY

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

#include <chrono>
#include <thread>
#include <utility>
#include <type_traits>


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
    /*!
    *\typedef monotonic_clock
    *\brief a clock guarenteed to be monotonic.
    *\note std::chrono::high_resolution_clock will be used only when std::chrono::high_resolution_clock::is_steady == true
    */
    using monotonic_clock = typename std::conditional<std::chrono::high_resolution_clock::is_steady,
                                                      std::chrono::high_resolution_clock,
                                                      std::chrono::steady_clock>::type;

    /*!
    *\struct stream_time_base
    *\brief a base class that provides typedefs of audio utilities
    */
    struct stream_time_base
    {
        using audio_clock = monotonic_clock;
        using duration = std::chrono::duration<double>;
        using time_point = audio_clock::time_point;
    };

    /*!
     *\typedef duration
     *\brief represents a duration of time based on the audio_clock
     */
    using duration = stream_time_base::duration;

    /*!
     *\typedef time_point
     *\brief represents a point in time based on the audio_clock
     */
    using time_point = stream_time_base::time_point;

    /*!
     *\fn duration_in_samples
     *\brief converts a duration to a duration that is based on a fixed sample rate
     */
    template<unsigned long SRATE,typename D>
    std::chrono::duration<double,std::ratio<1,SRATE>> duration_in_samples(D&& duration) noexcept
    {
        return std::chrono::duration_cast<std::chrono::duration<double,std::ratio<1,SRATE>>>(std::forward<D&&>(duration));
    }

    /*!
     *\enum use_thread_sleep
     *\brief tags that specify if thread sleep should be used
     */
    enum class use_thread_sleep
    {
        enable,
        disable
    };

    /*!
     *\struct sleep_type
     *\brief a sleep function object type
     */
    template<typename C,use_thread_sleep TS = use_thread_sleep::disable>
    struct sleep_type;

    /*!
     *\struct sleep_type
     *\brief a sleep function object type
     */
    template<typename C>
    struct sleep_type<C , use_thread_sleep::enable>
    {

        template<typename D>
        void operator()(D&& duration) noexcept
        {
            //timestamp in
            auto&& start = C::now();
            //sleep for most of the time requested
            std::this_thread::sleep_for( duration * 0.95 );
            //spin for the rest to ensure accuracy
            while ( std::chrono::duration_cast<D>( C::now() - start ) < duration ) { continue; }
        }
    };

    /*!
     *\struct sleep_type
     *\brief a sleep function object type
     */
    template<typename C>
    struct sleep_type<C , use_thread_sleep::disable>
    {
        template<typename D>
        void operator()(D&& duration) noexcept
        {
            auto&& start = C::now();//timestamp in

            while ( std::chrono::duration_cast<D>( C::now() - start ) < duration ) { continue; }
        }
    };

    /*!
     *\fn sleep
     *\brief a sleep function that can use an arbitrary duration
     */
    template<typename D>
    inline void sleep(D&& duration) noexcept
    {
        static sleep_type<monotonic_clock,use_thread_sleep::disable> _sleep;
        _sleep(std::forward<D&&>(duration));
    }

    /*!
     *\fn thread_sleep
     *\brief a sleep function that can use an arbitrary duration and puts the thread to sleep for a portion of the duration
     */
    template<typename D>
    inline void thread_sleep(D&& duration) noexcept
    {
        static sleep_type<monotonic_clock,use_thread_sleep::enable> _sleep;
        _sleep(std::forward<D&&>(duration));
    }
}

#endif
