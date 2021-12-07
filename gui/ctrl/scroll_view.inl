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
 * @brief     view to scroll child views
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace layout {

    inline bool scroll_view::is_in_scroll_event () const {
      return in_scroll_event;
    }

  } // namespace layout

  namespace ctrl {

    // --------------------------------------------------------------------------
    template<os::color B>
    void scroll_view<B>::create (container& parent,
                                 const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
      super::create_children(place);
    }

  } // ctrl

} // gui
