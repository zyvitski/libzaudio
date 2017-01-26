#ifndef BUFFER_VIEW_HPP
#define BUFFER_VIEW_HPP

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
        const iterator begin() const
        {
            return begin();
        }
        const iterator cbegin() const
        {
            return begin();
        }
        iterator end()
        {
            return _buffer + _size;
        }
        const iterator end() const
        {
            return end();
        }
        const iterator cend() const
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

            iterator& operator++()
            {
                _buffer += _size;
                return *this;
            }
            iterator operator++(int)
            {
                iterator iter =*this;
                ++(*this);
                return iter;
            }
            bool operator==(const iterator& other)
            {
                return (_buffer == other.buffer) && (_size == other._size);
            }
            bool operator !=(const iterator& other)
            {
                return (_buffer != other._buffer) || (_size != other._size);
            }
            frame_view<sample_t> operator*()
            {
                return frame_view<sample_t>(_buffer,_size);
            }
            const frame_view<sample_t> operator*() const
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
        sample_t* data()
        {
            return _buffer;
        }

        iterator begin()
        {
            return iterator(_buffer,_frame_width);
        }
        const iterator begin() const
        {
            return begin();
        }
        const iterator cbegin() const
        {
            return begin();
        }
        iterator end()
        {
            return iterator(_buffer + (_frame_count * _frame_width),_frame_width);
        }
        const iterator end() const
        {
            return end();
        }
        const iterator cend() const
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
