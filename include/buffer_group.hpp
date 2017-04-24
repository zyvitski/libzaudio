#ifndef BUFFER_GROUP_HPP
#define BUFFER_GROUP_HPP

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


#include "buffer_view.hpp"

namespace zaudio
{
template <typename sample_t>
struct buffer_group {
public:
    using buffer_view_type = buffer_view<sample_t>;
    buffer_group(const buffer_view_type& in, const buffer_view_type& out) noexcept : input(in), output(out)
    { }

    const buffer_view_type input;
    buffer_view_type       output;
};
}
#endif // ifndef BUFFER_GROUP_HPP
