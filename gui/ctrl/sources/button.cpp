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

    no_erase_window_class button::clazz("BUTTON",
#ifdef WIN32
      (os::color)(COLOR_BTNFACE + 1)
#endif // WIN32
#ifdef X11
      color::buttonColor()
#endif
    );

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
      register_event_handler(left_btn_down_event([&](os::key_state, const core::point&) {
        if (is_enabled()) {
          take_focus();
          set_pushed(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](os::key_state, const core::point& pos) {
        if (is_pushed()) {
          set_pushed(false);
          if (client_area().is_inside(pos)) {
            send_client_message(this, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
    }

    // --------------------------------------------------------------------------
    template<>
    toggle_button<false>::toggle_button () {
      register_event_handler(left_btn_down_event([&](os::key_state, const core::point&) {
        if (is_enabled()) {
          take_focus();
          set_pushed(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](os::key_state, const core::point& pos) {
        if (is_pushed()) {
          set_pushed(false);
          if (client_area().is_inside(pos)) {
            set_checked(!is_checked());
            send_client_message(this, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
    }

    // --------------------------------------------------------------------------
    template<>
    toggle_button<true>::toggle_button () {
      register_event_handler(left_btn_down_event([&](os::key_state, const core::point&) {
        if (is_enabled()) {
          take_focus();
          set_pushed(true);
        }
      }));
      register_event_handler(left_btn_up_event([&](os::key_state, const core::point& pos) {
        if (is_pushed()) {
          set_pushed(false);
          if (!is_checked() && client_area().is_inside(pos)) {
            set_checked(true);
            send_client_message(this, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
    }

    namespace paint {
      // --------------------------------------------------------------------------
      void push_button (const draw::graphics& graph,
                        const core::rectangle& r,
                        const std::string& text,
                        bool enabled,
                        bool focused,
                        bool hilited,
                        bool pushed) {
        using namespace draw;

        core::rectangle area = r;

        graph.fill(draw::rectangle(area), enabled && hilited ? color::buttonHighLightColor() : color::buttonColor());
        if (enabled && focused) {
          graph.frame(draw::rectangle(area), color::black);
          area.shrink({1, 1});
        }
        frame::deep_relief(graph, area, pushed);
        graph.text(text_box(text, area, center), font::system(), enabled ? color::windowTextColor() : color::disabledTextColor());
        if (enabled && focused) {
          area.shrink({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void push_button (const draw::graphics& graph, const win::button& btn, const std::string& text) {
        push_button(graph, btn.client_area(), text, btn.is_enabled(),
                    btn.has_focus(), btn.is_hilited(), btn.is_pushed());
      }

      // --------------------------------------------------------------------------
      void flat_button (const draw::graphics& graph,
                        const win::button& btn,
                        const std::string& text,
                        os::color foreground,
                        os::color background) {
        flat_button(graph, btn.client_area(), text, btn.is_enabled(),
                    btn.is_pushed(), btn.is_hilited(), foreground, background);
      }

      // --------------------------------------------------------------------------
      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        bool enabled,
                        bool pushed,
                        bool hilited,
                        os::color foreground,
                        os::color background) {
        os::color b = background;
        if (pushed && enabled) {
          b = color::darker(background, 0.25);
        } else if (hilited && enabled) {
          b = color::lighter(background, 0.25);
        }
        g.fill(draw::rectangle(r), b);

        os::color f = foreground;
        if (enabled) {
          if (pushed) {
            os::color b2 = color::invert(b);
            os::color f2 = color::invert(foreground);
            int i1 = color::compare(b, b2);
            int i2 = color::compare(b, f2);
            if (abs(i1) > abs(i2)) {
              f = b2;
            } else {
              f = f2;
            }
          }
        } else {
          f = color::darker(foreground);
        }
        g.text(draw::text_box(text, r, draw::center), draw::font::system(), f);
      }

      // --------------------------------------------------------------------------
      void radio_button (const draw::graphics& graph,
                         const core::rectangle& rec,
                         const std::string& text,
                         bool enabled,
                         bool focused,
                         bool pushed,
                         bool checked) {
        using namespace draw;

        core::rectangle area = rec;

        os::color col = enabled ? color::windowTextColor() : color::disabledTextColor();
        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point_type y = area.y() + area.size().height() / 2;
        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(ellipse(r), pushed ? color::very_light_gray
                                      : color::buttonColor(), col);
        if (checked) {
          r.shrink(core::size(2, 2));
          graph.fill(ellipse(r), pushed ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, vcenter_left), font::system(), col);
        if (focused) {
          graph.text(bounding_box(text, area, vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void radio_button (const draw::graphics& graph,
                         const win::button& btn,
                         const std::string& text) {
        radio_button(graph, btn.client_area(), text, btn.is_enabled(),
                     btn.has_focus(), btn.is_pushed(), btn.is_checked());
      }

      // --------------------------------------------------------------------------
      void check_box (const draw::graphics& graph, const win::button& btn, const std::string& text) {
        check_box(graph, btn.client_area(), text, btn.is_enabled(),
                  btn.has_focus(), btn.is_pushed(), btn.is_checked());
      }

      // --------------------------------------------------------------------------
      void check_box (const draw::graphics& graph,
                      const core::rectangle& rec,
                      const std::string& text,
                      bool enabled,
                      bool focused,
                      bool pushed,
                      bool checked) {
        using namespace draw;

        core::rectangle area = rec;

        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point_type y = area.y() + area.height() / 2;
        os::color col = enabled ? color::windowTextColor() : color::disabledTextColor();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   pushed ? color::very_light_gray
                          : color::buttonColor(),
                   col);

        if (checked) {
          r.shrink(core::size(2, 2));
          graph.fill(rectangle(r), pushed ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, vcenter_left), font::system(), col);
        if (focused) {
          graph.text(bounding_box(text, area, vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, pen::dot));
        }
      }
    }

  } // win

} // gui
