/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     text label control
 *
 * @license   MIT license. See accompanying file LICENSE.
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
      void label_base::create (win::container& parent,
                               const core::rectangle& place) {
        super::create(clazz::get(), parent, place);
      }

      void label_base::create (win::container& parent,
                               const text_source& txt,
                               const core::rectangle& place) {
        create(parent, place);
        set_text(txt);
      }

      void label_base::create (win::container& parent,
                               const std::string& txt,
                               const core::rectangle& place) {
        create(parent, const_text(txt), place);
      }

      label_base::label_base (const std::string& t)
        : text(const_text(t))
      {}

      label_base::label_base (const text_source& t)
        : text(t)
      {}

      label_base::label_base (text_source&& t) noexcept
        : text(std::move(t))
      {}

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
