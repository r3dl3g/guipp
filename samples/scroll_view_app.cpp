
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/scroll_view.h>

#include <gui/draw/pen.h>
#include <gui/draw/brush.h>
#include <gui/draw/font.h>

using namespace gui;

ctrl::paint_function create_paint1 (const win::window&, const bool& draw_invert, const core::angle& start, const core::angle& end);
ctrl::paint_function create_paint2 (const win::window&, const bool& draw_invert, const core::angle& start, const core::angle& end);

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<border::layouter</*50, 50, 50, 50*/>> main;
  ctrl::scroll_view<color::very_light_gray> scroll_view;
  ctrl::client_control<> window1;
  ctrl::client_control<> window2;

  bool at_paint1 = true;
  bool at_drag = false;
  bool draw_invert = false;
  core::native_point last_pos;
  core::angle start_angle(15), end_angle(245);

  const ctrl::paint_function paint1 = create_paint1(window1, draw_invert, start_angle, end_angle);
  const ctrl::paint_function paint2 = create_paint2(window1, draw_invert, start_angle, end_angle);

  main.get_layout().set_center(lay(scroll_view));

  window1.on_left_btn_down([&](gui::os::key_state, const core::native_point& p) {
    window1.to_front();
    at_drag = true;
    last_pos = p;
    window1.capture_pointer();
  });
  window1.on_left_btn_up([&](gui::os::key_state, const core::native_point& p) {
    window1.uncapture_pointer();
    at_drag = false;
  });
  window1.on_mouse_move([&] (gui::os::key_state, const core::native_point& p) {
    if (at_drag) {
      auto delta = p - last_pos;
      last_pos = p;
      window1.position(window1.position() + core::global::scale_from_native(delta));
      scroll_view.invalidate();
    }
  });

  window1.on_paint(draw::paint(paint1));
  window1.on_left_btn_dblclk([&] (gui::os::key_state, const core::native_point& p) {
    if (at_paint1) {
      at_paint1 = false;
      window1.unregister_event_handler<win::paint_event>(draw::paint(paint1));
      window1.on_paint(draw::paint(paint2));
    } else {
      at_paint1 = true;
      window1.unregister_event_handler<win::paint_event>(draw::paint(paint2));
      window1.on_paint(draw::paint(paint1));
    }
    window1.invalidate();
  });

  window1.on_wheel<orientation_t::horizontal>([&] (core::native_point::type delta,
                                                   const core::native_point& p) {
    if (window1.surface_geometry().is_inside(p)) {
      end_angle += angle(delta);
      window1.invalidate();
    }
  });
  window1.on_wheel<orientation_t::vertical>([&] (core::native_point::type delta,
                                                 const core::native_point& p) {
    if (window1.surface_geometry().is_inside(p)) {
      start_angle += angle(delta);
      window1.invalidate();
    }
  });

  window2.on_paint(draw::paint([&] (draw::graphics& graph) {
    graph.fill(draw::rectangle(window2.client_geometry()), color::gray);
  }));
  window2.on_left_btn_down([&] (gui::os::key_state, const core::native_point& p) {
    window2.to_front();
    scroll_view.invalidate();
  });
  window2.on_left_btn_dblclk([&] (gui::os::key_state, const core::native_point& p) {
    draw_invert = !draw_invert;
    scroll_view.invalidate();
  });

  main.on_create([&] () {
    scroll_view.create(main);
    window1.create(scroll_view, core::rectangle(10, 10, 200, 280));
    window2.create(scroll_view, core::rectangle(220, 10, 200, 280));
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Sroll view");
  main.set_visible();
  main.layout();

  return run_main_loop();
}

std::vector<core::point> calc_star (const core::point& pt, const core::angle& a) {
  const core::point::type r = 20;
  const core::point::type x = pt.x();
  const core::point::type y = pt.y();

  const auto ca = cos(a.rad());
  const auto sa = sin(a.rad());
  const auto ca2 = cos(a.rad() + M_PI / 4);
  const auto sa2 = sin(a.rad() + M_PI / 4);

  core::point::type rca1 = r * ca;
  core::point::type rsa1 = r * sa;
  core::point::type rca2 = ca2 * r / 3;
  core::point::type rsa2 = sa2 * r / 3;
  return {
        core::point(x - rsa1, y - rca1),
        core::point(x + rca2, y - rsa2),
        core::point(x + rca1, y - rsa1),
        core::point(x + rsa2, y + rca2),
        core::point(x + rsa1, y + rca1),
        core::point(x - rca2, y + rsa2),
        core::point(x - rca1, y + rsa1),
        core::point(x - rsa2, y - rca2),
  };
}

ctrl::paint_function create_paint1 (const win::window& win, const bool& draw_invert, const core::angle& start, const core::angle& end) {
  return [&] (draw::graphics& graph) {

    using namespace gui;
    using namespace gui::draw;

    auto area = win.client_geometry();
    auto pos = area.top_left();
    clip clp(graph, area);

    graph.fill(rectangle(area), color::very_very_light_gray);

    graph.frame(polygon(calc_star(pos + core::point{30, 30}, start)), color::blue);
    graph.fill(polygon(calc_star(pos + core::point{80, 30}, start)), color::dark_green);
    graph.draw(polygon(calc_star(pos + core::point{130, 30}, start)), color::yellow, color::red);

    graph.frame(polygon({ { 5, 5 }, { 155, 5 }, {75, 8} }), color::red);

    core::point pt = pos + core::point(10, 60);
    graph.fill(pie(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 1!", pt), font::system(), color::black);

    pt.move_y(15);
    graph.fill(pie(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 2!", pt), font::system_bold(), color::black);

    pt.move_y(15);
    graph.fill(pie(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 3!", pt), font::sans_serif().with_italic(true), color::black);

    pt.move_y(15);
    graph.fill(pie(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 4!", pt), font::serif(), color::black);

    pt.move_y(15);
    graph.fill(pie(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 5!", pt), font::monospace(), color::black);

    pt.move_y(15);
    graph.fill(pie(pt, 2, 0, 360), color::blue);
    graph.text(text("Hello World 6!", pt), font::sans_serif().with_size(18), color::blue);

    auto draw_text_box = [](draw::graphics& g,
                            const std::string& t,
                            core::rectangle r,
                            text_origin_t o) {
      g.frame(rectangle(r), color::blue);
      core::rectangle rb = r;
      g.text(bounding_box(t, rb, o), font::system(), color::red);
      g.frame(rectangle(rb), color::dark_green);
      g.text(text_box(t, r, o), font::system(), color::red);
    };

    auto draw_text = [](draw::graphics& g,
      const std::string& t,
      const core::point& p,
      text_origin_t o) {
      g.fill(pie(p, 2, 0, 360), color::blue);
      g.text(text(t, p, o), font::system(), color::red);
    };

    std::string texte[] = {
      "TL", "TC", "TR",
      "CL", "CC", "CR",
      "BL", "BC", "BR"
    };
    text_origin_t origins[] = {
      text_origin_t::top_left, text_origin_t::top_hcenter, text_origin_t::top_right,
      text_origin_t::vcenter_left, text_origin_t::center, text_origin_t::vcenter_right,
      text_origin_t::bottom_left, text_origin_t::bottom_hcenter, text_origin_t::bottom_right
    };

    for (int y = 0; y < 3; ++y) {
      for (int x = 0; x < 3; ++x) {
        int i = y * 3 + x;
        draw_text_box(graph, texte[i], { pos + core::point(10 + x * 55, 155 + y * 25), core::size(50, 22) }, origins[i]);
        draw_text(graph, texte[i], pos + core::point(10 + x * 80, 235 + y * 20), origins[i]);
      }
    }

    if (draw_invert) {
      graph.invert({ pos + core::point{25, 25}, core::size{150, 150}});
    }

  };
}

ctrl::paint_function create_paint2 (const win::window& win, const bool& draw_invert, const core::angle& start, const core::angle& end) {
  return [&](draw::graphics& graph) {

    using namespace gui;
    using namespace gui::draw;

    auto area = win.client_geometry();
    auto pos = area.top_left();
    clip clp(graph, area);
    graph.fill(rectangle(area), color::white);

    graph.draw_pixel(core::native_point(pos.os_x(graph.context()) + 3, pos.os_y(graph.context()) + 3), color::gray);
    graph.draw_pixel(core::native_point(pos.os_x(graph.context()) + 6, pos.os_y(graph.context()) + 6), color::gray);

    core::size sz(30, 50);
    core::size offs1(0, 60);
    core::size offs2(0, 120);

    core::point pos1 = pos + core::point(10, 10);
    graph.frame(rectangle(pos1, sz), color::blue);
    graph.fill(rectangle(pos1 + offs1, sz), color::dark_green);
    graph.draw(rectangle(pos1 + offs2, sz), color::red, color::cyan);

    core::point pos2 = pos + core::point(50, 10);
    graph.frame(ellipse(pos2, sz), color::blue);
    graph.fill(ellipse(pos2 + offs1, sz), color::dark_green);
    graph.draw(ellipse(pos2 + offs2, sz), color::red, color::cyan);

    core::point pos3 = pos + core::point(90, 10);
    core::size rd(10, 10);
    graph.frame(round_rectangle(core::rectangle(pos3, sz), rd), color::blue);
    graph.fill(round_rectangle(core::rectangle(pos3 + offs1, sz), rd), color::dark_green);
    graph.draw(round_rectangle(core::rectangle(pos3 + offs2, sz), rd), color::red, color::cyan);

    core::point pos4 = pos + core::point(150, 30);
    graph.frame(pie(pos4, 20, start, end), color::blue);
    graph.fill(pie(pos4 + offs1, 20, start, end), color::dark_green);
    graph.draw(pie(pos4 + offs2, 20, start, end), color::red, color::cyan);

    //color cyan = color::cyan;
    //color cyan_trans = cyan.transparency(0.5);
    //graph.fill(rectangle(core::rectangle(pos1 + core::size(20, 30), core::size(100, 120))), cyan_trans);

    if (draw_invert) {
      graph.invert({ pos + core::point{25, 25}, core::size{150, 150}});
    }

  };
}
