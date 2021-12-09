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
 * @brief     graphic functions
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace draw {

    inline os::graphics graphics::os () const {
      return gc();
    }

    inline os::graphics graphics::gc () const {
      return ctx->graphics();
    }

    inline os::drawable graphics::target () const {
      return ctx->drawable();
    }

    inline graphics::operator os::graphics () const {
      return gc();
    }

    inline graphics::operator os::drawable() const {
      return target();
    }

    inline const core::context& graphics::context () const {
      return *ctx;
    }

    inline core::context& graphics::context () {
      return *ctx;
    }

    template<typename F>
    graphics& graphics::frame (F drawer, const pen& p) {
      if (!is_transparent(p)) {
        drawer(*this, p);
      }
      return *this;
    }

    template<typename F>
    graphics& graphics::fill (F drawer, const brush& b) {
      if (!is_transparent(b)) {
        drawer(*this, b);
      }
      return *this;
    }

    template<typename F>
    graphics& graphics::draw (F drawer, const brush& b, const pen& p) {
      if (is_transparent(b) && is_transparent(p)) {
        return *this;
      }
      drawer(*this, b, p);
      return *this;
    }

    template<typename F>
    graphics& graphics::text (F drawer, const font& f, os::color c) {
      if (!color::is_transparent(c)) {
        drawer(*this, f, c);
      }
      return *this;
    }

    template<typename F>
    graphics& graphics::copy (F drawer, const core::point& pt) {
      drawer(*this, pt);
      return *this;
    }

    // --------------------------------------------------------------------------
    template<typename T, typename F>
    inline paint::paint (T* t, F f)
      : p(util::bind_method(t, f))
    {}


  } // draw

} // gui
