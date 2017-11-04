
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
#include <gui/core/time_util.h>
#include <gui/win/window.h>
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

#define NO_EXPORT

DEFINE_LOGGING_CORE(NO_EXPORT)


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

private:
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
  typedef win::sorted_file_tree file_tree;
  win::horizontal_split_view<simple_tree, file_tree> right_view;

  group_window<attach, color::rgb_gray<224>::value> client_view;

  htoggle_group<color::black, color::light_gray> segmented;
  vtoggle_group<color::light_gray, color::gray> vsegmented;

  client_window window1;
  rgbamap rgba[2];
  rgbmap bmp[2];
  graymap gray[2];
  maskmap bw[2];
};

// --------------------------------------------------------------------------
my_main_window::my_main_window ()
  : super(40, 30, 100, 250)
  , left_list(50, color::rgb_gray<224>::value)
  , right_view(simple_tree(20, color::very_light_gray), file_tree(20, color::very_light_gray))
{
//  right_view.first.set_item_size_and_background(20, color::very_light_gray);
//  right_view.second.set_item_size_and_background(20, color::very_light_gray);
  register_event_handler(REGISTER_FUNCTION, win::create_event(this, &my_main_window::onCreated));

  register_event_handler(REGISTER_FUNCTION, win::destroy_event([&]() {
    LogDebug << "Destroyed!";
    win::quit_main_loop();
  }));

  register_event_handler(REGISTER_FUNCTION, win::close_event(core::bind_method(this, &my_main_window::quit)));

  window1.register_event_handler(REGISTER_FUNCTION, paint_event([&](const graphics& graph){
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
      try {
        if (rgba[i]) {
          graph.copy_from(rgba[i], core::point(x, 1));
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
  menu.set_visible();

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'O', core::bind_method(this, &my_main_window::open), hot_key('O', state::control)),
    menu_entry("Sace As", 'S', core::bind_method(this, &my_main_window::save_as), hot_key('S', state::control)),
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
  file_sub_menu.data.register_hot_keys(this);

  core::rectangle icon_rect(0, 0, 16, 16);
  memmap cut_icon(16, 16);
  memmap copy_icon(16, 16);
  memmap paste_icon(16, 16);
  graphics(cut_icon).clear(color::transparent).text(text_box(u8"♠", icon_rect, text_origin::center), font::menu(), color::dark_red);
  graphics(copy_icon).clear(color::transparent).text(text_box(u8"♣", icon_rect, text_origin::center), font::menu(), color::dark_blue);
  graphics(paste_icon).clear(color::transparent).text(text_box(u8"♥", icon_rect, text_origin::center), font::menu(), color::dark_green);

  edit_sub_menu.data.add_entry(menu_entry("Cut", 't', core::bind_method(this, &my_main_window::cut), hot_key('X', state::control), false, cut_icon));
  edit_sub_menu.data.add_entry(menu_entry("Copy", 'C', core::bind_method(this, &my_main_window::copy), hot_key('C', state::control), false, copy_icon));
  edit_sub_menu.data.add_entry(menu_entry("Paste", 'P', core::bind_method(this, &my_main_window::paste), hot_key('V', state::control), false, paste_icon));
  edit_sub_menu.data.add_entry(menu_entry("Del", 'D', core::bind_method(this, &my_main_window::del), hot_key(keys::del)));
  edit_sub_menu.data.add_entry(menu_entry("Settings", 'S', [&]() { labels[0].set_text("settings"); }, hot_key(), false, memmap(), menu_state::disabled));
  edit_sub_menu.data.add_entry(menu_entry("Options", 'O', [&]() { labels[0].set_text("options"); }, hot_key(), true));
  edit_sub_menu.data.register_hot_keys(this);

  tool_bar.create(top_view);

  global::register_hot_key(hot_key(keys::f7), core::bind_method(this, &my_main_window::test_rgb), this);

  int i = 0;
  for (tool_bar_button& b : buttons) {
    b.create(tool_bar);
    if (i % 4 == 3) {
      separators[i / 4].create(tool_bar);
      tool_bar.get_layout().add_separator(&(separators[i / 4]));
    }
    b.register_event_handler(REGISTER_FUNCTION, hilite_changed_event([&, i](bool b) {
      labels[3].set_text(ostreamfmt("button " << i << (b ? " " : " un") << "hilited"));
    }));
    ++i;
  }

  buttons[0].set_text("cut");
  buttons[0].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::cut));
  buttons[1].set_text("copy");
  buttons[1].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::copy));
  buttons[2].set_text("paste");
  buttons[2].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::paste));
  buttons[3].set_text("rgb");
  buttons[3].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::test_rgb));
  buttons[4].set_text("bin");
  buttons[4].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::save_all_bin));
  buttons[5].set_text("ascii");
  buttons[5].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::save_all_ascii));
  buttons[6].set_text("src");
  buttons[6].register_event_handler(REGISTER_FUNCTION, button_clicked_event(this, &my_main_window::save_all_src));

  for (i = 7; i < 10; ++i) {
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
  left_list.set_drawer([](std::size_t idx,
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
  left_list.register_event_handler(REGISTER_FUNCTION, hilite_changed_event([&](bool){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " hilited"));
  }));
  left_list.register_event_handler(REGISTER_FUNCTION, selection_changed_event([&](event_source){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " selected"));
  }));
  left_list.register_event_handler(REGISTER_FUNCTION, selection_commit_event([&](){
    labels[0].set_text(ostreamfmt("list item " << left_list.get_hilite() << " commited"));
  }));

  client_view.create(*this);

  window1.register_event_handler(REGISTER_FUNCTION, right_btn_up_event([&](os::key_state, const core::point& pt){
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
    vsegmented.get_button(i).register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, i]() {
      labels[1].set_text(ostreamfmt("vsegment " << i << " selected"));
    }));
    vsegmented.get_button(i).register_event_handler(REGISTER_FUNCTION, hilite_changed_event([&, i](bool b) {
      labels[1].set_text(ostreamfmt("vsegment " << i << (b ? " " : " un") << "hilited"));
    }));
    segmented.get_button(i).register_event_handler(REGISTER_FUNCTION, button_clicked_event([&, i]() {
      labels[2].set_text(ostreamfmt("hsegment " << i << " selected"));
    }));
    segmented.get_button(i).register_event_handler(REGISTER_FUNCTION, hilite_changed_event([&, i](bool b) {
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

//-----------------------------------------------------------------------------
typedef void (yes_no_action) (bool);
void yes_no_dialog (win::container& parent,
                    const std::string& title,
                    const std::string& message,
                    const std::string& yes_label,
                    const std::string& no_label,
                    const std::function<yes_no_action>& action);

//-----------------------------------------------------------------------------
typedef void (file_selected) (const sys_fs::path&);
void file_open_dialog (win::container& parent,
                       const std::string& title,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       const std::function<file_selected>& action);

void file_save_dialog (win::container& parent,
                       const std::string& title,
                       const std::string& default_name,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       const std::function<file_selected>& action);

//-----------------------------------------------------------------------------
void yes_no_dialog (win::container& parent,
                    const std::string& title,
                    const std::string& message,
                    const std::string& yes_label,
                    const std::string& no_label,
                    const std::function<yes_no_action>& action) {

  layout_dialog_window<layout::border_layout<>, float, float, float, float> dialog(0, 45, 0, 0);
  group_window<layout::border_layout<>, color::very_light_gray, float, float, float, float> content_view(20, 15, 15, 15);
  group_window<horizontal_adaption<>, color::very_light_gray> buttons;
  basic_textbox<text_origin::center, draw::frame::sunken_relief, color::black, color::very_light_gray> message_view;
  text_button yes, no;

  yes.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&](){
    dialog.end_modal();
    if (action) {
      action(true);
    }
  }));
  no.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&](){
    dialog.end_modal();
    if (action) {
      action(false);
    }
  }));

  dialog.register_event_handler(REGISTER_FUNCTION, set_focus_event([&](window*){ yes.take_focus(); }));

  dialog.create(parent, core::rectangle(300, 200, 400, 170));
  dialog.set_title(title);
  buttons.create(dialog);
  content_view.create(dialog);
  message_view.create(content_view, message);
  no.create(buttons, no_label);
  yes.create(buttons, yes_label);
  dialog.get_layout().set_center_top_bottom_left_right(&content_view, nullptr, &buttons, nullptr, nullptr);
  content_view.get_layout().set_center(&message_view);
  dialog.set_children_visible();
  content_view.set_children_visible();
  dialog.set_visible();
  parent.disable();
  dialog.run_modal({ hot_key_action{ hot_key(keys::escape, state::none), [&] () {
    dialog.end_modal();
  }}});
  parent.enable();
  parent.take_focus();
}

