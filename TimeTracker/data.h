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

#include "filter.h"

#include <logging/logger.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include <iomanip>
#include <functional>

// --------------------------------------------------------------------------
namespace std {

  ostream& operator<< (ostream& o, util::time::time_point const& tp) {
    return util::time::operator<<(o, tp);
  }

  istream& operator>> (istream& i, util::time::time_point& tp) {
    return util::time::operator>>(i, tp);
  }

  ostream& operator<< (ostream& o, chrono::system_clock::duration const& d) {
    return util::time::format_duration_only_h(o, d);
  }

}

// --------------------------------------------------------------------------
namespace data {

  using time_point = util::time::time_point;
  using duration_type = std::chrono::system_clock::duration;
  using ptree = boost::property_tree::ptree;

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

    typedef std::weak_ptr<tt_project> project_t;

    static const std::string BEGIN;
    static const std::string END;
    static const std::string COMMENT;

    std::string id;
    time_point begin;
    time_point end;
    std::string comment;
    project_t project;

    duration_type duration () const {
      return end > begin ? end - begin : duration_type::zero();
    }

    double costs () const;
    double price_per_hour () const;

    ~tt_event () {
      clog::trace() << "Delete event '" << id << "'";
    }

    tt_event (project_t project, const std::string& id, const ptree& pt)
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
  struct time_filter {

    time_filter ()
      : first(tt_filter::any())
      , second(tt_filter::any())
    {}

    bool operator () (const time_point& t) const {
      return first(t) && second(t);
    }

    bool operator () (const std::shared_ptr<data::tt_event>& t) const {
      return operator()(t->begin);
    }

    std::function<tt_filter::filter_fnct> first;
    std::function<tt_filter::filter_fnct> second;
  };

  // --------------------------------------------------------------------------
  struct tt_project : std::enable_shared_from_this<tt_project> {

    typedef std::weak_ptr<tt_category> category_t;

    static const std::string PRICE_PER_HOUR;
    static const std::string PRICE_PER_HOUR_COUNT;

    std::string name;
    double pph;
    std::vector<std::shared_ptr<tt_event>> events;
    std::vector<tt_price_per_hour> prices_per_hour;
    category_t category;

    tt_project (category_t category, const std::string& name, double price_per_hour = 0.0)
      : name(name)
      , pph(price_per_hour)
      , category(category)
    {}

    tt_project (category_t category, const std::string& name, const ptree& pt)
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

    ~tt_project () {
      clog::trace() << "Delete project '" << name << "'";
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

    duration_type duration (const time_filter& f) const {
      duration_type d = duration_type::zero();
      for (auto& i : events) {
        if (f(i)) {
          d += i->duration();
        }
      }
      return d;
    }

    double costs (const time_filter& f) const {
      double d = 0;
      for (auto& i : events) {
        if (f(i)) {
          d += i->costs();
        }
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
            clog::fatal() << ex;
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

    ~tt_category () {
      clog::trace() << "Delete category '" << name << "'";
    }

    duration_type duration (const time_filter& f) const {
      duration_type d = duration_type::zero();
      for (auto& i : projects) {
        d += i->duration(f);
      }
      return d;
    }

    double costs (const time_filter& f) const {
      double d = 0;
      for (auto& i : projects) {
        d += i->costs(f);
      }
      return d;
    }

    double budget (const time_filter& f) const {
      double d = 0;
      for (auto& i : budgets) {
        if (f(i.begin)) {
          d += i.budget;
        }
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
            clog::fatal() << ex;
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
  inline double tt_event::costs () const {
    return project.lock()->price_per_second(begin) * std::chrono::duration_cast<std::chrono::seconds>(duration()).count();
  }

  inline double tt_event::price_per_hour () const {
    return project.lock()->price_per_hour(begin);
  }

} // namespace data
