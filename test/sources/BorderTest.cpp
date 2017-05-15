﻿
#include "window.h"
#include "label.h"
#include "menu.h"
#include "scroll_view.h"
#include "split_view.h"
#include "toggle_group.h"
#include "graphics.h"
#include "bitmap.h"
#include "pnm.h"
#include "file_tree.h"

#include <fstream>

#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)


using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::draw;


// --------------------------------------------------------------------------
const os::color nero = color::rgb<64,66,68>::value;
const os::color silver = color::rgb<0xC3,0xC6,0xC7>::value;


// --------------------------------------------------------------------------
class my_main_window : public layout_main_window<border_layout<40, 30, 100, 250>> {
public:
  my_main_window ();

  void onCreated (window*, const core::rectangle&);
  void cut ();
  void copy ();
  void paste ();
  void del ();
  void quit ();
  void test_rgb ();
  void save_all_bin ();
  void save_all_ascii ();

private:
  group_window<vertical_adaption<>, color::light_gray> top_view;
  group_window<horizontal_lineup<80, 2, 0>, nero> tool_bar;

  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;

  typedef flat_button<silver, nero> tool_bar_button;
  tool_bar_button buttons[10];
  separator_t<orientation::vertical, true, nero> separators[2];

  group_window<horizontal_adaption<2, 5>, color::rgb_gray<224>::value> status_bar;
  typedef labelT<text_origin::vcenter_left, frame::sunken_relief> StatusLabel;
  StatusLabel labels[4];

  vlist<50, color::rgb_gray<224>::value> left_list;

  typedef tree_view<20, color::very_light_gray> simple_tree;
  typedef win::file_tree<20, color::very_light_gray> file_tree;
  win::hsplit_view<simple_tree, file_tree> right_view;

  group_window<attach, color::rgb_gray<224>::value> client_view;

  htoggle_group<color::black, color::light_gray> segmented;
  vtoggle_group<color::light_gray, color::gray> vsegmented;

  client_window window1;
  rgbmap bmp[2];
  graymap gray[2];
  maskmap bw[2];
};

