
// --------------------------------------------------------------------------
//
// Common includes
//
#include <fstream>
#include <iomanip>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <base/time_util.h>
#include <gui/win/window.h>
#include <gui/win/attach_layout.h>
#include <gui/win/lineup_layout.h>
#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/io/pnm.h>
#include <gui/io/pnm_to_src.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/toggle_group.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/column_list.h>
#include <gui/ctrl/textbox.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/tab_group.h>
#include <gui/io/wavelength_to_rgb.h>


using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;


// --------------------------------------------------------------------------
const os::color nero = color::rgb<64,66,68>::value;
const os::color silver = color::rgb<0xC3,0xC6,0xC7>::value;


// --------------------------------------------------------------------------
class my_main_window : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;
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
  void save_all_src ();
  void open ();
  void save_as ();
  void wipe_space ();

  void start_thread ();
  void stop_thread ();

  void settings ();

private:
  volatile bool thread_is_active;
  std::thread background_action1;
  std::thread background_action2;

  group_window<vertical_adaption<>, color::light_gray> top_view;
  group_window<horizontal_lineup<80, 2, 0>, nero> tool_bar;

  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;

  typedef flat_button<silver, nero> tool_bar_button;
  tool_bar_button buttons[10];
  basic_separator<orientation::vertical, true, nero> separators[2];

  group_window<horizontal_adaption<2, 5>, color::rgb_gray<224>::value> status_bar;
  typedef basic_label<text_origin::vcenter_left, frame::sunken_relief> StatusLabel;
  StatusLabel labels[4];

  vertical_list left_list;

  typedef tree_view simple_tree;
  typedef ctrl::sorted_file_tree file_tree;
  ctrl::horizontal_split_view<simple_tree, file_tree> right_view;

  group_window<attach, color::rgb_gray<224>::value> client_view;

  top_tab_group<color::black, color::very_light_gray, 50, 80> hsegmented1;
  bottom_tab_group<color::black, color::very_light_gray, 40, 70> hsegmented2;
  left_tab_group<color::black, color::very_light_gray, 30, 100> vsegmented1;
  right_tab_group<color::black, color::very_light_gray, 50, 80> vsegmented2;

  client_control<color::rgb_gray<0xda>::value> window1;
  rgbamap rgba[2];
  rgbmap bmp[2];
  graymap gray[2];
  bwmap bw[2];

  rgbmap wave_color;
};

// --------------------------------------------------------------------------
void my_main_window::start_thread () {
  thread_is_active = true;

  background_action1 = std::thread([&] () {
    int counter = 0;
    while (thread_is_active) {
      win::run_on_main([&, counter] () {
        labels[3].set_text(ostreamfmt("Trip " << counter));
      });
      ++counter;
      std::this_thread::sleep_for(std::chrono::milliseconds(11));
    }
  });

  background_action2 = std::thread([&] () {
    int counter = 0;
    while (thread_is_active) {
      win::run_on_main([&, counter] () {
        labels[3].set_text(ostreamfmt("Trap " << counter));
      });
      ++counter;
      std::this_thread::sleep_for(std::chrono::milliseconds(17));
    }
  });
}

// --------------------------------------------------------------------------
void my_main_window::stop_thread () {
  thread_is_active = false;
  if (background_action1.joinable()) {
    background_action1.join();
  }
  if (background_action2.joinable()) {
    background_action2.join();
  }
}

