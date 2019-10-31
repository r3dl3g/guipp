
#include <gui/win/container.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<border_layout<>, float, float, float, float> main(100, 100, 100, 20);
  label_center first, second, third;
  horizontal_adaption<0, 2> client({win(first), lay(vertical_adaption<0, 2>({win(second), win(third)}))});

  label_center top_labels[5];
  horizontal_lineup<200, 5, 10, 0, gui::origin::center> top({win(top_labels[0]), win(top_labels[1]), win(top_labels[2]), win(top_labels[3]), win(top_labels[4])});

  label_center bottom_labels[5];
  grid_lineup<100, 50, 5, 10> bottom({win(bottom_labels[0]), win(bottom_labels[1]), win(bottom_labels[2]), win(bottom_labels[3]), win(bottom_labels[4])});

  label_center left_labels[5];
  grid_adaption<2, 4, 5, 10> left({win(left_labels[0]), win(left_labels[1]), win(left_labels[2]), win(left_labels[3]), win(left_labels[4])});

  main.get_layout().set_center(lay(client));
  main.get_layout().set_top(lay(top));
  main.get_layout().set_bottom(lay(bottom));
  main.get_layout().set_left(lay(left));

  main.on_create([&] (window* parent, const rectangle& rect) {
    first.create(main);
    second.create(main);
    third.create(main);
    first.set_text([&] () { return ostreamfmt(first.get_id() << " (" << first.place() << ")"); });
    second.set_text([&] () { return ostreamfmt(second.get_id() << " (" << second.place() << ")"); });
    third.set_text([&] () { return ostreamfmt(third.get_id() << " (" << third.place() << ")"); });
    int i = 0;
    for (label_center& l : top_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Top " << ++i));
    }
    for (label_center& l : bottom_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Bottom " << ++i));
    }
    for (label_center& l : left_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Left " << ++i));
    }
  });
  main.on_lost_focus([&] (window* next) {
    main.set_title(ostreamfmt("Lost focus to: " << (next ? next->get_id() : 0)));
  });
  main.on_set_focus([&] (window* prev) {
    main.set_title(ostreamfmt("Got focus from: " << (prev ? prev->get_id() : 0)));
  });
//  main.on_paint(gui::draw::paint([&] (const gui::draw::graphics& g) {
//    gui::ctrl::paint::text_item(g, main.client_area(), gui::color::very_light_gray, "Hello world", false, gui::text_origin::center);
//  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello New Border Layout");
  main.set_visible();

  return run_main_loop();
}
