
#include <gui/win/auto_refresh_window.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/layout_container.h>
#include <gui/ctrl/tab_view.h>
#include <gui/ctrl/label.h>
#include <gui/draw/clock.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>
#include <gui/draw/use.h>
#include <util/string_util.h>
#include <util/time_util.h>


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::ctrl;
using namespace gui::core;

// --------------------------------------------------------------------------
double round_to_step (double value, double step) {
  return std::round(value / step) * step;
}

// --------------------------------------------------------------------------
std::pair<point, size::type> get_center (const rectangle& r) {
  const auto square = r.squared();
  if (square.width() > 10) {
     return {{r.center_x(), square.center_y()}, square.width() * 0.45};
  }
  return {};
}
// --------------------------------------------------------------------------
struct clockview : public auto_refresh_window<1000> {
  clockview () {
    on_paint(draw::paint([&] (draw::graphics& g) {
      const auto c = get_center(client_geometry());
      if (c.second > 10) {
         const auto now = util::time::local_time_now();
         g.text(draw::clock_face(c.first, c.second), draw::font::system_bold(), color::white);
         g.frame(draw::clock_hands(c.first, c.second, now.tm_hour, now.tm_min, now.tm_sec), color::white);
      }
    }));
  }

};
// --------------------------------------------------------------------------
struct chronometerview :  public auto_refresh_window<100> {
  typedef auto_refresh_window<100> super;
  typedef util::time::chronometer chronometer_t;

  chronometerview ()
    : previous(chronometer_t::duration::zero())
    , chrono_active(false)
  {}

  chronometerview (background_repeater::action_t action)
    : super(action)
    , previous(chronometer_t::duration::zero())
    , chrono_active(false)
  {}

  void start () {
    chronometer.start();
    chrono_active = true;
    start_auto_refresh();
  }

  void stop () {
    chrono_active = false;
    previous += chronometer.stop();
    stop_auto_refresh();
  }

  void toggle () {
    if (chrono_active) {
      stop();
    } else {
      start();
    }
  }

  void reset () {
    chrono_active = false;
    previous = std::chrono::milliseconds::zero();
    stop_auto_refresh();
  }

  bool active () const {
    return chrono_active;
  }

  bool can_reset () const {
    return duration().count() > 0;
  }

  std::chrono::milliseconds duration () const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(previous + (chrono_active ? chronometer.stop() : chronometer_t::duration::zero()));
  }

private:
  chronometer_t chronometer;
  chronometer_t::duration previous;
  bool chrono_active;

};
// --------------------------------------------------------------------------
struct stopwatchview : public chronometerview {
  typedef chronometerview super;

  stopwatchview () {
    on_paint(draw::paint([&] (draw::graphics& g) {
      auto c = get_center(client_geometry());
      if (c.second > 10) {
        const std::chrono::milliseconds d = duration();

        const auto min_center = c.first.dy(-c.second * 0.40);
        g.text(draw::stopwatch_seconds_face(c.first, c.second), draw::font::system(), color::white);
        g.text(draw::stopwatch_minutes_face(min_center, c.second * 0.3), draw::font::system(), color::white);
        g.text(draw::stopwatch_digits(c.first.dy(c.second * 0.4), c.second, d), draw::font::system(), color::white);

        draw::pen pn(color::orange, 3, draw::pen::Style::solid, draw::pen::Cap::round);
        g.frame(draw::stopwatch_minutes_hands(min_center, c.second * 0.3, d), pn);
        g.frame(draw::stopwatch_seconds_hands(c.first, c.second, d), pn);
      }
    }));
  }

};
// --------------------------------------------------------------------------
struct timerview : public chronometerview {
  typedef chronometerview super;

