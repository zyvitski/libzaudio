#ifndef pa_stream_api_hpp
#define pa_stream_api_hpp

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

#include "zaudio.hpp"
#include <portaudio.h>
#include <tuple>



namespace zaudio
{
    namespace internal
    {


        template<typename sample_t>
        constexpr PaSampleFormat _type_to_pa_sample_format() noexcept
        {
            return -1;
        }

        template<>
        constexpr PaSampleFormat _type_to_pa_sample_format<sample<sample_format::f32>>() noexcept
        {
            return paFloat32;
        }
        template<>
        constexpr PaSampleFormat _type_to_pa_sample_format<sample<sample_format::i8>>() noexcept
        {
            return paInt8;
        }
        template<>
        constexpr PaSampleFormat _type_to_pa_sample_format<sample<sample_format::u8>>() noexcept
        {
            return paUInt8;
        }
        template<>
        constexpr PaSampleFormat _type_to_pa_sample_format<sample<sample_format::i16>>() noexcept
        {
            return paInt16;
        }
        template<>
        constexpr PaSampleFormat _type_to_pa_sample_format<sample<sample_format::i24>>() noexcept
        {
            return paInt24;
        }
        template<>
        constexpr PaSampleFormat _type_to_pa_sample_format<sample<sample_format::i32>>() noexcept
        {
            return paInt32;
        }
    }
    template<typename sample_t>
    class pa_stream_api : public stream_api<sample_t>
    {
        using base = stream_api<sample_t>;
        using audio_clock = typename base::audio_clock;
    public:
        static_assert(internal::_type_to_pa_sample_format<sample_t>() != -1, "Requested Sample Format Not Supported By PortAudio API");
        pa_stream_api()
        {
            if(_pa_invoke(Pa_Initialize) == no_error)
            {
                //TODO: implement me
            }
        }
        virtual ~pa_stream_api()
        {
            if(_pa_invoke(Pa_Terminate) == no_error)
            {
                //TODO: implement me
            }
        }
        using base::id;
        virtual std::string name() const noexcept
        {
            return "LibZaudio: PortAudio Stream API";
        }
        virtual std::string info() const noexcept
        {
            return Pa_GetVersionText();
        }
        virtual stream_error start() noexcept
        {
            return _pa_invoke(Pa_StartStream,stream);
        }
        virtual stream_error pause() noexcept
        {
            return _pa_invoke(Pa_StopStream,stream);
        }
        virtual stream_error stop() noexcept
        {
            return _pa_invoke(Pa_StopStream,stream);
        }
        virtual stream_error playback_state() noexcept
        {
            PaError err = Pa_IsStreamActive(stream);
            if(err == 1)
            {
                return running;
            }
            else if(err ==0)
            {
                return stopped;
            }
            else
            {
                return make_stream_error(stream_status::system_error,Pa_GetErrorText(err));
            }
        }
        virtual std::string get_error_string(const stream_error& err) noexcept
        {
            return err.second;
        }
        virtual stream_error open_stream(const stream_params<sample_t>& params) noexcept
        {
            auto&& compat = is_configuration_supported(params);

            if(compat == no_error)
            {
                _callback_info = callback_info(_callback,_error_callback,&const_cast<stream_params<sample_t>&>(params));
                double srate=0;
                std::tie(_inparams,_outparams,srate) = _native_params_to_pa(params);

                const PaStreamParameters* ip = params.input_frame_width() == 0 ? nullptr: &_inparams;
                const PaStreamParameters* op = params.output_frame_width() == 0 ? nullptr: &_outparams;

                return _pa_invoke(Pa_OpenStream,&stream,
                                  ip,
                                  op,
                                  srate,
                                  params.frame_count(),
                                  paNoFlag,
                                  &_pa_stream_api_callback,
                                  (void*)&_callback_info);
            }
            else
            {
                //TODO:
                //maybe we should try to negotiate a compatible format instead of erroring out?
                //would depend on if we knew what was wrong
                return compat;
            }

        }
        virtual stream_error close_stream() noexcept
        {
            return _pa_invoke(Pa_CloseStream,stream);
        }
        virtual long get_device_count() noexcept
        {
            return Pa_GetDeviceCount();
        }
        virtual device_info get_device_info(long id) noexcept
        {
            const PaDeviceInfo* info = Pa_GetDeviceInfo(id);
            return device_info(_pa_device_info_to_native(info));
        }
        virtual stream_error is_configuration_supported(const stream_params<sample_t>& params) noexcept
        {
            PaStreamParameters inparams;
            PaStreamParameters outparams;
            double srate=0;
            std::tie(inparams,outparams,srate) = _native_params_to_pa(params);

            PaError err;
            err = Pa_IsFormatSupported(&inparams,&outparams,srate);
            if(err == paFormatIsSupported)
            {
                return no_error;
            }
            else
            {
                return make_stream_error(stream_status::system_error,Pa_GetErrorText(err));
            }

        }
        virtual long default_input_device_id() const noexcept
        {
            return Pa_GetDefaultInputDevice();
        }
        virtual long default_output_device_id() const noexcept
        {
            return Pa_GetDefaultOutputDevice();
        }
    private:
        using callback_info =typename  base::callback_info;
        static int _pa_stream_api_callback( const void *input,void *output,unsigned long frameCount,const PaStreamCallbackTimeInfo* timeInfo,PaStreamCallbackFlags statusFlags,void *userData )
        {

            //cast buffers
            const sample_t* in = static_cast<const sample_t*>(input);
            sample_t* out = static_cast<sample_t*>(output);


            //gather call data
            callback_info* callbacks = static_cast<callback_info*>(userData);
            stream_callback<sample_t>* cb = nullptr;
            stream_error_callback* ecb = nullptr;
            stream_params<sample_t>* params = nullptr;

            std::tie(cb,ecb,params) = *callbacks;
            try
            {
                //invoke the function;
                const stream_error ret = (*cb)(in,out,audio_clock::now(),*params);

                //evaluate if there is an error
                if(ret != no_error)
                {
                    //invoke user error callback
                    (*ecb)(ret);
                    return -1;
                }
                else return 0;
            }
            catch(std::exception& e)
            {
                (*ecb)(make_stream_error(stream_status::system_error,e.what()));
                return -1;
            }
        }
        using base::_callback_info;
        PaStream* stream;
        using base::_error_callback;
        using base::_callback;


