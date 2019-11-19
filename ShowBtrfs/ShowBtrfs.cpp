

#include <gui/win/container.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/sorted_column_list.h>
#include <gui/ctrl/label.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <util/string_util.h>

#include <fstream>
#include <iomanip>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
struct percent {
  float value;

  percent (float value = 0.0F)
    : value(value)
  {}

  bool operator< (const percent& rhs) const {
    return value < rhs.value;
  }

};
// --------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& out, const percent& p) {
  out << std::fixed << std::setprecision(1) << p.value * 100.0F << "%";
  return out;
}
// --------------------------------------------------------------------------
struct file_size {
  size_t value;

  file_size (size_t value = 0)
    : value(value)
  {}

  bool operator< (const file_size& rhs) const {
    return value < rhs.value;
  }

};
// --------------------------------------------------------------------------
std::ostream& operator<< (std::ostream& out, file_size p) {
  static std::string exponents[] = {"", " KiB", " MiB", " GiB", " TiB", " PiB", " EiB", " ZiB", " YiB"};
  int exp = 0;
  while (p.value > 1023) {
    p.value /= 1024;
    ++exp;
  }
  out << p.value << exponents[exp];
  return out;
}
// --------------------------------------------------------------------------
struct subvolume {
  std::string name;
  int parent;
};
// --------------------------------------------------------------------------
void load_subs (const sys_fs::path& subs_file,
                std::map<int, subvolume>& subs_map,
                gui::win::container& main) {
  std::ifstream subs(subs_file);
  if (!subs.is_open()) {
    gui::ctrl::message_dialog::show(main, "Error!", "Could not open " + subs_file.string(), "Ok");
    return;
  }

  std::string path;
  std::getline(subs, path);
  std::getline(subs, path);
  subs >> std::skipws;

  int id = 0, gen = 0, top = 0;
  while (!subs.eof()) {
    subs >> id >> gen >> top >> path;
    subs_map[id] = {path, top};
  }
}
// --------------------------------------------------------------------------
typedef std::tuple<int, int, std::string, file_size, file_size, percent> data_entry;
// --------------------------------------------------------------------------
void load_qgroup (const sys_fs::path& qgroup_file,
                  std::map<int, subvolume>& subs_map,
                  std::vector<data_entry>& data,
                  gui::win::container& main) {
  std::ifstream qgroup(qgroup_file);
  if (!qgroup.is_open()) {
    gui::ctrl::message_dialog::show(main, "Error!", "Could not open " + qgroup_file.string(), "Ok");
    return;
  }

  std::string path;
  std::getline(qgroup, path);
  std::getline(qgroup, path);
  qgroup >> std::skipws;

  size_t full = 0, excl = 0, sum = 0;
  int id = 0, top = 0;
  char slash;
  while (!qgroup.eof()) {
    qgroup >> top >> slash >> id >> full >> excl;
    auto i = subs_map.find(id);
    if (i != subs_map.end()) {
      data.emplace_back(std::make_tuple(i->second.parent, id, i->second.name, full, excl, 0.0F));
    } else {
      data.emplace_back(std::make_tuple(-1, id, "Unknown", full, excl, 0.0F));
    }
    sum += full;
  }
  for (data_entry& i : data) {
    const float perc = (float)((double)std::get<3>(i).value / (double)sum);
    i = std::make_tuple(std::get<0>(i), std::get<1>(i), std::get<2>(i), std::get<3>(i), std::get<4>(i), perc);
  }
}
// --------------------------------------------------------------------------
#ifdef X11
std::string run (std::string cmd) {
  std::ostringstream result;
  std::vector<char> buffer(256);

  cmd.append(" 2>&1");

  FILE* in = popen(cmd.c_str(), "r");
  if (in) {
    while (!feof(in)) {
      if (fgets(buffer.data(), buffer.size(), in) != NULL) {
        result << buffer.data();
      }
    }
    pclose(in);
  }
  return result.str();
}
#endif // X11
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  using namespace gui;
  using namespace gui::win;
  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  sys_fs::path qgroup_file;
  std::map<int, subvolume> subs_map;

  typedef sorted_column_list_t<weight_column_list_layout, int, int, std::string, file_size, file_size, percent> my_column_list_t;

  auto weight_columns = {
    weight_column_info{ 80, text_origin::vcenter_left, 50, 0.0F },
    weight_column_info{ 80, text_origin::vcenter_left, 50, 0.0F },
    weight_column_info{ 300, text_origin::vcenter_left, 80, 1.0F },
    weight_column_info{ 120, text_origin::vcenter_right, 80, 0.0F },
    weight_column_info{ 100, text_origin::vcenter_right, 80, 0.0F },
    weight_column_info{ 80, text_origin::center, 50, 0.0F }
  };

  my_column_list_t::row_drawer column_list_drawer = {
    ctrl::cell_drawer<int, draw::frame::sunken_relief>,
    ctrl::cell_drawer<int, draw::frame::sunken_relief>,
    ctrl::cell_drawer<std::string, draw::frame::sunken_relief>,
    ctrl::cell_drawer<file_size, draw::frame::sunken_relief>,
    ctrl::cell_drawer<file_size, draw::frame::sunken_relief>,
    ctrl::cell_drawer<percent, draw::frame::sunken_relief>
  };

  layout_main_window<gui::layout::border_layout<>, float, float, float, float> main(20, 0, 0, 0);
  ctrl::main_menu menu;
  my_column_list_t client;

  client.header_label = {"Top", "Id", "Path", "Full", "Excl", "%"};

  menu.data.add_entries({
    menu_entry("Open sub", 's', [&] () {
      ctrl::file_open_dialog::show(main, "Open btrfs subvolume list", "Ok", "Cancel", [&] (const sys_fs::path& file) {
        load_subs(file, subs_map, main);
        if (!subs_map.empty() && !qgroup_file.empty()) {
          load_qgroup(qgroup_file, subs_map, client.data, main);
          client.data_changed();
        }
      });
    }, hot_key(keys::o, state::control), false),
    menu_entry("Open qgroup", 'q', [&] () {
      ctrl::file_open_dialog::show(main, "Open qgroup list", "Ok", "Cancel", [&] (const sys_fs::path& file) {
        qgroup_file = file;
        if (!subs_map.empty() && !qgroup_file.empty()) {
          load_qgroup(qgroup_file, subs_map, client.data, main);
          client.data_changed();
        }
      });
    }, hot_key(keys::q, state::control), false),
  });

  main.on_create([&] (window*, const rectangle& rect) {
    client.create(main, rect);
    menu.create(main);
    main.get_layout().set_top(layout::lay(menu));
    main.get_layout().set_center(layout::lay(client));
  });

  client.set_drawer(column_list_drawer);
  client.get_column_layout().set_columns(weight_columns);

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Show Btrfs");
  main.set_visible();

  return run_main_loop();
}
