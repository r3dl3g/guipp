
#include <gui/win/container.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/layout/weighted_layout.h>
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

typedef basic_label<text_origin_t::center,
                    draw::frame::raised_deep_relief,
                    color::black,
                    color::very_light_gray> label_t;

template<border::type_t T>
struct layout_group {

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

  layout_main_window<border::layouter<100, 100, 100, 100, border::type_t::all_symmetric>> main;

  label_t bottom_labels[5];
  horizontal_adaption<0, 2> bottom({lay(bottom_labels[0]), lay(bottom_labels[1]), lay(bottom_labels[2]), lay(bottom_labels[3]), lay(bottom_labels[4])});

  label_t top_labels[5];
  horizontal_lineup<200, 5, 10, 0, gui::origin_t::center> top({lay(top_labels[0]), lay(top_labels[1]), lay(top_labels[2]), lay(top_labels[3]), lay(top_labels[4])});

  label_t right_labels[5];
  grid_lineup<100, 50, 5, 10> right({lay(right_labels[0]), lay(right_labels[1]), lay(right_labels[2]), lay(right_labels[3]), lay(right_labels[4])});

  label_t left_labels[5];
  grid_adaption<2, 4, 5, 10> left({lay(left_labels[0]), lay(left_labels[1]), lay(left_labels[2]), lay(left_labels[3]), lay(left_labels[4])});

  layout_group<border::type_t::top_bottom_maximize> center0;
  layout_group<border::type_t::left_right_maximize> center1;
  layout_group<border::type_t::bottom_max_top_min> center2;
  layout_group<border::type_t::top_max_bottom_min> center3;
  layout_group<border::type_t::left_max_right_min> center4;
  layout_group<border::type_t::right_max_left_min> center5;

  label_t h_labels[4];
  label_t v_labels[4];

  horizontal_weighted<> center6;
  vertical_weighted<> center7;

  grid_adaption<2, 4, 5, 5> center({lay(center0.layouter), lay(center1.layouter), lay(center2.layouter), lay(center3.layouter),
                                    lay(center4.layouter), lay(center5.layouter), lay(center6), lay(center7)});

  main.get_layout().set_center(lay(center));
  main.get_layout().set_top(lay(top));
  main.get_layout().set_bottom(lay(bottom));
  main.get_layout().set_left(lay(left));
  main.get_layout().set_right(lay(right));

  main.on_create([&] (window* parent, const core::rectangle& rect) {
    int i = 0;
    for (auto& l : top_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Top " << ++i));
    }
    for (auto& l : bottom_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Bottom " << ++i));
    }
    for (auto& l : left_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Left " << ++i));
    }
    for (auto& l : right_labels) {
      l.create(main);
      l.set_text(ostreamfmt("Right " << ++i));
    }
    for (auto& l : h_labels) {
      l.create(main);
      l.set_text(ostreamfmt("H " << ++i));
    }
    for (auto& l : v_labels) {
      l.create(main);
      l.set_text(ostreamfmt("V " << ++i));
    }
    center6.add({{lay(h_labels[0]), 100, 0.0F}, {lay(h_labels[1]), 0, 1.0F}, {lay(h_labels[2]), 100, 0.0F}, {lay(h_labels[3]), 20, 0.5F}});
    center7.add({{lay(v_labels[0]), 20, 0.0F}, {lay(v_labels[1]), 0, 1.0F}, {lay(v_labels[2]), 20, 0.0F}, {lay(v_labels[3]), 10, 0.5F}});

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

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello New Border Layout");
  main.set_visible();

  return run_main_loop();
}
