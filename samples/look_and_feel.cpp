
#include <algorithm>

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/split_layout.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/edit.h>
#include <gui/ctrl/progress_bar.h>
#include <gui/ctrl/drop_down.h>
#include <gui/ctrl/scroll_bar.h>
#include <gui/ctrl/tree.h>
#include <gui/ctrl/tab_group.h>
#include <gui/ctrl/tab_view.h>

template<typename T, std::size_t N>
constexpr std::size_t size_of (const std::array<T, N>&) {
  return N;
}

const std::size_t COLUMNS = 4;

typedef gui::layout::fixed_grid_lineup<120, 22, COLUMNS + 1, 10, 4> layout_t;
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

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui;
  using namespace gui::win;
  using namespace gui::ctrl;

  typedef layout_main_window<layout::border::layouter<10, 10, 10, 10, layout::border::type_t::all_symmetric>> main_window_t;

  typedef layout::vertical_lineup<236, 10, 8> center_layout_t;
  typedef layout::vertical_lineup<40, 0, 8> bottom_layout_t;
  typedef layout::horizontal_adaption<10, 4, 0, 80, 200> center_grid_t;
  typedef group_window<center_grid_t, color::very_very_light_gray> page_t;

  main_window_t main;
  tab_view<alignment_t::top> tabs;
  top_grid_t first_view;
  page_t second_view;
  page_t third_view;

  tabs.add_page("First", first_view);
  tabs.add_page("Second", second_view);
  tabs.add_page("Third", third_view);
  main.get_layout().set_center(layout::lay(tabs));

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

  std::array<vertical_scrollable_list, 2> vlist;
  std::array<horizontal_scrollable_list, 2> hlist;
  std::array<edit_list, 2> elist;
  std::array<tree_view, 2> trees;

  std::array<label, 6> cheader_labels;
  std::array<layout::header_layout, size_of(cheader_labels)> cheader_layouts;

  for (int i = 0; i < size_of(cheader_labels); ++i) {
    cheader_layouts[i].set_header(layout::lay(cheader_labels[i]));
    second_view.get_layout().add(layout::lay(cheader_layouts[i]));
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

  vlist[0]->set_data(ctrl::indirect_list_data<std::string>(edata));
  vlist[1]->set_data(ctrl::indirect_list_data<std::string>(edata));
  elist[0].set_data(ctrl::indirect_list_data<std::string>(edata));
  elist[1].set_data(ctrl::indirect_list_data<std::string>(edata));

  tree_view::type root;
  root.label = "root";
  typedef ctrl::tree::node node;
  root.add_nodes({
    node("1 leaf"),
    node("2 sub", {
      node("2.1 sub", {
        node("2.1.1 leaf"),
        node("2.1.2 leaf")
      }),
      node("2.2 sub", {
        node("2.2.1leaf"),
        node("2.2.2 leaf"),
        node("2.2.3 leaf")
      }),
      node("2.3 leaf")
    }),
    node("3 leaf")
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
  vlist[0]->on_selection_changed([&](event_source src){
    vlist[1]->set_selection(vlist[0]->get_selection(), src);
  });

  std::array<label, 2> bheader_labels;
  std::array<bottom_layout_t, size_of(bheader_labels)> bheader_layouts;

  for (int i = 0; i < size_of(bheader_labels); ++i) {
    bheader_layouts[i].add(layout::lay(bheader_labels[i]));
    third_view.get_layout().add(layout::lay(bheader_layouts[i]));
  }

  hlist[0]->on_selection_changed([&](event_source src){
    hlist[1]->set_selection(hlist[0]->get_selection(), src);
  });

  i = 0;
  bheader_layouts[i++].add(layout::lay(hlist[0]));
  bheader_layouts[i++].add(layout::lay(hlist[1]));

  i = 0;
  bheader_labels[i++].set_text("Horizontal list");
  bheader_labels[i++].set_text("Disabled");

  typedef ctrl::const_list_data<std::string, default_list_item_drawer<std::string, text_origin_t::center>> hlist_data_t;
  hlist[0]->set_data(hlist_data_t({"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"}));
  hlist[1]->set_data(hlist_data_t({"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"}));

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

  main.on_create([&](){
    tabs.create(main);
  });

  tabs.on_create([&](){
    first_view.create(tabs);
    second_view.create(tabs);
    third_view.create(tabs);
  });

  first_view.on_create([&](){
    add(first_view, info[0], "label:");
    add(first_view, stdlabel, "stdlabel");
    add(first_view, leftlabel, "left label");
    add(first_view, centerlabel, "center label");
    add(first_view, rightlabel, "right label");

    add(first_view, info[1], "button:");
    add(first_view, buttons, "button");

    add(first_view, info[2], "radio button:");
    add(first_view, radios, "radio");

    add(first_view, info[3], "checkbox:");
    add(first_view, checks, "checkbox");

    add(first_view, info[4], "editline:");
    add(first_view, edits, "editline");

    add(first_view, info[5], "password:");
    add(first_view, passs, "password");

    add(first_view, info[6], "drop_down:");
    add(first_view, drop_downs, "drop_down");

    add(first_view, info[7], "progress_bar:");
    add(first_view, progress_bars, "0.0%");

    add(first_view, info[8], "scroll_bars:");
    add(first_view, scroll_bars, "scroll_bar");
  });

  second_view.on_create([&]() {
    for (auto& l : cheader_labels) {
      l.create(second_view);
    }
    vlist[0].create(second_view);
    vlist[1].create(second_view);
    elist[0].create(second_view);
    elist[1].create(second_view);
    trees[0].create(second_view);
    trees[1].create(second_view);
  });

  third_view.on_create([&]() {
    for (auto& l : bheader_labels) {
      l.create(third_view);
    }
    hlist[0].create(third_view);
    hlist[1].create(third_view);
  });

  vlist[1].disable();
  elist[1].disable();
  trees[1].disable();
  hlist[1].disable();

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Gui++ look & feel");
  main.set_visible();

  return run_main_loop();
}