//-----------------------------------------------------------------------------
void file_open_dialog (win::container& parent,
                       const std::string& title,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       const std::function<file_selected>& action) {
  layout_dialog_window<layout::border_layout<>, float, float, float, float> dialog(0, 45, 0, 0);
  group_window<horizontal_adaption<>, color::light_gray> buttons;

  text_button open, cancel;

  typedef sorted_file_tree dir_tree_type;
  typedef file_list file_list_type;

  win::vertical_split_view<dir_tree_type, file_list_type> main_view;

  dir_tree_type& dir_tree = main_view.first;
  file_list_type& files = main_view.second;

  dir_tree.root =
#ifdef WIN32
    sys_fs::path("c:\\");
#endif // WIN32
#ifdef X11
    sys_fs::path("/");
#endif // X11

  dir_tree.update_node_list();

  dir_tree.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](event_source) {
    int idx = dir_tree.get_selection();
    if (idx > -1) {
      files.set_path(dir_tree.get_item(idx));
    }
  }));

  open.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&](){
    dialog.end_modal();
    action(files.get_selected_path());
  }));
  cancel.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&](){
    dialog.end_modal();
  }));

  dialog.create(parent, core::rectangle(300, 200, 600, 400));
  dialog.set_title(title);

  main_view.create(dialog, core::rectangle(0, 0, 600, 300));
  main_view.set_split_pos(0.3);

  dir_tree.open_root();
  dir_tree.update_node_list();

  buttons.create(dialog);
  cancel.create(buttons, cancel_label);
  open.create(buttons, ok_label);

  dialog.get_layout().set_center_top_bottom_left_right(&main_view, nullptr, &buttons, nullptr, nullptr);

  dialog.set_children_visible();
  dialog.set_visible();
  parent.disable();
  dialog.run_modal();
  parent.enable();
  parent.take_focus();

}

