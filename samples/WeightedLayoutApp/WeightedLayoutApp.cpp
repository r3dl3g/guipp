
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/layout/weighted_layout.h>
#include <gui/ctrl/label.h>


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::ctrl;
using namespace gui::core;
using namespace gui::draw;

typedef basic_label<text_origin_t::center,
                    draw::frame::raised_deep_relief> label_t;

template<border::type_t T>
struct layout_group {

  label_t top, bottom, left, right, center;
  border::layouter<25, 25, 100, 100, T> layouter;

  void create (container& main) {
    top.create(main);
    bottom.create(main);
    left.create(main);
    right.create(main);
    center.create(main);
    top.set_text([&] () { return ostreamfmt("T (" << top.geometry() << ")"); });
    bottom.set_text([&] () { return ostreamfmt("B (" << bottom.geometry() << ")"); });
    left.set_text([&] () { return ostreamfmt("L (" << left.geometry() << ")"); });
    right.set_text([&] () { return ostreamfmt("R (" << right.geometry() << ")"); });
    center.set_text([&] () { return ostreamfmt("C (" << center.geometry() << ")"); });
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

  label_t h_labels[4];
  label_t v_labels[4];

  horizontal_weighted<> center0;
  vertical_weighted<> center1;

  grid_adaption<1, 2, 5, 5> center({lay(center0), lay(center1)});

  main.get_layout().set_center(lay(center));
  main.get_layout().set_top(lay(top));
  main.get_layout().set_bottom(lay(bottom));
  main.get_layout().set_left(lay(left));
  main.get_layout().set_right(lay(right));

  main.on_create([&] () {
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
    center0.add({{lay(h_labels[0]), 100, 0.0F}, {lay(h_labels[1]), 0, 1.0F}, {lay(h_labels[2]), 100, 0.0F}, {lay(h_labels[3]), 20, 0.5F}});
    center1.add({{lay(v_labels[0]), 20, 0.0F}, {lay(v_labels[1]), 0, 1.0F}, {lay(v_labels[2]), 20, 0.0F}, {lay(v_labels[3]), 10, 0.5F}});
  });
  main.on_lost_focus([&] () {
    main.set_title("Lost focus");
  });
  main.on_set_focus([&] () {
    main.set_title("Got focus");
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello weight Layout");
  main.set_visible();

  return run_main_loop();
}
