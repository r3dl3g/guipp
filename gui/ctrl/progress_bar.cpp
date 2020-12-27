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
 * @brief     C++ API: label
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/progress_bar.h>
#include <iomanip>

//#define SHOW_TEXT_AREA

namespace gui {

  namespace ctrl {

    namespace detail {

      progress_bar_base::data::data ()
        : min(0)
        , max(1)
        , value(0)
      {}

      // --------------------------------------------------------------------------
      progress_bar_base::progress_bar_base (const std::string& t)
        : super(t)
      {}

      progress_bar_base::progress_bar_base (const text_source& t)
        : super(t)
      {}

      void progress_bar_base::set_min (type mi) {
        data.min = mi;
        invalidate();
      }

      void progress_bar_base::set_max (type ma) {
        data.max = ma;
        invalidate();
      }

      void progress_bar_base::set_value (type v) {
        data.value = v;
        if (data.max == 1) {
          set_text(ostreamfmt(std::fixed << std::setprecision(1) << (v * 100) << "%"));
        } else {
          set_text(ostreamfmt(v));
        }
        invalidate();
      }

      void progress_bar_base::set_min_max (type mi, type ma) {
        data.min = mi;
        data.max = ma;
        invalidate();
      }

      void progress_bar_base::set_min_max_value (type mi, type ma, type v) {
        data.min = mi;
        data.max = ma;
        set_value(v);
      }

    } // detail

  } // ctrl

} // gui