// --------------------------------------------------------------------------
my_main_window::my_main_window ()
  : super(40, 30, 100, 250)
  , thread_is_active(false)
  , left_list(50, color::rgb_gray<224>::value)
  , right_view(simple_tree(20, color::very_light_gray), file_tree(20, color::very_light_gray))
{
  on_create(basepp::bind_method(this, &my_main_window::onCreated));

  on_destroy([&]() {
    LogDebug << *this << " Destroyed!";
    win::quit_main_loop();
  });

  on_close(basepp::bind_method(this, &my_main_window::quit));

  window1.on_paint(draw::paint([&](const graphics& graph){
    core::rectangle place = window1.client_area();
    frame::raised_relief(graph, place);

    core::point::type x = 1;
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
      try {
        if (rgba[i]) {
          graph.copy_from(rgba[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }
      x += 110;
    }

    if (wave_color) {
      graph.copy_from(wave_color, core::point::zero);
    }

  }));
}

// --------------------------------------------------------------------------
pixmap create_text_pixmap (const std::string& str,
                           const core::rectangle& rect,
                           const os::color color) {
  pixmap img(rect.size());
  graphics g(img);
  text_box box(str, rect, text_origin::center);
  g.clear(color::black);
  g.text(box, font::menu(), color);
  return img;
}

// --------------------------------------------------------------------------
void my_main_window::onCreated (win::window*, const core::rectangle&) {
  top_view.create(*this);

  menu.data.add_entries({
    main_menu_entry("File", 'F', [&]() {
      labels[0].set_text("File...");
      file_sub_menu.popup(menu);
    }), 
    main_menu_entry("Edit", 'E', [&]() {
      labels[0].set_text("Edit...");
      edit_sub_menu.popup(menu);
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
      help_sub_menu.popup(menu);
    })
  });
  menu.create(top_view);
  menu.set_visible();

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'O', basepp::bind_method(this, &my_main_window::open), hot_key('O', state::control)),
    menu_entry("Save As", 'S', basepp::bind_method(this, &my_main_window::save_as), hot_key('S', state::control)),
    menu_entry("Wipe empty space", 'W', basepp::bind_method(this, &my_main_window::wipe_space), hot_key('W', state::control)),
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
              sub_sub_menu2.popup(sub_sub_menu);
            }),
            menu_entry("item 3-4", '4', [&]() { labels[0].set_text("item 3-4"); })
          });
          sub_sub_menu.popup(select_sub_menu);
        }),
        sub_menu_entry("item 4", '4', [&]() {
          labels[0].set_text("item 4...");
          popup_menu sub_sub_menu;
          sub_sub_menu.data.add_entry(
            menu_entry("item 4-1", '1', [&]() { labels[0].set_text("item 4-1"); })
          );
          sub_sub_menu.popup(select_sub_menu);
        })
      });
      select_sub_menu.popup(file_sub_menu);
    }, true),
    menu_entry("Info", 'I', [&]() { labels[0].set_text("info"); }, hot_key('I', state::system)),
    menu_entry("Exit", 'x', basepp::bind_method(this, &my_main_window::quit), hot_key(keys::f4, state::alt), true)
  });
  file_sub_menu.data.register_hot_keys(this);

  const float icn_sz = core::global::scale(16.0F);
  core::rectangle icon_rect(0, 0, icn_sz, icn_sz);

  pixmap cut_icon = create_text_pixmap(IF_WIN32_ELSE(u8"\x2660", u8"♠"), icon_rect, color::dark_red);
  pixmap copy_icon = create_text_pixmap(IF_WIN32_ELSE(u8"\x2663", u8"♣"), icon_rect, color::dark_blue);
  pixmap paste_icon = create_text_pixmap(IF_WIN32_ELSE(u8"\x2665", u8"♥"), icon_rect, color::dark_green);

  edit_sub_menu.data.add_entry(menu_entry("Cut", 't', basepp::bind_method(this, &my_main_window::cut), hot_key('X', state::control), false, cut_icon));
  edit_sub_menu.data.add_entry(menu_entry("Copy", 'C', basepp::bind_method(this, &my_main_window::copy), hot_key('C', state::control), false, copy_icon));
  edit_sub_menu.data.add_entry(menu_entry("Paste", 'P', basepp::bind_method(this, &my_main_window::paste), hot_key('V', state::control), false, paste_icon));
  edit_sub_menu.data.add_entry(menu_entry("Del", 'D', basepp::bind_method(this, &my_main_window::del), hot_key(keys::del)));
  edit_sub_menu.data.add_entry(menu_entry("Settings", 'S', basepp::bind_method(this, &my_main_window::settings), hot_key(), true));
  edit_sub_menu.data.add_entry(menu_entry("Options", 'O', [&]() { labels[0].set_text("options"); }, hot_key(), false, pixmap(), menu_state::disabled));
  edit_sub_menu.data.register_hot_keys(this);

  tool_bar.create(top_view);

  global::register_hot_key(hot_key(keys::f7), basepp::bind_method(this, &my_main_window::test_rgb), this);

  int i = 0;
  for (tool_bar_button& b : buttons) {
    b.create(tool_bar);
    if (i % 4 == 3) {
      separators[i / 4].create(tool_bar);
      tool_bar.get_layout().add_separator(&(separators[i / 4]));
    }
    b.on_hilite_changed([&, i](bool b) {
      labels[3].set_text(ostreamfmt("button " << i << (b ? " " : " un") << "hilited"));
    });
    ++i;
  }

  buttons[0].set_text("cut");
  buttons[0].on_clicked(basepp::bind_method(this, &my_main_window::cut));
  buttons[1].set_text("copy");
  buttons[1].on_clicked(basepp::bind_method(this, &my_main_window::copy));
  buttons[2].set_text("paste");
  buttons[2].on_clicked(basepp::bind_method(this, &my_main_window::paste));
  buttons[3].set_text("rgb");
  buttons[3].on_clicked(basepp::bind_method(this, &my_main_window::test_rgb));
  buttons[4].set_text("bin");
  buttons[4].on_clicked(basepp::bind_method(this, &my_main_window::save_all_bin));
  buttons[5].set_text("ascii");
  buttons[5].on_clicked(basepp::bind_method(this, &my_main_window::save_all_ascii));
  buttons[6].set_text("src");
  buttons[6].on_clicked(basepp::bind_method(this, &my_main_window::save_all_src));
  buttons[7].set_text("start");
  buttons[7].on_clicked(basepp::bind_method(this, &my_main_window::start_thread));
  buttons[8].set_text("stop");
  buttons[8].on_clicked(basepp::bind_method(this, &my_main_window::stop_thread));

  for (i = 9; i < 10; ++i) {
    buttons[i].set_text(ostreamfmt(i));
  }

  status_bar.create(*this);

  i = 1;
  for (StatusLabel& l : labels) {
    l.create(status_bar);
    l.set_text(ostreamfmt("Status " << i++));
  }

  right_view.create(*this, core::rectangle(0, 0, 200, 400));
  simple_tree::type root;
  root.label = "root";
  root.add_nodes({
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
  right_view.first.set_root(std::move(root));
//  right_view.first.open_all();
  right_view.first.update_node_list();
  right_view.second.set_root(sys_fs::path(IF_WIN32_ELSE("c:\\", "/")));

  right_view.second.update_node_list();
  right_view.set_visible();


  left_list.create(*this);
  left_list.set_drawer([](std::size_t idx,
                          const graphics& g,
                          const core::rectangle& place,
                          const brush& /*background*/,
                          bool selected,
                          bool hilited) {
    g.fill(rectangle(place), selected ? color::dark_red : (hilited ? color::very_light_gray : color::light_gray));
    if (!selected) {
      frame::raised_relief(g, place);
    }
    g.text(text_box(ostreamfmt("Item " << idx), place, text_origin::center), font::system_bold(), selected ? color::light_yellow : color::black);
  });
  left_list.set_count(10);
  left_list.on_hilite_changed([&](bool){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " hilited"));
  });
  left_list.on_selection_changed([&](event_source){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " selected"));
  });
  left_list.on_selection_commit([&](){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " commited"));
  });

  client_view.create(*this);

  window1.on_right_btn_up([&](os::key_state, const core::point& pt){
    edit_sub_menu.popup_at(window1.window_to_screen(pt), window1);
  });

  window1.create(client_view, core::rectangle(69, 40, 600, 400));
  window1.set_visible();

  hsegmented1.add_buttons({"first", "second", "third", "fourth"});
  hsegmented1.create(client_view, core::rectangle(75, 16, 300, 25));
  hsegmented1.set_visible();

  hsegmented2.add_buttons({"eins", "zwei", "drei", "vier"});
  hsegmented2.create(client_view, core::rectangle(75, 439, 300, 25));
  hsegmented2.set_visible();

  hsegmented2.get_button(0).on_clicked([&]() {
    hsegmented1.get_button(0).disable();
    hsegmented2.get_button(3).disable();
    vsegmented1.get_button(1).disable();
    vsegmented2.disable();
  });

  hsegmented2.get_button(1).on_clicked([&]() {
    hsegmented1.get_button(0).enable();
    hsegmented2.get_button(3).enable();
    vsegmented1.get_button(1).enable();
    vsegmented2.enable();
  });

  vsegmented1.add_buttons({"one", "two", "three", "four"});
  vsegmented1.create(client_view, core::rectangle(10, 40, 60, 60));
  vsegmented1.set_visible();

  vsegmented2.add_buttons({"erster", "zweiter", "driter", "vierter"});
  vsegmented2.create(client_view, core::rectangle(700, 40, 60, 60));
  vsegmented2.set_visible();

  for (int i = 0; i < 4; ++i) {
    vsegmented1.get_button(i).on_clicked([&, i]() {
      labels[1].set_text(ostreamfmt("vsegment " << i << " selected"));
    });
    vsegmented1.get_button(i).on_hilite_changed([&, i](bool b) {
      labels[1].set_text(ostreamfmt("vsegment " << i << (b ? " " : " un") << "hilited"));
    });
    hsegmented1.get_button(i).on_clicked([&, i]() {
      labels[2].set_text(ostreamfmt("hsegment " << i << " selected"));
    });
    hsegmented1.get_button(i).on_hilite_changed([&, i](bool b) {
      labels[2].set_text(ostreamfmt("hsegment " << i << (b ? " " : " un") << "hilited"));
    });
  }

  client_view.get_layout().attach_fix<What::right, Where::width, -110>(&hsegmented1, &client_view);

  client_view.get_layout().attach_fix<What::right, Where::width, -110>(&hsegmented2, &client_view);
  client_view.get_layout().attach_fix<What::top, Where::height, -35>(&hsegmented2, &client_view);
  client_view.get_layout().attach_fix<What::bottom, Where::height, -10>(&hsegmented2, &client_view);

  client_view.get_layout().attach_relative<What::top, make_relative(0.1), 40>(&vsegmented1, &client_view);
  client_view.get_layout().attach_relative<What::bottom, make_relative(0.9)>(&vsegmented1, &client_view);

  client_view.get_layout().attach_relative<What::top, make_relative(0.1), 40>(&vsegmented2, &client_view);
  client_view.get_layout().attach_relative<What::bottom, make_relative(0.9)>(&vsegmented2, &client_view);
  client_view.get_layout().attach_fix<What::left, Where::width, -80>(&vsegmented2, &client_view);
  client_view.get_layout().attach_fix<What::right, Where::width, -10>(&vsegmented2, &client_view);

  client_view.get_layout().attach_fix<What::right, Where::width, -79>(&window1, &client_view);
  client_view.get_layout().attach_fix<What::bottom, Where::height, -34>(&window1, &client_view);

  get_layout().set_center_top_bottom_left_right(&client_view, &top_view, &status_bar, &left_list, &right_view);
  set_children_visible();
}

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void my_main_window::quit () {
  labels[0].set_text("quit");

  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      stop_thread();
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}

