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
#include <zaudio.hpp>



int main(int argc,char** argv)
{
    auto&& context = zaudio::make_stream_context<float>();

    std::cout<<"Probing Devices: "<<std::endl;
    std::cout<<"Found "<<context.get_device_count()<<" devices!"<<std::endl;
    std::cout<<context.get_device_info_list()<<std::endl;
    std::cin.get();

}
