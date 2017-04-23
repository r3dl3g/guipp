
#include "window.h"
#include "label.h"
#include "list.h"
#include "scroll_view.h"
#include "toggle_group.h"
#include "graphics.h"


#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::draw;


namespace gui {
  namespace win {
    namespace paint {
      void menu_item (const std::string& t,
                      const draw::graphics& g,
                      const core::rectangle& r,
                      const draw::brush& b,
                      bool s,
                      bool h) {
        if (s) {
          g.fill(rectangle(r), color::dark_gray);
        } else if (h) {
          g.fill(rectangle(r), color::very_light_gray);
        } else {
          g.fill(rectangle(r), b);
        }
        core::rectangle r2 = r + core::point(5, 0);
        g.text(text_box(t, r2, vcenter_left), font::system(), s ? color::white : color::dark_gray);
      }
    }
  }
}

typedef hlist<50, color::light_gray> main_menu;

// --------------------------------------------------------------------------
class popup_menu : public popup_window {
public:
  typedef popup_window super;
  static const int item_height = 20;

  void popup_at (const core::point& pt, main_menu& parent) {
    items.register_event_handler(show_event([&]() {
      items.capture_pointer();
    }));
    items.register_event_handler(hide_event([&]() {
      items.uncapture_pointer();
    }));
    items.register_event_handler(selection_changed_event([&]() {
      parent.clear_selection();
      end_modal();
    }));
    items.register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
      if (!items.place().is_inside(pt)) {
        parent.clear_selection();
        end_modal();
      }
    }));
    items.register_event_handler(mouse_move_event([&](os::key_state, const core::point& p) {
      core::point pt = items.window_to_screen(p);
      core::rectangle r = parent.absolute_place();
      if (r.is_inside(pt)) {
        int new_idx = parent.get_index_at_point(parent.screen_to_window(pt));
        if (parent.get_selection() != new_idx) {
          end_modal();
          parent.set_selection(new_idx);
        }
      }
    }));

    core::size sz(calc_width(), data.size() * item_height);

    register_event_handler(create_event([&](win::window* w, const core::rectangle& r) {
      items.create(*this, core::rectangle(sz), data);
    }));
    register_event_handler(show_event([&]() {
      items.set_visible();
    }));

    create(core::rectangle(pt, sz));
    set_visible();
    run_modal();
  }

  void add_entries (const std::initializer_list<std::string>& labels) {
    data.insert(data.end(), labels);
  }

private:
  core::size_type calc_width () {
    core::size_type w = 20;
    const font& f = font::system();
    for (std::string& s : data) {
      w = std::max(w, f.get_text_size(s).width() + 40);
    }
    return w;
  }

  typedef vlist<item_height, color::light_gray> list_type;
  typedef simple_list_data<std::string, paint::menu_item> data_type;

  list_type items;
  data_type data;
};

// --------------------------------------------------------------------------
//window_class popup_menu::clazz("popup_menu",
//#ifdef WIN32
//  (os::color)(COLOR_MENU + 1),
//  IDC_ARROW,
//  WS_OVERLAPPEDWINDOW | WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_THICKFRAME,
//  0
//#endif // WIN32
//#ifdef X11
//  color::menuColor()
//#endif
//);

// --------------------------------------------------------------------------
const os::color nero = color::rgb_color<64,66,68>::value;
const os::color silver = color::rgb_color<0xC3,0xC6,0xC7>::value;

// --------------------------------------------------------------------------
class my_main_window : public layout_main_window<border_layout<40, 30, 100, 100>> {
public:
  my_main_window ();

  void onCreated (window*, const core::rectangle&);

private:
  group_window<vertical_adaption<>, color::light_gray> top_view;
  main_menu menu;
  group_window<horizontal_lineup<30, 2, 0>, nero> tool_bar;

  typedef flat_button<silver, nero> tool_bar_button;
  tool_bar_button buttons[10];
  separator_t<false, true, nero> separators[2];

  group_window<horizontal_adaption<2, 5>, color::rgb_gray<224>::value> status_bar;
  typedef labelT<alignment_left, frame::sunken_relief> StatusLabel;
  StatusLabel labels[4];

  vlist<50, color::rgb_gray<224>::value> left_list;

  group_window<vertical_adaption<5, 5>, color::rgb_gray<160>::value> right_bar;

  group_window<attach, color::rgb_gray<224>::value> client_view;

  htoggle_group<color::black, color::light_gray> segmented;
  vtoggle_group<color::light_gray, color::gray> vsegmented;

  client_window window1;
};

