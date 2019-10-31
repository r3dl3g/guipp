/**
 * @copyright (c) 2017-2018 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    TimeTracker
 *
 * Customer   -
 *
 * @brief     C++ TimeTracker Tool
 *
 * @file
 */

#include <iomanip>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <base/bind_method.h>
#include <util/time_util.h>
#include <util/ostream_resetter.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/tree.h>
#include <gui/ctrl/column_list.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/std_dialogs.h>


using time_point = std::chrono::system_clock::time_point;
using duration_type = std::chrono::system_clock::duration;

// --------------------------------------------------------------------------
namespace std {

  std::ostream& operator<< (std::ostream& o, time_point const& tp) {
    return util::time::format_time(o, tp);
  }

  std::istream& operator>> (std::istream& i, time_point& tp) {
    return util::time::operator>>(i, tp);
  }

  std::ostream& operator<< (std::ostream& o, duration_type const& d) {
    return util::time::format_duration_only_h(o, d);
  }

}

// --------------------------------------------------------------------------
struct tt_category;
struct tt_project;
struct tt_event;
struct tt_price_per_hour;
struct tt_budget;

struct date {
  date (uint32_t year = 0, uint32_t month = 0, uint32_t day = 0)
    : year(year)
    , month(month)
    , day(day)
  {}

  uint32_t year:23;
  uint32_t month:4;
  uint32_t day:5;

  time_point to_time_point () const {
    std::tm t{ 0, 0, 0, (int)day, (int)(month - 1), (int)(year - 1900), 0 };
    return std::chrono::system_clock::from_time_t(std::mktime(&t));
  }

  static date from_time_point (const time_point & tp) {
    std::tm t = util::time::local_time(tp);
    return {(uint32_t)(t.tm_year + 1900), (uint32_t)(t.tm_mon + 1), (uint32_t)t.tm_mday};
  }
};

std::ostream& operator<< (std::ostream& out, const date& dt) {
  util::ostream_resetter r(out);
  out << std::setfill('0') << dt.year
      << '-' << std::setw(2) << dt.month
      << '-' << std::setw(2) << dt.day;
  return out;
}

std::istream& operator>> (std::istream& in, date& dt) {
  uint32_t y = 0, m = 1, d = 1;
  if (in.good()) {
    in >> y;
    while (in.good() && !isdigit(in.peek())) {
      in.ignore(1);
    }
    if (in.good()) {
      in >> m;
      while (in.good() && !isdigit(in.peek())) {
        in.ignore(1);
      }
      if (in.good()) {
        in >> d;
      }
    }
  }
  dt = {y, m, d};
  return in;
}

// --------------------------------------------------------------------------
struct tt_event {
  static const std::string BEGIN;
  static const std::string END;
  static const std::string COMMENT;

  tt_project& project;
  std::string id;
  time_point begin;
  time_point end;
  std::string comment;

  duration_type duration () const {
    return end > begin ? end - begin : duration_type::zero();
  }

  double costs () const;

  tt_event (tt_project& p, const std::string& id = create_id())
    : project(p)
    , id(id)
  {}

  tt_event (tt_project& p, const std::string& id, const boost::property_tree::ptree& pt)
    : project(p)
    , id(id)
    , begin(pt.get(BEGIN, time_point()))
    , end(pt.get(END, time_point()))
    , comment(pt.get(COMMENT, std::string()))
  {}

  static std::string create_id () {
    return ostreamfmt(std::hex << std::chrono::system_clock::now().time_since_epoch().count());
  }

  void start () {
    begin = std::chrono::system_clock::now();
  }

  void stop () {
    end = std::chrono::system_clock::now();
  }

  void write (boost::property_tree::ptree& pt) {
    pt.put(BEGIN, begin);
    pt.put(END, end);
    pt.put(COMMENT, comment);
  }

};

const std::string tt_event::BEGIN = "begin";
const std::string tt_event::END = "end";
const std::string tt_event::COMMENT = "comment";
const std::string PROPS = ".props";
const std::string SETTINGS = "settings.props";

// --------------------------------------------------------------------------
struct tt_price_per_hour {
  static const std::string PRICE_PER_HOUR;
  static const std::string BEGIN;
  static const std::string PRICE;

