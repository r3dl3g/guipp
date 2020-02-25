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
 * @brief     C++ API: drop down list
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/drop_down.h>


namespace gui {

  namespace layout {

    core::rectangle drop_down::label_place (const core::rectangle& r) const {
      core::size::type h = r.height() - 4;
      return core::rectangle(r.top_left() + core::point(2, 2), core::size(r.width() - h - 4, h));
    }

    core::rectangle drop_down::button_place (const core::rectangle& r) const {
      core::size::type h = r.height() - 4;
      return core::rectangle(r.top_left() + core::point(r.width() - h - 2, 2), core::size(h, h));
    }

    void drop_down::layout (const core::rectangle& r) const {
      log_trace << "drop_down::layout()";
      if (data.button) {
        data.button->place(button_place(r));
      }
    }

    void drop_down::init (win::container* c, win::window* b) {
      data.main = c;
      data.button = b;
    }

  } // layout

  namespace ctrl {

    namespace paint {

      void drop_down_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           item_state state) {
        if (item_state::selected == state) {
          g.fill(draw::rectangle(r), color::highLightColor());
        } else if (item_state::hilited == state) {
          g.fill(draw::rectangle(r), color::menuColorHighlight());
        } else {
          g.fill(draw::rectangle(r), background);
        }

        os::color col = item_state::selected == state ? color::highLightTextColor()
                        : color::black;

        g.text(draw::text_box(label, r, text_origin::vcenter_left), draw::font::system(), col);

      }

      void drop_down_button (const draw::graphics& graph,
                             const core::rectangle& area,
                             const button_state& state,
                             bool is_open) {
#ifdef BUILD_FOR_ARM
        paint::button_frame_w95(graph, area, state);
#else
        paint::button_frame(graph, area, state);
#endif
        core::rectangle r = area.shrinked(core::size(4, 5));
        if (!r.empty()) {
          std::vector<core::point> p;
          if (is_open) {
            p = {r.bottom_right(), {r.center_x(), r.y()}, r.bottom_left()};
          } else {
            p = {r.top_left(), {r.center_x(), r.y2()}, r.top_right()};
          }
          graph.fill(draw::polygon(p), color::black);
        }
      }

    } // paint

  } // ctrl

} // gui
