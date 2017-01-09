#ifndef ZAUDIO_ERROR_UTILITY
#define ZAUDIO_ERROR_UTILITY

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

#include <string>
#include <exception>
#include <future>


/*!
 *\namespace zaudio
 *\brief primary namespace for the zaudio library
 */
namespace zaudio
{
/*!
   *\enum stream_status
   *\brief a list of status codes to be used for realtime error signaling and messages
   */

  enum class stream_status
  {
      //valid states
      running,
      stopped,
      paused,
      //error states
      no_error,
      fatal_error,
      xrun,
      system_error,
      user_error
  };

  /*!
   *\fn stream_status_string
   *\brief converts a stream_status to a std::string
   */
  std::string stream_status_string(stream_status status);

  /*!
   *\fn stream_status operator<<
   *\brief enables printing of stream_status
   */
  std::ostream& operator<<(std::ostream& os,stream_status status);


  /*!
   *\typedef stream_error_message
   *\brief the data type to represent an error message
   */
  using stream_error_message = const char*;

  /*!
   *\namespace detail
   *\brief
   */
  namespace detail
  {
      /*!
       *\struct stream_error_type
       *\brief a workaround type for c++11 std::pair not having a constexpr contructor
       */
      struct stream_error_type
      {
          stream_error_type():first(stream_status::no_error),second(""){}

          constexpr stream_error_type(stream_status f,stream_error_message s):first(f),second(s){}

          stream_status first;

          stream_error_message second;

          friend bool operator==(const stream_error_type& lhs,const stream_error_type& rhs)
          {
              return lhs.first == lhs.first && rhs.second == rhs.second;
          }
          
          friend bool operator!=(const stream_error_type& lhs,const stream_error_type& rhs)
          {
              return lhs.first != lhs.first || rhs.second != rhs.second;
          }
      };
  }


  /*!
   *\typedef stream_error
   *\brief a grouping of a stream_status and a stream_error_message
   */
  //use std pair if c++14 else use stream_error_type
  using stream_error = std::conditional<__cplusplus == 201402L,
                                        std::pair<stream_status,
                                        stream_error_message>,
                                        detail::stream_error_type>::type;

 /*!
  *\fn stream_error operator<<
  *\brief enables printing of stream_error
  */
  std::ostream& operator<<(std::ostream& os, const stream_error& err);

  /*!
   *\fn make_stream_error
   *\brief helper for constructing stream_error objects
   */
  constexpr stream_error make_stream_error(stream_status status, stream_error_message code) noexcept
  {
      return stream_error(status,code);
  }
  constexpr stream_error make_stream_error(stream_status status) noexcept
  {
      return make_stream_error(status,"");
  }


  /*!
   *\class stream_exception
   *\brief an audio stream exception class
   */
  class stream_exception : public std::runtime_error
  {
  public:
      explicit stream_exception(const char* what_arg);
      explicit stream_exception(const std::string& what_arg);
      explicit stream_exception(const stream_error& err);
      explicit stream_exception(const stream_status& status);
      explicit stream_exception(const stream_status& status,const stream_error_message& code);
  private:
      const std::string _make_error_string(const stream_error& err);
  };

  //user defined error callback type
  //can throw
  //is called by host when an error code is reported
  /*!
   *\typedef stream_error_callback
   *\brief a function object type to represent stream_error callbacks
   */
  using stream_error_callback = std::function<void(const stream_error&) noexcept>;



  /*!
   *\fn default_stream_error_callback
   *\brief the default stream_error_callback used by zaudio
   */
  inline stream_error_callback default_stream_error_callback() noexcept
  {
      return [](const stream_error& err) noexcept
      {
          std::async(std::launch::async, [](const stream_error& err)
          {
              throw stream_exception(err);
          }, err);
      };
  }


  //global stream_error codes for general use
  /*!
   *\var running
   *\brief global stream_error to represent the running state
   */
  constexpr static stream_error running = make_stream_error(stream_status::running,"running");

  /*!
   *\var stopped
   *\brief global stream_error to represent the stopped state
   */
  constexpr static stream_error stopped = make_stream_error(stream_status::stopped,"stopped");

  /*!
   *\var paused
   *\brief global stream_error to represent the paused state
   */
  constexpr static stream_error paused  = make_stream_error(stream_status::paused,"paused");

  /*!
   *\var no_error
   *\brief global stream_error to represent the no_error state
   */
  constexpr static stream_error no_error = make_stream_error(stream_status::no_error,"no error");


}

#endif
