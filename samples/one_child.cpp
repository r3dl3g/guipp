

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/label.h>
#include <gui/draw/pen.h>
#include <gui/draw/brush.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::ctrl;

  layout_main_window<gui::layout::border::layouter<40, 10, 10, 10>> main;

  using my_label = basic_label<text_origin_t::center,
                               draw::frame::raised_relief,
                               color::red,
                               color::light_gray>;

  my_label label;
  client_window<color::white> drawing;

  drawing.on_paint(draw::paint([&] (draw::graphics& g) {
    g.draw(draw::rectangle(drawing.client_geometry()/*.shrinked({5, 5})*/), color::white, color::black);
    g.frame(draw::pie(drawing.client_geometry().shrinked({10, 10}), -45, 45), color::blue);
    g.frame(draw::arc(drawing.client_geometry().shrinked({15, 15}), -45, 45), color::green);
    g.frame(draw::pie(drawing.client_geometry().center(), 70, 45, 315), color::red);
  }));

  drawing.on_left_btn_dblclk([&] (os::key_state, const core::native_point&) {
    label.enable(!label.is_enabled());
    label.invalidate();
  });

  main.on_create([&] () {
    clog::info() << "main.on_create";
    label.create(main, "I'm the Child", {10, 10, 200, 20});
    drawing.create(main, {10, 40, 200, 200});
    label.set_visible();
  });

  main.get_layout().set_center(layout::lay(drawing));
  main.get_layout().set_top(layout::lay(label));
  main.create({50, 50, 800, 600});

  main.on_destroy(&quit_main_loop);
  main.set_title("One Child");
  main.set_visible();

  return run_main_loop();
}
