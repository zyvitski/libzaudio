#ifndef ZAUDIO_SAMPLE_UTILITY
#define ZAUDIO_SAMPLE_UTILITY

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

#include <ostream>
#include <cstdint>
#include <cstddef>

/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
    /*!
     *\enum sample_format
     *\brief tags that identify each sample format that can be used within zaudio
     */
    enum class sample_format
    {
        f32,
        f64,
        u8,
        i8,
        i16,
        i24,
        i32,
        i64,
        err
    };

    /*!
     *\fn sample_format operator <<
     *\brief enables printing of sample_format
     */
    std::ostream& operator<<(std::ostream& os, sample_format format);

    /*!
     *\namespace detail
     *\brief
     */
    namespace detail
    {
        #pragma pack(push)
        //TODO: make me better


        /*!
         *\struct s24
         *\brief represents a 24bit integer sample
         */
        struct s24
        {
            int value:3;
            inline operator int()
            {
                return value;
            }
            inline s24& operator=(int v)
            {
                value = v;
                return *this;
            }
        };
        #pragma pack(pop)


        /*!
         *\struct format_id_to_type
         *\brief converts a sample_format tag to a typename useable in a template
         */
        template<sample_format> struct format_id_to_type        { using type = void;         };
        template<> struct format_id_to_type<sample_format::f32> { using type = float;        };
        template<> struct format_id_to_type<sample_format::f64> { using type = double;       };
        template<> struct format_id_to_type<sample_format::i8>  { using type = std::int8_t;  };
        template<> struct format_id_to_type<sample_format::u8>  { using type = std::uint8_t; };
        template<> struct format_id_to_type<sample_format::i16> { using type = std::int16_t; };
        template<> struct format_id_to_type<sample_format::i24> { using type = s24;          };
        template<> struct format_id_to_type<sample_format::i32> { using type = std::int32_t; };
        template<> struct format_id_to_type<sample_format::i64> { using type = std::int64_t; };

        /*!
         *\struct type_to_format_id
         *\brief converts the template type to a sample_format tag
         */
        template<typename T> struct type_to_format_id     { constexpr static sample_format value = sample_format::err; };
        template<> struct type_to_format_id<float>        { constexpr static sample_format value = sample_format::f32; };
        template<> struct type_to_format_id<double>       { constexpr static sample_format value = sample_format::f64; };
        template<> struct type_to_format_id<std::int8_t>  { constexpr static sample_format value = sample_format::i8;  };
        template<> struct type_to_format_id<std::uint8_t> { constexpr static sample_format value = sample_format::u8;  };
        template<> struct type_to_format_id<std::int16_t> { constexpr static sample_format value = sample_format::i16; };
        template<> struct type_to_format_id<s24>          { constexpr static sample_format value = sample_format::i24; };
        template<> struct type_to_format_id<std::int32_t> { constexpr static sample_format value = sample_format::i32; };
        template<> struct type_to_format_id<std::int64_t> { constexpr static sample_format value = sample_format::i64; };

        /*!
         *\fn is_sample_type
         *\brief tests if the template type is a valid sample type
         */
        template<typename T>
        constexpr bool is_sample_type() noexcept
        {
            return type_to_format_id<T>::value != sample_format::err;
        }

        /*!
         *\struct fail_if_type_is_not_sample
         *\brief  causes compilation to fail if the template type is not a valid sample type
         */
        template<typename T,sample_format f = type_to_format_id<T>::value>
        struct fail_if_type_is_not_sample : type_to_format_id<T>
        {};

        template<typename T>
        struct fail_if_type_is_not_sample<T,sample_format::err> : type_to_format_id<T>
        {
            static_assert(is_sample_type<T>(),"Invalid sample type provided!");
        };

    }


    /*!
     *\typedef sample
     *\brief
     */
    template<sample_format F>
    using sample = typename detail::format_id_to_type<F>::type;

    /*!
     *\fn sample_size
     *\brief returns the size of a given sample_format in bytes based un the template argument
     */
    template<sample_format format>
    constexpr static std::size_t sample_size() noexcept
    {
        return sizeof(std::declval<typename detail::format_id_to_type<format>::type>());
    }

    /*!
     *\fn sample_size
     *\brief returns the size of a given sample_format in bytes based un the argument
     */
    constexpr static std::size_t sample_size(sample_format format) noexcept
    {
        //REPLACE WITH SWITCH IN C++14
        return format == sample_format::f32 ? sample_size<sample_format::f32>() :
              (format == sample_format::f64 ? sample_size<sample_format::f64>() :
              (format == sample_format::i8  ? sample_size<sample_format::i8> () :
              (format == sample_format::u8  ? sample_size<sample_format::u8> () :
              (format == sample_format::i16 ? sample_size<sample_format::i16>() :
              (format == sample_format::i24 ? sample_size<sample_format::i24>() :
              (format == sample_format::i32 ? sample_size<sample_format::i32>() :
              (format == sample_format::i64 ? sample_size<sample_format::i64>() :
              /*Error Case*/0)))))));
    }
}

#endif
