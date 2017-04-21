
#include "window.h"
#include "button.h"
#include "label.h"
#include "list.h"
#include "scroll_view.h"
#include "graphics.h"


#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::draw;

class my_main_window : public layout_main_window<border_layout<34, 30, 100, 100>> {
public:
  my_main_window ();

  void onCreated (window*, const core::rectangle&);

private:
  group_window<horizontal_lineup<30, 2, 5>, color::rgb_gray<128>::value> tool_bar;
  flat_button buttons[10];

  group_window<horizontal_adaption<2, 5>, color::rgb_gray<224>::value> status_bar;
  typedef labelT<alignment_left, frame::sunken_relief> StatusLabel;
  StatusLabel labels[3];

  vlist<50> left_list;

  group_window<vertical_adaption<5, 5>, color::rgb_gray<160>::value> right_bar;

  group_window<standard_layout, color::rgb_gray<224>::value> client_view;

  client_window window1;
};

my_main_window::my_main_window () {
  register_event_handler(win::create_event(core::bind_method(this, &my_main_window::onCreated)));

  window1.register_event_handler(paint_event([&](const graphics& graph){
    core::rectangle place = window1.client_area();
    frame::raised_relief(graph, place);
  }));

}

void my_main_window::onCreated (win::window*, const core::rectangle&) {
  tool_bar.create(*this);

  int i = 1;
  for (flat_button& b : buttons) {
    b.create(tool_bar);
    b.set_text(ostreamfmt(i++));
  }

  status_bar.create(*this);

  i = 1;
  for (StatusLabel& l : labels) {
    l.create(status_bar);
    l.set_text(ostreamfmt("Status " << i++));
  }

  right_bar.create(*this);
  left_list.create(*this);
  left_list.set_drawer([](int idx,
                          const graphics& g,
                          const core::rectangle& place,
                          const brush& background,
                          bool selected) {
    g.fill(rectangle(place), selected ? color::gray : color::light_gray);
    if (!selected) {
      frame::raised_relief(g, place);
    }
    g.text(text_box(ostreamfmt("Item " << idx), place, center), font::system(), selected ? color::white : color::dark_gray);
  });
  left_list.set_count(10);

  client_view.create(*this);
  window1.create(client_view, core::rectangle(10, 10, 600, 400));

  get_layout().set_center_top_bottom_left_right(&client_view, &tool_bar, &status_bar, &left_list, &right_bar);
  set_children_visible();
}

int gui_main(const std::vector<std::string>& args) {
  my_main_window main;

  const core::rectangle r = core::rectangle(50, 50, 800, 600);
  main.create(r);
  main.set_title("Border Test");

  main.set_visible();

  return win::run_main_loop();
}