  date begin;
  double price;

  tt_price_per_hour (const date& begin, double price = 0.0)
    : begin(begin)
    , price(price)
  {}

  tt_price_per_hour (const boost::property_tree::ptree& pt, int num)
    : begin(pt.get(ostreamfmt(PRICE_PER_HOUR << num << BEGIN), date()))
    , price(pt.get(ostreamfmt(PRICE_PER_HOUR << num << PRICE), 0.0))
  {}

  void write (boost::property_tree::ptree& pt, int num) {
    pt.put(ostreamfmt(PRICE_PER_HOUR << num << BEGIN), begin);
    pt.put(ostreamfmt(PRICE_PER_HOUR << num << PRICE), price);
  }
};

const std::string tt_price_per_hour::PRICE_PER_HOUR = "PricePerHour.";
const std::string tt_price_per_hour::BEGIN = ".key";
const std::string tt_price_per_hour::PRICE = ".value";

// --------------------------------------------------------------------------
struct tt_budget {
  static const std::string DATE;
  static const std::string BUDGET;

  date begin;
  double budget;

  tt_budget (const date& begin, double budget = 0.0)
    : begin(begin)
    , budget(budget)
  {}

  tt_budget (const boost::property_tree::ptree& pt, int num)
    : begin(pt.get(ostreamfmt(DATE << num), date()))
    , budget(pt.get(ostreamfmt(BUDGET << num), 0.0))
  {}

  void write (boost::property_tree::ptree& pt, int num) {
    pt.put(ostreamfmt(DATE << num), begin);
    pt.get(ostreamfmt(BUDGET << num), budget);
  }
};

const std::string tt_budget::DATE = "Date";
const std::string tt_budget::BUDGET = "Budget";

// --------------------------------------------------------------------------
struct tt_project {
  static const std::string PRICE_PER_HOUR;
  static const std::string PRICE_PER_HOUR_COUNT;

  tt_category& category;
  std::string name;
  double price_per_hour;
  std::vector<tt_event> events;
  std::vector<tt_price_per_hour> prices_per_hour;

  tt_project (tt_category& category, const std::string& name, double price_per_hour = 0.0)
    : category(category)
    , name(name)
    , price_per_hour(price_per_hour)
  {}

  tt_project (tt_category& category, const std::string& name, const boost::property_tree::ptree& pt)
    : category(category)
    , name(name)
    , price_per_hour(pt.get(PRICE_PER_HOUR, 0.0F))
  {
    int count = pt.get(PRICE_PER_HOUR_COUNT, 0);
    for (int i = 0; i < count; ++i) {
      prices_per_hour.emplace_back(tt_price_per_hour(pt, i));
    }
  }

  double price_per_second (const time_point& begin) const {
    // TBD!
    return price_per_hour / 3600.0;
  }

  duration_type duration () const {
    duration_type d = duration_type::zero();
    for (auto& i : events) {
      d += i.duration();
    }
    return d;
  }

  double costs () const {
    double d = 0;
    for (auto& i : events) {
      d += i.costs();
    }
    return d;
  }

  void read_events (const sys_fs::path& dir) {
    for (const sys_fs::directory_entry& f : sys_fs::directory_iterator(dir)) {
      if ((f.path().extension().string() == PROPS) && (f.path().filename().string() != SETTINGS)) {
        try {
          boost::property_tree::ptree pt;
          boost::property_tree::ini_parser::read_ini(f.path().string(), pt);
          events.emplace_back(tt_event(*this, f.path().stem().string(), pt));
        } catch (std::exception& ex) {
          LogFatal << ex;
        }
      }
    }
  }

  void write (boost::property_tree::ptree& pt) {
    pt.put(PRICE_PER_HOUR, price_per_hour);
    auto count = prices_per_hour.size();
    pt.put(PRICE_PER_HOUR_COUNT, count);
    for (auto i = 0; i < count; ++i) {
      prices_per_hour[i].write(pt, i);
    }
  }
};

const std::string tt_project::PRICE_PER_HOUR = "PricePerHour";
const std::string tt_project::PRICE_PER_HOUR_COUNT = "PricePerHour.count";

// --------------------------------------------------------------------------
struct tt_category {
  static const std::string BUDGET_COUNT;

