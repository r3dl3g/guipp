

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/ctrl/label.h>
#include <gui/draw/pen.h>


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::ctrl;

  layout_main_window<gui::layout::border::layouter<40, 10, 10, 10>> main;

  using my_label = basic_label<text_origin_t::vcenter_left,
                               draw::frame::sunken_relief,
                               color::red,
                               color::gray>;

  my_label label;
  client_window<color::white> drawing;

  drawing.on_paint(draw::paint([&] (const draw::graphics& g) {
    g.frame(draw::rectangle(drawing.client_area().shrinked({5, 5})), color::black);
    g.frame(draw::pie(drawing.client_area().shrinked({10, 10}), -45, 45), color::blue);
    g.frame(draw::arc(drawing.client_area().shrinked({15, 15}), -45, 45), color::green);
    g.frame(draw::pie(drawing.client_area().center(), 70, 45, 315), color::red);
  }));

  main.get_layout().set_center(layout::lay(drawing));
  main.get_layout().set_top(layout::lay(label));
  main.create({50, 50, 800, 600});
  label.create(main, "I'm the Child", {10, 10, 200, 20});
  drawing.create(main, {10, 40, 200, 200});

  main.on_destroy(&quit_main_loop);
  main.set_title("One Child");
  main.set_visible();
  label.set_visible();

  return run_main_loop();
}
