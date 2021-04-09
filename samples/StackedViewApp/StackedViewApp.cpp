
#include <stack>

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/weighted_layout.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/look/control.h>
#include <gui/win/stacked_view.h>

using namespace gui;
using namespace gui::win;
using namespace gui::ctrl;

// --------------------------------------------------------------------------
struct title_view : public group_window<layout::border::layouter<0, 0, 80, 0>, color::very_very_light_gray> {
  typedef group_window<layout::border::layouter<0, 0, 80, 0>, color::very_very_light_gray> super;

  title_view();

  void set_title (const std::string& title) {
    title_label.set_text(title);
  }

  void enable_back (bool b) {
    back_btn.enable(b);
  }

  void on_back (std::function<notification_fn>&& fn) {
    back_btn.on_clicked(std::move(fn));
  }

private:
  icon_push_button<draw::icon_t::left_arrow, color::black, color::very_very_light_gray> back_btn;
  label title_label;
};
// --------------------------------------------------------------------------
title_view::title_view() {
  get_layout().set_center(layout::lay(title_label));
  get_layout().set_left(layout::lay(back_btn));
  on_create([&] () {
    back_btn.create(*this);
    back_btn.set_visible(false);
    title_label.create(*this);
  });
}
// --------------------------------------------------------------------------
typedef stacked_view<title_view> stacked_view_t;
// --------------------------------------------------------------------------
typedef std::vector<std::string> directory_entries;
typedef std::map<std::string, directory_entries> directory;

directory fake_fs;

// --------------------------------------------------------------------------
//typedef file_list<path_tree::sorted_path_info> file_list_t;

class fake_file_list : public vertical_list {
public:
  typedef vertical_list super;

  fake_file_list (core::size::type item_size = list_defaults<>::item_size)
  : super(item_size) {
    init();
  }

  void set_path (const std::string& dir) {
    current_path = dir;
    current_dir = fake_fs[dir];
    super::clear_selection(event_source::logic);
    super::set_scroll_pos(core::point::zero);
    super::invalidate();
  }

  std::string get_selected_path () const {
    int selection = super::get_selection();
    if (selection > -1) {
      return current_path + current_dir[selection] + "/";
    }
    return {};
  }

private:
  std::vector<std::string> current_dir;
  std::string current_path;

  void init () {
    super::set_data(indirect_list_data<std::string>(current_dir));
  }
};
// --------------------------------------------------------------------------
typedef fake_file_list file_list_t;
typedef virtual_view<file_list_t> sub_view_t;
// --------------------------------------------------------------------------
void init_sub_view (file_list_t& view, stacked_view_t& client) {
  view.set_background(color::very_light_gray);
  view.on_selection_commit([&] () {
    auto path = view.get_selected_path();
    if (fake_fs.find(path) != fake_fs.end()) {
      if (client.view_stack.top().title == path) {
        clog::warn() << "Double push same view on stacked_view with title: '" << path << "'!";
        return;
      }
      std::shared_ptr<sub_view_t> sub_view = std::make_shared<sub_view_t>(50);
      sub_view_t::view_type& sub = sub_view->view;
      sub.set_path(path);
      init_sub_view(sub, client);
      client.push(sub_view, path);
    }
  });
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  fake_fs["/"] = {"bin", "dev", "etc", "home", "lib", "tmp", "usr", "var"};
  fake_fs["/bin/"] = {"bash", "cat", "chmod", "date", "dd", "grep", "mount", "ping", "touch"};
  fake_fs["/dev/"] = {"null", "zero", "tty"};
  fake_fs["/etc/"] = {"crontab", "fstab", "group", "hosts", "passwd", "shadow", "timezone"};
  fake_fs["/home/"] = {"user", "phablet"};
  fake_fs["/home/user/"] = {".bashrc", ".profile"};
  fake_fs["/home/phablet/"] = {".bashrc", ".profile", ".bash_history"};
  fake_fs["/lib/"] = {"modules", "udev"};
  fake_fs["/lib/udev/"] = {"hwclock-set"};
  fake_fs["/tmp/"] = {"tmp.0", "tmp.1"};
  fake_fs["/usr/"] = {"bin", "include", "lib", "local", "sbin", "share", "src"};
  fake_fs["/usr/local/"] = {"bin", "etc", "include", "lib", "sbin", "share", "src"};
  fake_fs["/var/"] = {"cache", "lib", "local", "log", "run", "tmp"};

  stacked_view_t client;
  layout_main_window<layout::border::zero_layout<stacked_view_t>, stacked_view_t&> main(client);

  std::shared_ptr<sub_view_t> base_view = std::make_shared<sub_view_t>(50);

  main.on_create([&] () {
    client.create(main);
    client.view_stack.push(base_view, "/");
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
