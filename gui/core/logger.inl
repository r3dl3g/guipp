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
* @brief     C++ API:logger
*
* @file
*/

#pragma once

namespace gui {

  namespace log {

    inline line_id::line_id (unsigned int i)
      : n(i)
    {}

    inline line_id::line_id ()
      : n(0)
    {}

    inline const line_id& record::line () const {
      return m_line;
    }

    inline const std::chrono::system_clock::time_point& record::time_point () const {
      return m_time_point;
    }

    inline const log::level& record::level () const {
      return m_level;
    }

    inline const std::string& record::thread_name () const {
      return m_thread_name;
    }

    inline const std::string& record::message () const {
      return m_message;
    }

    inline sink::sink (std::ostream* stream,
                       level lvl,
                       const record_formatter& formatter)
      : m_stream(stream)
      , m_level(lvl)
      , m_formatter(formatter)
    {}

    template <typename T>
    inline recorder& recorder::operator<< (T const& value) {
      m_buffer << value;
      return *this;
    }

    inline recorder& recorder::operator<< (const std::string& value) {
      return operator<<(value.c_str());
    }

    inline std::ostream& recorder::ostream () {
      return m_buffer;
    }

  } // namespace log

} // namespace gui
