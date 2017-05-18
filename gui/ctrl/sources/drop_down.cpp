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

#include "drop_down.h"

namespace gui {

  namespace layout {

    drop_down::drop_down (win::container* m)
      : main(m)
      , button(nullptr)
    {
      main->register_event_handler(__PRETTY_FUNCTION__, win::size_event(core::bind_method(this, &drop_down::layout)));
      main->register_event_handler(__PRETTY_FUNCTION__, win::show_event([&](){
        layout(main->client_size());
      }));
    }

    core::rectangle drop_down::label_place (const core::size& sz) {
      core::size_type h = sz.height() - 4;
      return core::rectangle(core::point(2, 2), core::size(sz.width() - h - 4, h));
    }

    core::rectangle drop_down::button_place (const core::size& sz) {
      core::size_type h = sz.height() - 4;
    return core::rectangle(core::point(sz.width() - h - 2, 2), core::size(h, h));
    }

    void drop_down::layout (const core::size& sz) {
      if (button) {
        button->place(button_place(sz));
      }
    }

    void drop_down::init (win::window* b) {
      button = b;
    }

  } // layout

  namespace win {

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
                             bool is_open,
                             bool enabled,
                             bool hilited,
                             bool pushed) {
        win::paint::push_button(graph, area, enabled, false, hilited, pushed);
        core::rectangle r = area.shrinked(core::size(4, 5));
        if (!r.empty()) {
          std::vector<core::point> p;
          if (is_open) {
            p = { {r.center_x(), r.y()}, r.bottom_right(), r.bottom_left() };
          } else {
            p = { r.top_left(), r.top_right(), {r.center_x(), r.y2()} };
          }
          graph.fill(draw::polygon(p), color::black);
        }
      }

      void drop_down_button (const draw::graphics& graph,
                             const win::button& btn,
                             bool is_open) {
        drop_down_button(graph, btn.client_area(), is_open,
                         btn.is_enabled(), btn.is_hilited(), btn.is_pushed());
      }

    } // paint

  } // win

} // gui
