
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
#include <util/time_util.h>
#include <gui/win/window.h>
#include <gui/layout/attach_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/draw/graphics.h>
#include <gui/draw/pen.h>
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
#include <gui/ctrl/progress_bar.h>
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
class my_main_window : public layout_main_window<gui::layout::border::layouter<40, 30, 100, 250>> {
public:
  typedef layout_main_window<gui::layout::border::layouter<40, 30, 100, 250>> super;
  my_main_window ();

  void onCreated ();
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

  void set_scale (double);

  void settings ();

private:
  volatile bool thread_is_active;
  std::thread background_action1;
  std::thread background_action2;

  group_window<vertical_adaption<>> top_view;
  group_window<horizontal_lineup<80, 2, 0>> tool_bar;

  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu select_sub_menu;
  popup_menu sub_sub_menu;
  popup_menu sub_sub_menu3;
  popup_menu sub_sub_menu4;
  popup_menu edit_sub_menu;
  popup_menu scale_sub_menu;
  popup_menu help_sub_menu;

  typedef flat_button<silver, nero> tool_bar_button;
  tool_bar_button buttons[10];
  basic_separator<orientation_t::vertical, true, nero> separators[2];

  group_window<horizontal_adaption<2, 5>> status_bar;
  typedef basic_label<text_origin_t::vcenter_left, frame::sunken_relief> StatusLabel;
  StatusLabel labels[4];

  list left_list;

  typedef tree_view simple_tree;
  typedef ctrl::sorted_file_tree file_tree;
  ctrl::horizontal_split_view<simple_tree, file_tree> right_view;

  group_window<attach> client_view;

  top_tab_group<origin_t::start, color::black, color::very_light_gray, 50, 80> hsegmented1;
  bottom_tab_group<origin_t::start, color::black, color::very_light_gray, 40, 70> hsegmented2;
  left_tab_group<origin_t::start, color::black, color::very_light_gray, 30, 100> vsegmented1;
  right_tab_group<origin_t::start, color::black, color::very_light_gray, 50, 80> vsegmented2;

  client_control<color::very_very_light_gray> window1;
  rgbamap rgbas[2];
  rgbmap rgbs[2];
  graymap grays[2];
  bwmap bws[2];

  rgbmap wave_color;
  core::point last_mouse_point;
};

