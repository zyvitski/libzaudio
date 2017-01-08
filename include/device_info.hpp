#ifndef ZAUDIO_DEVICE_INFO
#define ZAUDIO_DEVICE_INFO

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
#include <vector>
#include "time_utility.hpp"


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
    /*!
     *\struct device_info
     *\brief represents information about an audio device
     */
    struct device_info
    {
        device_info() noexcept;
        device_info(const char* nm,
                    std::size_t idx,
                    std::size_t mic,
                    std::size_t moc,
                    double srate,
                    duration dlil,
                    duration dhil,
                    duration dlol,
                    duration dhol) noexcept;


        const char* name;
        std::size_t device_index;
        std::size_t max_input_count;
        std::size_t max_output_count;
        double default_sample_rate;
        duration default_low_input_latency;
        duration default_high_input_latency;
        duration default_low_ouput_latency;
        duration default_high_output_latency;

        friend std::ostream& operator<<(std::ostream& os, const device_info& info);
    };
    /*!
     *\fn std::vector<device_info> operator<<
     *\brief enables printing of a vector of device_info objects
     */
    std::ostream& operator<<(std::ostream& os, const std::vector<device_info>& vinfo);


    /*!
     *\fn make_device_info
     *\brief helper function for creating device_info objects
     */
    template<typename... args_t>
    device_info make_device_info(args_t&&... args) noexcept
    {
        return device_info(std::forward<args_t&&>(args)...);
    }
}

#endif
