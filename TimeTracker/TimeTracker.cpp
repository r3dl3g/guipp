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

#include "data.h"

#include <gui/layout/border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/sorted_column_list.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/drop_down.h>

#include <util/bind_method.h>
#include <util/ostream_resetter.h>
#include <util/tuple_util.h>

#include <utility>

/* --------------------------------------------------------------------------
 * ToDo:
 * V Write a filter for time ranges.
 * V Write custom tree node info for this filter.
 * - Add dialogs to add/edit/remove categories, projects, events
 * - Read/Write settings.
 * - Add dir watcher (see: extern/dir_monitor)
-------------------------------------------------------------------------- */

using time_point = util::time::time_point;
using duration_type = std::chrono::system_clock::duration;

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

namespace compare {

  template<typename T>
  struct name {
    bool operator () (const T& lhs, const T& rhs) const {
      const auto& l = lhs->name;
      const auto& r = rhs->name;
      return l < r;
    }
  };

  template<typename T>
  struct begin {
    bool operator () (const T& lhs, const T& rhs) const {
      const auto& l = lhs->begin;
      const auto& r = rhs->begin;
      return l < r;
    }
  };

  template<typename T>
  struct end {
    bool operator () (const T& lhs, const T& rhs) const {
      const auto& l = lhs->end;
      const auto& r = rhs->end;
      return l < r;
    }
  };

  template<typename T>
  struct project {
    bool operator () (const T& lhs, const T& rhs) const {
      const auto& l = lhs->project.lock()->name;
      const auto& r = rhs->project.lock()->name;
      return l < r;
    }
  };

  template<typename T>
  struct duration {
    data::time_filter filter;
    bool operator () (const T& lhs, const T& rhs) const {
      return lhs->duration(filter) < rhs->duration(filter);
    }
  };

  template<>
  struct duration<std::shared_ptr<data::tt_event>> {
    bool operator () (const std::shared_ptr<data::tt_event>& lhs, const std::shared_ptr<data::tt_event>& rhs) const {
      return lhs->duration() < rhs->duration();
    }
  };

  template<typename T>
  struct costs {
    data::time_filter filter;
    bool operator () (const T& lhs, const T& rhs) const {
      return lhs->costs(filter) < rhs->costs(filter);
    }
  };

  template<typename T>
  struct budget {
    data::time_filter filter;
    bool operator () (const T& lhs, const T& rhs) const {
      return lhs->budget(filter) < rhs->budget(filter);
    }
  };

} // namespace compare

namespace sort {
  template<typename T, typename C>
  void up (std::vector<T>& v, C c) {
    std::stable_sort(v.begin(), v.end(), c);
  }