// --------------------------------------------------------------------------
void my_main_window::start_thread () {
  thread_is_active = true;

  background_action1 = std::thread([&] () {
    int counter = 0;
    while (thread_is_active) {
      win::run_on_main(*this, [&, counter] () {
        labels[3].set_text(ostreamfmt("Trip " << counter));
      });
      ++counter;
      std::this_thread::sleep_for(std::chrono::milliseconds(11));
    }
  });

  background_action2 = std::thread([&] () {
    int counter = 0;
    while (thread_is_active) {
      win::run_on_main(*this, [&, counter] () {
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

void my_main_window::set_scale (double f) {
  auto current_size = size();
  core::global::set_scale_factor(f);
  resize(current_size, true);
  layout();
}

// --------------------------------------------------------------------------
my_main_window::my_main_window ()
  : thread_is_active(false)
  , left_list(50, color::rgb_gray<224>::value)
  , right_view(simple_tree(20, color::very_light_gray), file_tree(20, color::very_light_gray))
{
  on_destroy(&win::quit_main_loop);
  on_create([&] () { onCreated(); });
  on_close([&] () { quit(); });

  window1.on_paint(draw::paint([&](graphics& graph){
    core::rectangle place = window1.client_geometry();
    frame::sunken_relief(graph, place);

    core::point::type x = 1;
    for (int i = 0; i < 2; ++i) {
      try {
        if (bws[i]) {
          graph.copy_from(bws[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        clog::fatal() << ex;
      }
      x += 110;
      try {
        if (grays[i]) {
          graph.copy_from(grays[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        clog::fatal() << ex;
      }
      x += 110;
      try {
        if (rgbs[i]) {
          graph.copy_from(rgbs[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        clog::fatal() << ex;
      }
      x += 110;
      try {
        if (rgbas[i]) {
          graph.copy_from(rgbas[i], core::point(x, 1));
        }
      } catch (std::exception& ex) {
        clog::fatal() << ex;
      }
      x += 110;
    }

    if (wave_color) {
      graph.copy_from(wave_color, core::point::zero);
    }

    if (last_mouse_point != core::point::zero) {
      graph.frame(draw::arc(window1.screen_to_client(last_mouse_point), 5, 0, 360), color::red);
    }

  }));
}

// --------------------------------------------------------------------------
masked_bitmap create_text_pixmap (const std::string& str,
                           const core::rectangle& rect,
                           const os::color color) {
  pixmap img(rect.size());
  graphics g(img);
  g.clear(color::black);
  text_box box(str, rect, text_origin_t::center);
  g.text(box, font::menu(), color);
  return masked_bitmap{std::move(img)};
}

// --------------------------------------------------------------------------
void my_main_window::onCreated () {
  top_view.create(*this);
  top_view.set_background(color::light_gray);

  menu.data.add_entries({
    main_menu_entry("File", 'F', [&]() {
      labels[0].set_text("File...");
      file_sub_menu.popup(menu);
    }),
    main_menu_entry("Edit", 'E', [&]() {
      labels[0].set_text("Edit...");
      edit_sub_menu.popup(menu);
    }),
    main_menu_entry("View", 'V', [&]() {
      labels[0].set_text("View...");
      scale_sub_menu.popup(menu);
    }),
    main_menu_entry("Window", 'W', [&]() {
      labels[0].set_text("Window...");
    }, menu_state::disabled),
    main_menu_entry("Help", 'H', [&]() {
      labels[0].set_text("Help...");
      help_sub_menu.popup(menu);
    })
  });

  using namespace gui::core;
  file_sub_menu.data.add_entries({
    menu_entry("Open", 'O', util::bind_method(this, &my_main_window::open), hot_key('O', state::control)),
    menu_entry("Save As", 'S', util::bind_method(this, &my_main_window::save_as), hot_key('S', state::control)),
    menu_entry("Wipe empty space", 'W', util::bind_method(this, &my_main_window::wipe_space), hot_key('W', state::control)),
    menu_entry("Close", 'C', [&]() { labels[0].set_text("close"); }, hot_key('I', state::shift)),
    sub_menu_entry("Select", 'S', [&]() {
      labels[0].set_text("select...");
      select_sub_menu.popup(file_sub_menu);
    }, true),
    menu_entry("Info", 'I', [&]() { labels[0].set_text("info"); }, hot_key('I', state::system)),
    menu_entry("Exit", 'x', util::bind_method(this, &my_main_window::quit), hot_key(keys::f4, state::alt), true)
  });
  file_sub_menu.data.register_hot_keys(this);

  select_sub_menu.data.add_entries({
    menu_entry( "item 1", '1', [&]() { labels[0].set_text("item 1"); }),
    menu_entry("item 2", '2', [&]() { labels[0].set_text("item 2"); }),
    sub_menu_entry("item 3", '3', [&]() {
      labels[0].set_text("item 3...");
      sub_sub_menu.popup(select_sub_menu);
    }),
    sub_menu_entry("item 4", '4', [&]() {
      labels[0].set_text("item 4...");
      sub_sub_menu4.popup(select_sub_menu);
    })
  });
  sub_sub_menu.data.add_entries({
    menu_entry("item 3-1", '1', [&]() { labels[0].set_text("item 3-1"); }),
    menu_entry("item 3-2", '2', [&]() { labels[0].set_text("item 3-2"); }),
    sub_menu_entry("item 3-3", '3', [&]() {
      labels[0].set_text("item 3-3...");
      sub_sub_menu3.popup(sub_sub_menu);
    }),
    menu_entry("item 3-4", '4', [&]() { labels[0].set_text("item 3-4"); })
  });
  sub_sub_menu3.data.add_entry(
    menu_entry("item 3-3-1", '1', [&]() { labels[0].set_text("item 3-3-1"); })
  );
  sub_sub_menu4.data.add_entry(
    menu_entry("item 4-1", '1', [&]() { labels[0].set_text("item 4-1"); })
  );

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&] () {
      labels[0].set_text("about");
      message_dialog::show(*this, "About", "BorderTest Version 0.0.1", "ok");
    })
  );

  menu.create(top_view);
  menu.set_visible();

  const float icn_sz = core::global::scale_from_native<float>(16);
  core::rectangle icon_rect(0, 0, icn_sz, icn_sz);

  auto cut_icon = create_text_pixmap(IF_MSC_ELSE(u8"\x2660", u8"♠"), icon_rect, color::dark_red);
  auto copy_icon = create_text_pixmap(IF_MSC_ELSE(u8"\x2663", u8"♣"), icon_rect, color::dark_blue);
  auto paste_icon = create_text_pixmap(IF_MSC_ELSE(u8"\x2665", u8"♥"), icon_rect, color::dark_green);

  edit_sub_menu.data.add_entry(menu_entry("Cut", 't', util::bind_method(this, &my_main_window::cut), hot_key('X', state::control), false, cut_icon));
  edit_sub_menu.data.add_entry(menu_entry("Copy", 'C', util::bind_method(this, &my_main_window::copy), hot_key('C', state::control), false, copy_icon));
  edit_sub_menu.data.add_entry(menu_entry("Paste", 'P', util::bind_method(this, &my_main_window::paste), hot_key('V', state::control), false, paste_icon));
  edit_sub_menu.data.add_entry(menu_entry("Del", 'D', util::bind_method(this, &my_main_window::del), hot_key(keys::del)));
  edit_sub_menu.data.add_entry(menu_entry("Settings", 'S', util::bind_method(this, &my_main_window::settings), hot_key(), true));
  edit_sub_menu.data.add_entry(menu_entry("Options", 'O', [&]() { labels[0].set_text("options"); }, hot_key(), false, masked_bitmap(), menu_state::disabled));
  edit_sub_menu.data.register_hot_keys(this);

  scale_sub_menu.data.add_entries({
    menu_entry{"Scale 0.5", '5', [&]() {set_scale(0.5); }},
    menu_entry{"Scale 0.666", '6', [&] () {set_scale(0.6666); }},
    menu_entry{"Scale 0.75", '7', [&] () {set_scale(0.75); }},
    menu_entry{"Scale 1", '1', [&] () {set_scale(1.0); }},
    menu_entry{"Scale 1.25", '8', [&] () {set_scale(1.25); }},
    menu_entry{"Scale 1.5", '5', [&] () {set_scale(1.5); }},
    menu_entry{"Scale 1.75", '4', [&] () {set_scale(1.75); }},
    menu_entry{"Scale 2", '2', [&] () {set_scale(2.0); }},
    menu_entry{"Scale 3", '3', [&] () {set_scale(3.0); }},
  });

  tool_bar.create(top_view);
  tool_bar.set_background(nero);

  gui::win::global::register_hot_key(hot_key(keys::f7), util::bind_method(this, &my_main_window::test_rgb), this);

  int i = 0;
  for (tool_bar_button& b : buttons) {
    b.create(tool_bar);
    tool_bar.get_layout().add(layout::lay(b));
    if (i % 4 == 3) {
      separators[i / 4].create(tool_bar);
      tool_bar.get_layout().add(layout::lay(separators[i / 4]), true);
    }
    b.on_hilite_changed([&, i](bool b) {
      labels[3].set_text(ostreamfmt("button " << i << (b ? " " : " un") << "hilited"));
    });
    ++i;
  }

  buttons[0].set_text("cut");
  buttons[0].on_clicked(util::bind_method(this, &my_main_window::cut));
  buttons[1].set_text("copy");
  buttons[1].on_clicked(util::bind_method(this, &my_main_window::copy));
  buttons[2].set_text("paste");
  buttons[2].on_clicked(util::bind_method(this, &my_main_window::paste));
  buttons[3].set_text("rgb");
  buttons[3].on_clicked(util::bind_method(this, &my_main_window::test_rgb));
  buttons[4].set_text("bin");
  buttons[4].on_clicked(util::bind_method(this, &my_main_window::save_all_bin));
  buttons[5].set_text("ascii");
  buttons[5].on_clicked(util::bind_method(this, &my_main_window::save_all_ascii));
  buttons[6].set_text("src");
  buttons[6].on_clicked(util::bind_method(this, &my_main_window::save_all_src));
  buttons[7].set_text("start");
  buttons[7].on_clicked(util::bind_method(this, &my_main_window::start_thread));
  buttons[8].set_text("stop");
  buttons[8].on_clicked(util::bind_method(this, &my_main_window::stop_thread));

  for (i = 9; i < 10; ++i) {
    buttons[i].set_text(ostreamfmt(i));
  }

  status_bar.create(*this);
  status_bar.set_background(color::rgb_gray<224>::value);

  i = 1;
  for (StatusLabel& l : labels) {
    l.create(status_bar);
    status_bar.get_layout().add(layout::lay(l));
    l.set_text(ostreamfmt("Status " << i++));
  }

  right_view.create(*this, core::rectangle(0, 0, 200, 400));
  simple_tree::type root;
  root.label = "root";
  typedef ctrl::tree::node node;
  root.add_nodes({
    node("leaf 1"),
    node("sub 2", {
      node("sub 2.1", {
        node("leaf 2.1.1"),
        node("leaf 2.1.2")
      }),
      node("sub 2.2", {
        node("leaf 2.2.1"),
        node("leaf 2.2.2"),
        node("leaf 2.2.3")
      }),
      node("leaf 2.3")
    }),
    node("leaf 3")
  });
  right_view.first.set_root(root);
  root.label = "root2";
  right_view.first.add_root(std::move(root));
//  right_view.first.open_all();
  right_view.first.update_node_list();
  right_view.second.set_roots(fs::get_all_root_file_infos());

  right_view.second.update_node_list();
  right_view.set_visible();

  struct mylist_data : public list_data {

    std::size_t size () const override {
      return 10;
    }

    void draw_at (std::size_t idx,
                  draw::graphics& g,
                  const core::rectangle& place,
                  const draw::brush&,
                  item_state state) const override {
      g.fill(draw::rectangle(place), state.is_selected() ? color::dark_red
                                                         : (state.is_hilited() ? color::very_light_gray
                                                                               : color::light_gray));
      if (!(state.is_selected())) {
        frame::raised_relief(g, place);
      }
      g.text(text_box(ostreamfmt("Item " << idx), place, text_origin_t::center), font::system_bold(), state.is_selected() ? color::light_yellow : color::black);
    }

  };

  left_list.create(*this);
  left_list->set_data(mylist_data());
  left_list->on_hilite_changed([&](bool){
    labels[0].set_text(ostreamfmt("list item " << left_list->get_hilite() << " hilited"));
  });
  left_list->on_selection_changed([&](event_source){
    labels[0].set_text(ostreamfmt("list item " << left_list->get_hilite() << " selected"));
  });
  left_list->on_selection_commit([&](){
    labels[0].set_text(ostreamfmt("list item " << left_list->get_hilite() << " commited"));
  });

  client_view.create(*this, core::rectangle(100, 40, 100, 100));
  client_view.set_background(color::rgb_gray<224>::value);

  window1.on_right_btn_up([&](gui::os::key_state, const core::native_point& pt){
    last_mouse_point = surface_to_screen(pt);
    edit_sub_menu.popup_at(window1.screen_to_client(last_mouse_point), window1);
  });

  window1.create(client_view, core::rectangle(69, 40, 600, 400));
  window1.set_visible();

  hsegmented1.add_buttons({"first", "second", "third", "fourth"});
  hsegmented1.create(client_view, core::rectangle(75, 16, 300, 25));
  hsegmented1.set_visible();

  hsegmented2.add_buttons({"eins", "zwei", "drei", "vier"});
  hsegmented2.create(client_view, core::rectangle(75, 439, 300, 25));
  hsegmented2.set_visible();

  hsegmented2.get_button(0)->on_clicked([&]() {
    hsegmented1.get_button(0)->disable();
    hsegmented2.get_button(3)->disable();
    vsegmented1.get_button(1)->disable();
    vsegmented2.disable();
  });

  hsegmented2.get_button(1)->on_clicked([&]() {
    hsegmented1.get_button(0)->enable();
    hsegmented2.get_button(3)->enable();
    vsegmented1.get_button(1)->enable();
    vsegmented2.enable();
  });

  vsegmented1.add_buttons({"one", "two", "three", "four"});
  vsegmented1.create(client_view, core::rectangle(10, 40, 60, 60));
  vsegmented1.set_visible();

  vsegmented2.add_buttons({"erster", "zweiter", "driter", "vierter"});
  vsegmented2.create(client_view, core::rectangle(700, 40, 60, 60));
  vsegmented2.set_visible();

  for (int i = 0; i < 4; ++i) {
    vsegmented1.get_button(i)->on_clicked([&, i]() {
      labels[1].set_text(ostreamfmt("vsegment " << i << " selected"));
    });
    vsegmented1.get_button(i)->on_hilite_changed([&, i](bool b) {
      labels[1].set_text(ostreamfmt("vsegment " << i << (b ? " " : " un") << "hilited"));
    });
    hsegmented1.get_button(i)->on_clicked([&, i]() {
      labels[2].set_text(ostreamfmt("hsegment " << i << " selected"));
    });
    hsegmented1.get_button(i)->on_hilite_changed([&, i](bool b) {
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

  top_view.get_layout().add({layout::lay(menu), layout::lay(tool_bar)});
  get_layout().set_center_top_bottom_left_right(layout::lay(client_view),
                                                layout::lay(top_view),
                                                layout::lay(status_bar),
                                                layout::lay(left_list),
                                                layout::lay(right_view));
}

//-----------------------------------------------------------------------------
void my_main_window::quit () {
  labels[0].set_text("quit");

  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (win::overlapped_window&, bool yes) {
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

  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (win::overlapped_window&, const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      gui::draw::basic_datamap img;
      gui::io::load_pnm(file.string(), img);
      rgbas[0] = img.convert<pixel_format_t::RGBA>();
      rgbs[0] = img.convert<pixel_format_t::RGB>();
      grays[0] = img.convert<pixel_format_t::GRAY>();
      bws[0] = img.convert<pixel_format_t::BW>();
      window1.invalidate();
    }
  });
}

void my_main_window::save_as () {
  labels[0].set_text("save");

  file_save_dialog::show(*this, "Save File", "new_file.h", "Name:", "Save", "Cancel", [&] (win::overlapped_window& dlg, const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      yes_no_dialog::ask(dlg, "Question!",
                         ostreamfmt("File '" << file << "' already exists!\nDo you want to overwrite the exiting file?"),
                         "Yes", "No", [&] (win::overlapped_window&, bool yes) {
        if (yes) {
          gui::io::src::save_pnm_src(file.string(), grays[0], "src");
        }
        take_focus();
      });
    } else {
      gui::io::src::save_pnm_src(file.string(), grays[0], "src");
      take_focus();
    }
  });
}

struct finally {
  const sys_fs::path& file;
  std::ofstream& stream;

  finally (const sys_fs::path& file, std::ofstream& stream)
    : file(file)
    , stream(stream)
  {}

  ~finally () {
    stream.close();
    sys_fs::remove(file);
  }
};

//-----------------------------------------------------------------------------
class progress_dialog : public standard_dialog<win::group_window<layout::border::layouter<20, 15, 15, 15>>> {
public:
  typedef ctrl::progress_bar progress_view_type;
  typedef win::group_window<layout::border::layouter<20, 15, 15, 15>> content_view_type;
  typedef standard_dialog<content_view_type> super;

  progress_dialog () {
    content_view.get_layout().set_center(layout::lay(progress_view));
  }

  void create (win::overlapped_window& parent,
               const std::string& title,
               const std::string& message,
               const std::string& ok_label,
               const core::rectangle& rect) {
    super::create(parent, title, rect, [&] (win::overlapped_window&, int) {
      end_modal();
    }, { ok_label });
    progress_view.create(content_view, message, rect);
  }

  progress_view_type progress_view;

};


void wipe_disk (const sys_fs::path& file, progress_dialog& dlg, uintmax_t space, volatile bool& active) {
  try {
    std::ofstream out(file.string());
    finally on_exit(file, out);
    std::vector<char> buffer(10 * 1024 * 1024, 0);
    uintmax_t written = 0;
    while (active && !out.write(buffer.data(), buffer.size()).fail()) {
      written += buffer.size();
      dlg.progress_view.set_value((float)((double)written / (double)space));
    }
  } catch (std::exception& ex) {
    message_dialog::show(win::global::get_application_main_window(), "BorderTest - Error", ex.what(), "Okay");
  }
  dlg.end_modal();
}

void my_main_window::wipe_space () {
  sys_fs::current_path("C:\\");
  dir_open_dialog::show(*this, "Choose target directory", "Wipe", "Cancel", [&] (win::overlapped_window&, const sys_fs::path& dir) {
    sys_fs::path file = /*"C:\\";*/ dir;
    file /= "empty.tmp";
    win::run_on_main(*this, [&, file] () {
      progress_dialog dlg;
      const uintmax_t space = sys_fs::space(file).free;
      dlg.progress_view.set_min_max_value(0, 1, 0);
      dlg.create(*this, "Wipe disk", "", "Abort", core::rectangle(300, 200, 400, 170));
      volatile bool active = true;
      std::thread t([&] () { wipe_disk(file, dlg, space, active); });
      dlg.show(*this);
      active = false;
      t.join();
    });
  });
}

void my_main_window::copy () {
  labels[0].set_text("copy");
  core::rectangle r = left_list.client_geometry();
  core::size sz = r.size();

  rgbas[0].create(sz);
  rgbs[0].create(sz);
  grays[0].create(sz);
  bws[0].create(sz);

  pixmap img(sz);
  // TODO: implement grap for window
//  draw::graphics(img).copy_from(left_list, r);
  rgbas[0] = img;
  rgbs[0] = img;
  grays[0] = img;
  bws[0] = img;

  window1.invalidate();
}

void my_main_window::settings () {
    core::rectangle r = window1.client_geometry();
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
    rgbas[i].clear();
    rgbs[i].clear();
    grays[i].clear();
    bws[i].clear();
  }
  window1.invalidate();
}

void my_main_window::cut () {
  labels[0].set_text("cut");

  core::rectangle r = left_list.client_geometry();
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
  // TODO: implement grap for window
//  draw::graphics(img).copy_from(left_list, r);
  rgbas[0] = img;
  rgbs[0] = img;
  grays[0] = img;
  bws[0] = img;

  std::ofstream("left_list_rgba.p6.ppm") << io::opnm<true, pixel_format_t::RGBA>(rgbas[0]);
  std::ofstream("left_list_rgba.p3.ppm") << io::opnm<false, pixel_format_t::RGBA>(rgbas[0]);

  io::ofpnm<io::PNM::P6>("left_list.p6") << rgbs[0];
  io::ofpnm<io::PNM::P3>("left_list.p3") << rgbs[0];

  io::ofpnm<io::PNM::P5>("left_list.p5") << grays[0];
  io::ofpnm<io::PNM::P2>("left_list.p2") << grays[0];

  io::ofpnm<io::PNM::P4>("left_list.p4") << bws[0];
  io::ofpnm<io::PNM::P1>("left_list.p1") << bws[0];

  rgbas[1].create(sz);
  rgbs[1].create(sz);
  grays[1].create(sz);
  bws[1].create(sz);

  rgbas[1] = drawer(sz);

  std::ofstream("p6-rgba.ppm") << io::opnm<true, pixel_format_t::RGBA>(rgbas[1]);
  std::ofstream("p3-rgba.ppm") << io::opnm<false, pixel_format_t::RGBA>(rgbas[1]);

  rgbs[1] = drawer(sz);

  io::ofpnm<io::PNM::P6>("p6") << rgbs[1];
  io::ofpnm<io::PNM::P3>("p3") << rgbs[1];

  grays[1] = drawer(sz);

  io::ofpnm<io::PNM::P5>("p5") << grays[1];
  io::ofpnm<io::PNM::P2>("p2") << grays[1];

  bws[1] = drawer(sz);

  io::ofpnm<io::PNM::P4>("p4") << bws[1];
  io::ofpnm<io::PNM::P1>("p1") << bws[1];

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
    clog::fatal() << ex;
  }
}

template<io::PNM P>
void read_write_rgba (datamap<pixel_format_t::RGBA>& bm) {
  try {
    int i = static_cast<int>(P);
    std::string iname = ostreamfmt("p" << i << "-rgba.ppm");
    std::string oname = ostreamfmt("test.p" << i << "-rgba.ppm");
    io::ipnm<pixel_format_t::RGBA> ip(bm);
    std::ifstream(iname) >> ip;
    std::ofstream(oname) << io::opnm<io::PNM2BPP<P>::bin, pixel_format_t::RGBA>(bm);
  }
  catch (std::exception& ex) {
    clog::fatal() << ex;
  }
}

void my_main_window::paste () {
  labels[0].set_text("paste");

  read_write<io::PNM::P1>(bws[0]);
  read_write<io::PNM::P2>(grays[0]);
  read_write<io::PNM::P3>(rgbs[0]);
  read_write<io::PNM::P4>(bws[1]);
  read_write<io::PNM::P5>(grays[1]);
  read_write<io::PNM::P6>(rgbs[1]);

  read_write_rgba<io::PNM::P3>(rgbas[0]);
  read_write_rgba<io::PNM::P6>(rgbas[1]);

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

  bws[0] = red;
  bws[1] = bwmap(red24);

  io::ifpnm<io::PNM::P3>("green32") >> green;
  io::ifpnm<io::PNM::P3>("green24") >> green24;

  grays[0] = green;
  grays[1] = graymap(green24);

  io::ifpnm<io::PNM::P3>("blue32") >> blue;
  io::ifpnm<io::PNM::P3>("blue24") >> blue24;

  rgbs[0] = blue;
  rgbs[1] = blue24;

  rgbas[0] = blue;
  rgbas[1] = rgbamap(blue24);

  window1.invalidate();
}

void my_main_window::save_all_bin () {
  ctrl::dir_open_dialog::show(*this, "Choose target directory", "Save", "Cancel", [&] (win::overlapped_window&, const sys_fs::path& file) {
    sys_fs::current_path(file);
    std::ofstream("rgba0.b.ppm") << io::opnm<true, pixel_format_t::RGBA>(rgbas[0]);
    std::ofstream("rgba1.b.ppm") << io::opnm<true, pixel_format_t::RGBA>(rgbas[1]);
    io::ofpnm<io::PNM::P6>("bmp0.b")  << rgbs[0];
    io::ofpnm<io::PNM::P6>("bmp1.b")  << rgbs[1];
    io::ofpnm<io::PNM::P5>("gray0.b") << grays[0];
    io::ofpnm<io::PNM::P5>("gray1.b") << grays[1];
    io::ofpnm<io::PNM::P4>("bw0.b")   << bws[0];
    io::ofpnm<io::PNM::P4>("bw1.b")   << bws[1];
  });
}

void my_main_window::save_all_ascii() {
  std::ofstream("rgba0.a.ppm") << io::opnm<false, pixel_format_t::RGBA>(rgbas[0]);
  std::ofstream("rgba1.a.ppm") << io::opnm<false, pixel_format_t::RGBA>(rgbas[1]);
  io::ofpnm<io::PNM::P3>("bmp0.a") << rgbs[0];
  io::ofpnm<io::PNM::P3>("bmp1.a") << rgbs[1];
  io::ofpnm<io::PNM::P2>("gray0.a") << grays[0];
  io::ofpnm<io::PNM::P2>("gray1.a") << grays[1];
  io::ofpnm<io::PNM::P1>("bw0.a") << bws[0];
  io::ofpnm<io::PNM::P1>("bw1.a") << bws[1];
}

void my_main_window::save_all_src () {
  io::src::save_pnm_src("bmp0.h", rgbs[0], "bmp0");
  io::src::save_pnm_src("bmp1.h", rgbs[1], "bmp1");
  io::src::save_pnm_src("gray0.h", grays[0], "gray0");
  io::src::save_pnm_src("gray1.h", grays[1], "gray1");
  io::src::save_pnm_src("bw0.h", bws[0], "bw0");
  io::src::save_pnm_src("bw1.h", bws[1], "bw1");
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
//  gui::core::global::set_scale_factor(1.0);
  my_main_window main;

  main.create({50, 50, 800, 600});
  main.set_title("Border Test");
  main.set_visible();

  return win::run_main_loop();
}
