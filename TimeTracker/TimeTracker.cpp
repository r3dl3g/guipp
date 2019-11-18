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
#include <functional>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <util/bind_method.h>
#include <util/time_util.h>
#include <util/ostream_resetter.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/tree.h>
#include <gui/ctrl/column_list.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/std_dialogs.h>

/* --------------------------------------------------------------------------
 * ToDo:
 * - Write a filter for time ranges.
 * - Write custom tree node info for this filter.
 * - Add dir watcher (see: extern/dir_monitor)
 * - Add dialogs to add/edit/remove categories, projects, events
 * - Read/Write settings.
-------------------------------------------------------------------------- */

using time_point = util::time::time_point;
using duration_type = std::chrono::system_clock::duration;
using ptree = boost::property_tree::ptree;

// --------------------------------------------------------------------------
namespace std {

  std::ostream& operator<< (std::ostream& o, time_point const& tp) {
    return util::time::operator<<(o, tp);
  }

  std::istream& operator>> (std::istream& i, time_point& tp) {
    return util::time::operator>>(i, tp);
  }

  std::ostream& operator<< (std::ostream& o, duration_type const& d) {
    return util::time::format_duration_only_h(o, d);
  }

}

// --------------------------------------------------------------------------
namespace data {

  struct tt_category;
  struct tt_project;
  struct tt_event;
  struct tt_price_per_hour;
  struct tt_budget;

  struct date {
    date ()
    {}

    date (uint32_t year, uint32_t month = 0, uint32_t day = 0)
      : value(util::time::mktime_point(year, month, day))
    {}

    date (const time_point& dt)
      : value(dt)
    {}

    operator const time_point&() const {
      return value;
    }

    time_point value;
  };

  std::ostream& operator<< (std::ostream& out, const date& dt) {
    return util::time::format_date(out, dt.value);
  }

  std::istream& operator>> (std::istream& in, date& dt) {
    dt = util::time::parse_date(in);
    return in;
  }

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

    tt_price_per_hour (const ptree& pt, int num)
      : begin(pt.get(ptree::path_type(ostreamfmt(PRICE_PER_HOUR << num << BEGIN), '/'), date()))
      , price(pt.get(ptree::path_type(ostreamfmt(PRICE_PER_HOUR << num << PRICE), '/'), 0.0))
    {}

    void write (ptree& pt, int num) {
      pt.put(ptree::path_type(ostreamfmt(PRICE_PER_HOUR << num << BEGIN), '/'), begin);
      pt.put(ptree::path_type(ostreamfmt(PRICE_PER_HOUR << num << PRICE), '/'), price);
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

    tt_budget (const ptree& pt, int num)
      : begin(pt.get(ostreamfmt(DATE << num), date()))
      , budget(pt.get(ostreamfmt(BUDGET << num), 0.0))
    {}

    void write (ptree& pt, int num) {
      pt.put(ostreamfmt(DATE << num), begin);
      pt.get(ostreamfmt(BUDGET << num), budget);
    }
  };

  const std::string tt_budget::DATE = "Date";
  const std::string tt_budget::BUDGET = "Budget";

  // --------------------------------------------------------------------------
  struct tt_event : std::enable_shared_from_this<tt_event> {
    static const std::string BEGIN;
    static const std::string END;
    static const std::string COMMENT;

    std::string id;
    time_point begin;
    time_point end;
    std::string comment;
    std::shared_ptr<tt_project> project;

    duration_type duration () const {
      return end > begin ? end - begin : duration_type::zero();
    }

    double costs () const;
    double price_per_hour () const;

//    tt_event ()
//    {}

    tt_event (std::shared_ptr<tt_project> project, const std::string& id, const ptree& pt)
      : id(id)
      , begin(pt.get(BEGIN, time_point()))
      , end(pt.get(END, time_point()))
      , comment(pt.get(COMMENT, std::string()))
      , project(project)
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

    void write (ptree& pt) {
      pt.put(BEGIN, begin);
      pt.put(END, end);
      pt.put(COMMENT, comment);
    }

    operator const time_point& () const {
      return begin;
    }

  };

  const std::string tt_event::BEGIN = "begin";
  const std::string tt_event::END = "end";
  const std::string tt_event::COMMENT = "comment";
  const std::string PROPS = ".props";
  const std::string SETTINGS = "settings.props";

