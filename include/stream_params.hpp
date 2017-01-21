#ifndef ZAUDIO_STREAM_PARAMS
#define ZAUDIO_STREAM_PARAMS

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


#include <cstddef>
#include <ostream>
#include <functional>
#include "sample_utility.hpp"
#include "time_utility.hpp"


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
    /*!
     *\class stream_params
     *\brief a collection of values that describe the audio stream setings
     */
    template<typename sample_t>
    class stream_params : public detail::fail_if_type_is_not_sample<sample_t>
    {
    public:
        constexpr stream_params() noexcept;

        constexpr stream_params(double sr,
                                std::size_t fc,
                                std::size_t fw) noexcept;

        constexpr stream_params(double sr,
                                std::size_t fc,
                                std::size_t ifw,
                                std::size_t ofw) noexcept;

        constexpr stream_params(double sr,
                                std::size_t fc,
                                std::size_t ifw,
                                std::size_t ofw,
                                long idid,
                                long odid) noexcept;

        constexpr const std::size_t& frame_count() const noexcept;

        constexpr const std::size_t& input_frame_width() const noexcept;

        constexpr const std::size_t& output_frame_width() const noexcept;

        constexpr const std::size_t input_sample_count() const noexcept;

        constexpr const std::size_t output_sample_count() const noexcept;

        constexpr const double& sample_rate() const noexcept;

        constexpr const double input_byte_rate() const noexcept;

        constexpr const double input_bit_rate() const noexcept;

        constexpr const double output_byte_rate() const noexcept;

        constexpr const double output_bit_rate() const noexcept;

        constexpr const long& input_device_id() const noexcept;

        constexpr const long& output_device_id() const noexcept;

        friend std::ostream& operator<<(std::ostream& os, stream_params<sample_t>& params);

    private:
        std::size_t _input_frame_width;

        std::size_t _output_frame_width;

        std::size_t _frame_count;

        double _sample_rate;

        long _input_device_id;

        long _output_device_id;

    };


    template<typename sample_t>
    constexpr stream_params<sample_t>::stream_params() noexcept: _input_frame_width(2),
                                                                 _output_frame_width(2),
                                                                 _frame_count(512),
                                                                 _sample_rate(44100),
                                                                 _input_device_id(-1),
                                                                 _output_device_id(-1)
    {}

    template<typename sample_t>
    constexpr stream_params<sample_t>::stream_params(double sr,
                                                     std::size_t fc,
                                                     std::size_t fw) noexcept : _input_frame_width(fw),
                                                                                _output_frame_width(fw),
                                                                                _frame_count(fc),
                                                                                _sample_rate(sr),
                                                                                _input_device_id(-1),
                                                                                _output_device_id(-1)
    {}

    template<typename sample_t>
    constexpr stream_params<sample_t>::stream_params(double sr,
                                                     std::size_t fc,
                                                     std::size_t ifw,
                                                     std::size_t ofw) noexcept : _input_frame_width(ifw),
                                                                                 _output_frame_width(ofw),
                                                                                 _frame_count(fc),
                                                                                 _sample_rate(sr),
                                                                                 _input_device_id(-1),
                                                                                 _output_device_id(-1)
    {}

    template<typename sample_t>
    constexpr stream_params<sample_t>::stream_params(double sr,
                                                     std::size_t fc,
                                                     std::size_t ifw,
                                                     std::size_t ofw,
                                                     long idid,
                                                     long odid) noexcept : _input_frame_width(ifw),
                                                                           _output_frame_width(ofw),
                                                                           _frame_count(fc),
                                                                           _sample_rate(sr),
                                                                           _input_device_id(idid),
                                                                           _output_device_id(odid)
    {}

    template<typename sample_t>
    constexpr const std::size_t& stream_params<sample_t>::frame_count() const noexcept
    {
        return _frame_count;
    }

    template<typename sample_t>
    constexpr const std::size_t& stream_params<sample_t>::input_frame_width() const noexcept
    {
        return _input_frame_width;
    }

    template<typename sample_t>
    constexpr const std::size_t& stream_params<sample_t>::output_frame_width() const noexcept
    {
        return _output_frame_width;
    }

    template<typename sample_t>
    constexpr const std::size_t stream_params<sample_t>::input_sample_count() const noexcept
    {
        return _input_frame_width * _frame_count;
    } // frame_width * frame_count

    template<typename sample_t>
    constexpr const std::size_t stream_params<sample_t>::output_sample_count() const noexcept
    {
        return _output_frame_width * _frame_count;
    } // frame_width * frame_count

    template<typename sample_t>
    constexpr const double& stream_params<sample_t>::sample_rate() const noexcept
    {
        return _sample_rate;
    }

    template<typename sample_t>
    constexpr const double stream_params<sample_t>::input_byte_rate() const noexcept
    {
        return sample_rate() * input_sample_count() * sizeof(sample_t);
    }

    template<typename sample_t>
    constexpr const double stream_params<sample_t>::input_bit_rate() const noexcept
    {
        return input_byte_rate() * 8;
    }

    template<typename sample_t>
    constexpr const double stream_params<sample_t>::output_byte_rate() const noexcept
    {
        return sample_rate() * output_sample_count() * sizeof(sample_t);
    }

    template<typename sample_t>
    constexpr const double stream_params<sample_t>::output_bit_rate() const noexcept
    {
        return output_byte_rate() * 8;
    }

    template<typename sample_t>
    constexpr const long& stream_params<sample_t>::input_device_id() const noexcept
    {
        return _input_device_id;
    }

    template<typename sample_t>
    constexpr const long& stream_params<sample_t>::output_device_id() const noexcept
    {
        return _output_device_id;
    }

    template<typename sample_t>
    std::ostream& operator<<(std::ostream& os, stream_params<sample_t>& params)
    {
        os<<"Input Frame Width: "<<params.input_frame_width()<<std::endl;
        os<<"Output Frame Width: "<<params.output_frame_width()<<std::endl;
        os<<"Frame Count: "<<params.frame_count()<<std::endl;
        os<<"Sample Rate: "<<params.sample_rate()<<std::endl;
        os<<"Input Device ID: "<<params.input_device_id()<<std::endl;
        os<<"Ouput Device ID: "<<params.output_device_id()<<std::endl;
        return os;
    }



    /*!
     *\fn make_stream_params
     *\brief helper function that creates a stream_params object
     */

    template<typename sample_t, typename... args_t>
    constexpr stream_params<typename std::decay<sample_t>::type> make_stream_params(args_t&&...args) noexcept
    {
        return stream_params<typename std::decay<sample_t>::type>(std::forward<args_t&&>(args)...);
    }


}

#endif
