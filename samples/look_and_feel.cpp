
#include <algorithm>

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/progress_bar.h>
#include <gui/ctrl/drop_down.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/tree.h>


const std::size_t COLUMNS = 4;

typedef gui::layout::fixed_grid_lineup<120, 22, COLUMNS + 1, 2, 4> layout_t;
typedef gui::win::group_window<layout_t, gui::color::very_very_light_gray> top_grid_t;

template<typename T>
typename std::enable_if<!std::is_base_of<gui::ctrl::button_base, T>::value>::type
set_state (T& t, int pos) {
  if (pos > 1) {
    t.disable();
  }
}

template<typename T>
typename std::enable_if<std::is_base_of<gui::ctrl::button_base, T>::value>::type
set_state (gui::ctrl::button_base& t, int pos) {
  t.set_checked(pos % 2);
  if (pos > 1) {
    t.disable();
  }
}

// --------------------------------------------------------------------------
template<typename T>
void add (top_grid_t& l, T& t, const std::string& label) {
  t.create(l);
  t.set_text(label);
  l.get_layout().add(t);
}

template<>
void add<gui::ctrl::drop_down_list> (top_grid_t& l, gui::ctrl::drop_down_list& t, const std::string& label) {
  t.create(l);
  t.set_data({label, label, label});
  l.get_layout().add(t);
}

template<>
void add<gui::ctrl::horizontal_scroll_bar> (top_grid_t& l, gui::ctrl::horizontal_scroll_bar& t, const std::string& label) {
  t.create(l);
  l.get_layout().add(t);
  t.set_value(0.5);
}

// --------------------------------------------------------------------------
template<typename T>
void add (top_grid_t& l, std::array<T, COLUMNS>& a, const std::string& label) {
  int pos = 0;
  for(auto& t : a) {
    add(l, t, label);
    set_state<T>(t, pos++);
  }
}

template<int H = 0>
class header_layout {
public:
  static constexpr int height = H;
  typedef gui::layout::layout_function layout_function;

  void set_header_body (layout_function header, layout_function body) {
    this->header = header;
    this->body = body;
  }

  void set_header (layout_function header) {
    this->header = header;
  }

  void set_body (layout_function body) {
    this->body = body;
  }

  void layout (const gui::core::rectangle& r) {
    if (header) {
      header(r.with_height(height));
    }
    if (body) {
      body(r.with_vertical(r.y() + height, r.height() - height));
    }
  }

protected:
  layout_function header;
  layout_function body;
};

