/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
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
 * @brief     C++ API: basic window types
 *
 * @file
 */

#ifdef GUIPP_X11

// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/native.h>
#include <gui/core/rectangle.h>


namespace gui {

  // --------------------------------------------------------------------------
  namespace core {

    namespace native {

#ifdef GUIPP_USE_XFT
      namespace x11 {

        XftDraw* get_xft_draw (context& ctx) {
          static XftDraw* s_xft = nullptr;

          if (!s_xft) {
            auto display = global::get_instance();
            auto screen = global::x11::get_screen();
            auto visual = global::x11::get_visual();
            auto colormap = DefaultColormap(display, screen);

            // XVisualInfo vinfo;
            // XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo);

            // auto visual = vinfo.visual;
            // auto colormap = XCreateColormap(display, target, visual, AllocNone);

            s_xft = XftDrawCreate(display, ctx, visual, colormap);
          } else {
            XftDrawChange(s_xft, ctx);
          }
          return s_xft;
        }

      } // namespace x11
#endif // GUIPP_USE_XFT

      // --------------------------------------------------------------------------
      void set_clip_rect (context& ctx, const gui::os::rectangle& r) {
        XSetClipRectangles(global::get_instance(), ctx, 0, 0, const_cast<gui::os::rectangle*>(&r), 1, Unsorted);
#ifdef GUIPP_USE_XFT
        XftDrawSetClipRectangles(x11::get_xft_draw(ctx), 0, 0, &r, 1);
#endif // GUIPP_USE_XFT
      }

      // --------------------------------------------------------------------------
      void clear_clip_rect (context& ctx) {
        XSetClipMask(global::get_instance(), ctx, None);
#ifdef GUIPP_USE_XFT
        XftDrawSetClip(x11::get_xft_draw(ctx), None);
#endif // GUIPP_USE_XFT
      }

    } // namespace native

  } // namespace core

} // namespace gui

#endif // GUIPP_X11