  // --------------------------------------------------------------------------
  struct tt_project : std::enable_shared_from_this<tt_project> {
    static const std::string PRICE_PER_HOUR;
    static const std::string PRICE_PER_HOUR_COUNT;

    std::string name;
    double pph;
    std::vector<std::shared_ptr<tt_event>> events;
    std::vector<tt_price_per_hour> prices_per_hour;
    std::shared_ptr<tt_category> category;

    tt_project (std::shared_ptr<tt_category> category, const std::string& name, double price_per_hour = 0.0)
      : name(name)
      , pph(price_per_hour)
      , category(category)
    {}

    tt_project (std::shared_ptr<tt_category> category, const std::string& name, const ptree& pt)
      : name(name)
      , pph(pt.get(PRICE_PER_HOUR, 0.0F))
      , category(category)
    {
      int count = pt.get(ptree::path_type(PRICE_PER_HOUR_COUNT, '/'), 0);
      for (int i = 0; i < count; ++i) {
        prices_per_hour.emplace_back(tt_price_per_hour(pt, i));
      }
      std::sort(prices_per_hour.begin(), prices_per_hour.end(), [](const tt_price_per_hour&l, const tt_price_per_hour&r) {
        return l.begin.value > r.begin.value;
      });
    }

    double price_per_hour (const time_point& begin) const {
      for (const tt_price_per_hour& pPH : prices_per_hour) {
        if (pPH.begin.value <= begin) {
          return pPH.price;
        }
      }
      return pph;
    }

    double price_per_second (const time_point& begin) const {
      for (const tt_price_per_hour& pPH : prices_per_hour) {
        if (pPH.begin.value <= begin) {
          return pPH.price / 3600.0;
        }
      }
      return pph / 3600.0;
    }

    duration_type duration () const {
      duration_type d = duration_type::zero();
      for (auto& i : events) {
        d += i->duration();
      }
      return d;
    }

    double costs () const {
      double d = 0;
      for (auto& i : events) {
        d += i->costs();
      }
      return d;
    }

    void read_events (const sys_fs::path& dir) {
      for (const sys_fs::directory_entry& f : sys_fs::directory_iterator(dir)) {
        if ((f.path().extension().string() == PROPS) && (f.path().filename().string() != SETTINGS)) {
          try {
            ptree pt;
            boost::property_tree::ini_parser::read_ini(f.path().string(), pt);
            events.emplace_back(std::make_shared<tt_event>(shared_from_this(), f.path().stem().string(), pt));
          } catch (std::exception& ex) {
            LogFatal << ex;
          }
        }
      }
    }

    void write (ptree& pt) {
      pt.put(PRICE_PER_HOUR, pph);
      auto count = prices_per_hour.size();
      pt.put(ptree::path_type(PRICE_PER_HOUR_COUNT, '/'), count);
      for (auto i = 0; i < count; ++i) {
        prices_per_hour[i].write(pt, i);
      }
    }
  };

  const std::string tt_project::PRICE_PER_HOUR = "PricePerHour";
  const std::string tt_project::PRICE_PER_HOUR_COUNT = "PricePerHour.count";

  // --------------------------------------------------------------------------
  struct tt_category : std::enable_shared_from_this<tt_category> {
    static const std::string BUDGET_COUNT;

    std::string name;
    std::vector<std::shared_ptr<tt_project>> projects;
    std::vector<tt_budget> budgets;

    tt_category (const std::string& name)
      : name (name)
    {}

    tt_category (const std::string& name, const ptree& pt)
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
        d += i->duration();
      }
      return d;
    }

    double costs () const {
      double d = 0;
      for (auto& i : projects) {
        d += i->costs();
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
            ptree pt;
            sys_fs::path sub(f.path());
            sub /= SETTINGS;
            if (sys_fs::exists(sub)) {
              boost::property_tree::ini_parser::read_ini(sub.string(), pt);
            }
            std::shared_ptr<tt_project> prj = std::make_shared<tt_project>(shared_from_this(), f.path().stem().string(), pt);
            prj->read_events(f.path());
            projects.emplace_back(std::move(prj));
          } catch (std::exception& ex) {
            LogFatal << ex;
          }
        }
      }
    }

    void write (ptree& pt) {
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
    return project->price_per_second(begin) * std::chrono::duration_cast<std::chrono::seconds>(duration()).count();
  }

  double tt_event::price_per_hour () const {
    return project->price_per_hour(begin);
  }

} // namespace data

