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
    window_class text_button::clazz(
#ifdef WIN32
      window_class::custom_class("PUSH_BUTTON++", 0,
                                 WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                 WS_EX_NOPARENTNOTIFY)
#else // !WIN32
      window_class::custom_class("PUSH_BUTTON++",
                                 draw::color::buttonColor())
#endif // !WIN32
    );

    window_class radio_button::clazz(
#ifdef WIN32
      window_class::custom_class("RADIO_BUTTON++", 0,
                                 WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN |WS_VISIBLE | WS_TABSTOP,
                                 WS_EX_NOPARENTNOTIFY)
#else // !WIN32
      window_class::custom_class("RADIO_BUTTON++",
                                 draw::color::buttonColor())
#endif // !WIN32
    );

    window_class check_box::clazz(
#ifdef WIN32
        window_class::custom_class("CHECKBOX++", 0,
                                   WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_TABSTOP,
                                   WS_EX_NOPARENTNOTIFY)
#else // !WIN32
        window_class::custom_class("CHECKBOX++",
                                   draw::color::buttonColor())
#endif // !WIN32
    );

    // --------------------------------------------------------------------------
    button::button ()
      : checked(false)
      , hilited(false)
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
    }

    bool button::is_checked () const {
      return checked;
    }

    void button::set_checked (bool f) {
      if (checked != f) {
        checked = f;
        send_client_message(this, detail::BN_STATE_MESSAGE, f ? 1 : 0);
        redraw_later();
      }
    }

    bool button::is_hilited () const {
      return hilited;
    }

    void button::set_hilited (bool h) {
      hilited = h;
      redraw_later();
    }

    namespace paint {
      // --------------------------------------------------------------------------
      void push_button (const draw::graphics& graph, const win::window& btn, bool is_checked, bool, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();
        core::rectangle area = btn.client_area();

        if (focus) {
          graph.frame(draw::rectangle(area), color::black());
          area.shrink({1, 1});
        }
        draw::frame::deep_relief(graph, area, is_checked);
        graph.text(draw::text_box(text, area, center), font::system(), btn.is_enabled() ? color::black() : color::gray());
        if (focus) {
          area.shrink({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void flat_button (const draw::graphics& g, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text) {
        core::rectangle r = btn.client_area() - core::size{ 1, 1 };

        if (is_checked) {
          g.fill(draw::rectangle(r), draw::color::darkGray());
        } else
        if (is_hilited) {
          g.fill(draw::rectangle(r), draw::color::lightGray());
        } else {
          g.fill(draw::rectangle(r), draw::color::workSpaceColor());
        }
        g.text(draw::text_box(text, r, draw::center), draw::font::system(), draw::color::white());
      }

      // --------------------------------------------------------------------------
      void radio_button (const draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();

        color col = btn.is_enabled() ? color::black() : color::gray();
        core::rectangle area = btn.client_area();
        core::point_type y = area.position().y() + area.size().height() / 2;
        core::point pt(area.position().x() + 6, y);
        graph.draw(arc(pt, 5, 0, 360),
                   is_hilited ? color::veryLightGray()
                              : color::buttonColor(),
                   col);
        if (is_checked) {
          graph.fill(arc(pt, 3, 0, 360), col);
        }
        area.x(20);
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(draw::bounding_box(text, area, vcenter_left), font::system(), color::black());
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }

      // --------------------------------------------------------------------------
      void check_box (const draw::graphics& graph, const win::window& btn, bool is_checked, bool is_hilited, const std::string& text) {
        using namespace draw;

        const bool focus = btn.has_focus();

        core::rectangle area = btn.client_area();
        core::point_type y = area.y() + area.height() / 2;

        color col = btn.is_enabled() ? color::black() : color::gray();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   is_hilited ? color::veryLightGray()
                              : color::buttonColor(),
                   col);
        if (is_checked) {
          core::point center = r.center();
          /*
          graph.draw_lines({ core::point(r.x() + 1, center.y()),
                             core::point(center.x() - 1, r.y2() - 2),
                             core::point(r.x2() - 1, r.y() + 1 ) }, pen(col, pen::solid, 2));
          */
          r.shrink(core::size(2, 2));
          graph.fill(rectangle(r), col);
        }
        area += core::point(20, 0);
        graph.text(draw::text_box(text, area, vcenter_left), font::system(), col);
        if (focus) {
          graph.text(draw::bounding_box(text, area, vcenter_left), font::system(), color::black());
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), draw::pen(color::black(), draw::pen::dot));
        }
      }
    }

    // --------------------------------------------------------------------------
    text_button::text_button () {
      register_event_handler(paint_event([&] (const draw::graphics& graph) {
        paint::push_button(graph, *this, is_checked(), is_hilited(), get_text());
      }));
    }

    // --------------------------------------------------------------------------
    radio_button::radio_button () {
      register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
        paint::radio_button(graph, *this, is_checked(), is_hilited(), get_text());
      }));
    }

    // --------------------------------------------------------------------------
    check_box::check_box () {
      register_event_handler(win::paint_event([&] (const draw::graphics& graph) {
        paint::check_box(graph, *this, is_checked(), is_hilited(), get_text());
      }));
    }

  } // win

} // gui
