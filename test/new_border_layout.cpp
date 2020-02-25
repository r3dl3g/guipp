
#include <gui/win/container.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/textbox.h>
#include <logging/core.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::ctrl;
using namespace gui::core;
using namespace gui::draw;

template<border::type T>
struct layout_group {
  typedef basic_label<text_origin::center,
                      draw::frame::raised_deep_relief,
                      color::black,
                      color::very_light_gray> label_t;

  label_t top, bottom, left, right, center;
  border::layouter<50, 50, 150, 150, T> layouter;

  void create (container& main) {
    top.create(main);
    bottom.create(main);
    left.create(main);
    right.create(main);
    center.create(main);
    top.set_text([&] () { return ostreamfmt("T (" << top.place() << ")"); });
    bottom.set_text([&] () { return ostreamfmt("B (" << bottom.place() << ")"); });
    left.set_text([&] () { return ostreamfmt("L (" << left.place() << ")"); });
    right.set_text([&] () { return ostreamfmt("R (" << right.place() << ")"); });
    center.set_text([&] () { return ostreamfmt("C (" << center.place() << ")"); });
    layouter.set_center_top_bottom_left_right(lay(center), lay(top), lay(bottom), lay(left), lay(right));
  }
};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  layout_main_window<border::layouter<100, 100, 100, 100, border::type::all_symmetric>> main;

  label_center bottom_labels[5];
  horizontal_adaption<0, 2> bottom({lay(bottom_labels[0]), lay(bottom_labels[1]), lay(bottom_labels[2]), lay(bottom_labels[3]), lay(bottom_labels[4])});

  label_center top_labels[5];
  horizontal_lineup<200, 5, 10, 0, gui::origin::center> top({lay(top_labels[0]), lay(top_labels[1]), lay(top_labels[2]), lay(top_labels[3]), lay(top_labels[4])});

  label_center right_labels[5];
  grid_lineup<100, 50, 5, 10> right({lay(right_labels[0]), lay(right_labels[1]), lay(right_labels[2]), lay(right_labels[3]), lay(right_labels[4])});

  label_center left_labels[5];
  grid_adaption<2, 4, 5, 10> left({lay(left_labels[0]), lay(left_labels[1]), lay(left_labels[2]), lay(left_labels[3]), lay(left_labels[4])});

  layout_group<border::type::top_bottom_maximize> center0;
  layout_group<border::type::left_right_maximize> center1;
  layout_group<border::type::bottom_max_top_min> center2;
  layout_group<border::type::top_max_bottom_min> center3;
  layout_group<border::type::left_max_right_min> center4;
  layout_group<border::type::right_max_left_min> center5;

  grid_adaption<2, 3, 5, 5> center({lay(center0.layouter), lay(center1.layouter), lay(center2.layouter), lay(center3.layouter), lay(center4.layouter), lay(center5.layouter)});

  main.get_layout().set_center(lay(center));
  main.get_layout().set_top(lay(top));
  main.get_layout().set_bottom(lay(bottom));
  main.get_layout().set_left(lay(left));
  main.get_layout().set_right(lay(right));

  main.on_create([&] (window* parent, const core::rectangle& rect) {
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
    for (label_center& l : right_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Right " << ++i));
    }
    center0.create(main);
    center1.create(main);
    center2.create(main);
    center3.create(main);
    center4.create(main);
    center5.create(main);
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
