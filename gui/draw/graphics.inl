/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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

    template<typename F>
    inline const graphics& graphics::frame (F drawer, const pen& p) const {
      drawer(*this, p);
      return *this;
    }

    template<typename F>
    inline const graphics& graphics::fill (F drawer, const brush& b) const {
      drawer(*this, b);
      return *this;
    }

    template<typename F>
    inline const graphics& graphics::draw (F drawer,
                                           const brush& b,
                                           const pen& p) const {
      drawer(*this, b, p);
      return *this;
    }

    template<typename F>
    inline const graphics& graphics::text (F drawer,
                                           const font& f,
                                           os::color c) const {
      drawer(*this, f, c);
      return *this;
    }

    template<typename F>
    inline const graphics& graphics::copy (F drawer,
                                           const core::point& pt) const {
      drawer(*this, pt);
      return *this;
    }

    // --------------------------------------------------------------------------
    template<typename T, typename F>
    inline paint::paint (T* t, F f)
      : p(util::bind_method(t, f))
    {}

    // --------------------------------------------------------------------------
    template<typename T, typename F>
    inline buffered_paint::buffered_paint (T* t, F f)
      : p(util::bind_method(t, f))
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