//-----------------------------------------------------------------------------
void my_main_window::open () {
  labels[0].set_text("open");

  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      gui::draw::basic_datamap img;
      gui::io::load_pnm(file.string(), img);
      rgba[0] = img.convert<PixelFormat::RGBA>();
      bmp[0] = img.convert<PixelFormat::RGB>();
      gray[0] = img.convert<PixelFormat::GRAY>();
      bw[0] = img.convert<PixelFormat::BW>();
      window1.invalidate();
    }
  });
}

void my_main_window::save_as () {
  labels[0].set_text("save");

  file_save_dialog::show(*this, "Save File", "new_file.h", "Name:", "Save", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      yes_no_dialog::ask(*this, "Question!",
                         ostreamfmt("File '" << file << "' already exists!\nDo you want to overwrite the exiting file?"),
                         "Yes", "No", [&] (bool yes) {
        if (yes) {
          gui::io::src::save_pnm_src(file.string(), gray[0], "src");
        }
        take_focus();
      });
    } else {
      gui::io::src::save_pnm_src(file.string(), gray[0], "src");
      take_focus();
    }
  });
}

void my_main_window::wipe_space () {
//  sys_fs::current_path("C:\\");
//  dir_open_dialog::show(*this, "Choose target directory", "Wipe", "Cancel", [](const sys_fs::path& dir) {
  sys_fs::path file = "C:\\";// dir;
    file /= "empty.tmp";
    std::ofstream f(file);
    const auto buffer_size = 1024 * 1024;
    char buffer[buffer_size];
    memset(buffer, 0, buffer_size);
    while (!f.write(buffer, buffer_size).fail()) {
    }
    f.close();
    sys_fs::remove(file);
//  });
}