  std::string name;
  std::vector<tt_project> projects;
  std::vector<tt_budget> budgets;

  tt_category (const std::string& name)
    : name (name)
  {}

  tt_category (const std::string& name, const boost::property_tree::ptree& pt)
    : name (name)
  {
    int count = pt.get(BUDGET_COUNT, 0);
    for (int i = 0; i < count; ++i) {
      budgets.emplace_back(tt_budget(pt, i));
    }
  }

  duration_type duration () const {
    duration_type d = duration_type::zero();
    for (auto& i : projects) {
      d += i.duration();
    }
    return d;
  }

  double costs () const {
    double d = 0;
    for (auto& i : projects) {
      d += i.costs();
    }
    return d;
  }

  double budget () const {
    double d = 0;
    for (auto& i : budgets) {
      d += i.budget;
    }
    return d;
  }

  void read_projects (const sys_fs::path& dir) {
    for (const sys_fs::directory_entry& f : sys_fs::directory_iterator(dir)) {
      if (f.status().type() == sys_fs::file_type::directory) {
        try {
          boost::property_tree::ptree pt;
          sys_fs::path sub(f.path());
          sub /= SETTINGS;
          if (sys_fs::exists(sub)) {
            boost::property_tree::ini_parser::read_ini(sub.string(), pt);
          }
          tt_project prj(*this, f.path().stem().string(), pt);
          prj.read_events(f.path());
          projects.emplace_back(prj);
        } catch (std::exception& ex) {
          LogFatal << ex;
        }
      }
    }
  }

  void write (boost::property_tree::ptree& pt) {
    auto count = budgets.size();
    pt.put(BUDGET_COUNT, count);
    for (auto i = 0; i < count; ++i) {
      budgets[i].write(pt, i);
    }
  }

};

const std::string tt_category::BUDGET_COUNT = "BudgetCount";

// --------------------------------------------------------------------------
double tt_event::costs () const {
  return project.price_per_second(begin) * std::chrono::duration_cast<std::chrono::seconds>(duration()).count();
}

// --------------------------------------------------------------------------
using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;

void name_drawer (const std::string& txt, const draw::graphics& g, const core::rectangle& r, const draw::brush& b,  item_state s, text_origin align) {
  ctrl::paint::text_item(g, r, b, txt, s, align);
  draw::frame::lines(g, r);
}

void duration_drawer (const duration_type& d, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, item_state s, text_origin align) {
  ctrl::paint::text_item(g, r, b, ostreamfmt(d), s, align);
  draw::frame::lines(g, r);
}

void money_drawer (const double& v, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, item_state s, text_origin align) {
  ctrl::paint::text_item(g, r, b, ostreamfmt(std::setprecision(2) << std::fixed << v << " €"), s, align);
  draw::frame::lines(g, r);
}

void time_drawer (const time_point& t, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, item_state s, text_origin align) {
  ctrl::paint::text_item(g, r, b, ostreamfmt(t), s, align);
  draw::frame::lines(g, r);
}