template<int H>
struct gui::layout::is_layout<header_layout<H>> {
  enum {
    value = true
  };
};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::ctrl;


  typedef layout_main_window<layout::border::layouter<10, 10, 10, 10, layout::border::type_t::all_symmetric>> main_window_t;
  typedef layout::vertical_lineup<236, 0, 8> center_layout_t;
  typedef layout::vertical_lineup<40, 0, 8> bottom_layout_t;
  typedef layout::horizontal_adaption<0, 4, 0, 80, 200> center_grid_t;

  main_window_t main;
  center_layout_t center;
  top_grid_t tgrid;
  center_grid_t cgrid;
  center_grid_t bgrid;

  std::array<label, 9> info;

  label_left leftlabel;
  label_right rightlabel;
  label_center centerlabel;
  label stdlabel;

  std::array<text_button, COLUMNS> buttons;
  std::array<radio_button<>, COLUMNS> radios;
  std::array<check_box<>, COLUMNS> checks;
  std::array<edit, COLUMNS> edits;
  std::array<pass, COLUMNS> passs;
  std::array<progress_bar, COLUMNS> progress_bars;
  std::array<horizontal_scroll_bar, COLUMNS> scroll_bars;
  std::array<drop_down_list, COLUMNS> drop_downs;

  std::array<vertical_list, 2> vlist;
  std::array<horizontal_list, 2> hlist;
  std::array<edit_list, 2> elist;
  std::array<tree_view, 2> trees;

  std::array<label, 6> cheader_labels;
  std::array<header_layout<22>, cheader_labels.size()> cheader_layouts;

  for (int i = 0; i < cheader_labels.size(); ++i) {
    cheader_layouts[i].set_header(layout::lay(cheader_labels[i]));
    cgrid.add(layout::lay(cheader_layouts[i]));
  }

  int i = 0;
  cheader_layouts[i++].set_body(layout::lay(vlist[0]));
  cheader_layouts[i++].set_body(layout::lay(vlist[1]));
  cheader_layouts[i++].set_body(layout::lay(elist[0]));
  cheader_layouts[i++].set_body(layout::lay(elist[1]));
  cheader_layouts[i++].set_body(layout::lay(trees[0]));
  cheader_layouts[i++].set_body(layout::lay(trees[1]));

  i = 0;
  cheader_labels[i++].set_text("Vertical list");
  cheader_labels[i++].set_text("Disabled");
  cheader_labels[i++].set_text("Editable list");
  cheader_labels[i++].set_text("Disabled");
  cheader_labels[i++].set_text("Tree");
  cheader_labels[i++].set_text("Disabled");

  std::vector<std::string> edata;
  edata.insert(edata.end(), { "Eins", "Zwei", "Drei", "View", "Fünf", "Sechs", "Sieben", "Acht", "Neun", "Zehn", "Fuß" });

  center.add(tgrid);
  center.add(layout::lay(cgrid));
  center.add(layout::lay(bgrid));

  vlist[0].set_data(ctrl::indirect_list_data<std::string>(edata));
  vlist[1].set_data(ctrl::indirect_list_data<std::string>(edata));
  elist[0].set_data(ctrl::indirect_list_data<std::string>(edata));
  elist[1].set_data(ctrl::indirect_list_data<std::string>(edata));

  tree_view::type root;
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
  trees[0].set_root(root);
  trees[1].set_root(root);
  trees[0].open_all();
  trees[1].open_all();
  trees[0].update_node_list();
  trees[1].update_node_list();

  trees[0].on_selection_changed([&](event_source src){
    trees[1].set_selection(trees[0].get_selection(), src);
  });
  vlist[0].on_selection_changed([&](event_source src){
    vlist[1].set_selection(vlist[0].get_selection(), src);
  });

  std::array<label, 2> bheader_labels;
  std::array<bottom_layout_t, bheader_labels.size()> bheader_layouts;

  for (int i = 0; i < bheader_labels.size(); ++i) {
    bheader_layouts[i].add(layout::lay(bheader_labels[i]));
    bgrid.add(layout::lay(bheader_layouts[i]));
  }

  hlist[0].on_selection_changed([&](event_source src){
    hlist[1].set_selection(hlist[0].get_selection(), src);
  });

  i = 0;
  bheader_layouts[i++].add(layout::lay(hlist[0]));
  bheader_layouts[i++].add(layout::lay(hlist[1]));

  i = 0;
  bheader_labels[i++].set_text("Horizontal list");
  bheader_labels[i++].set_text("Disabled");

  typedef ctrl::const_list_data<std::string, default_list_item_drawer<std::string, text_origin_t::center>> hlist_data_t;
  hlist[0].set_data(hlist_data_t({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}));
  hlist[1].set_data(hlist_data_t({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"}));

  auto invalidate_lists = [&] () {
    vlist[0].invalidate();
    vlist[1].invalidate();
    elist[0].invalidate();
    elist[1].invalidate();
  };

  auto source = [&](int idx) {
    return edata[idx];
  };
  auto target = [&](int idx, const std::string& s) {
    edata[idx] = s;
    invalidate_lists();
  };
  elist[0].set_data_source_and_target(source, target);
  elist[1].set_data_source_and_target(source, target);

  for (int i = 0; i < COLUMNS; ++i) {
    scroll_bars[i].on_scroll([&, i] (const core::point::type& v) {
      progress_bars[i].set_value(v);
    });
  }

  edits[0].on_selection_changed([&](event_source src){
    edits[2].set_selection(edits[0].get_selection(), src);
  });
  edits[0].on_content_changed([&](){
    edits[2].set_text(edits[0].get_text());
  });

  edits[1].on_selection_changed([&](event_source src){
    edits[3].set_selection(edits[1].get_selection(), src);
  });
  edits[1].on_content_changed([&](){
    edits[3].set_text(edits[1].get_text());
  });

  main.get_layout().set_center(layout::lay(center));

  tgrid.on_create([&](){
    add(tgrid, info[0], "label:");
    add(tgrid, stdlabel, "stdlabel");
    add(tgrid, leftlabel, "left label");
    add(tgrid, centerlabel, "center label");
    add(tgrid, rightlabel, "right label");

    add(tgrid, info[1], "button:");
    add(tgrid, buttons, "button");

    add(tgrid, info[2], "radio button:");
    add(tgrid, radios, "radio");

    add(tgrid, info[3], "checkbox:");
    add(tgrid, checks, "checkbox");

    add(tgrid, info[4], "editline:");
    add(tgrid, edits, "editline");

    add(tgrid, info[5], "password:");
    add(tgrid, passs, "password");

    add(tgrid, info[6], "drop_down:");
    add(tgrid, drop_downs, "drop_down");

    add(tgrid, info[7], "progress_bar:");
    add(tgrid, progress_bars, "0.0%");

    add(tgrid, info[8], "scroll_bars:");
    add(tgrid, scroll_bars, "scroll_bar");
  });

  main.on_create([&](){
    tgrid.create(main);
    for (auto& l : cheader_labels) {
      l.create(main);
    }
    vlist[0].create(main);
    vlist[1].create(main);
    elist[0].create(main);
    elist[1].create(main);
    trees[0].create(main);
    trees[1].create(main);

    for (auto& l : bheader_labels) {
      l.create(main);
    }
    hlist[0].create(main);
    hlist[1].create(main);
  });

  vlist[1].disable();
  elist[1].disable();
  trees[1].disable();
  hlist[1].disable();

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Gui++ look & feel");
  main.set_visible();
  main.set_children_visible();

  return run_main_loop();
}