namespace tt_filter {

  typedef bool (filter_fnct) (const std::shared_ptr<data::tt_event>& t);

  struct any {
    bool operator () (const std::shared_ptr<data::tt_event>& t) const {
      return true;
    }
  };

  struct none {
    bool operator () (const std::shared_ptr<data::tt_event>& t) const {
      return false;
    }
  };

  struct min_max {
    min_max (const time_point& min, const time_point& max)
      : min(min)
      , max(max)
    {}

    min_max (time_point&& min, time_point&& max)
      : min(std::move(min))
      , max(std::move(max))
    {}

    bool operator () (const std::shared_ptr<data::tt_event>& t) const {
      return (min <= t->begin) && (t->begin < max);
    }

    const time_point min;
    const time_point max;
  };

  struct weekday {
    weekday (int day)
      : day(day)
    {}

    bool operator () (const time_point& t) const {
      return day == util::time::local_time(t).tm_wday;
    }

    const int day;
  };

  inline min_max mk_year (int y) {
    return {util::time::mktime_point(y, 1, 1), util::time::mktime_point(y + 1, 1, 1)};
  }

  inline min_max mk_quarter (int y, int q) {
    return {util::time::mktime_point(y, q * 3 - 2, 1), util::time::mktime_point(y, q * 3 + 1, 1)};
  }

  inline min_max mk_month (int y, int m) {
    return {util::time::mktime_point(y, m, 1), util::time::mktime_point(y, m + 1, 1)};
  }

  inline min_max mk_week (int y, int w) {
    return {util::time::mktime_point(y, 1, w * 7 - 6), util::time::mktime_point(y, 1, w * 7 + 1)};
  }

  inline min_max mk_day (int y, int m, int d) {
    return {util::time::mktime_point(y, m, d), util::time::mktime_point(y, m, d + 1)};
  }

  inline min_max mk_hour_offset (const time_point& now, const std::chrono::hours& h) {
    return {now - h, now + h};
  }

  struct node {
    typedef std::vector<node> node_list;
    typedef node_list::const_iterator iterator;

    node ()
    {}

    node (const std::string& label, std::initializer_list<node> nodes)
      : label(label)
      , sub_nodes(nodes)
      , filter(none())
    {}

    node (const std::string& label, std::function<filter_fnct> filter)
      : label(label)
      , filter(filter)
    {}

    void add_nodes (std::initializer_list<node> nodes) {
      sub_nodes.insert(sub_nodes.end(), nodes.begin(), nodes.end());
    }

    std::string label;
    std::function<filter_fnct> filter;
    node_list sub_nodes;
  };

  struct node_info {
    typedef node type;
    typedef node::iterator iterator;
    typedef const node* reference;
    typedef gui::core::range<iterator> node_range;

    static bool has_sub_nodes (const type& n) {
      return !n.sub_nodes.empty();
    }

    static node_range sub_nodes (type const& n) {
      return node_range(n.sub_nodes.begin(), n.sub_nodes.end());
    }

    static reference make_reference (type const& n) {
      return &n;
    }

    static type const& dereference (reference const& r) {
      return *r;
    }

    static std::string label (type const& n) {
      return n.label;
    }

    static const gui::draw::masked_bitmap& icon (type const&,
                                            bool has_children,
                                            bool is_open,
                                            bool selected) {
      return gui::ctrl::tree::standard_icon(has_children, is_open, selected);
    }


  };

} // namespace filter

// --------------------------------------------------------------------------
using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;
using namespace gui::color;

// --------------------------------------------------------------------------
class status_bar_t : public group_window<horizontal_adaption<2, 10>, very_very_light_gray> {
public:
  typedef group_window<gui::layout::horizontal_adaption<2, 10>, very_very_light_gray> super;

  using status_label = basic_label<text_origin::vcenter_left, frame::sunken_relief, black, very_very_light_gray>;

  status_bar_t ();

  status_label labels[4];
};

// --------------------------------------------------------------------------
status_bar_t::status_bar_t () {
  on_create([&] (window*, const core::rectangle&) {
    for (status_label& l : labels) {
      l.create(*this);
      get_layout().add(lay(l));
    }
    set_children_visible();
  });
}

