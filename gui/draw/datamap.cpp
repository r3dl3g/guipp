/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui lib
 *
 * Customer   -
 *
 * @brief     C++ API: bitmap definition
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/datamap.h>


namespace gui {

  namespace draw {

    bool basic_datamap::is_valid () const {
      return data.size() > 0;
    }

    basic_datamap::operator bool () const {
      return is_valid();
    }

    const bitmap_info& basic_datamap::get_info () const {
      return info;
    }

    bitmap_info& basic_datamap::get_info () {
      return info;
    }

    const blob& basic_datamap::get_data () const {
      return data;
    }

    blob& basic_datamap::get_data () {
      return data;
    }

    core::size basic_datamap::size () const {
      return info.size();
    }

    byte basic_datamap::depth () const {
      return info.depth();
    }

    void basic_datamap::create (const blob& data, const bitmap_info& bmi) {
      this->data = data;
      info = bmi;
    }

    BPP basic_datamap::bits_per_pixel () const {
      return info.bits_per_pixel;
    }

    void basic_datamap::clear () {
      data.clear();
    }

  } // namespace draw

} // namespace gui