// --------------------------------------------------------------------------
my_main_window::my_main_window () {
  register_event_handler(win::create_event(core::bind_method(this, &my_main_window::onCreated)));

  register_event_handler(win::destroy_event([&]() {
    LogDebug << "Destroyed!";
    super::quit();
  }));

  window1.register_event_handler(paint_event([&](const graphics& graph){
    core::rectangle place = window1.client_area();
    frame::raised_relief(graph, place);

    core::point_type x = 1;
    for (int i = 0; i < 2; ++i) {
      try {
        if (bw[i]) {
          graph.copy_from(bw[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }
      x += 110;
      try {
        if (gray[i]) {
          graph.copy_from(gray[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }
      x += 110;
      try {
        if (bmp[i]) {
          graph.copy_from(bmp[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }
      x += 110;
    }
  }));
}

// --------------------------------------------------------------------------
void my_main_window::onCreated (win::window*, const core::rectangle&) {
  top_view.create(*this);

  menu.data.add_entries({
    main_menu_entry("File", 'F', [&]() {
      labels[0].set_text("File...");
      file_sub_menu.popup_at(menu.sub_menu_position(0), menu);
    }), 
    main_menu_entry("Edit", 'E', [&]() {
      labels[0].set_text("Edit...");
      edit_sub_menu.popup_at(menu.sub_menu_position(1), menu);
    }),
    main_menu_entry("Window", 'W', [&]() {
      labels[0].set_text("Window...");
    }, menu_state::disabled),
    main_menu_entry("Help", 'H', [&]() {
      labels[0].set_text("Help...");
      popup_menu help_sub_menu;
      help_sub_menu.data.add_entry(
        menu_entry("About", 'A', [&]() { labels[0].set_text("about"); })
      );
      help_sub_menu.popup_at(menu.sub_menu_position(3), menu);
    })
  });
  menu.create(top_view);

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'O', [&]() { labels[0].set_text("open"); }, hot_key('O', state::control)),
    menu_entry("Close", 'C', [&]() { labels[0].set_text("close"); }, hot_key('I', state::shift)),
    sub_menu_entry("Select", 'S', [&]() {
      labels[0].set_text("select...");
      popup_menu select_sub_menu;
      select_sub_menu.data.add_entry(
        menu_entry( "item 1", '1', [&]() { labels[0].set_text("item 1"); })
      );
      select_sub_menu.data.add_entries({
        menu_entry("item 2", '2', [&]() { labels[0].set_text("item 2"); }),
        sub_menu_entry("item 3", '3', [&]() {
          labels[0].set_text("item 3...");
          popup_menu sub_sub_menu;
          sub_sub_menu.data.add_entries({
            menu_entry("item 3-1", '1', [&]() { labels[0].set_text("item 3-1"); }),
            menu_entry("item 3-2", '2', [&]() { labels[0].set_text("item 3-2"); }),
            sub_menu_entry("item 3-3", '3', [&]() {
              labels[0].set_text("item 3-3...");
              popup_menu sub_sub_menu2;
              sub_sub_menu2.data.add_entry(
                menu_entry("item 3-3-1", '1', [&]() { labels[0].set_text("item 3-3-1"); })
              );
              sub_sub_menu2.popup_at(sub_sub_menu.sub_menu_position(2), sub_sub_menu);
            }),
            menu_entry("item 3-4", '4', [&]() { labels[0].set_text("item 3-4"); })
          });
          sub_sub_menu.popup_at(select_sub_menu.sub_menu_position(2), select_sub_menu);
        }),
        sub_menu_entry("item 4", '4', [&]() {
          labels[0].set_text("item 4...");
          popup_menu sub_sub_menu;
          sub_sub_menu.data.add_entry(
            menu_entry("item 4-1", '1', [&]() { labels[0].set_text("item 4-1"); })
          );
          sub_sub_menu.popup_at(select_sub_menu.sub_menu_position(3), select_sub_menu);
        })
      });
      select_sub_menu.popup_at(file_sub_menu.sub_menu_position(2), file_sub_menu);
    }, true),
    menu_entry("Info", 'I', [&]() { labels[0].set_text("info"); }, hot_key('I', state::system)),
    menu_entry("Exit", 'x', core::bind_method(this, &my_main_window::quit), hot_key(keys::f4, state::alt), true)
  });
  file_sub_menu.data.register_hot_keys();

  core::rectangle icon_rect(0, 0, 16, 16);
  memmap cut_icon(16, 16);
  memmap copy_icon(16, 16);
  memmap paste_icon(16, 16);
  graphics(cut_icon).clear(color::transparent).text(text_box(u8"♠", icon_rect, text_origin::center), font::menu(), color::dark_red);
  graphics(copy_icon).clear(color::transparent).text(text_box(u8"♣", icon_rect, text_origin::center), font::menu(), color::dark_blue);
  graphics(paste_icon).clear(color::transparent).text(text_box(u8"♥", icon_rect, text_origin::center), font::menu(), color::dark_green);

  edit_sub_menu.data.add_entry(menu_entry("Cut", 't', core::bind_method(this, &my_main_window::cut), hot_key('A', state::control), false, cut_icon));
  edit_sub_menu.data.add_entry(menu_entry("Copy", 'C', core::bind_method(this, &my_main_window::copy), hot_key('C', state::control), false, copy_icon));
  edit_sub_menu.data.add_entry(menu_entry("Paste", 'P', core::bind_method(this, &my_main_window::paste), hot_key('V', state::control), false, paste_icon));
  edit_sub_menu.data.add_entry(menu_entry("Del", 'D', core::bind_method(this, &my_main_window::del), hot_key(keys::del)));
  edit_sub_menu.data.add_entry(menu_entry("Settings", 'S', [&]() { labels[0].set_text("settings"); }, hot_key(), false, memmap(), menu_state::disabled));
  edit_sub_menu.data.add_entry(menu_entry("Options", 'O', [&]() { labels[0].set_text("options"); }, hot_key(), true));
  edit_sub_menu.data.register_hot_keys();

  tool_bar.create(top_view);

  global::register_hot_key(hot_key(keys::f7), core::bind_method(this, &my_main_window::test_rgb));

  int i = 0;
  for (tool_bar_button& b : buttons) {
    b.create(tool_bar);
    if (i % 4 == 3) {
      separators[i / 4].create(tool_bar);
      tool_bar.get_layout().add_separator(&(separators[i / 4]));
    }
    b.register_event_handler(hilite_changed_event([&, i](bool b) {
      labels[3].set_text(ostreamfmt("button " << i << (b ? " " : " un") << "hilited"));
    }));
    ++i;
  }

  buttons[0].set_text("cut");
  buttons[0].register_event_handler(button_clicked_event(core::bind_method(this, &my_main_window::cut)));
  buttons[1].set_text("copy");
  buttons[1].register_event_handler(button_clicked_event(core::bind_method(this, &my_main_window::copy)));
  buttons[2].set_text("paste");
  buttons[2].register_event_handler(button_clicked_event(core::bind_method(this, &my_main_window::paste)));
  buttons[3].set_text("rgb");
  buttons[3].register_event_handler(button_clicked_event(core::bind_method(this, &my_main_window::test_rgb)));
  buttons[4].set_text("bin");
  buttons[4].register_event_handler(button_clicked_event(core::bind_method(this, &my_main_window::save_all_bin)));
  buttons[5].set_text("ascii");
  buttons[5].register_event_handler(button_clicked_event(core::bind_method(this, &my_main_window::save_all_ascii)));

  for (i = 6; i < 10; ++i) {
    buttons[i].set_text(ostreamfmt(i));
  }

  status_bar.create(*this);

  i = 1;
  for (StatusLabel& l : labels) {
    l.create(status_bar);
    l.set_text(ostreamfmt("Status " << i++));
  }

  right_view.create(*this, core::rectangle(0, 0, 200, 400));
  right_view.first.root.label = "root";
  right_view.first.root.add_nodes({
    tree::node("leaf 1"),
    tree::node("sub 2", {
      tree::node("sub 2.1", {
        tree::node("leaf 2.1.1"),
        tree::node("leaf 2.1.2")
      }),
      tree::node("sub 2.2", {
        tree::node("leaf 2.2.1"),
        tree::node("leaf 2.2.2"),
        tree::node("leaf 2.2.3")
      }),
      tree::node("leaf 2.3")
    }),
    tree::node("leaf 3")
  });
  right_view.first.open_all();
  right_view.first.update_node_list();
  right_view.second.root = 
#ifdef WIN32
    sys_fs::path("c:\\");
#endif // WIN32
#ifdef X11
    sys_fs::path("/");
#endif // X11

  right_view.second.update_node_list();
  right_view.set_visible();


  left_list.create(*this);
  left_list.set_drawer([](int idx,
                          const graphics& g,
                          const core::rectangle& place,
                          const brush& background,
                          bool selected,
                          bool hilited) {
    g.fill(rectangle(place), selected ? color::dark_red : (hilited ? color::very_light_gray : color::light_gray));
    if (!selected) {
      frame::raised_relief(g, place);
    }
    g.text(text_box(ostreamfmt("Item " << idx), place, text_origin::center), font::system_bold(), selected ? color::light_yellow : color::black);
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

  window1.register_event_handler(right_btn_up_event([&](os::key_state, const core::point& pt){
    edit_sub_menu.popup_at(window1.window_to_screen(pt), window1);
  }));

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

  get_layout().set_center_top_bottom_left_right(&client_view, &top_view, &status_bar, &left_list, &right_view);
  set_children_visible();
}

void my_main_window::quit () {
  labels[0].set_text("quit");

  layout_dialog_window<layout::border_layout<20, 55, 15, 15>> dialog;
  group_window<lineup<alignment::right, 80, 15, 10, 2>, color::light_gray> buttons;
  labelT<text_origin::center, draw::frame::sunken_relief, color::black, color::light_gray> message;
  text_button yes, no;

  bool result = false;
  yes.register_event_handler(button_clicked_event([&](){
    result = true;
    dialog.end_modal();
  }));
  no.register_event_handler(button_clicked_event([&](){
    result = false;
    dialog.end_modal();
  }));

  dialog.register_event_handler(set_focus_event([&](window*){ yes.take_focus(); }));

  dialog.create(*this, core::rectangle(300, 200, 400, 170));
  dialog.set_title("Question!");
  buttons.create(dialog);
  message.create(dialog, "Do you realy want to exit?");
  yes.create(buttons, "Yes");
  no.create(buttons, "No");
  dialog.get_layout().set_center_top_bottom_left_right(&message, nullptr, &buttons, nullptr, nullptr);
  dialog.set_children_visible();
  dialog.set_visible();
  disable();
  dialog.run_modal();
  enable();

  if (result) {
    super::quit();
  }
}

void my_main_window::copy () {
  labels[0].set_text("copy");
  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  bmp[0].create(sz);
  gray[0].create(sz);
  bw[0].create(sz);

  memmap img(sz);
  draw::graphics(img).copy_from(left_list, r);
  bmp[0].put(img);
  gray[0].put(img);
  bw[0].put(img);

  window1.redraw_later();
}

void my_main_window::del () {
  labels[0].set_text("del");
  for (int i = 0; i < 2; ++i) {
    bmp[i].clear();
    gray[i].clear();
    bw[i].clear();
  }
  window1.redraw_later();
}

void my_main_window::cut () {
  labels[0].set_text("cut");

  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  auto drawer = [&](core::size& sz) -> memmap {
    memmap img(sz);
    graphics g(img);
    g.fill(draw::rectangle(r), color::white);
    g.draw(draw::ellipse(r.shrinked({20, 20})), color::light_blue, draw::pen(color::dark_green, 5));
    g.fill(draw::ellipse(r.with_size({50, 50})), color::black);
    return img;
  };

  memmap img(sz);
  draw::graphics(img).copy_from(left_list, r);
  bmp[0] = img;
  gray[0] = img;
  bw[0] = img;

  io::ofpnm<io::PNM::P6>("left_list.p6") << bmp[0];
  io::ofpnm<io::PNM::P3>("left_list.p3") << bmp[0];

  io::ofpnm<io::PNM::P5>("left_list.p5") << gray[0];
  io::ofpnm<io::PNM::P2>("left_list.p2") << gray[0];

  io::ofpnm<io::PNM::P4>("left_list.p4") << bw[0];
  io::ofpnm<io::PNM::P1>("left_list.p1") << bw[0];

  bmp[1].create(sz);
  gray[1].create(sz);
  bw[1].create(sz);

  bmp[1] = drawer(sz);

  io::ofpnm<io::PNM::P6>("p6") << bmp[1];
  io::ofpnm<io::PNM::P3>("p3") << bmp[1];

  gray[1] = drawer(sz);

  io::ofpnm<io::PNM::P5>("p5") << gray[1];
  io::ofpnm<io::PNM::P2>("p2") << gray[1];

  bw[1] = drawer(sz);

  io::ofpnm<io::PNM::P4>("p4") << bw[1];
  io::ofpnm<io::PNM::P1>("p1") << bw[1];

  window1.redraw_later();
}

template<io::PNM P>
void read_write (datamap<io::PNM2BPP<P>::bpp>& bm) {
  try {
    int i = static_cast<int>(P);
    std::string iname = ostreamfmt("p" << i);
    std::string oname = ostreamfmt("test.p" << i);
    io::ifpnm<P>(iname) >> bm;
    io::ofpnm<P>(oname) << bm;
  }
  catch (std::exception& ex) {
    LogFatal << ex;
  }
}

void my_main_window::paste () {
  labels[0].set_text("paste");

  read_write<io::PNM::P1>(bw[0]);
  read_write<io::PNM::P2>(gray[0]);
  read_write<io::PNM::P3>(bmp[0]);
  read_write<io::PNM::P4>(bw[1]);
  read_write<io::PNM::P5>(gray[1]);
  read_write<io::PNM::P6>(bmp[1]);

  window1.redraw_later();
}

void my_main_window::test_rgb () {
  memmap red(100, 100), blue(100, 100), green(100, 100);
  rgbmap red24(100, 100), blue24(100, 100), green24(100, 100);

  graphics(red).clear(color::red);
  graphics(green).clear(color::green);
  graphics(blue).clear(color::blue);

  red24 = red;
  green24 = green;
  blue24 = blue;

  io::ofpnm<io::PNM::P3>("red32") << red;
  io::ofpnm<io::PNM::P3>("red24") << red24;

  io::ofpnm<io::PNM::P3>("green32") << green;
  io::ofpnm<io::PNM::P3>("green24") << green24;

  io::ofpnm<io::PNM::P3>("blue32") << blue;
  io::ofpnm<io::PNM::P3>("blue24") << blue24;

  io::ifpnm<io::PNM::P3>("red32") >> red;
  io::ifpnm<io::PNM::P3>("red24") >> red24;

  bw[0] = red;
  bw[1] = red24;

  io::ifpnm<io::PNM::P3>("green32") >> green;
  io::ifpnm<io::PNM::P3>("green24") >> green24;

  gray[0] = green;
  gray[1] = green24;

  io::ifpnm<io::PNM::P3>("blue32") >> blue;
  io::ifpnm<io::PNM::P3>("blue24") >> blue24;

  bmp[0] = blue;
  bmp[1] = blue24;

  window1.redraw_later();
}

void my_main_window::save_all_bin () {
  io::ofpnm<io::PNM::P6>("bmp0.b")  << bmp[0];
  io::ofpnm<io::PNM::P6>("bmp1.b")  << bmp[1];
  io::ofpnm<io::PNM::P5>("gray0.b") << gray[0];
  io::ofpnm<io::PNM::P5>("gray1.b") << gray[1];
  io::ofpnm<io::PNM::P4>("bw0.b")   << bw[0];
  io::ofpnm<io::PNM::P4>("bw1.b")   << bw[1];
}
  
void my_main_window::save_all_ascii () {
  io::ofpnm<io::PNM::P3>("bmp0.b") << bmp[0];
  io::ofpnm<io::PNM::P3>("bmp1.b") << bmp[1];
  io::ofpnm<io::PNM::P2>("gray0.b") << gray[0];
  io::ofpnm<io::PNM::P2>("gray1.b") << gray[1];
  io::ofpnm<io::PNM::P1>("bw0.b") << bw[0];
  io::ofpnm<io::PNM::P1>("bw1.b") << bw[1];
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
