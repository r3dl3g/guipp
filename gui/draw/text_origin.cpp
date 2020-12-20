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
 * @brief     C++ API: bitmap
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <ostream>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/text_origin.h>


namespace std {

  using namespace gui;

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, placement_t v) {
    const placement_t hvalue = static_cast<placement_t>(v & (placement_t::left|placement_t::hcenter|placement_t::right));
    switch (hvalue) {
      case placement_t::left:     out << "left | ";    break;
      case placement_t::hcenter:  out << "hcenter | "; break;
      case placement_t::right:    out << "right | ";   break;
    }
    const placement_t vvalue = static_cast<placement_t>(v & (placement_t::top|placement_t::vcenter|placement_t::bottom));
    switch (vvalue) {
      case placement_t::top:      out << "top";     break;
      case placement_t::vcenter:  out << "vcenter"; break;
      case placement_t::bottom:   out << "bottom";  break;
    }
    return out;
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, line_handling_t v) {
    const line_handling_t value = static_cast<line_handling_t>(v & (line_handling_t::singleline | line_handling_t::wordbreak));
    switch (value) {
      case line_handling_t::singleline:      out << "top";     break;
      case line_handling_t::wordbreak:  out << "vcenter"; break;
    }
    return out;
  }

  GUIPP_CORE_EXPORT ostream& operator<< (ostream& out, gui::text_origin_t v) {
    out << static_cast<placement_t>(v) << " | " << static_cast<line_handling_t>(v);
    return out;
  }

} // namespace std
