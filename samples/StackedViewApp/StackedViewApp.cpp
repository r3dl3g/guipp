
#include <stack>

#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/dynamic_border_layout.h>
#include <gui/layout/weighted_layout.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/ctrl/file_tree.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/look/control.h>
#include <gui/ctrl/stacked_view.h>
#include <gui/ctrl/title_view.h>
#include <gui/ctrl/toggle_group.h>

using namespace gui;
using namespace gui::win;
using namespace gui::ctrl;


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
  typedef ctrl::htoggle_group<> toggle_group_t;

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
    for (const auto& p : util::string::split<'/'>(current_path)) {
      if (!p.empty()) {
        buttons.add_button(p);
      }
    }
  }

  std::string get_selected_path () const {
    int selection = super::get_selection().get_first_index();
    if (selection > -1) {
      return current_path + current_dir[selection] + "/";
    }
    return {};
  }

  toggle_group_t& get_buttons () {
    return buttons;
  }

private:
  std::vector<std::string> current_dir;
  std::string current_path;
  toggle_group_t buttons;

  void init () {
    super::set_data(indirect_list_data<std::string>(current_dir));
    buttons.set_foreground(color::light_gray);
    buttons.set_background(color::dark_gray);
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
      if (client.get_data(client.view_stack.top()).title == path) {
        logging::warn() << "Double push same view on stacked_view with title: '" << path << "'!";
        return;
      }
      std::shared_ptr<sub_view_t> sub_view = std::make_shared<sub_view_t>(50);
      sub_view_t::view_type& sub = sub_view->view;
      sub.set_path(path);
      init_sub_view(sub, client);
      client.push(sub_view, path, sub.get_buttons(), sub.get_buttons().count() * 50);
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
  client.title_view.set_fore_and_background(color::light_gray, color::dark_gray);

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
