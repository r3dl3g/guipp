/**
 * @copyright (c) 2016-2022 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     pasive separator view control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/ctrl/edit.h>
#include <gui/layout/split_layout.h>
#include <gui/ctrl/button.h>
#include <util/time_util.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace detail {

      template<int O>
      struct offset_converter {
        static int parse (const std::string& t) {
          return util::string::convert::to<int>(t) - O;
        }

        static std::string format (const int& t) {
          return util::string::convert::from<int>(t + O);
        }
      };

      struct duration_converter {
        static inline util::time::duration parse (const std::string& t) {
          return util::time::parse_duration(t);
        }

        static inline std::string format (const util::time::duration& t) {
          return util::time::format_duration(t);
        }
      };

      struct datetime_converter {
        static inline util::time::time_point parse (const std::string& t) {
          return util::time::parse_datetime(t);
        }

        static inline std::string format (const util::time::time_point& t) {
          return util::time::format_datetime(t);
        }
      };

    } // namespace detail

    // --------------------------------------------------------------------------
    template<typename T,
             typename F = default_converter<T>,
             typename B = button_pair<orientation_t::vertical>,
             typename L = layout::split_layout<alignment_t::right, 20>>
    class number_edit : public group_control<L> {
    public:
      typedef group_control<L> super;
      typedef L layout_t;
      typedef basic_edit<T, F> edit_t;
      typedef typename edit_t::data data;
      typedef B buttons_t;

      number_edit (const T& v = T(), T i = T(1));
      number_edit (std::reference_wrapper<T> d, T i = T(1));
      number_edit (data, T i = T(1));

      void init ();

      void set (T v);
      T get () const;

      void step (T i);

      void inc ();
      void dec ();

      T increment;
      edit_t edit;
      buttons_t buttons;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT date_time_edit : public group_control<layout::horizontal_adaption<>> {
    public:
      typedef group_control<layout::horizontal_adaption<>> super;
      typedef number_edit<int, detail::offset_converter<1900>> year_edit_t;
      typedef number_edit<int, detail::offset_converter<1>> month_edit_t;
      typedef number_edit<int> int_edit_t;
      typedef util::time::time_point time_point;
      typedef std::function<time_point&()> data;

      date_time_edit (const time_point& d = std::chrono::system_clock::now());
      date_time_edit (std::reference_wrapper<time_point> d);
      date_time_edit (data);

      void init ();
      void update ();

      void set (time_point v);
      time_point get () const;

    private:
      data value;
      std::tm date;

      year_edit_t year;
      month_edit_t month;
      int_edit_t day;
      int_edit_t hour;
      int_edit_t min;
      int_edit_t sec;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT duration_edit : public group_control<layout::horizontal_adaption<>> {
    public:
      typedef group_control<layout::horizontal_adaption<>> super;
      typedef number_edit<int> int_edit_t;
      typedef util::time::duration duration;
      typedef std::function<duration&()> data;

      duration_edit (const duration& d = std::chrono::seconds(0));
      duration_edit (std::reference_wrapper<duration> d);
      duration_edit (data);

      void update ();

      void set (duration v);
      duration get () const;

    protected:
      data value;
      util::time::duration_parts parts;

      int_edit_t hour;
      int_edit_t min;
      int_edit_t sec;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT duration_edit_ms : public duration_edit {
    public:
      typedef duration_edit super;

      duration_edit_ms (const duration& d = std::chrono::seconds(0));
      duration_edit_ms (std::reference_wrapper<duration> d);
      duration_edit_ms (data);

    protected:
      int_edit_t micros;
    };

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/number_edit.inl>
