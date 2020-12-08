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

// --------------------------------------------------------------------------
//
// Common includes
//
#include <math.h>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/graphics.h>
#include <gui/ctrl/button.h>
#include <gui/io/pnm.h>


namespace gui {

  namespace image_data {

#include <gui/ctrl/res/button_frame.h>
#include <gui/ctrl/res/button_pressed_frame.h>
#include <gui/ctrl/res/button_rot_frame.h>
#include <gui/ctrl/res/button_pressed_rot_frame.h>
#include <gui/ctrl/res/simple_frame.h>

  } // namespace image_data

  namespace ctrl {

#ifdef GUIPP_QT
    bool get_button_state (const core::event& e) {
      return e.cast<win::QClientEvent>().l1() != 0;
    }
#endif // GUIPP_QT

    namespace detail {

      template<typename T, size_t N>
      inline std::string make_string (T(&t)[N]) {
        return std::string((const char*)t, N);
      }

      draw::graymap build_image (std::string name) {
        draw::graymap bmp;
        std::istringstream in(name);
        io::load_pnm(in, bmp);
        return bmp;
      }

      draw::graymap build_button_frame_image (bool pressed, bool rot) {
        return build_image(pressed ? (rot ? make_string(image_data::button_pressed_rot_frame_bytes)
                                          : make_string(image_data::button_pressed_frame_bytes))
                                   : (rot ? make_string(image_data::button_rot_frame_bytes)
                                          : make_string(image_data::button_frame_bytes)));
      }

      draw::graymap build_simple_frame_image () {
        return build_image(make_string(image_data::simple_frame_bytes));
      }

    } // namespace detail

    template<bool rot, bool pressed>
    const draw::graymap& get_button_frame () {
      static draw::graymap image(detail::build_button_frame_image(pressed, rot));
      return image;
    }

    const draw::graymap& get_simple_frame () {
      static draw::graymap image(detail::build_simple_frame_image());
      return image;
    }

    template<alignment_t A>
    const draw::graymap& get_tab_frame (bool pressed);

