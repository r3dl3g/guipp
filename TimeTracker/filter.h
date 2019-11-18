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

#include <util/time_util.h>
#include <gui/ctrl/tree.h>

#include <limits>

// --------------------------------------------------------------------------
namespace tt_filter {

  using time_point = util::time::time_point;

  typedef bool (filter_fnct) (const time_point& t);

  struct any {
    bool operator () (const time_point& t) const {
      return true;
    }
  };

  struct none {
    bool operator () (const time_point& t) const {
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

    bool operator () (const time_point& t) const {
      return (min <= t) && (t < max);
    }

    const time_point min;
    const time_point max;
  };

  struct last {
    last (const time_point& min)
      : min(min)
    {}

    last (time_point&& min)
      : min(std::move(min))
    {}

    bool operator () (const time_point& t) const {
      return min <= t;
    }

    const time_point min;
  };

  struct weekday {
    weekday (const std::string& label, int day)
      : label(label)
      , day(day)
    {}

    bool operator () (const time_point& t) const {
      return day == util::time::local_time(t).tm_wday;
    }

    const std::string label;
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

  inline last mk_hour_offset (const time_point& now, const std::chrono::hours& h) {
    return {now - h};
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

namespace gui {
  namespace ctrl {

    template<>
    inline std::string convert_to_string<tt_filter::weekday> (const tt_filter::weekday& wd) {
      return wd.label;
    }

  }
}