void my_main_window::copy () {
  labels[0].set_text("copy");
  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  rgba[0].create(sz);
  bmp[0].create(sz);
  gray[0].create(sz);
  bw[0].create(sz);

  pixmap img(sz);
  draw::graphics(img).copy_from(left_list, r);
  rgba[0] = img;
  bmp[0] = img;
  gray[0] = img;
  bw[0] = img;

  window1.invalidate();
}

void my_main_window::settings () {
    core::rectangle r = window1.client_area();
    core::size sz = r.size();

    pixmap img(sz);
    graphics g(img);
    double scale = 500.0 / sz.width();
    for (int x = 0; x < sz.width(); ++x) {
        os::color rgb = io::optics::wave_length_to_rgb(static_cast<double>(330.0 + x * scale));
        g.draw_lines({core::point{(float)x, r.y()}, core::point{(float)x, r.y2()}}, draw::pen(rgb));
    }
    wave_color = img;
    window1.invalidate();
}

  void my_main_window::del () {
  labels[0].set_text("del");
  for (int i = 0; i < 2; ++i) {
    rgba[i].clear();
    bmp[i].clear();
    gray[i].clear();
    bw[i].clear();
  }
  window1.invalidate();
}

void my_main_window::cut () {
  labels[0].set_text("cut");

  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  auto drawer = [&](core::size& sz) -> pixmap {
    pixmap img(sz);
    graphics g(img);
    g.fill(draw::rectangle(r), color::white);
    g.draw(draw::ellipse(r.shrinked({20, 20})), color::light_blue, draw::pen(color::dark_green, 5));
    g.fill(draw::ellipse(r.with_size({50, 50})), color::black);
    return img;
  };

  pixmap img(sz);
  draw::graphics(img).copy_from(left_list, r);
  rgba[0] = img;
  bmp[0] = img;
  gray[0] = img;
  bw[0] = img;

  std::ofstream("left_list_rgba.p6.ppm") << io::opnm<true, PixelFormat::RGBA>(rgba[0]);
  std::ofstream("left_list_rgba.p3.ppm") << io::opnm<false, PixelFormat::RGBA>(rgba[0]);

  io::ofpnm<io::PNM::P6>("left_list.p6") << bmp[0];
  io::ofpnm<io::PNM::P3>("left_list.p3") << bmp[0];

  io::ofpnm<io::PNM::P5>("left_list.p5") << gray[0];
  io::ofpnm<io::PNM::P2>("left_list.p2") << gray[0];

  io::ofpnm<io::PNM::P4>("left_list.p4") << bw[0];
  io::ofpnm<io::PNM::P1>("left_list.p1") << bw[0];

  rgba[1].create(sz);
  bmp[1].create(sz);
  gray[1].create(sz);
  bw[1].create(sz);

  rgba[1] = drawer(sz);

  std::ofstream("p6-rgba.ppm") << io::opnm<true, PixelFormat::RGBA>(rgba[1]);
  std::ofstream("p3-rgba.ppm") << io::opnm<false, PixelFormat::RGBA>(rgba[1]);

  bmp[1] = drawer(sz);

  io::ofpnm<io::PNM::P6>("p6") << bmp[1];
  io::ofpnm<io::PNM::P3>("p3") << bmp[1];

  gray[1] = drawer(sz);

  io::ofpnm<io::PNM::P5>("p5") << gray[1];
  io::ofpnm<io::PNM::P2>("p2") << gray[1];

  bw[1] = drawer(sz);

  io::ofpnm<io::PNM::P4>("p4") << bw[1];
  io::ofpnm<io::PNM::P1>("p1") << bw[1];

  window1.invalidate();
}

