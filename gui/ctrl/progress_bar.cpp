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
 * @brief     progress bar control
 *
 * @license   MIT license. See accompanying file LICENSE.
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

      // --------------------------------------------------------------------------
      progress_bar_base::progress_bar_base (os::color bc, os::color fg, os::color bg)
        : data{0, 1, 0, bc, fg, {}}
      {
        set_text([&] () {
          if (data.max == 1) {
            return ostreamfmt(std::fixed << std::setprecision(1) << (data.value * 100) << "%");
          } else {
            return ostreamfmt(data.value);
          }
        });
        set_background(bg);
      }

      void progress_bar_base::set_min (type mi) {
        data.min = mi;
      }

      void progress_bar_base::set_max (type ma) {
        data.max = ma;
      }

      void progress_bar_base::set_value (type v) {
        data.value = v;
      }

      void progress_bar_base::set_min_max (type mi, type ma) {
        data.min = mi;
        data.max = ma;
      }

      void progress_bar_base::set_min_max_value (type mi, type ma, type v) {
        set_min_max(mi, ma);
        set_value(v);
      }

      void progress_bar_base::set_prefix (const std::string& t) {
        set_text([&, t] () {
          if (data.max == 1) {
            return ostreamfmt(t << std::fixed << std::setprecision(1) << (data.value * 100) << "%");
          } else {
            return ostreamfmt(t << data.value);
          }
        });
      }

      void progress_bar_base::set_prefix (const core::text_source& t) {
        set_text([&, t] () {
          if (data.max == 1) {
            return ostreamfmt(t() << std::fixed << std::setprecision(1) << (data.value * 100) << "%");
          } else {
            return ostreamfmt(t() << data.value);
          }
        });
      }

      os::color progress_bar_base::get_bar_color () const {
        return data.bar_color;
      }

      void progress_bar_base::set_bar_color (os::color c) {
        data.bar_color = c;
      }

      os::color progress_bar_base::get_foreground () const {
        return data.foreground;
      }

      void progress_bar_base::set_foreground (os::color c) {
        data.foreground = c;
      }

      template<>
      void progress_bar_drawer<orientation_t::horizontal> (draw::graphics& graph,
                                                           const progress_bar_base& pb,
                                                           text_origin_t origin,
                                                           draw::frame::drawer frame) {
        gui::core::rectangle place = frame(graph, pb.client_geometry());
        auto pos = (pb.get_value() - pb.get_min()) / (pb.get_max() - pb.get_min()) * place.width();
        gui::look::horizontal_progress_bar(graph, place, pb.get_text(), pb.get_foreground(), pb.get_background(), origin, pb.get_bar_color(), pos);
      }

      template<>
      void progress_bar_drawer<orientation_t::vertical> (draw::graphics& graph,
                                                         const progress_bar_base& pb,
                                                         text_origin_t origin,
                                                         draw::frame::drawer frame) {
        gui::core::rectangle place = frame(graph, pb.client_geometry());
        auto pos = (pb.get_value() - pb.get_min()) / (pb.get_max() - pb.get_min()) * place.height();
        gui::look::vertical_progress_bar(graph, place, pb.get_text(), pb.get_foreground(), pb.get_background(), origin, pb.get_bar_color(), pos);
      }

    } // detail

  } // ctrl

} // gui
