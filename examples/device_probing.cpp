#include <iostream>
#include <libzaudio/zaudio.hpp>

int main(int argc,char** argv)
{
    auto&& context = zaudio::make_stream_context<float>();

    std::cout<<"Probing Devices: "<<std::endl;
    std::cout<<"Found "<<context.get_device_count()<<" devices!"<<std::endl;
    std::cout<<context.get_device_info_list()<<std::endl;

}