namespace drawer {

  // --------------------------------------------------------------------------
  void name (const std::string& txt, const draw::graphics& g, const core::rectangle& r, const draw::brush& b,  item_state s, text_origin align) {
    ctrl::paint::text_item(g, r, b, txt, s, align);
    draw::frame::lines(g, r);
  }

  void duration (const duration_type& d, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, item_state s, text_origin align) {
    ctrl::paint::text_item(g, r, b, ostreamfmt(d), s, align);
    draw::frame::lines(g, r);
  }

  void money (const double& v, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, item_state s, text_origin align) {
    ctrl::paint::text_item(g, r, b, ostreamfmt(std::setprecision(2) << std::fixed << v << " €"), s, align);
    draw::frame::lines(g, r);
  }

  void time (const time_point& t, const draw::graphics& g, const core::rectangle& r, const draw::brush& b, item_state s, text_origin align) {
    ctrl::paint::text_item(g, r, b, ostreamfmt(t), s, align);
    draw::frame::lines(g, r);
  }

}

// --------------------------------------------------------------------------
class TimeTracker : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;

  TimeTracker ()
    : super(20, 30, 0, 0)
    , left_view(content_view.first)
    , filter_tree(left_view.first)
    , weekdays(left_view.second)
    , right_view(content_view.second)
    , main_view(right_view.first)
    , top_view(main_view.first)
    , entry_view(main_view.second)
    , category_view(top_view.first)
    , project_view(top_view.second)
  {
    on_create(util::bind_method(this, &TimeTracker::onCreated));

    category_view.set_drawer(category_view_type::row_drawer{drawer::name, drawer::duration, drawer::money, drawer::money});
    project_view.set_drawer(project_view_type::row_drawer{drawer::name, drawer::duration, drawer::money, drawer::money, drawer::money});
    entry_view.set_drawer(entry_view_type::row_drawer{drawer::name, drawer::time, drawer::time, drawer::duration, drawer::money, drawer::money, drawer::name, drawer::money});
  }

  void onCreated (window* w, const core::rectangle& r) {
    content_view.create(*this, r.with_pos(core::point::zero), 0.25);
    content_view.first.set_split_pos(0.8);
    right_view.set_split_pos(0.8);
    status_bar.create(*this);

    menu.data.add_entries(
    {
      main_menu_entry("Open", 'O', util::bind_method(this, &TimeTracker::open)),
      main_menu_entry("Start", 'S', util::bind_method(this, &TimeTracker::start)),
      main_menu_entry("Stop", 't', util::bind_method(this, &TimeTracker::stop)),
      main_menu_entry("Category", 'C', [&] () {
        popup_menu({
          {"Add", 'A', util::bind_method(this, &TimeTracker::category_add)},
          {"Remove", 'R', util::bind_method(this, &TimeTracker::category_remove)},
          {"Edit", 'E', util::bind_method(this, &TimeTracker::category_edit)},
          {"Budgets", 'B', util::bind_method(this, &TimeTracker::category_budgets)},
          {"Add Budget", 'g', util::bind_method(this, &TimeTracker::category_add_budget)}
        }).popup(menu);
      }),
      main_menu_entry("Project", 'P', [&] () {
        popup_menu({
          {"Add", 'A', util::bind_method(this, &TimeTracker::project_add)},
          {"Remove", 'R', util::bind_method(this, &TimeTracker::project_remove)},
          {"Edit", 'E', util::bind_method(this, &TimeTracker::project_edit)},
          {"Price per Hour", 'P', util::bind_method(this, &TimeTracker::project_proce)}
        }).popup(menu);
      }),
      main_menu_entry("Event", 'E', [&] () {
        popup_menu({
          {"Add", 'A', util::bind_method(this, &TimeTracker::event_add)},
          {"Remove", 'R', util::bind_method(this, &TimeTracker::event_remove)},
          {"Edit", 'E', util::bind_method(this, &TimeTracker::event_edit)}
        }).popup(menu);
      }),
      main_menu_entry("About", 'A', util::bind_method(this, &TimeTracker::about))
    });
    menu.create(*this);

    const auto now = util::time::local_time_now();
    const int year = now.tm_year + 1900;
    const int month = now.tm_mon + 1;
    const int quarter = now.tm_mon / 4 + 1;
    const int week = now.tm_yday / 7;
    const int day = now.tm_mday;

    tt_filter::node root({"Filter", {
      {"Years", {
        {"This year", tt_filter::mk_year(year)},
        {"Last year", tt_filter::mk_year(year - 1)},
        {"2 year ago", tt_filter::mk_year(year - 2)},
        {"3 year ago", tt_filter::mk_year(year - 3)},
        {"4 year ago", tt_filter::mk_year(year - 4)}
      }},
      {"Quarters", {
        {"This quarter", tt_filter::mk_quarter(year, quarter)},
        {"Last quarter", tt_filter::mk_quarter(year, quarter - 1)},
        {"2 quarters ago", tt_filter::mk_quarter(year, quarter - 2)},
        {"3 quarters ago", tt_filter::mk_quarter(year, quarter - 3)},
        {"4 quarters ago", tt_filter::mk_quarter(year, quarter - 4)},
      }},
      {"Months", {
        {"This month", tt_filter::mk_month(year, month)},
        {"Last month", tt_filter::mk_month(year, month - 1)},
        {"2 months ago", tt_filter::mk_month(year, month - 2)},
        {"3 months ago", tt_filter::mk_month(year, month - 3)},
        {"4 months ago", tt_filter::mk_month(year, month - 4)},
        {"5 months ago", tt_filter::mk_month(year, month - 5)},
        {"6 months ago", tt_filter::mk_month(year, month - 6)}
      }},
      {"Weeks", {
        {"This week", tt_filter::mk_week(year, week)},
        {"Last week", tt_filter::mk_week(year, week - 1)},
        {"2 week ago", tt_filter::mk_week(year, week - 2)},
        {"3 week ago", tt_filter::mk_week(year, week - 3)},
        {"4 week ago", tt_filter::mk_week(year, week - 4)},
        {"5 week ago", tt_filter::mk_week(year, week - 5)},
        {"6 week ago", tt_filter::mk_week(year, week - 6)},
        {"7 week ago", tt_filter::mk_week(year, week - 7)},
        {"8 week ago", tt_filter::mk_week(year, week - 8)},
        {"9 week ago", tt_filter::mk_week(year, week - 9)},
      }},
      {"Days", {
        {"Today",  tt_filter::mk_day(year, month, day)},
        {"Yesterday", tt_filter::mk_day(year, month, day - 1)},
        {"2 days ago", tt_filter::mk_day(year, month, day - 2)},
        {"3 days ago", tt_filter::mk_day(year, month, day - 3)},
        {"4 days ago", tt_filter::mk_day(year, month, day - 4)},
        {"5 days ago", tt_filter::mk_day(year, month, day - 5)},
        {"6 days ago", tt_filter::mk_day(year, month, day - 6)},
        {"7 days ago", tt_filter::mk_day(year, month, day - 7)},
        {"8 days ago", tt_filter::mk_day(year, month, day - 8)},
        {"9 days ago", tt_filter::mk_day(year, month, day - 9)},
        {"10 days ago", tt_filter::mk_day(year, month, day - 10)},
        {"11 days ago", tt_filter::mk_day(year, month, day - 11)},
        {"12 days ago", tt_filter::mk_day(year, month, day - 12)},
        {"13 days ago", tt_filter::mk_day(year, month, day - 13)},
        {"14 days ago", tt_filter::mk_day(year, month, day - 14)},
      }},
    }});
    filter_tree.set_root(root);
    filter_tree.open_all();
    filter_tree.update_node_list();
    weekdays.set_data<std::string>({"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"});

    filter_tree.on_selection_changed([&] (event_source) {
      update_events();
    });

    category_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    category_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    category_view.header.set_labels({"Category", "Duration", "Costs", "Budget"});
    category_view.list.on_selection_changed([&] (event_source) {
      auto csel = category_view.list.get_selection();
      if (csel < categories.size()) {
        auto& category = categories[csel];
        project_view.list.set_count(category->projects.size());
      } else {
        project_view.list.set_count(0);
      }
      update_events();
    });

    project_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    project_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    project_view.header.set_labels({"Project", "Duration", "Costs", "Budget", "Costs/h"});
    project_view.list.on_selection_changed([&] (event_source) {
      update_events();
    });

    entry_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    entry_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    entry_view.get_column_layout().set_column_align(7, text_origin::vcenter_left);
    entry_view.header.set_labels({"Project", "Begin", "End", "Duration", "Price", "Budget", "Comment", "Costs/h"});

    category_view.set_data([&](std::size_t i) {
      auto& c = categories[i];
      return category_view_type::row{c->name, c->duration(), c->costs(), c->budget()};
    }, 0);
    project_view.set_data([&](std::size_t i) {
      auto csel = category_view.list.get_selection();
      if ((csel > -1) && (csel < categories.size())) {
        auto& c = categories[csel];
        if (i < c->projects.size()) {
          auto& p = c->projects[i];
          return project_view_type::row{p->name, p->duration(), p->costs(), c->budget(), p->pph};
        }
      }
      return project_view_type::row{};
    }, 0);
    entry_view.set_data([&] (std::size_t i) {
      auto& e = events[i];
      const auto& p = e->project;
      const auto& c = p->category;
      return entry_view_type::row{p->name, e->begin, e->end, e->duration(), e->costs(), c->budget(), e->comment, e->price_per_hour()};
    }, 0);

    get_layout().set_center_top_bottom_left_right(layout::lay(content_view), layout::lay(menu), layout::lay(status_bar), nullptr, nullptr);
    set_children_visible();
  }

  void update_events () {
    std::function<tt_filter::filter_fnct> time_filter;
    if (filter_tree.get_selection() > -1) {
      time_filter = filter_tree.get_item(filter_tree.get_selection())->filter;
    } else {
      time_filter = tt_filter::any();
    }

    events.clear();
    auto csel = category_view.list.get_selection();
    if ((csel > -1) && (csel < categories.size())) {
      auto& c = categories[csel];
      auto psel = project_view.list.get_selection();
      if ((psel > -1) && (psel < c->projects.size())) {
        auto& p = c->projects[psel];
        std::copy_if(p->events.begin(), p->events.end(), std::back_inserter(events), time_filter);
      } else {
        for (const auto& p : c->projects) {
          std::copy_if(p->events.begin(), p->events.end(), std::back_inserter(events), time_filter);
        }
      }
    }
    entry_view.list.set_count(events.size());
  }

  void start () {}

  void stop () {}

  void open () {
    dir_open_dialog::show(*this, "Open Tracks", "Open", "Cancel", [&] (const sys_fs::path& dir) {
      if (sys_fs::exists(dir)) {
        for (const sys_fs::directory_entry& f : sys_fs::directory_iterator(dir)) {
          if (f.status().type() == sys_fs::file_type::directory) {
            try {
              ptree pt;
              sys_fs::path sub(f.path());
              sub /= data::SETTINGS;
              if (sys_fs::exists(sub)) {
                boost::property_tree::ini_parser::read_ini(sub.string(), pt);
              }
              auto cat = std::make_shared<data::tt_category>(f.path().stem().string(), pt);
              cat->read_projects(f.path());
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
  typedef tree::basic_tree<tt_filter::node_info> filter_tree_view;
  typedef horizontal_split_view<filter_tree_view, vertical_list> left_view_type;
  typedef column_list_t<weight_column_list_layout,
                        std::string, duration_type, double, double>
          category_view_type;
  typedef column_list_t<weight_column_list_layout,
                        std::string, duration_type, double, double, double>
          project_view_type;
  typedef column_list_t<weight_column_list_layout,
                        std::string, time_point, time_point,
                        duration_type, double, double, std::string, double>
          entry_view_type;
  typedef vertical_split_view<category_view_type, project_view_type> top_view_type;
  typedef horizontal_split_view<top_view_type, entry_view_type> main_view_type;
  typedef client_control<> bottom_view_type;
  typedef horizontal_split_view<main_view_type, bottom_view_type> right_view_type;
  typedef vertical_split_view<left_view_type, right_view_type> content_view_type;

  main_menu menu;
  content_view_type content_view;
  status_bar_t status_bar;

  std::vector<std::shared_ptr<data::tt_category>> categories;
  std::vector<std::shared_ptr<data::tt_event>> events;

  left_view_type& left_view;
  filter_tree_view& filter_tree;
  vertical_list& weekdays;

  right_view_type& right_view;
  main_view_type& main_view;

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
