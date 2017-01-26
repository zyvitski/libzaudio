#ifndef BUFFER_VIEW_HPP
#define BUFFER_VIEW_HPP

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

#include <cstddef>
#include <stdexcept>
#include <string>

namespace zaudio
{
    template<typename sample_t>
    class frame_view
    {
    public:
        using iterator = sample_t*;
        using const_iterator = const sample_t*;

        explicit frame_view(sample_t* buffer,
                             const std::size_t& size) noexcept: _buffer(buffer),_size(size)
        {}
        explicit frame_view(const sample_t* buffer,
                             const std::size_t& size) noexcept: _buffer(const_cast<sample_t*>(buffer)),_size(size)
        {}

        const sample_t& operator[](const std::size_t& idx) const
        {
            return _buffer[idx];
        }
        sample_t& operator[](const std::size_t& idx)
        {
            return _buffer[idx];
        }
        const sample_t& at(const std::size_t& idx) const
        {
            return at(idx);
        }
        sample_t& at(const std::size_t& idx)
        {
            if(idx < size())
            {
                return (*this)[idx];
            }
            else
            {
                std::string err = "Index: " + std::to_string(idx) + " is out of bounds!";
                throw std::out_of_range(err);
            }
        }
        const std::size_t size() const noexcept
        {
            return _size;
        }
        sample_t* data()
        {
            return _buffer;
        }

        iterator begin()
        {
            return _buffer;
        }
        const_iterator begin() const
        {
            return begin();
        }
        const_iterator cbegin() const
        {
            return begin();
        }
        iterator end()
        {
            return _buffer + _size;
        }
        const_iterator end() const
        {
            return end();
        }
        const_iterator cend() const
        {
            return end();
        }
    protected:
        sample_t* _buffer;
        std::size_t _size;
    };

    template<typename sample_t>
    class buffer_view
    {
    public:
        class iterator
        {
        public:
            iterator():_buffer(nullptr),_size(0){}
            iterator(sample_t* buffer, const std::size_t& size):_buffer(buffer),_size(size)
            {}

            iterator& operator++() noexcept
            {
                _buffer += _size;
                return *this;
            }
            iterator operator++(int) noexcept
            {
                iterator iter =*this;
                ++(*this);
                return iter;
            }
            bool operator==(const iterator& other) const noexcept
            {
                return (_buffer == other.buffer) && (_size == other._size);
            }
            bool operator !=(const iterator& other) const  noexcept
            {
                return (_buffer != other._buffer) || (_size != other._size);
            }
            frame_view<sample_t> operator*() noexcept
            {
                return frame_view<sample_t>(_buffer,_size);
            }
            const frame_view<sample_t> operator*() const noexcept
            {
                return frame_view<sample_t>(_buffer,_size);
            }
        private:
            sample_t* _buffer;
            std::size_t _size;
        };
        using const_iterator = const iterator;

        explicit buffer_view(sample_t* buffer,
                             const std::size_t& frame_count,
                             const std::size_t& frame_width) noexcept: _buffer(buffer),
                                                                       _frame_count(frame_count),
                                                                       _frame_width(frame_width)
        {}
        explicit buffer_view(const sample_t* buffer,
                             const std::size_t& frame_count,
                             const std::size_t& frame_width) noexcept: _buffer(const_cast<sample_t*>(buffer)),
                                                                       _frame_count(frame_count),
                                                                       _frame_width(frame_width)
        {}

        const frame_view<sample_t> operator[](const std::size_t& idx) const noexcept
        {
            return frame_view<sample_t>{_buffer + (idx * _frame_width),_frame_width};
        }
        frame_view<sample_t>  operator[](const std::size_t& idx) noexcept
        {
            return frame_view<sample_t>{_buffer + (idx * _frame_width),_frame_width};
        }
        const sample_t& at(const std::size_t& idx) const
        {
            return at(idx);
        }
        sample_t& at(const std::size_t& idx)
        {
            if(idx < size())
            {
                return (*this)[idx];
            }
            else
            {
                std::string err = "Index: " + std::to_string(idx) + " is out of bounds!";
                throw std::out_of_range(err);
            }
        }
        const std::size_t size() const noexcept
        {
            return _frame_width * _frame_count;
        }
        const std::size_t& frame_width() const noexcept
        {
            return _frame_width;
        }
        const std::size_t& frame_count() const noexcept
        {
            return _frame_count;
        }
        sample_t* data() noexcept
        {
            return _buffer;
        }

        iterator begin() noexcept
        {
            return iterator(_buffer,_frame_width);
        }
        const_iterator begin() const noexcept
        {
            return begin();
        }
        const_iterator cbegin() const noexcept
        {
            return begin();
        }
        iterator end() noexcept
        {
            return iterator(_buffer + (_frame_count * _frame_width),_frame_width);
        }
        const_iterator end() const noexcept
        {
            return end();
        }
        const_iterator cend() const noexcept
        {
            return end();
        }
    protected:
        sample_t* _buffer;
        std::size_t _frame_count;
        std::size_t _frame_width;
    };
}

#endif