  template<typename T, typename C>
  void down (std::vector<T>& v, C c) {
    std::stable_sort(v.rbegin(), v.rend(), c);
  }
} // namespace sort

  namespace controls {

    //-----------------------------------------------------------------------------
    template<typename L, typename E, int N, int I, typename T>
    struct add {
      static void to (L& layout, E labels[N], T& controls) {
        layout.add(layout::lay(labels[I]));
        layout.add(layout::lay(std::get<I>(controls)));
        add<L, E, N, I + 1, T>::to(layout, labels, controls);
      }
    };

    template<typename L, typename E, int N, typename T>
    struct add<L, E, N, N, T> {
      static void to (L&, E[N], T&) {
      }
    };

    template<typename E>
    using init_function = std::function<void(E&e)>;

    //-----------------------------------------------------------------------------
    template<typename E, int N, int I, typename T, typename Initials>
    struct create {
      static void in (container& main, E labels[N], const std::vector<std::string>& message, T& ctrls, const Initials& initials) {
        labels[I].create(main, message[I]);
        std::get<I>(ctrls).create(main);
        std::get<I>(initials)(std::get<I>(ctrls));
        create<E, N, I + 1, T, Initials>::in(main, labels, message, ctrls, initials);
      }
    };

    template<typename E, int N, typename T, typename Initials>
    struct create<E, N, N, T, Initials> {
      static void in (container&, E[N], const std::vector<std::string>&, T&, const Initials&) {
      }
    };
  }

    //-----------------------------------------------------------------------------
    struct input_traits {
      typedef std::tuple<edit_left> Controls;
      typedef std::tuple<controls::init_function<edit_left>> Initials;
      typedef std::tuple<std::string> Types;
    };

    //-----------------------------------------------------------------------------
    template<typename T>
    class /*GUIPP_CTRL_EXPORT*/ multi_control_dialog :
        public standard_dialog<win::group_window<layout::vertical_lineup<20, 15, 2>,
                                                 color::very_light_gray>> {
    public:
      typedef layout::vertical_lineup<20, 15, 2> Layout;
      typedef win::group_window<Layout, color::very_light_gray> content_view_type;
      typedef standard_dialog<content_view_type> super;
      using label_t = basic_label<text_origin::bottom_left,
                                  draw::frame::no_frame,
                                  color::black,
                                  color::very_light_gray>;

      using Controls = typename T::Controls;
      using Types = typename T::Types;
      using Initials = typename T::Initials;

      static constexpr size_t N = std::tuple_size<Types>();

      typedef void (action) (const Controls&);

      multi_control_dialog () {
        controls::add<Layout, label_t, N, 0, Controls>::to(content_view.get_layout(), labels, controls);
      }

      void create (win::container& parent,
                   const std::string& title,
                   const std::vector<std::string>& message,
                   const Initials& initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   const core::rectangle& rect,
                   std::function<action> action) {
        super::create(parent, title, rect, [&, action] (int i) {
          if (i == 1) {
            action(controls);
          }
        }, {cancel_label, ok_label});
        controls::create<label_t, N, 0, Controls, Initials>::in(content_view, labels, message, controls, initial);
      }

      static void ask (win::container& parent,
                       const std::string& title,
                       const std::vector<std::string>& message,
                       const Initials& initial,
                       const std::string& ok_label,
                       const std::string& cancel_label,
                       std::function<action> action) {
        multi_control_dialog dialog;
        dialog.create(parent, title, message, initial, ok_label, cancel_label,
                      core::rectangle(300, 200, 400, 85 + N * 44), action);
        dialog.show(parent);
      }

      label_t labels[N];
      Controls controls;
    };


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
    , event_view(main_view.second)
    , category_view(top_view.first)
    , project_view(top_view.second)
    , weekday_filters({
      {"Sunday", 0},
      {"Monday", 1},
      {"Tuesday", 2},
      {"Wednesday", 3},
      {"Thursday", 4},
      {"Friday", 5},
      {"Saturday", 6},
    })
  {
    on_create(util::bind_method(this, &TimeTracker::onCreated));

    category_view.set_drawer(category_view_type::row_drawer{drawer::name, drawer::duration, drawer::money, drawer::money});
    project_view.set_drawer(project_view_type::row_drawer{drawer::name, drawer::duration, drawer::money, drawer::money, drawer::money});
    event_view.set_drawer(event_view_type::row_drawer{drawer::name, drawer::time, drawer::time, drawer::duration, drawer::money, drawer::money, drawer::name, drawer::money});
  }

  ~TimeTracker () {
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
          {"Price per Hour", 'P', util::bind_method(this, &TimeTracker::project_pph)}
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

    const auto now = std::chrono::system_clock::now();
    const auto dt = util::time::local_time(now);
    const int year = dt.tm_year + 1900;
    const int month = dt.tm_mon + 1;
    const int quarter = dt.tm_mon / 3 + 1;
    const int week = dt.tm_yday / 7 + 1;
    const int day = dt.tm_mday;

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
       {"Hours", {
         {"Last Hour",  tt_filter::mk_hour_offset(now, std::chrono::hours(1))},
         {"Last 2 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(2))},
         {"Last 4 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(4))},
         {"Last 8 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(8))},
         {"Last 12 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(12))},
         {"Last 24 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(24))},
         {"Last 36 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(36))},
         {"Last 48 Hours",  tt_filter::mk_hour_offset(now, std::chrono::hours(48))},
      }},
    }});
    filter_tree.set_root(root);
    filter_tree.open_all();
    filter_tree.update_node_list();

    filter_tree.on_selection_changed([&] (event_source) {
      update_events();
    });

    weekdays.set_data(weekday_filters);
    weekdays.on_selection_changed([&] (event_source) {
      update_events();
    });

    category_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    category_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    category_view.header_label = {"Category", "Duration", "Costs", "Budget"};
    category_view.list.on_selection_changed([&] (event_source) {
      if (category_view.list.has_selection()) {
        auto& category = categories[category_view.list.get_selection()];
        project_view.list.set_count(category->projects.size());
      } else {
        project_view.list.set_count(0);
      }
      update_events();
    });

    project_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    project_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    project_view.header_label = {"Project", "Duration", "Costs", "Budget", "Costs/h"};
    project_view.list.on_selection_changed([&] (event_source) {
      update_events();
    });

    event_view.get_column_layout().set_default_align(text_origin::vcenter_right);
    event_view.get_column_layout().set_column_align(0, text_origin::vcenter_left);
    event_view.get_column_layout().set_column_align(7, text_origin::vcenter_left);
    event_view.header_label = {"Project", "Begin", "End", "Duration", "Price", "Budget", "Comment", "Costs/h"};

    
    category_view.set_data([&](std::size_t i) {
      auto& c = categories[i];
      return category_view_type::row{c->name, c->duration(current_time_filter), c->costs(current_time_filter), c->budget(current_time_filter)};
    }, 0);
    category_view.on_sort([&] (util::sort::order dir, int column) {
      switch (dir) {
        case util::sort::order::up:
          switch (column) {
            case 0: sort::up(categories, compare::name<category_t>()); return true;
            case 1: sort::up(categories, compare::duration<category_t>{current_time_filter}); return true;
            case 2: sort::up(categories, compare::costs<category_t>{current_time_filter}); return true;
            case 3: sort::up(categories, compare::budget<category_t>{current_time_filter}); return true;
          }
          break;
        case util::sort::order::down:
          switch (column) {
            case 0: sort::down(categories, compare::name<category_t>()); return true;
            case 1: sort::down(categories, compare::duration<category_t>{current_time_filter}); return true;
            case 2: sort::down(categories, compare::costs<category_t>{current_time_filter}); return true;
            case 3: sort::down(categories, compare::budget<category_t>{current_time_filter}); return true;
          }
          break;
      }
      return false;
    });
    category_view.list.on_left_btn_dblclk([&] (os::key_state, core::point) {
      category_edit();
    });

    project_view.set_data([&](std::size_t i) {
      if (category_view.list.has_selection()) {
        auto& c = categories[category_view.list.get_selection()];
        if (i < c->projects.size()) {
          auto& p = c->projects[i];
          return project_view_type::row{p->name, p->duration(current_time_filter), p->costs(current_time_filter), c->budget(current_time_filter), p->pph};
        }
      }
      return project_view_type::row{};
    }, 0);
    project_view.on_sort([&] (util::sort::order dir, int column) {
      if (!category_view.list.has_selection()) {
        return false;
      }
      auto& c = categories[category_view.list.get_selection()];
      auto& projects = c->projects;

      switch (dir) {
        case util::sort::order::up:
          switch (column) {
            case 0: sort::up(projects, compare::name<project_t>()); return true;
            case 1: sort::up(projects, compare::duration<project_t>{current_time_filter}); return true;
            case 2: sort::up(projects, compare::costs<project_t>{current_time_filter}); return true;
          }
          break;
        case util::sort::order::down:
          switch (column) {
            case 0: sort::down(projects, compare::name<project_t>()); return true;
            case 1: sort::down(projects, compare::duration<project_t>{current_time_filter}); return true;
            case 2: sort::down(projects, compare::costs<project_t>{current_time_filter}); return true;
          }
          break;
      }
      return false;
    });
    project_view.list.on_left_btn_dblclk([&] (os::key_state, core::point) {
      project_edit();
    });

    event_view.set_data([&] (std::size_t i) {
      auto& e = events[i];
      const auto& p = e->project.lock();
      const auto& c = p->category.lock();
      return event_view_type::row{p->name, e->begin, e->end, e->duration(), e->costs(), c->budget(current_time_filter), e->comment, e->price_per_hour()};
    }, 0);
    event_view.on_sort([&] (util::sort::order dir, int column) {
      switch (dir) {
        case util::sort::order::up:
          switch (column) {
            case 0: sort::up(events, compare::project<event_t>()); return true;
            case 1: sort::up(events, compare::begin<event_t>{}); return true;
            case 2: sort::up(events, compare::end<event_t>{}); return true;
            case 3: sort::up(events, compare::duration<event_t>{}); return true;
          }
          break;
        case util::sort::order::down:
          switch (column) {
            case 0: sort::down(events, compare::project<event_t>()); return true;
            case 1: sort::down(events, compare::begin<event_t>{}); return true;
            case 2: sort::down(events, compare::end<event_t>{}); return true;
            case 3: sort::down(events, compare::duration<event_t>{}); return true;
          }
          break;
      }
      return false;
    });

    get_layout().set_center_top_bottom_left_right(layout::lay(content_view), layout::lay(menu), layout::lay(status_bar), nullptr, nullptr);
    set_children_visible();
  }

  void update_filter () {
    current_time_filter = {};
    if (filter_tree.has_selection()) {
      current_time_filter.first = filter_tree.get_item(filter_tree.get_selection())->filter;
    }
    if (weekdays.has_selection()) {
      current_time_filter.second = weekday_filters[weekdays.get_selection()];
    }
  }

  void update_events () {
    update_filter();

    events.clear();
    if (category_view.list.has_selection()) {
      auto& c = categories[category_view.list.get_selection()];
      if (project_view.list.has_selection()) {
        auto& p = c->projects[project_view.list.get_selection()];
        std::copy_if(p->events.begin(), p->events.end(), std::back_inserter(events), current_time_filter);
      } else {
        for (const auto& p : c->projects) {
          std::copy_if(p->events.begin(), p->events.end(), std::back_inserter(events), current_time_filter);
        }
      }
    }
    event_view.sort();
    event_view.list.set_count(events.size());
    category_view.list.invalidate();
    project_view.list.invalidate();
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

  void category_add () {
    input_dialog::ask(*this, "Create category", "Enter new name:", "new", "Okay", "Cancel",
                      [&] (const std::string& name) {
      auto cat = std::make_shared<data::tt_category>(name);
      categories.emplace_back(cat);
      category_view.list.set_count(categories.size());
    });
  }

  void category_remove () {
    if (category_view.list.has_selection()) {
      auto& c = categories[category_view.list.get_selection()];
      yes_no_dialog::ask(*this, "Remove category", ostreamfmt("Are you sure you wan't remove category '" << c->name << "' ?"),
                         "Yes", "Abort",
                         [&] (bool yes) {
        if (yes) {
          categories.erase(categories.cbegin() + category_view.list.get_selection());
          category_view.list.set_count(categories.size());
        }
      });
    }
  }

  void category_edit () {
    if (category_view.list.has_selection()) {
      auto& c = categories[category_view.list.get_selection()];
      input_dialog::ask(*this, "Rename category", "Enter new name:", c->name, "Okay", "Cancel",
                        [&] (const std::string& name) {
        c->name = name;
        category_view.list.invalidate();
      });
    }
  }

  void category_budgets () {}
  void category_add_budget () {}

  void project_add () {
    if (category_view.list.has_selection()) {
      auto& c = categories[category_view.list.get_selection()];
      multi_input_dialog<std::string, double>::ask(*this, "Create Project",
                                                   std::vector<std::string>({"Project name:", "Costs per Hour:"}),
                                                   std::make_tuple(std::string("new"), 60.0),
                                                   "Okay", "Cancel",
                                                   [&] (const std::tuple<std::string, double>& t) {
        auto name = std::get<0>(t);
        auto costs = std::get<1>(t);

        auto prj = std::make_shared<data::tt_project>(c, name, costs);
        c->projects.emplace_back(prj);
        project_view.list.set_count(c->projects.size());
      });
    }
  }
  void project_remove () {
    if (category_view.list.has_selection() && project_view.list.has_selection()) {
      auto& c = categories[category_view.list.get_selection()];
      auto& p = c->projects[project_view.list.get_selection()];
      yes_no_dialog::ask(*this, "Remove project",
                         ostreamfmt("Are you sure you wan't remove project '" << p->name << "' from category '" << c->name << "'?"),
                         "Yes", "Abort",
                         [&] (bool yes) {
        if (yes) {
          c->projects.erase(c->projects.cbegin() + project_view.list.get_selection());
          project_view.list.set_count(c->projects.size());
        }
      });
    }
  }

  void project_edit () {
    if (category_view.list.has_selection() && project_view.list.has_selection()) {
      auto& c = categories[category_view.list.get_selection()];
      auto& p = c->projects[project_view.list.get_selection()];
      multi_input_dialog<std::string, double>::ask(*this, "Edit project",
                                                   /*std::vector<std::string>(*/{"New project name:", "Costs per Hour:"}/*)*/,
                                                   std::make_tuple(p->name, p->pph),
                                                   "Okay", "Cancel",
                                                   [&] (const std::tuple<std::string, double>& t) {
        p->name = std::get<0>(t);
        p->pph = std::get<1>(t);
        project_view.list.invalidate();
      });
    }
  }

  void project_pph () {}

  void event_add () {}

  void event_remove () {}

  void event_edit () {
    if (event_view.list.has_selection()) {
      auto& e = events[event_view.list.get_selection()];

      typedef drop_down_list<std::string> drop_down;
      typedef controls::init_function<edit_left> edit_init;
      typedef controls::init_function<drop_down> drop_init;

      struct event_traits {
        typedef std::tuple<edit_left, edit_left, edit_left, drop_down> Controls;
        typedef std::tuple<edit_init, edit_init, edit_init, drop_init> Initials;
        typedef std::tuple<std::string, time_point, time_point, std::string> Types;
      };

      std::vector<std::string> data({e->comment, "Eins", "Zwei", "Drei"});

      event_traits::Initials inits = event_traits::Initials(
                                       [e] (edit_left& ctrl) {
                                         ctrl.set_text(e->id);
                                       },
                                       [e] (edit_left& ctrl) {
                                         ctrl.set_text(ostreamfmt(e->begin));
                                       },
                                       [e] (edit_left& ctrl) {
                                         ctrl.set_text(ostreamfmt(e->end));
                                       },
                                       [&] (drop_down& ctrl) {
                                         ctrl.set_data([&] (size_t idx) {
                                           return data[idx];
                                         }, data.size());
                                         ctrl.set_selected_item(data[0]);
                                       }
                                     );

      multi_control_dialog<event_traits>::ask(*this, "Edit event",
                                              std::vector<std::string>({"Id:", "Begin:", "End:", "Comment:"}),
                                              inits,
                                              "Okay", "Cancel",
                                              [&] (const event_traits::Controls& ctrls) {
        auto id = std::get<0>(ctrls).get_text();
        auto begin = std::get<1>(ctrls).get_text();
        auto end = std::get<2>(ctrls).get_text();
        auto comment = std::get<3>(ctrls).get_selected_item();

        message_dialog::show(*this, "Info", ostreamfmt("Id:" << id << ", begin:" << begin << ", end:" << end << ", comment:" << comment), "Ok");
      });

    }

  }

private:
  typedef tree::basic_tree<tt_filter::node_info> filter_tree_view;
  typedef horizontal_split_view<filter_tree_view, vertical_list> left_view_type;
  typedef sorted_column_list_t<weight_column_list_layout,
                               std::string, duration_type, double, double>
          category_view_type;
  typedef sorted_column_list_t<weight_column_list_layout,
                               std::string, duration_type, double, double, double>
          project_view_type;
  typedef sorted_column_list_t<weight_column_list_layout,
                               std::string, time_point, time_point,
                               duration_type, double, double, std::string, double>
          event_view_type;
  typedef vertical_split_view<category_view_type, project_view_type> top_view_type;
  typedef horizontal_split_view<top_view_type, event_view_type> main_view_type;
  typedef client_control<> bottom_view_type;
  typedef horizontal_split_view<main_view_type, bottom_view_type> right_view_type;
  typedef vertical_split_view<left_view_type, right_view_type> content_view_type;
  typedef gui::ctrl::simple_list_data<tt_filter::weekday> weekday_filters_type;
  typedef std::shared_ptr<data::tt_category> category_t;
  typedef std::shared_ptr<data::tt_project> project_t;
  typedef std::shared_ptr<data::tt_event> event_t;

  main_menu menu;
  content_view_type content_view;
  status_bar_t status_bar;

  std::vector<category_t> categories;
  std::vector<event_t> events;

  weekday_filters_type weekday_filters;
  data::time_filter current_time_filter;

  left_view_type& left_view;
  filter_tree_view& filter_tree;
  vertical_list& weekdays;

  right_view_type& right_view;
  main_view_type& main_view;

  top_view_type& top_view;

  category_view_type& category_view;
  project_view_type& project_view;
  event_view_type& event_view;
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