template<io::PNM P>
void read_write (datamap<io::PNM2BPP<P>::px_fmt>& bm) {
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

template<io::PNM P>
void read_write_rgba (datamap<PixelFormat::RGBA>& bm) {
  try {
    int i = static_cast<int>(P);
    std::string iname = ostreamfmt("p" << i << "-rgba.ppm");
    std::string oname = ostreamfmt("test.p" << i << "-rgba.ppm");
    io::ipnm<PixelFormat::RGBA> ip(bm);
    std::ifstream(iname) >> ip;
    std::ofstream(oname) << io::opnm<io::PNM2BPP<P>::bin, PixelFormat::RGBA>(bm);
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

  read_write_rgba<io::PNM::P3>(rgba[0]);
  read_write_rgba<io::PNM::P6>(rgba[1]);

  window1.invalidate();
}

void my_main_window::test_rgb () {
  pixmap red(100, 100), blue(100, 100), green(100, 100);
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

  rgba[0] = blue;
  rgba[1] = blue24;

  window1.invalidate();
}

void my_main_window::save_all_bin () {
  ctrl::dir_open_dialog::show(*this, "Choose target directory", "Save", "Cancel", [&] (const sys_fs::path& file) {
    sys_fs::current_path(file);
    std::ofstream("rgba0.b.ppm") << io::opnm<true, PixelFormat::RGBA>(rgba[0]);
    std::ofstream("rgba1.b.ppm") << io::opnm<true, PixelFormat::RGBA>(rgba[1]);
    io::ofpnm<io::PNM::P6>("bmp0.b")  << bmp[0];
    io::ofpnm<io::PNM::P6>("bmp1.b")  << bmp[1];
    io::ofpnm<io::PNM::P5>("gray0.b") << gray[0];
    io::ofpnm<io::PNM::P5>("gray1.b") << gray[1];
    io::ofpnm<io::PNM::P4>("bw0.b")   << bw[0];
    io::ofpnm<io::PNM::P4>("bw1.b")   << bw[1];
  });
}

void my_main_window::save_all_ascii() {
  std::ofstream("rgba0.a.ppm") << io::opnm<false, PixelFormat::RGBA>(rgba[0]);
  std::ofstream("rgba1.a.ppm") << io::opnm<false, PixelFormat::RGBA>(rgba[1]);
  io::ofpnm<io::PNM::P3>("bmp0.a") << bmp[0];
  io::ofpnm<io::PNM::P3>("bmp1.a") << bmp[1];
  io::ofpnm<io::PNM::P2>("gray0.a") << gray[0];
  io::ofpnm<io::PNM::P2>("gray1.a") << gray[1];
  io::ofpnm<io::PNM::P1>("bw0.a") << bw[0];
  io::ofpnm<io::PNM::P1>("bw1.a") << bw[1];
}

void my_main_window::save_all_src () {
  io::src::save_pnm_src("bmp0.h", bmp[0], "bmp0");
  io::src::save_pnm_src("bmp1.h", bmp[1], "bmp1");
  io::src::save_pnm_src("gray0.h", gray[0], "gray0");
  io::src::save_pnm_src("gray1.h", gray[1], "gray1");
  io::src::save_pnm_src("bw0.h", bw[0], "bw0");
  io::src::save_pnm_src("bw1.h", bw[1], "bw1");
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
//  gui::core::global::set_scale_factor(1.0);
  my_main_window main;

  const core::rectangle r = core::rectangle(50, 50, 800, 600);
  main.create(r);
  main.set_title("Border Test");

  main.set_visible();

  int ret = win::run_main_loop();
  LogDebug << main << " run_main_loop finished!";
  return ret;
}