  timerview ()
    : super([&] () { refresh(); })
    , countdown(std::chrono::seconds(90))
  {
    on_paint(draw::paint([&] (draw::graphics& g) {
      auto c = get_center(client_geometry());
      if (c.second > 10) {
        const auto r0 = c.second * 0.9;
        const auto r1 = c.second * 0.4;
        const auto step = 360.0F / 60.0F / 5.0F;
        const auto d = duration();
        if (d > countdown) {
          if (active()) {
            g.frame(draw::centerlines(c.first, r0, c.second, step, 0, 360.0), draw::pen(color::dark_green, 2));
            g.text(draw::stopwatch_digits(c.first.dy(r1), c.second, std::chrono::milliseconds::zero()),
                   draw::font::system(), color::white);
          } else {
            g.frame(draw::centerlines(c.first, r0, c.second, step, 0, 360.0), draw::pen(color::dark_gray, 2));
            g.text(draw::stopwatch_digits(c.first.dy(r1), c.second, countdown),
                   draw::font::system(), color::white);
          }
        } else {
          const auto pos = round_to_step(360.0 * d / countdown, step);
          g.frame(draw::centerlines(c.first, r0, c.second, step, 0, pos), draw::pen(color::dark_gray, 2));
          g.frame(draw::centerlines(c.first, r0, c.second, step, pos, 360.0), draw::pen(color::orange, 2));
          g.text(draw::stopwatch_digits(c.first.dy(r1), c.second, (countdown - d)),
                 draw::font::system(), color::white);
        }
      }
    }));
  }

  void refresh () {
    if (duration() > countdown) {
      stop_auto_refresh();
    }
    invalidate();
  }

  void set_countdown (int sec) {
    countdown = std::chrono::seconds(sec);
  }

private:
  std::chrono::seconds countdown;
};
// --------------------------------------------------------------------------
struct stopwatch_layout {

  stopwatch_layout (window& view, window& btn1, window& btn2)
    : view(view)
    , btn1(btn1)
    , btn2(btn2)
  {}

  void layout (const gui::core::rectangle& sz) {
    auto square = sz.squared();
    const point center{sz.center_x(), square.center_y()};
    square.set_center(center);

    const bool portrait = sz.width() < sz.height();

    core::rectangle left, right;
    if (portrait) {
      left = {core::point{sz.x(), square.y2()}, core::point{sz.center_x(), sz.y2()}};
      right = {left.top_right(), sz.bottom_right()};
    } else {
      left = {sz.top_left(), square.bottom_left()};
      right = {square.top_right(), sz.bottom_right()};
    }

    left *= 0.75;
    right *= 0.75;

    view.geometry(square);
    btn1.geometry(left);
    btn2.geometry(right);
  }

private:
  window& view;
  window& btn1;
  window& btn2;
};
// --------------------------------------------------------------------------
size::type roundness (const core::size& sz) {
  return (sz.height() + sz.width()) / 20;
}
// --------------------------------------------------------------------------
size::type icon_size (const core::size& sz) {
  return std::min(sz.height(), sz.width()) / 3;
}
// --------------------------------------------------------------------------
template<typename T>
struct chronometer_page : group_window<stopwatch_layout, color::black, window&, window&, window&> {
  typedef group_window<stopwatch_layout, color::black, window&, window&, window&> super;
  typedef custom_push_button button_t;
  typedef T page_t;

