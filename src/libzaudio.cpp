#include <zaudio.hpp>

namespace zaudio
{
    std::ostream& operator<<(std::ostream& os, sample_format format)
    {
        switch(format)
        {
        case sample_format::f32:
            os<<"float32";
            break;
        case sample_format::f64:
            os<< "float64";
            break;
        case sample_format::i8:
            os<<"signed int8";
            break;
        case sample_format::u8:
            os<<"unsigned int8";
            break;
        case sample_format::i16:
            os<<"signed int16";
            break;
        case sample_format::i24:
            os<<"signed int24";
            break;
        case sample_format::i32:
            os<< "signed int32";
            break;
        case sample_format::i64:
            os<<"signed int64";
            break;
        default:
            os<<"error";
            break;
        }
        return os;
    }


    std::string stream_status_string(stream_status status)
    {
        switch(status)
        {
        case stream_status::running:
            return "running";
            break;
        case stream_status::paused:
            return "paused";
            break;
        case stream_status::stopped:
            return "stopped";
            break;
        case stream_status::no_error:
            return "no error";
            break;
        case stream_status::fatal_error:
            return "fatal error";
            break;
        case stream_status::xrun:
            return "xrun";
            break;
        case stream_status::system_error:
            return "system error";
            break;
        case stream_status::user_error:
            return "user error";
            break;
        default:
            return "invalid";
            break;
        }
    }
    std::ostream& operator<<(std::ostream& os,stream_status status)
    {
        os<<stream_status_string(status);
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const stream_error& err)
    {
        os<<"Stream Error, type:"<<err.first<<" code:  "<<err.second;
        return os;
    }






        stream_exception::stream_exception(const char* what_arg) : std::runtime_error(what_arg){}
        stream_exception::stream_exception(const std::string& what_arg):std::runtime_error(what_arg){}
        stream_exception::stream_exception(const stream_error& err) : std::runtime_error(_make_error_string(err)){}
        stream_exception::stream_exception(const stream_status& status) : std::runtime_error(_make_error_string(make_stream_error(status))){}
        stream_exception::stream_exception(const stream_status& status,const stream_error_message& code) : std::runtime_error(_make_error_string(make_stream_error(status,code))){}
        const std::string stream_exception::_make_error_string(const stream_error& err)
        {
            std::stringstream sstr;
            sstr<<err;
            return sstr.str();
        }



        device_info::device_info() noexcept: name(""),
                                             device_index(0),
                                             max_input_count(0),
                                             max_output_count(0),
                                             default_sample_rate(0),
                                             default_low_input_latency(0),
                                             default_high_input_latency(0),
                                             default_low_ouput_latency(0),
                                             default_high_output_latency(0){}

        device_info::device_info(const char* nm,
                                 std::size_t idx,
                                 std::size_t mic,
                                 std::size_t moc,
                                 double srate,
                                 duration dlil,
                                 duration dhil,
                                 duration dlol,
                                 duration dhol) noexcept:name(nm),
                                                         device_index(idx),
                                                         max_input_count(mic),
                                                         max_output_count(moc),
                                                         default_sample_rate(srate),
                                                         default_low_input_latency(dlil),
                                                         default_high_input_latency(dhil),
                                                         default_low_ouput_latency(dlol),
                                                         default_high_output_latency(dhol){}

         std::ostream& operator<<(std::ostream& os, const device_info& info)
         {
             os<<"Device: "<<info.name<<", id: "<<info.device_index<<std::endl;
             os<<"\tMax Input Count: "<<info.max_input_count<<std::endl;
             os<<"\tMax Ouput Count: "<<info.max_output_count<<std::endl;
             os<<"\tDefault Sample Rate: "<<info.default_sample_rate<<std::endl;
             os<<"\tDefault Input Latency Range:  <"<<info.default_low_input_latency.count()<<"s , "<<info.default_high_input_latency.count()<<"s>"<<std::endl;
             os<<"\tDefault Output Latency Range: <"<<info.default_low_ouput_latency.count()<<"s , "<<info.default_high_output_latency.count()<<"s>"<<std::endl;
             return os;
         }



}
