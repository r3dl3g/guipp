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
 * @brief     C++ API: label
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/label.h>


//#define SHOW_TEXT_AREA

namespace gui {

  namespace ctrl {

    namespace detail {

      // --------------------------------------------------------------------------
      label_base::label_base (const std::string& t)
        : text(const_text(t))
      {}

      label_base::label_base (const text_source& t)
        : text(t)
      {}

      label_base::label_base (const label_base& rhs)
        : super(rhs)
        , text(rhs.text)
      {}

      label_base::label_base (label_base&& rhs)
        : super(rhs)
      {
        std::swap(text, rhs.text);
      }

      void label_base::set_text (const std::string& t) {
        set_text(const_text(t));
      }

      void label_base::set_text (const text_source& t) {
        text = t;
        invalidate();
      }

      std::string label_base::get_text () const {
        return text();
      }

    } // detail

  } // ctrl

} // gui
