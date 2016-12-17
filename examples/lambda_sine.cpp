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


#include <iostream>
#include <cmath>
#include <libzaudio/zaudio.hpp>

int main(int argc, char** argv)
{
    try
    {
        using namespace zaudio;
        using sample_type = sample<sample_format::f32>;
        auto&& context = make_stream_context<sample_type>();
        auto&& params = make_stream_params<sample_type>(44100,512,0,2);


        constexpr sample_type _2pi = M_PI * 2.0;
        float hz = 440.0;
        sample_type phs =0 ;
        sample_type stp = hz / params.sample_rate() * _2pi;

        auto&& callback = [&](const sample_type* input, sample_type* output, duration stream_time, stream_params<sample_type>& params) noexcept
        {
            for(std::size_t i = 0; i < params.frame_count(); ++i)
            {
                for(std::size_t j = 0; j < params.output_frame_width(); ++j)
                {
                    *(output++) = std::sin(phs);
                }
                phs += stp;
                if(phs > _2pi)
                {
                    phs -= _2pi;
                }
            }
            return no_error;
        };

        auto&& stream = make_audio_stream<sample_type>(params,context,callback);
        stream.start();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        stream.stop();
    }
    catch (std::exception& e)
    {
        std::cout<<e.what()<<std::endl;
    }
    return 0;
}