//-----------------------------------------------------------------------------
void file_save_dialog (win::container& parent,
                       const std::string& title,
                       const std::string& default_name,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       const std::function<file_selected>& action) {
  layout_dialog_window<layout::border_layout<>, float, float, float, float> dialog(25, 45, 0, 0);
  group_window<horizontal_adaption<>, color::light_gray> buttons;

  group_window<layout::border_layout<>, color::very_light_gray, float, float, float, float> top_view(1, 2, 1, 2);
  edit input_line;

  text_button open, cancel;

  typedef sorted_file_tree dir_tree_type;
  typedef file_list file_list_type;

  win::vertical_split_view<dir_tree_type, file_list_type> main_view;

  dir_tree_type& dir_tree = main_view.first;
  file_list_type& files = main_view.second;

  dir_tree.root =
#ifdef WIN32
    sys_fs::path("c:\\");
#endif // WIN32
#ifdef X11
    sys_fs::path("/");
#endif // X11

  dir_tree.update_node_list();

  dir_tree.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](event_source) {
    int idx = dir_tree.get_selection();
    if (idx > -1) {
      files.set_path(dir_tree.get_item(idx));
    }
  }));

  files.list.register_event_handler(REGISTER_FUNCTION, win::selection_changed_event([&](event_source) {
    input_line.set_text(files.get_selected_path().filename().string());
  }));

  open.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&](){
    dialog.end_modal();
    int idx = dir_tree.get_selection();
    if (idx > -1) {
      sys_fs::path path = dir_tree.get_item(idx);
      path /= input_line.get_text();
      action(path);
    }
  }));

  cancel.register_event_handler(REGISTER_FUNCTION, button_clicked_event([&](){
    dialog.end_modal();
  }));

  dialog.create(parent, core::rectangle(300, 200, 600, 400));
  dialog.set_title(title);

  top_view.create(dialog, core::rectangle(0, 0, 100, 100));
  top_view.get_layout().set_center(&input_line);

  input_line.create(top_view, core::rectangle(0, 0, 100, 100));

  main_view.create(dialog, core::rectangle(0, 0, 600, 100));
  main_view.set_split_pos(0.3);

  dir_tree.open_root();
  dir_tree.update_node_list();

  buttons.create(dialog);
  cancel.create(buttons, cancel_label);
  open.create(buttons, ok_label);

  dialog.get_layout().set_center_top_bottom_left_right(&main_view, &top_view, &buttons, nullptr, nullptr);

  input_line.set_text(default_name);

  dialog.set_children_visible();
  dialog.set_visible();
  parent.disable();
  dialog.run_modal();
  parent.enable();
  parent.take_focus();

}

