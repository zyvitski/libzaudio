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
        //bring the needed zaudio components into scope
        using zaudio::no_error;
        using zaudio::sample;
        using zaudio::sample_format;
        using zaudio::stream_params;
        using zaudio::time_point;
        using zaudio::make_stream_context;
        using zaudio::make_stream_params;
        using zaudio::make_audio_stream;
        using zaudio::start_stream;
        using zaudio::stop_stream;
        using zaudio::thread_sleep;

        //create an alias for a 32 bit float sample
        using sample_type = sample<sample_format::f32>;

        //create a stream context with the default api (portaudio currently)
        auto&& context = make_stream_context<sample_type>();

        //create a stream params object
        auto&& params = make_stream_params<sample_type>(44100,512,0,2);

        //setup to generate a sine wave
        constexpr sample_type _2pi = M_PI * 2.0;
        float hz = 440.0;
        sample_type phs = 0;
        sample_type stp = hz / params.sample_rate() * _2pi;

        //create a zaudio::stream_callback compliant lambda that generates a sine wave
        auto&& callback = [&](const sample_type* input,
                              sample_type* output,
                              time_point stream_time,
                              stream_params<sample_type>& params) noexcept
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

        //create an audio stream using the params, context, and callback created above. Uses the default error callback
        auto&& stream = make_audio_stream<sample_type>(params,context,callback);

        //start the stream
        start_stream(stream);
        //run for 1 second
        thread_sleep(std::chrono::seconds(1));
        //stop the stream
        stop_stream(stream);
    }
    catch (std::exception& e)
    {
        std::cout<<e.what()<<std::endl;
    }
    return 0;
}
