/**
* @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
* @brief     C++ API: std::string utilities
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <string>
#include <vector>
#include <iterator>
#include <sstream>


/**
* Provides an API to stream into OutputDebugString.
*/
namespace gui {

  namespace string {

    // --------------------------------------------------------------------------
    std::string utf16_to_utf8 (const std::wstring&);
    std::string utf16_to_utf8 (wchar_t wc);
    std::wstring utf8_to_utf16 (const std::string&);

    // --------------------------------------------------------------------------
    bool starts_with (const std::string& str, const std::string& prefix);
    bool ends_with (const std::string& str, const std::string& suffix);

    // --------------------------------------------------------------------------
    std::string::size_type find_left_space (const std::string& text, std::size_t cursor_pos);
    std::string::size_type find_right_space(const std::string& text, std::size_t cursor_pos);

    // --------------------------------------------------------------------------
    template<char delimiter>
    class delimited : public std::string {};

    template<char delimiter>
    std::istream& operator>> (std::istream& is, delimited<delimiter>& output) {
      std::getline(is, output, delimiter);
      return is;
    }

    template<char delimiter>
    std::vector<std::string> split (const std::string& t) {
      std::istringstream iss(t);
      typedef std::istream_iterator<delimited<delimiter>> iterator;
      std::vector<std::string> v(iterator{iss}, iterator{});
      if (!t.empty() && (t.back() == '\n')) {
        v.emplace_back(std::string());
      }
      return v;
    }

    template<char delimiter>
    std::string merge (const std::vector<std::string>& v) {
      std::ostringstream oss;
      if (!v.empty()) {
        std::copy(v.begin(), std::prev(v.end()), std::ostream_iterator<std::string>(oss, std::string(1, delimiter).c_str()));
        oss << v.back();
      }
      return oss.str();
    }

  } // namespace string

  namespace utf8 {

    bool is_continuation_char (char ch);

    std::size_t get_left_char (const std::string& s, std::size_t pos);
    std::size_t get_right_char (const std::string& s, std::size_t pos);

  } // namespace utf8

} // namespace gui