  chronometer_page ()
    : super(page, reset_btn, start_stop_btn)
  {
    on_create([&] () {
      page.create(*this);
      reset_btn.create(*this);
      start_stop_btn.create(*this);
    });
    reset_btn.set_drawer([&] (draw::graphics& g,
                              const core::rectangle& r,
                              const core::button_state::is& s) {
      if (r.width() > 8) {
        const auto col = page.can_reset() ? color::gray : color::very_dark_gray;
        draw::pen pn(col, 3, draw::pen::Style::solid, draw::pen::Cap::round);
        g.draw(draw::round_rectangle(r, roundness(r.size())), color::very_very_dark_gray, pn.with_size(1));
        g.frame(draw::icon<draw::icon_t::reset>(r.center(), icon_size(r.size())), pn);
      }
    });
    start_stop_btn.set_drawer([&] (draw::graphics& g,
                              const core::rectangle& r,
                              const core::button_state::is& s) {
      if (r.width() > 8) {
        const auto col = page.active() ? color::dark_red : color::dark_green;
        draw::pen pn(col, 3, draw::pen::Style::solid, draw::pen::Cap::round);
        g.draw(draw::round_rectangle(r, roundness(r.size())), color::darker(col, 0.85), pn.with_size(1));
        if (page.active()) {
          g.frame(draw::icon<draw::icon_t::stop>(r.center(), icon_size(r.size())), pn);
        } else if (page.can_reset()) {
          g.frame(draw::icon<draw::icon_t::pause>(r.center(), icon_size(r.size())), pn);
        } else {
          g.frame(draw::icon<draw::icon_t::play>(r.center(), icon_size(r.size())), pn);
        }
      }
    });
    reset_btn.on_clicked([&] () {
      page.reset();
      start_stop_btn.invalidate();
    });
    start_stop_btn.on_clicked([&] () {
      page.toggle();
      reset_btn.invalidate();
    });
  }

  void activate (bool on) {
    if (on && page.active()) {
      page.start_auto_refresh();
    } else {
      page.stop_auto_refresh();
    }
  }

  page_t page;
  button_t reset_btn;
  button_t start_stop_btn;
};
// --------------------------------------------------------------------------
template<draw::icon_t I, gui::os::color F = color::dark_gray, gui::os::color B = color::very_very_dark_gray>
struct icon_push_button : public custom_push_button {
  typedef custom_push_button super;

  icon_push_button () {
    super::set_drawer([&] (draw::graphics& g,
                      const core::rectangle& r,
                      const core::button_state::is&) {
      g.erase(r, B);
      g.frame(draw::icon<I>(r.center(), r.max_radius() / 2), F);
    });
  }
};
// --------------------------------------------------------------------------
struct icon_toggle_button : public custom_toggle_button<true> {
  typedef custom_toggle_button<true> super;

  icon_toggle_button (const text_source&)
  {}
};
// --------------------------------------------------------------------------
struct digit_label : public client_window<color::black> {

  digit_label (const std::function<int()> value) {
    on_paint(draw::paint([&, value] (draw::graphics& g) {
      const auto geo = client_geometry();
      draw::font fnt = draw::font::system().with_size(geo.width() / 2);
      g.text(draw::text_box(ostreamfmt(std::setfill('0') << std::setw(2) << value()), geo, text_origin_t::center), fnt, color::white);
    }));
  }

};

const int max_seconds = 100*60 - 1;

// --------------------------------------------------------------------------
struct timerview_page : public chronometer_page<timerview> {
  typedef chronometer_page<timerview> super;