//-----------------------------------------------------------------------------
void my_main_window::quit () {
  labels[0].set_text("quit");

  yes_no_dialog(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}

//-----------------------------------------------------------------------------
void my_main_window::open () {
  labels[0].set_text("open");

  file_open_dialog(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      gui::draw::bitmap img;
      gui::io::load_pnm(file.string(), img);
      rgba[0] = img;
      bmp[0] = img;
      gray[0] = img;
      bw[0] = img;
      window1.redraw_later();
    }
  });
}

void my_main_window::save_as () {
  labels[0].set_text("save");

  file_save_dialog(*this, "Save File", "new_file.h", "Save", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      yes_no_dialog(*this, "Question!",
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

void my_main_window::copy () {
  labels[0].set_text("copy");
  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  rgba[0].create(sz);
  bmp[0].create(sz);
  gray[0].create(sz);
  bw[0].create(sz);

  memmap img(sz);
  draw::graphics(img).copy_from(left_list, r);
  rgba[0].put(img);
  bmp[0].put(img);
  gray[0].put(img);
  bw[0].put(img);

  window1.redraw_later();
}

void my_main_window::del () {
  labels[0].set_text("del");
  for (int i = 0; i < 2; ++i) {
    rgba[i].clear();
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
  rgba[0] = img;
  bmp[0] = img;
  gray[0] = img;
  bw[0] = img;

  std::ofstream("left_list_rgba.p6.ppm") << io::opnm<true>(rgba[0]);
  std::ofstream("left_list_rgba.p3.ppm") << io::opnm<false>(rgba[0]);

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

  std::ofstream("p6-rgba.ppm") << io::opnm<true>(rgba[1]);
  std::ofstream("p3-rgba.ppm") << io::opnm<false>(rgba[1]);

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

template<io::PNM P>
void read_write (datamap<BPP::RGBA>& bm) {
  try {
    int i = static_cast<int>(P);
    std::string iname = ostreamfmt("p" << i << "-rgba.ppm");
    std::string oname = ostreamfmt("test.p" << i << "-rgba.ppm");
    io::ipnm ip(bm);
    std::ifstream(iname) >> ip;
    std::ofstream(oname) << io::opnm<io::PNM2BPP<P>::bin>(bm);
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

  read_write<io::PNM::P3>(rgba[0]);
  read_write<io::PNM::P6>(rgba[1]);

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

  rgba[0] = blue;
  rgba[1] = blue24;

  window1.redraw_later();
}

void my_main_window::save_all_bin () {
  std::ofstream("rgba0.b.ppm") << io::opnm<true>(rgba[0]);
  std::ofstream("rgba1.b.ppm") << io::opnm<true>(rgba[1]);
  io::ofpnm<io::PNM::P6>("bmp0.b")  << bmp[0];
  io::ofpnm<io::PNM::P6>("bmp1.b")  << bmp[1];
  io::ofpnm<io::PNM::P5>("gray0.b") << gray[0];
  io::ofpnm<io::PNM::P5>("gray1.b") << gray[1];
  io::ofpnm<io::PNM::P4>("bw0.b")   << bw[0];
  io::ofpnm<io::PNM::P4>("bw1.b")   << bw[1];
}

void my_main_window::save_all_ascii() {
  std::ofstream("rgba0.a.ppm") << io::opnm<false>(rgba[0]);
  std::ofstream("rgba1.a.ppm") << io::opnm<false>(rgba[1]);
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
