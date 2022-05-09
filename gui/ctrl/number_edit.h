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
      typedef typename edit_t::type type;
      typedef B buttons_t;

      number_edit (T&& v = type(), type i = type(1));

      void init ();

      void set (const type& v);
      type get () const;

      void step (const type& i);

      void inc ();
      void dec ();

      type increment;
      edit_t edit;
      buttons_t buttons;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT date_time_edit_base : public group_control<layout::horizontal_adaption<>> {
    public:
      typedef group_control<layout::horizontal_adaption<>> super;
      typedef number_edit<int&, detail::offset_converter<1900>> year_edit_t;
      typedef number_edit<int&, detail::offset_converter<1>> month_edit_t;
      typedef number_edit<int&> int_edit_t;
      typedef util::time::time_point time_point;

      date_time_edit_base (const time_point& v);

    protected:
      std::tm parts;

      year_edit_t year;
      month_edit_t month;
      int_edit_t day;
      int_edit_t hour;
      int_edit_t min;
      int_edit_t sec;
    };

    // --------------------------------------------------------------------------
    template<typename T = util::time::time_point>
    class date_time_edit : public date_time_edit_base {
    public:
      typedef date_time_edit_base super;

      date_time_edit (T t);

      void update ();

      void set (const time_point& v);
      time_point get () const;

    protected:
      T value;
    };

    // --------------------------------------------------------------------------
    class GUIPP_CTRL_EXPORT duration_edit_base : public group_control<layout::horizontal_adaption<>> {
    public:
      typedef group_control<layout::horizontal_adaption<>> super;
      typedef number_edit<int&> int_edit_t;
      typedef util::time::duration duration;

      duration_edit_base (const duration&);

    protected:
      util::time::duration_parts parts;

      int_edit_t hour;
      int_edit_t min;
      int_edit_t sec;
    };

    // --------------------------------------------------------------------------
    template<typename T = util::time::duration>
    class duration_edit : public duration_edit_base {
    public:
      typedef duration_edit_base super;

      duration_edit (T t);

      void update ();

      void set (const duration& v);
      duration get () const;

    protected:
      T value;
    };

    // --------------------------------------------------------------------------
    template<typename T = util::time::duration>
    class duration_edit_ms : public duration_edit<T> {
    public:
      typedef duration_edit<T> super;

      duration_edit_ms (T t);

    protected:
      typename super::int_edit_t micros;
    };

    // --------------------------------------------------------------------------
  } // ctrl

} // gui

#include <gui/ctrl/number_edit.inl>
