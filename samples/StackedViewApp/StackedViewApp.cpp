
#include <stack>

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/weighted_layout.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/look/control.h>
#include <gui/win/animation.h>

using namespace gui;
using namespace gui::win;
using namespace gui::ctrl;

// --------------------------------------------------------------------------
struct title_view_t : public group_window<layout::border::layouter<0, 0, 80, 0>, color::very_very_light_gray> {
  typedef group_window<layout::border::layouter<0, 0, 80, 0>, color::very_very_light_gray> super;

  title_view_t();

  icon_push_button<draw::icon_t::left_arrow, color::black, color::very_very_light_gray> back_btn;
  label title;
};
// --------------------------------------------------------------------------
title_view_t::title_view_t() {
  get_layout().set_center(layout::lay(title));
  get_layout().set_left(layout::lay(back_btn));
  on_create([&] () {
    back_btn.create(*this);
    back_btn.set_visible(false);
    title.create(*this);
  });
}
// --------------------------------------------------------------------------
struct stacked_view_base : public container {
  typedef container super;

  stacked_view_base ();

  void push (const window_ptr& view, const std::string& title);
  void pop ();

  std::size_t count () const;

protected:
  struct stack_entry {
    window_ptr view;
    std::string title;
  };

  std::stack<stack_entry> view_stack;

  virtual void finish_push (const window_ptr& view, const std::string& title);
  virtual void prepare_pop (const window_ptr& view);
  virtual void finish_pop ();

  animation animator;
};
// --------------------------------------------------------------------------
stacked_view_base::stacked_view_base ()
  : animator(*this, std::chrono::milliseconds(250)) {
}
// --------------------------------------------------------------------------
std::size_t stacked_view_base::count () const {
  return view_stack.size();
}
// --------------------------------------------------------------------------
void stacked_view_base::finish_push (const window_ptr& v, const std::string& t) {
  v->set_visible();
}
// --------------------------------------------------------------------------
void stacked_view_base::push (const window_ptr& v, const std::string& t) {
  if (view_stack.empty()) {
    v->create(v->get_window_class(), *this);
    view_stack.push({v, t});
    finish_push(v, t);
  } else {
    if (view_stack.top().title == t) {
      clog::warn() << "Double push same view on stacked_view with title: '" << t << "'!";
      return;
    }
    window_ptr v0 = view_stack.top().view;
    const auto r = v0->geometry();
    view_stack.push({v, t});
    v->create(v->get_window_class(), *this, r);
    v->set_visible(false);
    animator.add(move_animation(v, r.top_right(), r.top_left()));
    animator.add(move_animation(v0, r.top_left(), r.top_left().dx(-100)));
    animator.start([&, v, v0, t] () {
      animator.clear();
      v0->set_visible(false);
      finish_push(v, t);
    });
  }
}

void stacked_view_base::prepare_pop (const window_ptr& view) {
  view->set_visible();
}

void stacked_view_base::finish_pop () {

}

void stacked_view_base::pop () {
  if (view_stack.size() > 1) {
    window_ptr v = view_stack.top().view;
    view_stack.pop();
    window_ptr v0 = view_stack.top().view;
    prepare_pop(v0);
    const auto r = v->geometry();
    animator.add(move_animation(v, r.top_left(), r.top_right()));
    animator.add(move_animation(v0, r.top_left().dx(-100), r.top_left()));
    animator.start([&, v] () {
      animator.clear();
      v->remove_from_parent();
      finish_pop();
    });
  }
}
// --------------------------------------------------------------------------
struct stacked_view : public gui::win::detail::layout_container_base<stacked_view_base, layout::border::layouter<50>> {
  typedef gui::win::detail::layout_container_base<stacked_view_base, layout::border::layouter<50>> super;
  typedef no_erase_window_class<stacked_view> clazz;

  stacked_view ();

  void create (container& parent,
               const core::rectangle& r = core::rectangle::def);

protected:
  title_view_t title_view;

  void finish_push (const window_ptr& view, const std::string& title) override;
  void prepare_pop (const window_ptr& view) override;
  void finish_pop () override;
};
// --------------------------------------------------------------------------
stacked_view::stacked_view () {
  on_create([&] () {
    title_view.create(*this);
  });
  title_view.back_btn.on_clicked([&] () {
    pop();
  });
  get_layout().set_top(layout::lay(title_view));
}

void stacked_view::create (container& parent, const core::rectangle& r) {
  super::create(clazz::get(), parent, r);
}

void stacked_view::prepare_pop (const window_ptr& view) {
  get_layout().set_center(layout::lay(view.get()));
  super::prepare_pop(view);
}

void stacked_view::finish_pop () {
  title_view.title.set_text(view_stack.top().title);
  title_view.back_btn.enable(view_stack.size() > 1);
  layout();
  invalidate();
}

void stacked_view::finish_push (const window_ptr& view, const std::string& t) {
  title_view.title.set_text(t);
  title_view.back_btn.enable(view_stack.size() > 1);
  get_layout().set_center(layout::lay(view.get()));
  layout();
  invalidate();
}

// --------------------------------------------------------------------------
void my_file_item_drawer (const fs::file_info& path,
                          draw::graphics& g,
                          const core::rectangle& r,
                          const draw::brush& b,
                          item_state state) {
  using namespace draw;
  g.fill(rectangle(r), gui::look::get_background_color(state, b.color()));
  g.text(text_box(path.filepath(), r.shrinked(0, 0, 3, 3), text_origin_t::vcenter_left), font::system(), color::invert(b.color()));
}
// --------------------------------------------------------------------------
typedef file_list<path_tree::sorted_path_info> file_list_t;
typedef virtual_view<file_list_t> sub_view_t;

void init_sub_view (file_list_t& view, stacked_view_base& client) {
  view.set_background(color::very_light_gray);
  view.on_selection_commit([&] () {
    auto path = view.get_selected_path();
    if (sys_fs::is_directory(path)) {
      std::shared_ptr<sub_view_t> sub_view = std::make_shared<sub_view_t>(50);
      sub_view_t::view_type& sub = sub_view->view;
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
  layout_main_window<layout::border::zero_layout<stacked_view>, stacked_view&> main(client);

  std::shared_ptr<sub_view_t> base_view = std::make_shared<sub_view_t>(50);

  main.on_create([&] () {
    client.create(main);
    client.push(base_view, "/");
    sub_view_t::view_type& view = base_view->view;
    view.set_path("/");

    init_sub_view(view, client);
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("StackedView");
  main.set_visible();

  return run_main_loop();
}
