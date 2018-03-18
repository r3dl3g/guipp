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
 * @brief     C++ API: color definition
 *
 * @file
 */

#pragma once


namespace gui {

  namespace draw {

    inline os::graphics graphics::os () const {
      return gc;
    }

    inline graphics::operator os::graphics () const {
      return gc;
    }

    inline graphics::operator os::drawable() const {
      return target;
    }

    // --------------------------------------------------------------------------
    template<typename T, typename F>
    inline paint::paint (T* t, F f)
      : p(basepp::bind_method(t, f))
    {}

    // --------------------------------------------------------------------------
    template<typename T, typename F>
    inline buffered_paint::buffered_paint (T* t, F f)
      : p(basepp::bind_method(t, f))
    {}

    // --------------------------------------------------------------------------
    inline clip::clip (const graphics& g, const core::rectangle& r)
      : g(g) {
      g.push_clipping(r);
    }

    inline clip::~clip () {
      g.pop_clipping();
    }

  } // draw

} // gui
