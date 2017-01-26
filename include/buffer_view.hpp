#ifndef BUFFER_VIEW_HPP
#define BUFFER_VIEW_HPP

#include <cstddef>
#include <stdexcept>
#include <string>

namespace zaudio
{
    template<typename sample_t>
    class buffer_view
    {
    public:
        explicit buffer_view(sample_t* buffer,const std::size_t& size) noexcept: _buffer(buffer), _size(size)
        {}
        explicit buffer_view(const sample_t* buffer,const std::size_t& size) noexcept: _buffer(const_cast<sample_t*>(buffer)), _size(size)
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
        const std::size_t& size() const noexcept
        {
            return _size;
        }
        sample_t* data()
        {
            return _buffer;
        }

        sample_t* begin()
        {
            return _buffer[0];
        }
        const sample_t* begin() const
        {
            return begin();
        }
        const sample_t* cbegin() const
        {
            return begin();
        }
        sample_t* end()
        {
            return _buffer[_size-1];
        }
        const sample_t* end() const
        {
            return end();
        }
        const sample_t* cend() const
        {
            return end();
        }
    protected:
        sample_t* _buffer;
        std::size_t _size;
    };
}

#endif
