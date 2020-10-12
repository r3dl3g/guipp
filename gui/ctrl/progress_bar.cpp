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

      progress_bar_base::progress_bar_base (const progress_bar_base& rhs)
        : super(rhs)
        , data(rhs.data)
      {}

      progress_bar_base::progress_bar_base (progress_bar_base&& rhs)
        : super(std::move(rhs))
        , data(std::move(rhs.data))
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

    namespace paint {

      void progress_bar (const draw::graphics& graph,
                         const core::rectangle& area,
                         const std::string& text,
                         os::color foreground,
                         os::color background,
                         text_origin_t origin,
                         os::color bar_color,
                         core::size::type bar_pos) {
        using namespace gui::draw;
        graph.fill(draw::rectangle(area.with_width(bar_pos)), bar_color);
        graph.fill(draw::rectangle({bar_pos, area.y(), area.x2() - bar_pos, area.height()}), background);
        graph.text(draw::text_box(text, area, origin), font::system(), foreground);
#ifdef SHOW_TEXT_AREA
        graph.text(draw::bounding_box(text, area, origin), font::system(), color::black);
        graph.frame(draw::rectangle(area), draw::pen(color::black, draw::pen::dot));
#endif // SHOW_TEXT_AREA
      }

    } //paint

  } // ctrl

} // gui