  timerview_page ()
    : last_seconds(90)
    , seconds(90)
    , seconds_label([&] () { return seconds % 60; })
    , minutes_label([&] () { return seconds / 60; })
  {
    inc_minutes.on_clicked([&] () {
      seconds = std::min(seconds + 60, max_seconds);
      update();
    });
    dec_minutes.on_clicked([&] () {
      seconds = std::max(seconds - 60, 0);
      update();
    });
    inc_seconds.on_clicked([&] () {
      seconds = std::min(seconds + 1, max_seconds);
      update();
    });
    dec_seconds.on_clicked([&] () {
      seconds = std::max(seconds - 1, 0);
      update();
    });
    on_create([&] () {
      inc_minutes.create(*this);
      minutes_label.create(*this);
      dec_minutes.create(*this);
      inc_seconds.create(*this);
      seconds_label.create(*this);
      dec_seconds.create(*this);
    });
    on_layout([&] (const core::rectangle& r) {
      const auto geo = page.geometry() * 0.125;
      inc_minutes.geometry(geo + core::point{-geo.width() / 1.8F, -geo.height() * 2});
      minutes_label.geometry(geo + core::point{-geo.width() / 1.8F, -geo.height()});
      dec_minutes.geometry(geo + core::point{-geo.width() / 1.8F, 0});
      inc_seconds.geometry(geo + core::point{geo.width() / 1.8F, -geo.height() * 2});
      seconds_label.geometry(geo + core::point{geo.width() / 1.8F, -geo.height()});
      dec_seconds.geometry(geo + core::point{geo.width() / 1.8F, 0});
    });
    super::page.on_left_btn_down([&] (gui::os::key_state, const core::native_point& pt) {
      last_mouse_position = pt;
      last_seconds = seconds;
    });
    super::page.on_mouse_move([&] (gui::os::key_state st, const core::native_point& pt) {
      if (core::state::left_button == st) {
        const int delta = core::global::scale_from_native<int>(last_mouse_position.y() - pt.y()) / 5;
        const auto geo = page.surface_geometry();
        const int step = (pt.x() > geo.center_x() ? 1 : 60);
        seconds = std::min(std::max(last_seconds + step * delta, 0), max_seconds);
        update();
      }
    });
  }

  void update () {
    super::page.set_countdown(seconds);
    invalidate();
  }

  int seconds;
  icon_push_button<draw::icon_t::up> inc_minutes;
  icon_push_button<draw::icon_t::down> dec_minutes;
  icon_push_button<draw::icon_t::up> inc_seconds;
  icon_push_button<draw::icon_t::down> dec_seconds;
  digit_label minutes_label;
  digit_label seconds_label;

  core::native_point last_mouse_position;
  int last_seconds;
};
// --------------------------------------------------------------------------
template<typename T>
void icon_drawer (draw::graphics& g,
                  const core::rectangle& r,
                  const core::button_state::is& s) {
  static draw::pen icon_pen(color::white, 3, draw::pen::Style::solid, draw::pen::Cap::round);
  g.erase(r, color::very_very_dark_gray);
  g.frame(T(r.center(), r.height() / 4), icon_pen.with_color(s.checked() ? color::orange : color::dark_gray));
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  typedef icon_toggle_button button_t;
  typedef htoggle_group<color::white, color::black, button_t, layout::horizontal_adaption<>> tab_group_type;
  typedef tab_view<alignment_t::bottom, color::black, tab_group_type, layout::split_layout<alignment_t::bottom, 80>> tabs_t;

  tabs_t tabs;
  clockview page_0;
  chronometer_page<stopwatchview> page_1;
  timerview_page page_2;
  layout_main_window<border::zero_layout<tabs_t>, tabs_t&> main(tabs);

  tabs.add_page("Clock", page_0);
  tabs.add_page("Stoppwatch", page_1);
  tabs.add_page("Timer", page_2);

  tabs.get_buttons().get_button(0)->set_drawer(icon_drawer<draw::icon<draw::icon_t::clock>>);
  tabs.get_buttons().get_button(1)->set_drawer(icon_drawer<draw::icon<draw::icon_t::stopwatch>>);
  tabs.get_buttons().get_button(2)->set_drawer(icon_drawer<draw::icon<draw::icon_t::timer>>);

  main.on_create([&] () {
    tabs.create(main);
    page_0.create(tabs);
    page_1.create(tabs);
    page_2.create(tabs);

    tabs.show_page(0);
  });

  tabs.on_selection_changed([&] (event_source ) {
    const auto idx = tabs.get_selection_index();
    page_0.activate(idx == 0);
    page_1.activate(idx == 1);
    page_2.activate(idx == 2);
  });

  main.create({0, 0, 600, 800});
  main.on_destroy(&quit_main_loop);
  main.set_title("clock");
  main.set_visible();
  page_0.start_auto_refresh();

  return run_main_loop();
}
