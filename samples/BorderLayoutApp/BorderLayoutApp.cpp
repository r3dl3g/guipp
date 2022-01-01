
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/textbox.h>


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

  layout_main_window<border::layouter<10, 10, 10, 10, border::type_t::all_symmetric>> main;

  layout_group<border::type_t::top_bottom_maximize> center0;
  layout_group<border::type_t::left_right_maximize> center1;
  layout_group<border::type_t::bottom_max_top_min> center2;
  layout_group<border::type_t::top_max_bottom_min> center3;
  layout_group<border::type_t::left_max_right_min> center4;
  layout_group<border::type_t::right_max_left_min> center5;
  layout_group<border::type_t::top_left_maximize> center6;
  layout_group<border::type_t::bottom_right_maximize> center7;
  layout_group<border::type_t::all_symmetric> center8;

  grid_adaption<3, 3, 5, 5> center({lay(center0.layouter), lay(center1.layouter), lay(center2.layouter),
                                    lay(center3.layouter), lay(center4.layouter), lay(center5.layouter),
                                    lay(center6.layouter), lay(center7.layouter), lay(center8.layouter)});

  main.get_layout().set_center(lay(center));

  main.on_create([&] () {
    center0.create(main);
    center1.create(main);
    center2.create(main);
    center3.create(main);
    center4.create(main);
    center5.create(main);
    center6.create(main);
    center7.create(main);
    center8.create(main);
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello New Border Layout");
  main.set_visible();

  return run_main_loop();
}
