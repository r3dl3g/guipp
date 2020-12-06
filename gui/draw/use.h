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
 * @brief     C++ API: basic window types
 *
 * @file
 */


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/guidefs.h>
#include <gui++-draw-export.h>


namespace gui {

  namespace draw {

#ifdef WIN32
    template<typename T>
    struct GUIPP_DRAW_EXPORT Use {
      Use (os::graphics g, const T& t)
        : g(g)
        , obj(t)
        , old(set(t))
      {}

      HGDIOBJ set (HGDIOBJ t) {
        return SelectObject(g, t);
      }

      void unset (HGDIOBJ t) {
        SelectObject(g, t);
      }

      inline ~Use () {
        unset(old);
      }

      os::graphics g;
      HGDIOBJ obj;
      HGDIOBJ old;
    };

#endif // WIN32

#ifdef X11
    // --------------------------------------------------------------------------
    template<typename T>
    struct GUIPP_DRAW_EXPORT Use {
      Use (os::graphics g, const T& t)
        : g(g) {
        set(t);
      }

      void set (const T& t);

      os::graphics g;
    };

    // --------------------------------------------------------------------------
    struct pen;
    struct brush;
    struct font;

    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen&);

    template<>
    void Use<brush>::set (const brush&);

#ifndef USE_XFT
    template<>
    void Use<font>::set(const font&);
#endif // USE_XFT

#endif // X11

#ifdef QT_WIDGETS_LIB
    // --------------------------------------------------------------------------
    template<typename T>
    struct GUIPP_DRAW_EXPORT Use {
      Use (os::graphics g, const T& t)
        : g(g) {
        set(t);
      }

      void set (const T& t);

      os::graphics g;
    };

    // --------------------------------------------------------------------------
    struct pen;
    struct brush;
    struct font;

    // --------------------------------------------------------------------------
    template<>
    void Use<pen>::set (const pen&);

    template<>
    void Use<brush>::set (const brush&);

    template<>
    void Use<font>::set(const font&);

#endif // QT_WIDGETS_LIB

  } // namespace draw

} // namespace gui
