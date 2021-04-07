
#include <stack>

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/weighted_layout.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>

using namespace gui;
using namespace gui::win;

// --------------------------------------------------------------------------
struct title_view : public group_window<layout::border::layouter<0, 0, 120, 120>, color::very_very_light_gray> {
  typedef group_window<layout::border::layouter<0, 0, 120, 120>, color::very_very_light_gray> super;

  title_view();

  ctrl::text_button back_btn;
  ctrl::label_center title;
};

// --------------------------------------------------------------------------
title_view::title_view() {
  get_layout().set_center(layout::lay(title));
  get_layout().set_left(layout::lay(back_btn));
  on_create([&] () {
    back_btn.create(*this, "< Back");
    back_btn.set_visible(false);
    title.create(*this);
  });
}

// --------------------------------------------------------------------------
struct stacked_view : public group_window<layout::border::layouter<50>> {
  typedef group_window<layout::border::layouter<50>> super;
  typedef std::shared_ptr<win::window> view_ptr;
  struct stack_entry {
    view_ptr view;
    std::string title;
  };

  stacked_view ();

  void push (const view_ptr& view, const std::string& title);
  void pop ();

  title_view title;
  std::stack<stack_entry> view_stack;
};

// --------------------------------------------------------------------------
stacked_view::stacked_view () {
  get_layout().set_top(layout::lay(title));
  on_create([&] () {
    title.create(*this);
  });
  title.back_btn.on_clicked([&] () {
    pop();
  });
}

void stacked_view::push (const view_ptr& v, const std::string& t) {
  if (!view_stack.empty()) {
    view_stack.top().view.get()->set_visible(false);
  }
  window* w = v.get();
  view_stack.push({v, t});
  title.title.set_text(t);
  title.back_btn.set_visible(view_stack.size() > 1);
  w->create(w->get_window_class(), *this);
  get_layout().set_center(layout::lay(w));
  w->set_visible();
  layout();
  invalidate();
}

void stacked_view::pop () {
  if (view_stack.size() > 1) {
    view_stack.top().view.get()->remove_from_parent();
    view_stack.pop();
    get_layout().set_center(layout::lay(view_stack.top().view.get()));
    title.title.set_text(view_stack.top().title);
    title.back_btn.set_visible(view_stack.size() > 1);
    view_stack.top().view.get()->set_visible();
    layout();
    invalidate();
  }
}

typedef gui::ctrl::file_list<gui::ctrl::path_tree::sorted_path_info> file_list_t;
typedef gui::ctrl::virtual_view<file_list_t> sub_view_t;

void init_sub_view (file_list_t& view, stacked_view& client) {
  view.on_selection_commit([&] () {
    auto path = view.get_selected_path();
    if (sys_fs::is_directory(path)) {
      std::shared_ptr<sub_view_t> sub_view = std::make_shared<sub_view_t>(50);
      sub_view_t::view_type& sub = sub_view.get()->view;
      sub.set_path(path);
      init_sub_view(sub, client);
      client.push(sub_view, path.string());
    }
  });
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  stacked_view client;
//  layout_main_window<layout::border::zero_layout<stacked_view>, stacked_view&> main(client);
  layout_main_window<layout::border::layouter<>> main;
  main.get_layout().set_center(layout::lay(client));

  std::shared_ptr<sub_view_t> base_view = std::make_shared<sub_view_t>(50);

  main.on_create([&] () {
    client.create(main);
    client.push(base_view, "/");
    sub_view_t::view_type& view = base_view.get()->view;
    view.set_path("/");

    init_sub_view(view, client);
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("StackedView");
  main.set_visible();

  return run_main_loop();
}
