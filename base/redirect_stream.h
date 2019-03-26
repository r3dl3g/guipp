/**
* @copyright (c) 2015-2017 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API:redirect stream
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <sstream>


/**
* Provides a stream api to write into string based output.
*/
namespace basepp {

  /**
  * basic_redirect_buf collects the characters, provided by the stream
  * into a simple basic_stringbuf.
  * everytime sync() ist called, the collected string will be forwarded
  * to the target_type.
  *
  * target_type L must provide a constructor
  * and an operator() taking a std::string.
  */
  template<typename L, typename E, typename T = std::char_traits<E> >
  class basic_redirect_buf : public std::basic_stringbuf<E, T> {
  public:
    typedef std::basic_stringbuf<E, T> super;
    typedef L target_type;

    template<typename... Args>
    basic_redirect_buf (Args&&... args)
      :m_target(args...)
    {}

    ~basic_redirect_buf () {
      sync();
    }

  protected:
    int sync () override {
      m_target(super::str());
      super::str(std::basic_string<E, T>());
      return 0;
    }

    target_type m_target;
  };

  /**
  * basic_redirect_stream is just a convenience class for automatical
  * instantiation of the required basic_redirect_buf.
  */
  template<typename L, typename E, typename T = std::char_traits<E> >
  class basic_redirect_stream : public std::basic_ostream<E, T> {
  public:
    typedef std::basic_ostream<E, T> super;
    typedef basic_redirect_buf<L, E, T> buffer_type;

    template<typename... Args>
    basic_redirect_stream (Args&&... args)
      : std::basic_ostream<E, T>(new buffer_type(args...))
    {}

    ~basic_redirect_stream () {
      delete super::rdbuf();
    }
  };

  template<typename L>
  using oredirect_stream = basic_redirect_stream<L, char>;

} // namespace basepp
