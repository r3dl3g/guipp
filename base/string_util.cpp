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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <iostream>
#include <locale>
#include <codecvt>
#include <algorithm>

// --------------------------------------------------------------------------
//
// Library includes
//
#include "string_util.h"


namespace basepp {

  namespace string {

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> s_utf8_converter;

    struct codecvt_wchar_t_char : public std::codecvt<wchar_t, char, mbstate_t> {};

    std::wstring_convert<codecvt_wchar_t_char> s_converter;

    std::string utf16_to_utf8 (const std::wstring& ws) {
      return s_utf8_converter.to_bytes(ws);
    }

    std::string utf16_to_utf8 (wchar_t wc) {
      return s_utf8_converter.to_bytes(wc);
    }

    std::wstring utf8_to_utf16 (const std::string& s) {
      try {
        return s_utf8_converter.from_bytes(s);
      } catch (...) {
        return s_converter.from_bytes(s);
      }
    }

    bool starts_with (const std::string& str, const std::string& prefix) {
      const int r = str.compare(0, prefix.length(), prefix);
      return r == 0;
    }

    bool ends_with (const std::string& str, const std::string& suffix) {
      std::string::size_type l = str.length(), sl = suffix.length();
      return (l >= sl) && (str.compare(l - sl, sl, suffix) == 0);
    }

    static const std::string white_space = " (){}[],.;:'\"!@#$%^&/*-+";

    std::string::size_type find_left_space (const std::string& text, std::size_t cursor_pos) {
      std::string::size_type pos = text.find_last_not_of(white_space, cursor_pos - 1);
      if (pos != std::string::npos) {
        std::string::size_type pos2 = text.find_last_of(white_space, pos);
        if (pos2 != std::string::npos) {
          return pos2 + 1;
        }
      }
      return 0;
    }

    std::string::size_type find_right_space(const std::string& text, std::size_t cursor_pos) {
      std::string::size_type pos = text.find_first_of(white_space, cursor_pos + 1);
      if (pos != std::string::npos) {
        std::string::size_type pos2 = text.find_first_not_of(text[pos], pos);
        if (pos2 != std::string::npos) {
          return pos2;
        } else {
          return pos;
        }
      } else {
        return text.length();
      }
    }

    std::string merge (const std::vector<std::string>& v, char delimiter) {
      std::ostringstream oss;
      if (!v.empty()) {
        std::copy(v.begin(), std::prev(v.end()), std::ostream_iterator<std::string>(oss, std::string(1, delimiter).c_str()));
        oss << v.back();
      }
      return oss.str();
    }


  } // namespace string

  namespace utf8 {

    bool is_continuation_char (char ch) {
      return ((ch & 0b11000000) == 0b10000000);
    }

    std::size_t get_left_char (const std::string& s, std::size_t pos) {
      if (pos < 1) {
        return pos;
      }
      std::size_t cp = pos - 1;
      while ((cp > 0) && utf8::is_continuation_char(s.at(cp))) {
        --cp;
      }
      return cp;
    }

    std::size_t get_right_char (const std::string& s, std::size_t pos) {
      std::size_t sz = s.size();
      std::size_t cp = std::min(sz, pos + 1);
      while ((cp < sz) && utf8::is_continuation_char(s.at(cp))) {
        ++cp;
      }
      return cp;
    }

  } // utf8

} // namespace basepp