        //attempt to invoke a pa function, on failure call user error callback;
        template<typename F, typename...args_t>
        stream_error _pa_invoke(F f,args_t&&... args)
        {
            PaError err = f(std::forward<args_t&&>(args)...);
            if(err != paNoError)
            {
                stream_error serr = make_stream_error(stream_status::system_error,Pa_GetErrorText(err));
                (*_error_callback)(serr);
                return serr;
            }
            return no_error;
        }
        device_info _pa_device_info_to_native(const PaDeviceInfo* info)
        {
            return make_device_info(info->name,
                                    info->hostApi,
                                    info->maxInputChannels,
                                    info->maxOutputChannels,
                                    info->defaultSampleRate,
                                    duration(info->defaultLowInputLatency),
                                    duration(info->defaultHighInputLatency),
                                    duration(info->defaultLowOutputLatency),
                                    duration(info->defaultHighOutputLatency));
        }

        std::tuple<PaStreamParameters,PaStreamParameters,double> _native_params_to_pa(const stream_params<sample_t>& params)
        {
            PaStreamParameters inparams;
            PaStreamParameters outparams;
            double srate=params.sample_rate();

            inparams.channelCount = params.input_frame_width();
            outparams.channelCount = params.output_frame_width();

            inparams.sampleFormat = internal::_type_to_pa_sample_format<sample_t>();
            outparams.sampleFormat = internal::_type_to_pa_sample_format<sample_t>();

            inparams.hostApiSpecificStreamInfo=nullptr;
            outparams.hostApiSpecificStreamInfo=nullptr;

            if (params.input_device_id()==-1)
            {
                inparams.device = Pa_GetDefaultInputDevice();
            }
            else
            {
                inparams.device = params.input_device_id();
            }

            if(params.output_device_id()==-1)
            {
                outparams.device = Pa_GetDefaultOutputDevice();
            }
            else
            {
                outparams.device = params.output_device_id();
            }

            inparams.suggestedLatency = Pa_GetDeviceInfo(inparams.device)->defaultLowInputLatency;
            outparams.suggestedLatency = Pa_GetDeviceInfo(outparams.device)->defaultLowOutputLatency;

            return std::make_tuple(inparams,outparams,srate);
        }
        PaStreamParameters _inparams;
        PaStreamParameters _outparams;

    };


}

#endif
