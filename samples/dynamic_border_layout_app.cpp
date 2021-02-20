
#include <gui/win/overlapped_window.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/dynamic_border_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/label.h>


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

template<border_layout_type T>
struct layout_group {

  label_t top, bottom, left, right, center;
  dynamic_border_layout<T> layouter;

  layout_group (scroll_bar& ls, scroll_bar& rs, scroll_bar& ts, scroll_bar& bs)
    : layouter(20, 20, 100, 100)
  {
    ls.on_scroll([&] (point::type v) {
      layouter.set_left_width(v);
    });
    rs.on_scroll([&] (point::type v) {
      layouter.set_right_width(v);
    });
    ts.on_scroll([&] (point::type v) {
      layouter.set_top_height(v);
    });
    bs.on_scroll([&] (point::type v) {
      layouter.set_bottom_height(v);
    });
  }

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

  layout_main_window<border::layouter<20, 20, 20, 20, border::type_t::all_symmetric>> main;

  vertical_scroll_bar left;
  vertical_scroll_bar right;
  horizontal_scroll_bar top;
  horizontal_scroll_bar bottom;

  left.set_min_max_step_value(0, 200, 1, 100);
  right.set_min_max_step_value(0, 200, 1, 100);
  top.set_min_max_step_value(0, 200, 1, 20);
  bottom.set_min_max_step_value(0, 200, 1, 20);

  layout_group<border_layout_type::top_bottom_maximize> center0(left, right, top, bottom);
  layout_group<border_layout_type::left_right_maximize> center1(left, right, top, bottom);
  layout_group<border_layout_type::bottom_max_top_min> center2(left, right, top, bottom);
  layout_group<border_layout_type::top_max_bottom_min> center3(left, right, top, bottom);
  layout_group<border_layout_type::left_max_right_min> center4(left, right, top, bottom);
  layout_group<border_layout_type::right_max_left_min> center5(left, right, top, bottom);
  layout_group<border_layout_type::top_left_maximize> center6(left, right, top, bottom);
  layout_group<border_layout_type::bottom_right_maximize> center7(left, right, top, bottom);
  layout_group<border_layout_type::all_symmetric> center8(left, right, top, bottom);

  grid_adaption<3, 3, 5, 5> center({lay(center0.layouter), lay(center1.layouter), lay(center2.layouter),
                                    lay(center3.layouter), lay(center4.layouter), lay(center5.layouter),
                                    lay(center6.layouter), lay(center7.layouter), lay(center8.layouter)});

  main.get_layout().set_center_top_bottom_left_right(lay(center), lay(top), lay(bottom), lay(left), lay(right));

  main.on_create([&] () {
    left.create(main);
    right.create(main);
    top.create(main);
    bottom.create(main);

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
  main.on_paint(draw::paint([&] (draw::graphics& gc) {
    gc.clear(color::very_very_light_gray);
  }));

  left.on_scroll([&] (point::type) {
    main.layout();
  });
  right.on_scroll([&] (point::type) {
    main.layout();
  });
  top.on_scroll([&] (point::type) {
    main.layout();
  });
  bottom.on_scroll([&] (point::type) {
    main.layout();
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello Border Layout");
  main.set_visible();

  return run_main_loop();
}
