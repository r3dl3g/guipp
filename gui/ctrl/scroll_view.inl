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
 * @brief     C++ API: scrolled views
 *
 * @file
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
    void scroll_view<B>::create (const container& parent,
                                 const core::rectangle& place) {
      super::create(clazz::get(), parent, place);
      super::create_children(place);
    }

  } // ctrl

} // gui
