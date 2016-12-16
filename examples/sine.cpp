#include <iostream>
#include <cmath>
#include <thread>
#include <libzaudio/zaudio.hpp>


using namespace zaudio;
template<typename sample_t>
class example : public audio_process<sample_t>
{
public:
    example():phs(0.0)
    {}
    virtual stream_error on_process(const sample_t* input, sample_t* output,duration stream_time, stream_params<sample_t>& params) noexcept
    {
        std::cout<< stream_time.count()<<std::endl;
        constexpr sample_t _2pi = M_PI * 2.0;
        stp = hz / params.sample_rate() * _2pi;

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
    }

private:
    float hz = 55.0;
    sample_t phs;
    sample_t stp;
};


int main(int agrc, char** argv)
{
    try
    {
        using sample_type = sample<sample_format::f32>;
        auto&& context = make_stream_context<sample_type>();
        auto&& params = make_stream_params<sample_type>(44100,512,0,2);
        if(context.is_configuration_supported(params) == no_error)
        {
            example<sample_type> ex;

            auto&& stream = make_audio_stream<sample_type>(params,context,ex);

            stream.start();
            std::cout<<stream.playback_state().first<<std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            stream.stop();
            std::cout<<stream.playback_state().first<<std::endl;
        }
        else
        {
            std::cout<<"UNSUPPORTED"<<std::endl;
        }
    }
    catch(std::exception& e)
    {
        std::cerr<<e.what()<<std::endl;
    }
    return 0;
}
