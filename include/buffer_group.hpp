#ifndef BUFFER_GROUP_HPP
#define BUFFER_GROUP_HPP

#include "buffer_view.hpp"

namespace zaudio
{
    template<typename sample_t>
    struct buffer_group
    {
    public:
        using buffer_view_type = buffer_view<sample_t>;
        buffer_group(const buffer_view_type& in,const buffer_view_type& out) noexcept : input(in), output(out)
        {}
        const buffer_view_type input;
        buffer_view_type output;
    };
}
#endif
