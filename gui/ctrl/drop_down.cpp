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

    void drop_down::init () {
      data.main->on_layout(basepp::bind_method(this, &drop_down::layout));
    }

    core::rectangle drop_down::label_place (const core::size& sz) {
      core::size::type h = sz.height() - 4;
      return core::rectangle(core::point(2, 2), core::size(sz.width() - h - 4, h));
    }

    core::rectangle drop_down::button_place (const core::size& sz) {
      core::size::type h = sz.height() - 4;
      return core::rectangle(core::point(sz.width() - h - 2, 2), core::size(h, h));
    }

    void drop_down::layout (const core::size& sz) {
      LogTrace << "drop_down::layout()";
      if (data.button) {
        data.button->place(button_place(sz));
      }
    }

    void drop_down::init (win::window* b) {
      data.button = b;
    }

  } // layout

  namespace ctrl {

    namespace paint {

      void drop_down_item (const draw::graphics& g,
                           const core::rectangle& r,
                           const draw::brush& background,
                           const std::string& label,
                           bool selected,
                           bool hilited) {
        if (selected) {
          g.fill(draw::rectangle(r), color::highLightColor());
        } else if (hilited) {
          g.fill(draw::rectangle(r), color::menuColorHighlight());
        } else {
          g.fill(draw::rectangle(r), background);
        }

        os::color col = selected ? color::highLightTextColor()
                        : color::black;

        g.text(draw::text_box(label, r, text_origin::vcenter_left), draw::font::system(), col);

      }

      void drop_down_button (const draw::graphics& graph,
                             const core::rectangle& area,
                             const button_state& state,
                             bool is_open) {
        paint::button_frame(graph, area, state);
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
