﻿
#include "window.h"
#include "label.h"
#include "menu.h"
#include "scroll_view.h"
#include "toggle_group.h"
#include "graphics.h"


#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::draw;


// --------------------------------------------------------------------------
const os::color nero = color::rgb_color<64,66,68>::value;
const os::color silver = color::rgb_color<0xC3,0xC6,0xC7>::value;

// --------------------------------------------------------------------------
class my_main_window : public layout_main_window<border_layout<40, 30, 100, 100>> {
public:
  my_main_window ();

  void onCreated (window*, const core::rectangle&);
  void copy (int);
  void quit (int);

private:
  group_window<vertical_adaption<>, color::light_gray> top_view;
  group_window<horizontal_lineup<30, 2, 0>, nero> tool_bar;

  main_menu menu;

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

  menu.add_entries({
    sub_menu_entry("File", [&](int i) {
      labels[0].set_text("File...");
      popup_menu file_sub_menu;
      file_sub_menu.add_entries({
        menu_entry("open", [&](int) { labels[0].set_text("open"); }, "Strg+O"),
        menu_entry("close", [&](int) { labels[0].set_text("close"); } ), 
        sub_menu_entry("select", [&](int i) {
          labels[0].set_text("select...");
          popup_menu select_sub_menu;
          select_sub_menu.add_entry(
            menu_entry( "item 1", [&](int) { labels[0].set_text("item 1"); })
          );
          select_sub_menu.add_entries({
            menu_entry("item 2", [&](int) { labels[0].set_text("item 2"); }),
            sub_menu_entry("item 3", [&](int) {
              labels[0].set_text("item 3...");
              popup_menu sub_sub_menu;
              sub_sub_menu.add_entries({
                menu_entry("item 3-1", [&](int) { labels[0].set_text("item 3-1"); }),
                menu_entry("item 3-2", [&](int) { labels[0].set_text("item 3-2"); }),
                sub_menu_entry("item 3-3", [&](int i) {
                  labels[0].set_text("item 3-3...");
                  popup_menu sub_sub_menu2;
                  sub_sub_menu2.add_entry(
                    menu_entry("item 3-3-1", [&](int) { labels[0].set_text("item 3-3-1"); })
                  );
                  sub_sub_menu2.popup_at(sub_sub_menu.sub_menu_position(i), sub_sub_menu);
                }),
                menu_entry("item 3-4", [&](int i) { labels[0].set_text("item 3-4"); })
              });
              sub_sub_menu.popup_at(select_sub_menu.sub_menu_position(i), select_sub_menu);
            }),
            sub_menu_entry("item 4", [&](int i) {
              labels[0].set_text("item 4...");
              popup_menu sub_sub_menu;
              sub_sub_menu.add_entry(
                menu_entry("item 4-1", [&](int) { labels[0].set_text("item 4-1"); })
              );
              sub_sub_menu.popup_at(select_sub_menu.sub_menu_position(i), select_sub_menu);
            })
          });
          select_sub_menu.popup_at(file_sub_menu.sub_menu_position(i), file_sub_menu);
        }, true),
        menu_entry("exit", core::bind_method(this, &my_main_window::quit), "Alt+F4", true)
      });
      file_sub_menu.popup_at(menu.sub_menu_position(i), menu);
    }), 
    sub_menu_entry("Edit", [&](int i) {
      labels[0].set_text("Edit...");
      popup_menu edit_sub_menu;
      edit_sub_menu.add_entries({
        menu_entry("cut", [&](int) { labels[0].set_text("cut"); }, "Strg+X", false, u8"♠"),
        menu_entry("copy", core::bind_method(this, &my_main_window::copy), "Strg+C", false, u8"♣"),
        menu_entry("paste", [&](int) { labels[0].set_text("paste"); }, "Strg+V", false, u8"♥"),
        menu_entry("options", [&](int) { labels[0].set_text("options"); }, std::string(), true)
      });
      edit_sub_menu.popup_at(menu.sub_menu_position(i), menu);
    }),
    sub_menu_entry("Help", [&](int i) {
      labels[0].set_text("Help...");
      popup_menu help_sub_menu;
      help_sub_menu.add_entry( 
        menu_entry("about", [&](int) { labels[0].set_text("about"); })
      );
      help_sub_menu.popup_at(menu.sub_menu_position(i), menu);
    })
  });
  menu.create(top_view);

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

void my_main_window::quit (int) {
  labels[0].set_text("exit");
  super::quit();
}

void my_main_window::copy (int) {
  labels[0].set_text("copy");
#ifdef X11
  auto display = core::global::get_instance();
  auto screen = core::global::get_screen();
  auto r = left_list.client_area();
  unsigned int w = r.os_width();
  unsigned int h = r.os_height();
  auto gc = DefaultGC(display, screen);
  XImage* im = XGetImage(display, left_list.get_id(), r.os_x(), r.os_y(), w, h, AllPlanes, ZPixmap);
  Pixmap pm = XCreatePixmap(display, left_list.get_id(), w, h, DisplayPlanes(display, screen));
  XPutImage(display, pm, gc, im, 0, 0, 0, 0, w, h);
  XWriteBitmapFile(display, "left_list.bitmap", pm, w, h, -1, -1);

  XPutImage(display, window1.get_id(), gc, im, 0, 0, 0, 0, w, h);
  XCopyArea(display, left_list.get_id(), window1.get_id(), gc, 0, 0, w, h, 120, 20);
  XCopyArea(display, pm, window1.get_id(), gc, 0, 0, w, h, 240, 20);


  XDestroyImage(im);
  XFreePixmap(display, pm);

  int x, y;
  Pixmap pm2;
  XReadBitmapFile(display, window1.get_id(), "left_list.bitmap", &w, &h, &pm2, &x, &y);

  XCopyArea(display, pm2, window1.get_id(), gc, 0, 0, w, h, 360, 20);
  XFreePixmap(display, pm2);
#endif
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  my_main_window main;

  const core::rectangle r = core::rectangle(50, 50, 800, 600);
  main.create(r);
  main.set_title("Border Test");

  main.set_visible();

  int ret = win::run_main_loop();
  LogDebug << "run_main_loop finished!";
  return ret;
}