// --------------------------------------------------------------------------
class TimeTracker : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;

  TimeTracker ()
    : super(20, 30, 0, 0)
    , left_view(content_view.first)
    , right_view(content_view.second)
    , filter_tree(left_view.first)
    , weekdays(left_view.second)
    , main_view(right_view.first)
    , bottom_view(right_view.second)
    , top_view(main_view.first)
    , entry_view(main_view.second)
    , category_view(top_view.first)
    , project_view(top_view.second)
  {
    on_create(basepp::bind_method(this, &TimeTracker::onCreated));

    category_view.set_drawer(category_view_type::row_drawer{name_drawer, duration_drawer, money_drawer, money_drawer});
    project_view.set_drawer(project_view_type::row_drawer{name_drawer, duration_drawer, money_drawer, money_drawer, money_drawer});
    entry_view.set_drawer(entry_view_type::row_drawer{name_drawer, time_drawer, time_drawer, duration_drawer, money_drawer, money_drawer, money_drawer, name_drawer, money_drawer});
  }

  void onCreated (window* w, const core::rectangle& r) {
    content_view.create(*this, r.with_pos(core::point::zero), 0.25);
    content_view.first.set_split_pos(0.65);
    right_view.set_split_pos(0.65);
    status_bar.create(*this);

    menu.data.add_entries(
    {
      main_menu_entry("Open", 'O', basepp::bind_method(this, &TimeTracker::open)),
      main_menu_entry("Start", 'S', basepp::bind_method(this, &TimeTracker::start)),
      main_menu_entry("Stop", 't', basepp::bind_method(this, &TimeTracker::stop)),
      main_menu_entry("Category", 'C', [&] () {
        popup_menu({
          {"Add", 'A', basepp::bind_method(this, &TimeTracker::category_add)},
          {"Remove", 'R', basepp::bind_method(this, &TimeTracker::category_remove)},
          {"Edit", 'E', basepp::bind_method(this, &TimeTracker::category_edit)},
          {"Budgets", 'B', basepp::bind_method(this, &TimeTracker::category_budgets)},
          {"Add Budget", 'g', basepp::bind_method(this, &TimeTracker::category_add_budget)}
        }).popup(menu);
      }),
      main_menu_entry("Project", 'P', [&] () {
        popup_menu({
          {"Add", 'A', basepp::bind_method(this, &TimeTracker::project_add)},
          {"Remove", 'R', basepp::bind_method(this, &TimeTracker::project_remove)},
          {"Edit", 'E', basepp::bind_method(this, &TimeTracker::project_edit)},
          {"Price per Hour", 'P', basepp::bind_method(this, &TimeTracker::project_proce)}
        }).popup(menu);
      }),
      main_menu_entry("Event", 'E', [&] () {
        popup_menu({
          {"Add", 'A', basepp::bind_method(this, &TimeTracker::event_add)},
          {"Remove", 'R', basepp::bind_method(this, &TimeTracker::event_remove)},
          {"Edit", 'E', basepp::bind_method(this, &TimeTracker::event_edit)}
        }).popup(menu);
      }),
      main_menu_entry("About", 'A', basepp::bind_method(this, &TimeTracker::about))
    });
    menu.create(*this);

    tree_view::type root("Filter");
    root.add_nodes({
      {"Years", {
        "This year", "Last year", "2 year ago", "3 year ago", "4 year ago"
      }},
      {"Months", {
        "This month", "Last month", "2 months ago", "3 months ago",
         "4 months ago", "5 months ago", "6 months ago"
      }},
      {"Weeks", {
        "This week", "Last week", "2 week ago", "3 week ago", "4 week ago",
        "5 week ago", "6 week ago", "7 week ago", "8 week ago", "9 week ago"
      }},
      {"Days", {
        "Today", "Yesterday", "2 days ago", "3 days ago", "4 days ago",
        "5 days ago", "6 days ago", "7 days ago", "8 days ago", "9 days ago",
        "10 days ago", "11 days ago", "12 days ago", "13 days ago", "14 days ago"
      }},
    });
    filter_tree.set_root(root);
    filter_tree.open_all();
    filter_tree.update_node_list();
    weekdays.set_data<std::string>({"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"});

    category_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    category_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    category_view.header.set_labels({"Category", "Duration", "Costs", "Budget"});
    category_view.list.on_selection_changed([&] (event_source) {
      auto csel = category_view.list.get_selection();
      if (csel < categories.size()) {
        tt_category& category = categories[csel];
        project_view.list.set_count(category.projects.size());
      } else {
        project_view.list.set_count(0);
      }
      entry_view.list.set_count(0);
    });

    project_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    project_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    project_view.header.set_labels({"Project", "Duration", "Costs", "Budget", "Costs/h"});

    project_view.list.on_selection_changed([&] (event_source) {
      auto csel = category_view.list.get_selection();
      if (csel < categories.size()) {
        tt_category& category = categories[csel];
        auto psel = project_view.list.get_selection();
        if (psel < category.projects.size()) {
          tt_project& project = category.projects[psel];
          entry_view.list.set_count(project.events.size());
          return;
        }
      }
      entry_view.list.set_count(0);
    });

    entry_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    entry_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    entry_view.get_column_layout().set_column_align(7, text_origin::vcenter_left);
    entry_view.header.set_labels({"Project", "Begin", "End", "Duration", "Price", "Costs", "Budget", "Comment", "Costs/h"});

    category_view.set_data([&](std::size_t i) {
      auto& c = categories[i];
      return category_view_type::row{c.name, c.duration(), c.costs(), c.budget()};
    }, 0);
    project_view.set_data([&](std::size_t i) {
      auto csel = category_view.list.get_selection();
      if ((csel > -1) && (csel < categories.size())) {
        auto& c = categories[csel];
        if (i < c.projects.size()) {
          auto& p = c.projects[i];
          return project_view_type::row{p.name, p.duration(), p.costs(), c.budget(), p.price_per_hour};
        }
      }
      return project_view_type::row{};
    }, 0);
    entry_view.set_data([&] (std::size_t i) {
      auto csel = category_view.list.get_selection();
      auto psel = project_view.list.get_selection();
      if ((csel > -1) && (csel < categories.size()) && (psel > -1)) {
        auto& c = categories[csel];
        if (psel < c.projects.size()) {
          auto& p = c.projects[psel];
          if (i < p.events.size()) {
            auto& e = p.events[i];
            return entry_view_type::row{p.name, e.begin, e.end, e.duration(), e.costs(), e.costs(), c.budget(), e.comment, p.price_per_hour};
          }
        }
      }
      return entry_view_type::row{};
    }, 0);

    get_layout().set_center_top_bottom_left_right(&content_view, &menu, &status_bar, nullptr, nullptr);
    set_children_visible();
  }

  void start () {}

  void stop () {}

  void open () {
    dir_open_dialog::show(*this, "Open Tracks", "Open", "Cancel", [&] (const sys_fs::path& dir) {
      if (sys_fs::exists(dir)) {
        for (const sys_fs::directory_entry& f : sys_fs::directory_iterator(dir)) {
          if (f.status().type() == sys_fs::file_type::directory) {
            try {
              boost::property_tree::ptree pt;
              sys_fs::path sub(f.path());
              sub /= SETTINGS;
              if (sys_fs::exists(sub)) {
                boost::property_tree::ini_parser::read_ini(sub.string(), pt);
              }
              tt_category cat(f.path().stem().string(), pt);
              cat.read_projects(f.path());
              categories.emplace_back(cat);
            } catch (std::exception& ex) {
              LogFatal << ex;
            }
          }
        }
        category_view.list.set_count(categories.size());
      }
    });

  }
  void about () {
    message_dialog::show(*this, "About TimeTracker", "TimeTracker Version 0.0.1", "Ok");
  }

  void category_add () {}
  void category_remove () {}
  void category_edit () {}
  void category_budgets () {}
  void category_add_budget () {}

  void project_add () {}
  void project_remove () {}
  void project_edit () {}
  void project_proce () {}

  void event_add () {}

  void event_remove () {}
  void event_edit () {}

