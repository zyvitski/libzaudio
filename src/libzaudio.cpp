#include <zaudio.hpp>
#include <sstream>
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
        os<<"Stream Error"<<std::endl<<"{"<<std::endl;
        os<<"Type: "<<err.first<<std::endl;
        if(std::string(err.second) != "" && err.second != stream_status_string(err.first))
        {
            os<<"Message: "<<err.second<<std::endl;
            os<<"}"<<std::endl;
        }
        return os;
    }






    stream_exception::stream_exception(const char* what_arg) : std::runtime_error(what_arg),_error(stream_status::system_error,what_arg){}
    stream_exception::stream_exception(const std::string& what_arg):std::runtime_error(what_arg),_error(stream_status::system_error,what_arg.c_str()){}
    stream_exception::stream_exception(const stream_error& err) : std::runtime_error(_make_error_string(err)),_error(err){}
    stream_exception::stream_exception(const stream_status& status) : std::runtime_error(_make_error_string(make_stream_error(status))),_error(status,stream_status_string(status).c_str()){}
    stream_exception::stream_exception(const stream_status& status,const stream_error_message& code) : std::runtime_error(_make_error_string(make_stream_error(status,code))),_error(status,code){}
    const stream_error& stream_exception::stream_exception::error() const noexcept
    {
        return _error;
    }
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
         os<<"Device: "<<info.name<<", ID: "<<info.device_index<<std::endl;
         os<<"\tMax Input Count: "<<info.max_input_count<<std::endl;
         os<<"\tMax Ouput Count: "<<info.max_output_count<<std::endl;
         os<<"\tDefault Sample Rate: "<<info.default_sample_rate<<std::endl;
         os<<"\tDefault Input Latency Range:  <"<<info.default_low_input_latency.count()<<"s , "<<info.default_high_input_latency.count()<<"s>"<<std::endl;
         os<<"\tDefault Output Latency Range: <"<<info.default_low_ouput_latency.count()<<"s , "<<info.default_high_output_latency.count()<<"s>"<<std::endl;
         return os;
     }

     std::ostream& operator<<(std::ostream& os, const std::vector<device_info>& vinfo)
     {
         for(auto idx = 0ul; idx < vinfo.size(); ++idx)
         {
             os<<"@index: "<<idx<<std::endl<<"\t";
             os<<vinfo[idx]<<std::endl;
         }
         return os;
     }



}
