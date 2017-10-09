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

    button_state::button_state ()
      : hilited(false)
      , pushed(false)
      , checked(false)
    {}

    // --------------------------------------------------------------------------
    button_base::button_base () {
      init();
    }

    button_base::button_base (const button_base& rhs)
      : super(rhs)
      , data(rhs.data)
    {
      init();
    }

    button_base::button_base (button_base&& rhs)
      : super(std::move(rhs))
      , data(std::move(rhs.data))
    {
      init();
    }

    void button_base::init() {
      set_accept_focus(true);
#ifdef X11
      detail::init_control_messages();
#endif // X11
      register_event_handler(REGISTER_FUNCTION, set_focus_event([&](window*) {
        redraw_later();
      }));
      register_event_handler(REGISTER_FUNCTION, lost_focus_event([&](window*) {
        redraw_later();
      }));
      register_event_handler(REGISTER_FUNCTION, mouse_enter_event([&]() {
        set_hilited(true);
      }));
      register_event_handler(REGISTER_FUNCTION, mouse_leave_event([&]() {
        set_hilited(false);
      }));
      register_event_handler(REGISTER_FUNCTION, any_key_down_event([&](os::key_state m, os::key_symbol k, const std::string&) {
        if ((k == keys::enter) || (k == keys::space)) {
          set_pushed(true);
        }
      }));
      register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state, const core::point&) {
        if (is_enabled()) {
          take_focus();
          set_pushed(true);
        }
      }));
    }

    void button_base::set_hilited (bool h) {
      if (data.hilited != h) {
        data.hilited = h;
        send_client_message(this, detail::HILITE_CHANGE_MESSAGE, h);
        redraw_later();
      }
    }

    void button_base::set_pushed (bool h) {
      if (data.pushed != h) {
        data.pushed = h;
        send_client_message(this, h ? detail::BN_PUSHED_MESSAGE : detail::BN_UNPUSHED_MESSAGE);
        redraw_later();
      }
    }

    void button_base::set_checked (bool f) {
      if (data.checked != f) {
        data.checked = f;
        send_client_message(this, detail::BN_STATE_MESSAGE, f ? 1 : 0);
        redraw_later();
      }
    }

    // --------------------------------------------------------------------------
    void push_button_traits::init (button_base& btn) {
      btn.register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&](os::key_state, const core::point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.client_area().is_inside(pos)) {
            send_client_message(&btn, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
      btn.register_event_handler(REGISTER_FUNCTION, any_key_up_event([&](os::key_state m, os::key_symbol k) {
        if (((k == keys::enter) || (k == keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          send_client_message(&btn, detail::BN_CLICKED_MESSAGE);
        }
      }));
    }
    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<false>::init (button_base& btn) {
      btn.register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&](os::key_state, const core::point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (btn.client_area().is_inside(pos)) {
            btn.set_checked(!btn.is_checked());
            send_client_message(&btn, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
      btn.register_event_handler(REGISTER_FUNCTION, any_key_up_event([&](os::key_state m, os::key_symbol k) {
        if (((k == keys::enter) || (k == keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          btn.set_checked(!btn.is_checked());
          send_client_message(&btn, detail::BN_CLICKED_MESSAGE);
        }
      }));
    }
    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<true>::init (button_base& btn) {
      btn.register_event_handler(REGISTER_FUNCTION, left_btn_up_event([&](os::key_state, const core::point& pos) {
        if (btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked() && btn.client_area().is_inside(pos)) {
            btn.set_checked(true);
            send_client_message(&btn, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
      btn.register_event_handler(REGISTER_FUNCTION, any_key_up_event([&](os::key_state m, os::key_symbol k) {
        if (((k == keys::enter) || (k == keys::space)) && btn.is_pushed()) {
          btn.set_pushed(false);
          if (!btn.is_checked()) {
            btn.set_checked(true);
            send_client_message(&btn, detail::BN_CLICKED_MESSAGE);
          }
        }
      }));
    }
    // --------------------------------------------------------------------------
    namespace paint {

      const int dot_line_width = 1;
      const draw::pen::Style dot_line_style = draw::pen::Style::dot;

      // --------------------------------------------------------------------------
      void button_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         const button_state& state,
                         bool focused,
                         bool enabled) {
        using namespace draw;

        core::rectangle area = r;

        graph.fill(draw::rectangle(area), enabled && state.hilited ? color::buttonHighLightColor() : color::buttonColor());
        if (enabled && focused) {
          graph.frame(draw::rectangle(area), color::black);
          area.shrink({1, 1});
        }
        frame::deep_relief(graph, area, state.pushed);
        if (enabled && focused) {
          area.shrink({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
        }
      }

      // --------------------------------------------------------------------------
      void push_button (const draw::graphics& graph,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        bool focused,
                        bool enabled) {
        button_frame(graph, r, state, focused, enabled);

        using namespace draw;
        graph.text(text_box(text, r, text_origin::center), font::system(),
                   enabled ? color::windowTextColor() : color::disabledTextColor());
      }

      // --------------------------------------------------------------------------
      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        bool focused,
                        bool enabled,
                        os::color foreground,
                        os::color background) {
        os::color b = background;
        if (state.pushed && enabled) {
          b = color::darker(background, 0.25);
        } else if (state.hilited && enabled) {
          b = color::lighter(background, 0.25);
        }
        g.fill(draw::rectangle(r), b);

        os::color f = foreground;
        if (enabled) {
          if (state.pushed) {
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
        g.text(draw::text_box(text, r, text_origin::center), draw::font::system(), f);
        if (enabled && focused) {
          g.frame(draw::rectangle(r), draw::pen(f, dot_line_width, dot_line_style));
        }
      }

      // --------------------------------------------------------------------------
      void radio_button (const draw::graphics& graph,
                         const core::rectangle& rec,
                         const std::string& text,
                         const button_state& state,
                         bool focused,
                         bool enabled) {
        using namespace draw;

        core::rectangle area = rec;

        os::color col = enabled ? color::windowTextColor() : color::disabledTextColor();
        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point_type y = area.y() + area.size().height() / 2;
        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(ellipse(r), state.pushed ? color::very_light_gray
                                      : color::buttonColor(), col);
        if (state.checked) {
          r.shrink(core::size(2, 2));
          graph.fill(ellipse(r), state.pushed ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, text_origin::vcenter_left), font::system(), col);
        if (focused) {
          graph.text(bounding_box(text, area, text_origin::vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
        }
      }

      // --------------------------------------------------------------------------
      void check_box (const draw::graphics& graph,
                      const core::rectangle& rec,
                      const std::string& text,
                      const button_state& state,
                      bool focused,
                      bool enabled) {
        using namespace draw;

        core::rectangle area = rec;

        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point_type y = area.y() + area.height() / 2;
        os::color col = enabled ? color::windowTextColor() : color::disabledTextColor();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   state.pushed ? color::very_light_gray
                                : color::buttonColor(),
                   col);

        if (state.checked) {
          r.shrink(core::size(2, 2));
          graph.fill(rectangle(r), state.pushed ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, text_origin::vcenter_left), font::system(), col);
        if (focused) {
          graph.text(bounding_box(text, area, text_origin::vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
        }
      }
    }

  } // win

} // gui