// --------------------------------------------------------------------------
my_main_window::my_main_window () {
  register_event_handler(win::create_event(core::bind_method(this, &my_main_window::onCreated)));

  window1.register_event_handler(paint_event([&](const graphics& graph){
    core::rectangle place = window1.client_area();
    frame::raised_relief(graph, place);
  }));
}

// --------------------------------------------------------------------------
void my_main_window::onCreated (win::window*, const core::rectangle&) {
  top_view.create(*this);

  menu.create(top_view);
  menu.set_data(simple_list_data<std::string, paint::menu_item>({ "File", "Edit", "Help"}));
  menu.register_event_handler(selection_changed_event([&]() {
    int idx = menu.get_selection();
    if (idx > -1) {
      popup_menu sub_menu;
      sub_menu.add_entries({"one", "two", "three"});
      auto r = menu.absolute_position();
      int sz = menu.item_size;
      sub_menu.popup_at(r + core::point(idx * sz, menu.size().height()), menu);
    }
  }));

  tool_bar.create(top_view);

  int i = 0;
  for (tool_bar_button& b : buttons) {
    b.create(tool_bar);
    b.set_text(ostreamfmt(++i));
    if (i % 4 == 3) {
      separators[i / 4].create(tool_bar);
      tool_bar.get_layout().add_separator(&(separators[i / 4]));
    }
    b.register_event_handler(hilite_changed_event([&, i](bool b) {
      labels[3].set_text(ostreamfmt("button " << i << (b ? " " : " un") << "hilited"));
    }));
    b.register_event_handler(button_clicked_event([&, i]() {
      labels[3].set_text(ostreamfmt("button " << i << " clicked"));
    }));
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
                          bool selected,
                          bool hilited) {
    g.fill(rectangle(place), selected ? color::gray : (hilited ? color::very_light_gray : color::light_gray));
    if (!selected) {
      frame::raised_relief(g, place);
    }
    g.text(text_box(ostreamfmt("Item " << idx), place, center), font::system(), selected ? color::white : color::dark_gray);
  });
  left_list.set_count(10);
  left_list.register_event_handler(hilite_changed_event([&](bool){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " hilited"));
  }));
  left_list.register_event_handler(selection_changed_event([&](){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " selected"));
  }));
  left_list.register_event_handler(selection_commit_event([&](){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " commited"));
  }));

  client_view.create(*this);

  window1.create(client_view, core::rectangle(100, 40, 600, 400));
  window1.set_visible();

  segmented.add_buttons({"first", "second", "third", "fourth"});
  segmented.create(client_view, core::rectangle(100, 10, 300, 25));
  segmented.set_visible();

  vsegmented.add_buttons({"one", "two", "three", "four"});
  vsegmented.create(client_view, core::rectangle(10, 40, 80, 200));
  vsegmented.set_visible();

  for (int i = 0; i < 4; ++i) {
    vsegmented.get_button(i).register_event_handler(button_clicked_event([&, i]() {
      labels[1].set_text(ostreamfmt("vsegment " << i << " selected"));
    }));
    vsegmented.get_button(i).register_event_handler(hilite_changed_event([&, i](bool b) {
      labels[1].set_text(ostreamfmt("vsegment " << i << (b ? " " : " un") << "hilited"));
    }));
    segmented.get_button(i).register_event_handler(button_clicked_event([&, i]() {
      labels[2].set_text(ostreamfmt("hsegment " << i << " selected"));
    }));
    segmented.get_button(i).register_event_handler(hilite_changed_event([&, i](bool b) {
      labels[2].set_text(ostreamfmt("hsegment " << i << (b ? " " : " un") << "hilited"));
    }));
  }

  client_view.get_layout().attach_relative<What::left, make_relative(0.1), 100>(&segmented, &client_view);
  client_view.get_layout().attach_relative<What::right, make_relative(0.9)>(&segmented, &client_view);

  client_view.get_layout().attach_relative<What::top, make_relative(0.1), 40>(&vsegmented, &client_view);
  client_view.get_layout().attach_relative<What::bottom, make_relative(0.9)>(&vsegmented, &client_view);

  client_view.get_layout().attach_fix<What::right, Where::width, -10>(&window1, &client_view);
  client_view.get_layout().attach_fix<What::bottom, Where::height, -10>(&window1, &client_view);

  get_layout().set_center_top_bottom_left_right(&client_view, &top_view, &status_bar, &left_list, &right_bar);
  set_children_visible();
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  my_main_window main;

  const core::rectangle r = core::rectangle(50, 50, 800, 600);
  main.create(r);
  main.set_title("Border Test");

  main.set_visible();

  return win::run_main_loop();
}
