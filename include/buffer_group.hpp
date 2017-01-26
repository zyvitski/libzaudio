#ifndef BUFFER_GROUP_HPP
#define BUFFER_GROUP_HPP

#include "buffer_view.hpp"

namespace zaudio
{
    template<typename sample_t>
    class buffer_group
    {
    public:
        using buffer_view_type = buffer_view<sample_t>;
        buffer_group(const buffer_view_type& input,const buffer_view_type& output) noexcept : _input(input), _output(output)
        {}
        const buffer_view_type& input() const
        {
            return _input;
        }
        buffer_view_type& output()
        {
            return _output;
        }
    protected:
        const buffer_view_type _input;
        buffer_view_type _output;
    };
}
#endif
