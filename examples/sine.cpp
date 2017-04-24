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


#include <iostream>
#include <cmath>
#include <thread>
#include <zaudio.hpp>


using namespace zaudio;
template <typename sample_t>
class example : public audio_process<sample_t>
{
public:
    using base        = audio_process<sample_t>;
    using audio_clock = typename base::audio_clock;
    example() : phs(0.0), start(audio_clock::now())
    { }

    virtual stream_error
    on_process(buffer_group<sample_t>& buffers, time_point stream_time, stream_params<sample_t>& params) noexcept
    {
        std::cerr << "Time: " << duration_in_samples(stream_time - start, params.sample_rate()).count() << std::endl;

        stp = hz / params.sample_rate() * two_pi;

        for (auto&& frame: buffers.output) {
            auto&& value = std::sin(phs);
            if ((phs += stp) > two_pi) phs -= two_pi; for (auto&& samp: frame) {
                samp = value;
            }
        }
        return no_error;
    }

private:

    float hz = 440.0;
    sample_t phs;
    sample_t stp;
    time_point start;
};


int
main(int agrc, char ** argv)
{
    try
    {
        using sample_type = sample<sample_format::f32>;
        auto&& context = make_stream_context<sample_type>();
        auto&& params  = make_stream_params<sample_type>(44100, 512, 0, 2);
        if (context.is_configuration_supported(params) == no_error) {
            example<sample_type> ex;

            auto&& stream = make_audio_stream<sample_type>(params, context, ex);

            stream.start();
            std::cout << stream.playback_state().first << std::endl;
            sleep(std::chrono::seconds(1));
            stream.stop();
            std::cout << stream.playback_state().first << std::endl;
        } else   {
            std::cout << "UNSUPPORTED" << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
