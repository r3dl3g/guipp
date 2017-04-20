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
* @brief     C++ API: button
*
* @file
*/

#include "button.h"
#include "graphics.h"

namespace gui {

  namespace win {

    // --------------------------------------------------------------------------
    button::button ()
      : hilited(false)
      , pushed(false)
      , checked(false)
    {
#ifdef X11
      if (!detail::BN_CLICKED_MESSAGE) {
        detail::BN_CLICKED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_CLICKED_MESSAGE", False);
      }
      if (!detail::BN_PUSHED_MESSAGE) {
        detail::BN_PUSHED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_PUSHED_MESSAGE", False);
      }
      if (!detail::BN_UNPUSHED_MESSAGE) {
        detail::BN_UNPUSHED_MESSAGE = XInternAtom(core::global::get_instance(), "BN_UNPUSHED_MESSAGE", False);
      }
      if (!detail::BN_STATE_MESSAGE) {
        detail::BN_STATE_MESSAGE = XInternAtom(core::global::get_instance(), "BN_STATE_MESSAGE", False);
      }
#endif // X11
      register_event_handler(set_focus_event([&](window*){
        redraw_later();
      }));
      register_event_handler(lost_focus_event([&](window*){
        redraw_later();
      }));
      register_event_handler(win::mouse_enter_event([&]() {
        set_hilited(true);
      }));
      register_event_handler(win::mouse_leave_event([&]() {
        set_hilited(false);
      }));
    }

    void button::set_hilited (bool h) {
      hilited = h;
      redraw_later();
    }

    void button::set_pushed (bool h) {
      if (pushed != h) {
        pushed = h;
        send_client_message(this, pushed ? detail::BN_PUSHED_MESSAGE : detail::BN_UNPUSHED_MESSAGE);
        redraw_later();
      }
    }

    void button::set_checked (bool f) {
      if (checked != f) {
        checked = f;
        send_client_message(this, detail::BN_STATE_MESSAGE, f ? 1 : 0);
        redraw_later();
      }
    }

    // --------------------------------------------------------------------------
    push_button::push_button () {
      register_event_handler(left_btn_down_event([&](const core::point&) {
        if (is_enabled()) {
          take_focus();
          set_pushed(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](const core::point& pos) {
        if (is_pushed()) {
          set_pushed(false);
          if (client_area().is_inside(pos)) {
            send_client_message(this, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
    }

    // --------------------------------------------------------------------------
    toggle_button::toggle_button () {
      register_event_handler(left_btn_down_event([&](const core::point&) {
        if (is_enabled()) {
          take_focus();
          set_pushed(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](const core::point& pos) {
        if (is_pushed()) {
          set_pushed(false);
          if (client_area().is_inside(pos)) {
            set_checked(!is_checked());
            send_client_message(this, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
    }

    namespace paint {
      // --------------------------------------------------------------------------
      void push_button (const draw::graphics& graph, const win::button& btn, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();
        const bool enabled = btn.is_enabled();
        core::rectangle area = btn.client_area();

        graph.fill(draw::rectangle(area), enabled && btn.is_hilited() ? color::buttonHighLightColor() : color::buttonColor());
        if (enabled && focus) {
          graph.frame(draw::rectangle(area), color::black);
          area.shrink({1, 1});
        }
        frame::deep_relief(graph, area, btn.is_pushed());
        graph.text(text_box(text, area, center), font::system(), enabled ? color::windowTextColor() : color::disabledTextColor());
        if (enabled && focus) {
          area.shrink({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void flat_button (const draw::graphics& g, const win::button& btn, const std::string& text) {
        using namespace draw;

        core::rectangle r = btn.client_area();
        const bool enabled = btn.is_enabled();

        if (btn.is_pushed()) {
          g.fill(draw::rectangle(r), color::dark_gray);
        } else if (btn.is_hilited() && btn.is_enabled()) {
          g.fill(draw::rectangle(r), color::light_gray);
        } else {
          g.fill(draw::rectangle(r), color::workSpaceColor());
        }
        g.text(text_box(text, r, center),
               font::system(),
               enabled ? color::white : color::gray);
      }

      // --------------------------------------------------------------------------
      void radio_button (const draw::graphics& graph, const win::button& btn, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();
        const bool enabled = btn.is_enabled();
        const bool pushed = btn.is_pushed();

        os::color col = enabled ? color::windowTextColor() : color::disabledTextColor();
        core::rectangle area = btn.client_area();
        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point_type y = area.y() + area.size().height() / 2;
        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(ellipse(r), pushed ? color::very_light_gray
                                      : color::buttonColor(), col);
        if (btn.is_checked()) {
          r.shrink(core::size(2, 2));
          graph.fill(ellipse(r), pushed ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(bounding_box(text, area, vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void check_box (const draw::graphics& graph, const win::button& btn, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();
        const bool enabled = btn.is_enabled();
        const bool pushed = btn.is_pushed();

        core::rectangle area = btn.client_area();
        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point_type y = area.y() + area.height() / 2;
        os::color col = enabled ? color::windowTextColor() : color::disabledTextColor();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   pushed ? color::very_light_gray
                          : color::buttonColor(),
                   col);

        if (btn.is_checked()) {
          r.shrink(core::size(2, 2));
          graph.fill(rectangle(r), pushed ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(bounding_box(text, area, vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, pen::dot));
        }
      }
    }

    // --------------------------------------------------------------------------
    text_button::text_button () {
      register_event_handler(paint_event([&] (const draw::graphics& graph) {
        paint::push_button(graph, *this, get_text());
      }));
    }

    // --------------------------------------------------------------------------
    flat_button::flat_button () {
      register_event_handler(paint_event([&] (const draw::graphics& graph) {
        paint::flat_button(graph, *this, get_text());
      }));
    }

    // --------------------------------------------------------------------------
    radio_button::radio_button () {
      register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
        paint::radio_button(graph, *this, get_text());
      }));
    }

    // --------------------------------------------------------------------------
    check_box::check_box () {
      register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
        paint::check_box(graph, *this, get_text());
      }));
    }

  } // win

} // gui