private:
  typedef horizontal_split_view<tree_view, vertical_list> left_view_type;
  typedef column_list_t<weight_column_list_layout,
                        std::string, duration_type, double, double>
          category_view_type;
  typedef column_list_t<weight_column_list_layout,
                        std::string, duration_type, double, double, double>
          project_view_type;
  typedef column_list_t<weight_column_list_layout,
                        std::string, time_point, time_point,
                        duration_type, double, double, double, std::string, double>
          entry_view_type;
  typedef vertical_split_view<category_view_type, project_view_type> top_view_type;
  typedef horizontal_split_view<top_view_type, entry_view_type> main_view_type;
  typedef client_control<> bottom_view_type;
  typedef horizontal_split_view<main_view_type, bottom_view_type> right_view_type;
  typedef vertical_split_view<left_view_type, right_view_type> content_view_type;

  main_menu menu;
  content_view_type content_view;
  group_window<horizontal_adaption<2, 5>, color::rgb_gray<224>::value> status_bar;

  std::vector<tt_category> categories;

  left_view_type& left_view;
  tree_view& filter_tree;
  vertical_list& weekdays;

  right_view_type& right_view;
  main_view_type& main_view;
  bottom_view_type& bottom_view;

  top_view_type& top_view;
  entry_view_type& entry_view;

  category_view_type& category_view;
  project_view_type& project_view;
};


// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  TimeTracker main;

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("TimeTracker");
  main.set_visible();

  return run_main_loop();
}