    template<>
    const draw::graymap& get_tab_frame<alignment_t::top> (bool pressed) {
      static draw::graymap image(get_button_frame<false, false>().sub(1, 0, 7, 24));
      static draw::graymap image_pressed(get_button_frame<false, true>().sub(1, 0, 7, 24));
      return !pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::bottom> (bool pressed) {
      static draw::graymap image(get_button_frame<false, true>().sub(1, 4, 7, 24));
      static draw::graymap image_pressed(get_button_frame<false, false>().sub(1, 4, 7, 24));
      return !pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::left> (bool pressed) {
      static draw::graymap image(get_button_frame<true, false>().sub(0, 1, 24, 7));
      static draw::graymap image_pressed(get_button_frame<true, true>().sub(0, 1, 24, 7));
      return !pressed ? image_pressed : image;
    }

    template<>
    const draw::graymap& get_tab_frame<alignment_t::right> (bool pressed) {
      static draw::graymap image(get_button_frame<true, true>().sub(4, 1, 24, 7));
      static draw::graymap image_pressed(get_button_frame<true, false>().sub(4, 1, 24, 7));
      return !pressed ? image_pressed : image;
    }

    // --------------------------------------------------------------------------
    button_base::button_base () {
    }

    button_base::button_base (const button_base& rhs)
      : super(rhs)
    {}

    button_base::button_base (button_base&& rhs)
      : super(std::move(rhs))
    {}

    void button_base::on_clicked (button_clicked_event::function&& f) {
      on<button_clicked_event>(std::move(f));
    }

    void button_base::on_pushed (button_pushed_event::function&& f) {
      on<button_pushed_event>(std::move(f));
    }

    void button_base::on_released (button_released_event::function&& f) {
      on<button_released_event>(std::move(f));
    }

    void button_base::on_state_changed (button_state_event::function&& f) {
      on<button_state_event>(std::move(f));
    }

    // --------------------------------------------------------------------------
    void basic_button_traits::set_hilited (button_base& btn, bool h) {
      if (btn.get_state().set_hilited(h)) {
        btn.notify_event(detail::HILITE_CHANGE_MESSAGE, h);
        btn.invalidate();
      }
    }

    void basic_button_traits::set_pushed (button_base& btn, bool p) {
      if (btn.get_state().set_pushed(p)) {
        btn.notify_event(p ? detail::BN_PUSHED_MESSAGE : detail::BN_UNPUSHED_MESSAGE);
        btn.invalidate();
      }
    }

    void basic_button_traits::set_checked (button_base& btn, bool f) {
      if (btn.get_state().set_checked(f)) {
        btn.notify_event(detail::BN_STATE_MESSAGE, f ? 1 : 0);
        btn.invalidate();
      }
    }

    // --------------------------------------------------------------------------
    void push_button_traits::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          set_pushed(btn, false);
          if (btn.client_area().is_inside(pos)) {
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          set_pushed(btn, false);
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<false>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          set_pushed(btn, false);
          if (btn.client_area().is_inside(pos)) {
            set_checked(btn, !btn.is_checked());
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          set_pushed(btn, false);
          set_checked(btn, !btn.is_checked());
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    void toggle_button_traits<true>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          set_pushed(btn, false);
          if (!btn.is_checked() && btn.client_area().is_inside(pos)) {
            set_checked(btn, true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          set_pushed(btn, false);
          if (!btn.is_checked()) {
            set_checked(btn, true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
    }

    // --------------------------------------------------------------------------
    basic_animated_button_traits::basic_animated_button_traits ()
      : animation_step(1.0F)
    {}

    basic_animated_button_traits::~basic_animated_button_traits () {
      if (animation_thread.joinable()) {
        animation_thread.join();
      }
    }

    void basic_animated_button_traits::prepare_animation () {
      if (animation_thread.joinable()) {
        animation_thread.join();
      }
      animation_step = 0.0F;
    }

    void basic_animated_button_traits::start_animation (button_base& btn) {
      animation_thread = std::thread([&] () {
        while (animation_step < 1.0F) {
          animation_step += 0.1F;
          win::run_on_main([&] () {
            btn.redraw();
          });
          std::this_thread::sleep_for(std::chrono::milliseconds(25));
        }
      });
    }

    void basic_animated_button_traits::set_checked (button_base& btn, bool f) {
      prepare_animation();
      start_animation(btn);
      basic_button_traits::set_checked(btn, f);
    }

    // --------------------------------------------------------------------------
    template<>
    void animated_button_traits<false>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          set_pushed(btn, false);
          if (btn.client_area().is_inside(pos)) {
            set_checked(btn, !btn.is_checked());
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          set_pushed(btn, false);
          set_checked(btn, !btn.is_checked());
          btn.notify_event(detail::BN_CLICKED_MESSAGE);
        }
      });
    }

    // --------------------------------------------------------------------------
    template<>
    void animated_button_traits<true>::init (button_base& btn) {
      btn.on_left_btn_up([&] (os::key_state, const core::point & pos) {
        if (btn.is_pushed()) {
          set_pushed(btn, false);
          if (!btn.is_checked() && btn.client_area().is_inside(pos)) {
            set_checked(btn, true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
      btn.on_any_key_up([&] (os::key_state, os::key_symbol k) {
        if (((k == win::keys::enter) || (k == win::keys::space)) && btn.is_pushed()) {
          set_pushed(btn, false);
          if (!btn.is_checked()) {
            set_checked(btn, true);
            btn.notify_event(detail::BN_CLICKED_MESSAGE);
          }
        }
      });
    }

    // --------------------------------------------------------------------------
    namespace paint {

      const int dot_line_width = 1;
      const draw::pen::Style dot_line_style = draw::pen::Style::dot;

      // --------------------------------------------------------------------------
      void simple_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         bool hilite,
                         uint32_t horizontal,
                         uint32_t vertical) {
        graph.copy(draw::frame_image(r, hilite ? get_simple_frame().brightness(1.025F) : get_simple_frame(), horizontal, vertical), r.top_left());
      }

      // --------------------------------------------------------------------------
      void button_frame_w95 (const draw::graphics& graph,
                             const core::rectangle& r,
                             const button_state& state) {
        button_frame_w95(graph, r, state.is_enabled(), state.is_pushed(),
                         state.is_hilited(), state.has_focus());
      }

      // --------------------------------------------------------------------------
      void button_frame_w95 (const draw::graphics& graph,
                             const core::rectangle& r,
                             bool enabled, bool pushed, bool hilited, bool focused) {
        core::rectangle area = r;
        graph.fill(draw::rectangle(area), enabled && hilited ? color::buttonHighLightColor() : color::buttonColor());
        if (enabled && focused) {
          graph.frame(draw::rectangle(area), color::black);
          area.shrink({1, 1});
        }

        draw::frame::deep_relief(graph, area, pushed);

        if (enabled && focused && !pushed) {
          core::rectangle area = r;
          area.shrink({6, 6});
          graph.frame(draw::rectangle(area), draw::pen(color::light_gray, dot_line_width, dot_line_style));
        }
      }

      // --------------------------------------------------------------------------
      void button_frame (const draw::graphics& graph,
                         const core::rectangle& r,
                         const button_state& state) {
        bool enabled = state.is_enabled();
        if (enabled && state.is_hilited()) {
          graph.copy(draw::frame_image(r, get_button_frame<false, false>().brightness(1.025F), 4), r.top_left());
        } else {
          graph.copy(draw::frame_image(r, get_button_frame<false, false>(), 4), r.top_left());
        }
        bool pushed = state.is_pushed();
        if (pushed) {
          draw::frame::sunken_relief(graph, r.shrinked(core::size::two));
        }

        if (enabled && state.has_focus() && !pushed) {
          core::rectangle area = r;
          area.shrink({2, 2});
          graph.frame(draw::rectangle(area), draw::pen(color::very_light_blue, 2, draw::pen::Style::solid));
        }
      }

      // --------------------------------------------------------------------------
      void push_button (const draw::graphics& graph,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state) {
#if defined(BUILD_FOR_ARM) || defined(GUIPP_BUILD_FOR_MOBILE)
        button_frame_w95(graph, r, state);
#else
        button_frame(graph, r, state);
#endif
        using namespace draw;
        graph.text(text_box(text, r, text_origin_t::center), font::system(),
                   state.is_enabled() ? color::windowTextColor() : color::disabledTextColor());
      }

      // --------------------------------------------------------------------------
      void flat_button (const draw::graphics& g,
                        const core::rectangle& r,
                        const std::string& text,
                        const button_state& state,
                        os::color foreground,
                        os::color background) {
        bool enabled = state.is_enabled();
        os::color b = background;
        if (state.is_pushed() && enabled) {
          b = color::darker(background, 0.25);
        } else if (state.is_hilited() && enabled) {
          b = color::lighter(background, 0.25);
        }
        g.fill(draw::rectangle(r), b);

        os::color f = foreground;
        if (enabled) {
          if (state.is_pushed()) {
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
        g.text(draw::text_box(text, r, text_origin_t::center), draw::font::system(), f);
        if (enabled && state.has_focus()) {
          g.frame(draw::rectangle(r), draw::pen(f, dot_line_width, dot_line_style));
        }
      }

      // --------------------------------------------------------------------------
      void tab_button (const draw::graphics& g,
                       const core::rectangle& r,
                       const std::string& text,
                       const button_state& state,
                       os::color foreground,
                       alignment_t a) {
        if (state.is_checked()) {
          switch (a) {
            case alignment_t::top:    g.copy(draw::frame_image(r, get_tab_frame<alignment_t::top>(true), 3, 3, 3, 0), r.top_left());    break;
            case alignment_t::bottom: g.copy(draw::frame_image(r, get_tab_frame<alignment_t::bottom>(true), 3, 0, 3, 3), r.top_left()); break;
            case alignment_t::left:   g.copy(draw::frame_image(r, get_tab_frame<alignment_t::left>(true), 3, 3, 0, 3), r.top_left());   break;
            case alignment_t::right:  g.copy(draw::frame_image(r, get_tab_frame<alignment_t::right>(true), 0, 3, 3, 3), r.top_left());  break;
          }
        } else if (state.is_enabled()) {
          if (state.is_hilited()) {
            switch (a) {
              case alignment_t::top:    g.copy(draw::frame_image(r, get_tab_frame<alignment_t::top>(false).brightness(1.025F), 3, 3, 3, 0), r.top_left());    break;
              case alignment_t::bottom: g.copy(draw::frame_image(r, get_tab_frame<alignment_t::bottom>(false).brightness(1.025F), 3, 0, 3, 3), r.top_left()); break;
              case alignment_t::left:   g.copy(draw::frame_image(r, get_tab_frame<alignment_t::left>(false).brightness(1.025F), 3, 3, 0, 3), r.top_left());   break;
              case alignment_t::right:  g.copy(draw::frame_image(r, get_tab_frame<alignment_t::right>(false).brightness(1.025F), 0, 3, 3, 3), r.top_left());  break;
            }
          } else {
            switch (a) {
              case alignment_t::top:    g.copy(draw::frame_image(r, get_tab_frame<alignment_t::top>(false), 3, 3, 3, 0), r.top_left());    break;
              case alignment_t::bottom: g.copy(draw::frame_image(r, get_tab_frame<alignment_t::bottom>(false), 3, 0, 3, 3), r.top_left()); break;
              case alignment_t::left:   g.copy(draw::frame_image(r, get_tab_frame<alignment_t::left>(false), 3, 3, 0, 3), r.top_left());   break;
              case alignment_t::right:  g.copy(draw::frame_image(r, get_tab_frame<alignment_t::right>(false), 0, 3, 3, 3), r.top_left());  break;
            }
          }
        } else {
          switch (a) {
            case alignment_t::top:    g.copy(draw::frame_image(r, get_tab_frame<alignment_t::top>(false).brightness(0.75F), 3, 3, 3, 0), r.top_left());    break;
            case alignment_t::bottom: g.copy(draw::frame_image(r, get_tab_frame<alignment_t::bottom>(false).brightness(0.75F), 3, 0, 3, 3), r.top_left()); break;
            case alignment_t::left:   g.copy(draw::frame_image(r, get_tab_frame<alignment_t::left>(false).brightness(0.75F), 3, 3, 0, 3), r.top_left());   break;
            case alignment_t::right:  g.copy(draw::frame_image(r, get_tab_frame<alignment_t::right>(false).brightness(0.75F), 0, 3, 3, 3), r.top_left());  break;
          }
        }
        g.text(draw::text_box(text, r.shrinked({4, 4}), text_origin_t::center), draw::font::system(), foreground);
      }

      // --------------------------------------------------------------------------
      void switch_button (const draw::graphics& graph,
                          const core::rectangle& rect,
                          const std::string& text,
                          const button_state& state) {
        animated_switch_button(graph, rect, text, state);
      }

      // --------------------------------------------------------------------------
      void animated_switch_button (const draw::graphics& graph,
                                   const core::rectangle& rect,
                                   const std::string& text,
                                   const button_state& state,
                                   float animation_step) {
        bool enabled = state.is_enabled();
        core::size::type height = rect.height();
        core::size::type width = height * 2;
        core::size::type edge = height / 2;
        graph.fill(draw::rectangle(rect), color::buttonColor());

        float step = state.is_checked() ? animation_step : 1.0F - animation_step;

        core::rectangle switch_rect{rect.top_left(), core::size(width, height)};
        os::color thumb_col = enabled && state.is_hilited() ? color::lighter(color::buttonColor()) : color::buttonColor();
        os::color fill_col = color::merge(color::highLightColor(), color::very_light_gray, 1.0F - step);
        core::rectangle thumb = {core::point{rect.x() + height * step, rect.y()}, core::size(height)};

        graph.draw(draw::round_rectangle(switch_rect, core::size(edge)), fill_col, color::medium_gray);
        auto center = thumb.center();
        graph.draw(draw::arc(center, edge, 0, 360), thumb_col, color::medium_gray);
        graph.frame(draw::arc(center, edge - 1, 40, 250), color::white);
        graph.frame(draw::arc(center, edge - 1, 250, 40), color::medium_gray);
        graph.fill(draw::arc(center, edge - 2, 0, 360), thumb_col);

        os::color text_col = enabled ? color::windowTextColor() : color::disabledTextColor();
        graph.text(draw::text_box(text, rect + core::point(width + 10, 0), text_origin_t::vcenter_left), draw::font::system(), text_col);
      }

      // --------------------------------------------------------------------------
      void radio_button (const draw::graphics& graph,
                         const core::rectangle& rec,
                         const std::string& text,
                         const button_state& state) {
        using namespace draw;

        core::rectangle area = rec;

        os::color col = state.is_enabled() ? color::windowTextColor() : color::disabledTextColor();
        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point::type y = area.y() + area.size().height() / 2;
        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(ellipse(r), state.is_pushed() ? color::very_light_gray
                   : color::buttonColor(), col);
        if (state.is_checked()) {
          r.shrink(core::size(2, 2));
          graph.fill(ellipse(r), state.is_pushed() ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, text_origin_t::vcenter_left), font::system(), col);
        if (state.has_focus()) {
          graph.text(bounding_box(text, area, text_origin_t::vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
        }
      }

      // --------------------------------------------------------------------------
      void check_box (const draw::graphics& graph,
                      const core::rectangle& rec,
                      const std::string& text,
                      const button_state& state) {
        using namespace draw;

        core::rectangle area = rec;

        graph.fill(draw::rectangle(area), color::buttonColor());

        core::point::type y = area.y() + area.height() / 2;
        os::color col = state.is_enabled() ? color::windowTextColor() : color::disabledTextColor();

        core::rectangle r(core::point(area.x() + 1, y - 5), core::size(10, 10));
        graph.draw(rectangle(r),
                   state.is_pushed() ? color::very_light_gray
                   : color::buttonColor(),
                   col);

        if (state.is_checked()) {
          r.shrink(core::size(2, 2));
          graph.fill(rectangle(r), state.is_pushed() ? color::dark_gray : col);
        }
        area.x(20);
        graph.text(text_box(text, area, text_origin_t::vcenter_left), font::system(), col);
        if (state.has_focus()) {
          graph.text(bounding_box(text, area, text_origin_t::vcenter_left), font::system(), color::black);
          area.grow({3, 3});
          graph.frame(draw::rectangle(area), pen(color::black, dot_line_width, dot_line_style));
        }
      }

    }

  } // ctrl

} // gui
