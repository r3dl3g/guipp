
#include "window.h"
#include "label.h"
#include "menu.h"
#include "scroll_view.h"
#include "toggle_group.h"
#include "graphics.h"
#include "bitmap.h"
#include "pnm.h"

#include <fstream>

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
  void cut (int);
  void copy (int);
  void paste (int);
  void del (int);
  void quit (int);

private:
  group_window<vertical_adaption<>, color::light_gray> top_view;
  group_window<horizontal_lineup<30, 2, 0>, nero> tool_bar;

  main_menu menu;
  popup_menu edit_sub_menu;

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
  bitmap bmp[2];
  bitmap gray[2];
  bitmap bw[2];

  bitmap cut_icon;
  bitmap copy_icon;
  bitmap paste_icon;
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

    int x = 1;
    for (int i = 0; i < 2; ++i) {
      try {
        if (bw[i]) {
          graph.copy_from(bw[i], core::rectangle(bw[i].size()), core::point(x, 1));
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }
      x += 110;
      try {
        if (gray[i]) {
          graph.copy_from(gray[i], core::rectangle(gray[i].size()), core::point(x, 1));
        }
      } catch (std::exception& ex) {
        LogFatal << ex;
      }
      x += 110;
      try {
        if (bmp[i]) {
          graph.copy_from(bmp[i], core::rectangle(bmp[i].size()), core::point(x, 1));
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
    main_menu_entry("File", [&](int i) {
      labels[0].set_text("File...");
      popup_menu file_sub_menu;
      file_sub_menu.data.add_entries({
        menu_entry("open", [&](int) { labels[0].set_text("open"); }, "Strg+O"),
        menu_entry("close", [&](int) { labels[0].set_text("close"); } ), 
        sub_menu_entry("select", [&](int i) {
          labels[0].set_text("select...");
          popup_menu select_sub_menu;
          select_sub_menu.data.add_entry(
            menu_entry( "item 1", [&](int) { labels[0].set_text("item 1"); })
          );
          select_sub_menu.data.add_entries({
            menu_entry("item 2", [&](int) { labels[0].set_text("item 2"); }),
            sub_menu_entry("item 3", [&](int) {
              labels[0].set_text("item 3...");
              popup_menu sub_sub_menu;
              sub_sub_menu.data.add_entries({
                menu_entry("item 3-1", [&](int) { labels[0].set_text("item 3-1"); }),
                menu_entry("item 3-2", [&](int) { labels[0].set_text("item 3-2"); }),
                sub_menu_entry("item 3-3", [&](int i) {
                  labels[0].set_text("item 3-3...");
                  popup_menu sub_sub_menu2;
                  sub_sub_menu2.data.add_entry(
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
              sub_sub_menu.data.add_entry(
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
    main_menu_entry("Edit", [&](int i) {
      labels[0].set_text("Edit...");
      edit_sub_menu.popup_at(menu.sub_menu_position(i), menu);
    }),
    main_menu_entry("Window", [&](int) {
      labels[0].set_text("Window...");
    }, true),
    main_menu_entry("Help", [&](int i) {
      labels[0].set_text("Help...");
      popup_menu help_sub_menu;
      help_sub_menu.data.add_entry(
        menu_entry("about", [&](int) { labels[0].set_text("about"); })
      );
      help_sub_menu.popup_at(menu.sub_menu_position(i), menu);
    })
  });
  menu.create(top_view);

  core::rectangle icon_rect(0, 0, 16, 16);
  bitmap cut_icon(16, 16);
  bitmap copy_icon(16, 16);
  bitmap paste_icon(16, 16);
  graphics(cut_icon).clear(color::black).text(text_box(u8"♠", icon_rect, center), font::menu(), color::dark_red);
  graphics(copy_icon).clear(color::black).text(text_box(u8"♣", icon_rect, center), font::menu(), color::dark_blue);
  graphics(paste_icon).clear(color::black).text(text_box(u8"♥", icon_rect, center), font::menu(), color::dark_green);

  edit_sub_menu.data.add_entries({
    menu_entry("cut", core::bind_method(this, &my_main_window::cut), "Strg+X", false, cut_icon),
    menu_entry("copy", core::bind_method(this, &my_main_window::copy), "Strg+C", false, copy_icon),
    menu_entry("paste", core::bind_method(this, &my_main_window::paste), "Strg+V", false, paste_icon),
    menu_entry("del", core::bind_method(this, &my_main_window::del), "del"),
    menu_entry("settings", [&](int) { labels[0].set_text("settings"); }, std::string(), false, bitmap(), true),
    menu_entry("options", [&](int) { labels[0].set_text("options"); }, std::string(), true)
  });

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
    switch (i) {
    case 1:
      b.register_event_handler(button_clicked_event([&, i]() {
        cut(i);
      }));
      break;
    case 2:
      b.register_event_handler(button_clicked_event([&, i]() {
        copy(i);
      }));
      break;
    case 3:
      b.register_event_handler(button_clicked_event([&, i]() {
        paste(i);
      }));
      break;
    default:
      b.register_event_handler(button_clicked_event([&, i]() {
        labels[3].set_text(ostreamfmt("button " << i << " clicked"));
      }));
      break;
    }
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
    g.fill(rectangle(place), selected ? color::dark_red : (hilited ? color::very_light_gray : color::light_gray));
    if (!selected) {
      frame::raised_relief(g, place);
    }
    g.text(text_box(ostreamfmt("Item " << idx), place, center), font::system(), selected ? color::light_yellow : color::black);
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

  get_layout().set_center_top_bottom_left_right(&client_view, &top_view, &status_bar, &left_list, &right_bar);
  set_children_visible();
}

void my_main_window::quit (int) {
  labels[0].set_text("quit");

  layout_dialog_window<layout::border_layout<20, 55, 15, 15>> dialog;
  group_window<horizontal_lineup<80, 15, 10, 2, true>, color::light_gray> buttons;
  labelT<alignment_center, draw::frame::sunken_relief, color::black, color::light_gray> message;
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

  dialog.create(*this, core::rectangle(300, 200, 400, 200));
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

void my_main_window::copy (int) {
  labels[0].set_text("copy");
  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  bmp[0].create(sz);
  gray[0].create(sz, 8);
  bw[0].create(sz, 1);

  draw::graphics(bmp[0]).copy_from(left_list, r);
  gray[0].put(bmp[0]);
  bw[0].put(gray[0]);

  gray[0].make_compatible();
  bw[0].make_compatible();

  window1.redraw_later();
}

void my_main_window::del (int) {
  labels[0].set_text("del");
  for (int i = 0; i < 2; ++i) {
    bmp[i].clear();
    gray[i].clear();
    bw[i].clear();
  }
  window1.redraw_later();
}

void my_main_window::cut (int) {
  labels[0].set_text("cut");

  core::rectangle r = left_list.client_area();
  core::size sz = r.size();

  auto drawer = [&](bitmap& b) {
    graphics g(b);
    g.fill(draw::rectangle(r), color::white);
    g.draw(draw::ellipse(r.shrinked({20, 20})), color::light_blue, draw::pen(color::dark_green, 5));
    g.fill(draw::ellipse(r.with_size({50, 50})), color::black);
  };

  for (int i = 0; i < 2; ++i) {
    bmp[i].create(sz);
    gray[i].create(sz, 8);
    bw[i].create(sz, 1);
  }

  draw::graphics(bmp[0]).copy_from(left_list, r);
  gray[0].put(bmp[0]);
  bw[0].put(gray[0]);

  std::ofstream("left_list.p6.ppm") << io::opnm<true>(bmp[0]);
  std::ofstream("left_list.p3.ppm") << io::opnm<false>(bmp[0]);

  std::ofstream("left_list.p5.pgm") << io::opnm<true>(gray[0]);
  std::ofstream("left_list.p2.pgm") << io::opnm<false>(gray[0]);

  std::ofstream("left_list.p4.pbm") << io::opnm<true>(bw[0]);
  std::ofstream("left_list.p1.pbm") << io::opnm<false>(bw[0]);

  drawer(bmp[1]);

  std::ofstream(ostreamfmt("p6." << io::pnm<6>::suffix)) << io::opnm<true>(bmp[1]);
  std::ofstream(ostreamfmt("p3." << io::pnm<3>::suffix)) << io::opnm<false>(bmp[1]);

  drawer(gray[1]);

  std::ofstream("p5.pgm") << io::opnm<true>(gray[1]);
  std::ofstream("p2.pgm") << io::opnm<false>(gray[1]);

  drawer(bw[1]);

  std::ofstream("p4.pbm") << io::opnm<true>(bw[1]);
  std::ofstream("p1.pbm") << io::opnm<false>(bw[1]);

  gray[0].make_compatible();
  bw[0].make_compatible();

  gray[1].make_compatible();
  bw[1].make_compatible();

  window1.redraw_later();
}

void my_main_window::paste (int) {
  labels[0].set_text("paste");

  for (int i = 0; i < 2; ++i) {

    io::ipnm in1(bw[i]);
    std::ifstream(ostreamfmt("left_list.p" << (i * 3 + 1) << ".pbm")) >> in1;
    std::ofstream(ostreamfmt("test.p" << (i * 3 + 1) << ".pbm")) << io::opnm<false>(bw[i]);

    io::ipnm in2(gray[i]);
    std::ifstream(ostreamfmt("left_list.p" << (i * 3 + 2) << ".pgm")) >> in2;
    std::ofstream(ostreamfmt("test.p" << (i * 3 + 2) << ".pgm")) << io::opnm<false>(gray[i]);

    io::ipnm in3(bmp[i]);
    std::ifstream(ostreamfmt("left_list.p" << (i * 3 + 3) << ".ppm")) >> in3;
    std::ofstream(ostreamfmt("test.p" << (i * 3 + 3) << ".ppm")) << io::opnm<false>(bmp[i]);

    bw[i].make_compatible();
    gray[i].make_compatible();
    bmp[i].make_compatible();

  }

  window1.redraw_later();
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
