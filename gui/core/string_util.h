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

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui++-export.h>


/**
* Provides an API to stream into OutputDebugString.
*/
namespace gui {

  namespace string {

    // --------------------------------------------------------------------------
    GUIPP_EXPORT std::string utf16_to_utf8 (const std::wstring&);
    GUIPP_EXPORT std::string utf16_to_utf8 (wchar_t wc);
    GUIPP_EXPORT std::wstring utf8_to_utf16 (const std::string&);

    // --------------------------------------------------------------------------
    GUIPP_EXPORT bool starts_with (const std::string& str, const std::string& prefix);
    GUIPP_EXPORT bool ends_with (const std::string& str, const std::string& suffix);

    // --------------------------------------------------------------------------
    GUIPP_EXPORT std::string::size_type find_left_space (const std::string& text, std::size_t cursor_pos);
    GUIPP_EXPORT std::string::size_type find_right_space(const std::string& text, std::size_t cursor_pos);

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

    GUIPP_EXPORT std::string merge (const std::vector<std::string>& v, char delimiter);

    template<char delimiter>
    std::string merge (const std::vector<std::string>& v) {
      return merge(v, delimiter);
    }

  } // namespace string

  namespace utf8 {

    GUIPP_EXPORT bool is_continuation_char (char ch);

    GUIPP_EXPORT std::size_t get_left_char (const std::string& s, std::size_t pos);
    GUIPP_EXPORT std::size_t get_right_char (const std::string& s, std::size_t pos);

  } // namespace utf8

} // namespace gui
