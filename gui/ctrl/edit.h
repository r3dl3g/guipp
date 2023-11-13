/**
 * @copyright (c) 2016-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     edit control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/draw/font.h>
#include <gui/ctrl/control.h>
#include <gui/ctrl/look/edit.h>
#include <util/string_util.h>


namespace gui {

  namespace ctrl {

    // --------------------------------------------------------------------------
    namespace filter {

      typedef std::string (text_filter_f) (std::string);

      std::string GUIPP_CTRL_EXPORT alpha_filter (std::string);
      std::string GUIPP_CTRL_EXPORT alpha_numeric_filter (std::string);
      std::string GUIPP_CTRL_EXPORT digits_filter (std::string);
      std::string GUIPP_CTRL_EXPORT xdigits_filter (std::string);
      std::string GUIPP_CTRL_EXPORT lower_filter (std::string);
      std::string GUIPP_CTRL_EXPORT upper_filter (std::string);
      std::string GUIPP_CTRL_EXPORT float_filter (std::string);
      std::string GUIPP_CTRL_EXPORT integer_filter (std::string);
      std::string GUIPP_CTRL_EXPORT unsigned_filter (std::string);

    }

    namespace detail {

      // --------------------------------------------------------------------------
      class GUIPP_CTRL_EXPORT edit_base : public colored_control {
      public:
        typedef colored_control super;
        typedef size_t pos_t;

        typedef win::window_class<edit_base, color::white, win::cursor_type::ibeam> clazz;

        typedef std::function<filter::text_filter_f> input_filter;
        typedef core::range<pos_t> range;

        edit_base ();
        ~edit_base ();

        void create (win::container& parent,
                     const std::string& txt,
                     const core::rectangle& place = core::rectangle::def);

        void create (win::container& parent,
                     const core::rectangle& place = core::rectangle::def);

        void init ();

        void init_text (const std::string&);
        virtual std::string get_text () const = 0;

        void set_text_limit (pos_t max_chars);
        pos_t get_text_limit () const;
        pos_t get_text_length () const;

        void handle_key (os::key_state, os::key_symbol, const std::string&);

        void set_selection(const range &sel, event_source);
        range get_selection () const;
        void make_selection_visible (bool update = true);

        pos_t get_cursor_pos () const;
        void set_cursor_pos (pos_t pos, bool shift = false);
        pos_t get_scroll_pos () const;

        void replace_selection (const std::string& new_text);
        std::string get_text_in_range (const range&) const;
        virtual std::string get_selected_text () const;

        void set_text_filter (input_filter);
        std::string filter_text (const std::string&) const;

        bool is_insert_mode () const;
        void set_insert_mode (bool);

      protected:
        void register_handler ();

      private:
        virtual void set_text (const std::string&) = 0;

        void prepare_input ();
        pos_t get_position_at_point (const core::point& pt) const;

        struct GUIPP_CTRL_EXPORT data {
          data ();

          range selection;
          pos_t cursor_pos;
          pos_t text_limit;
          pos_t scroll_pos;
          core::native_point last_mouse_point;
          input_filter filter;
          bool insert_mode;
        } data;

      };

    } // detail

    // --------------------------------------------------------------------------
    template<typename U>
    struct default_converter {
      using T = typename std::remove_reference<U>::type;

      static T parse (const std::string& t) {
        return util::string::convert::to<T>(t);
      }

      static std::string format (const T& t) {
        return util::string::convert::from<T>(t);
      }
    };

    // --------------------------------------------------------------------------
    template<typename U>
    struct default_limiter {
      using T = typename std::remove_reference<U>::type;

      inline T operator() (const T& t) {
        return t;
      }
    };

    // --------------------------------------------------------------------------
    template<typename T>
    bool default_insert_mode () {
      return false;
    }

    // --------------------------------------------------------------------------
    template<typename T,
             typename F = default_converter<T>,
             typename L = default_limiter<T>>
    class edit_t : public detail::edit_base {
    public:
      typedef detail::edit_base super;
      typedef F converter_type;
      typedef L limiter_type;

      using type = typename std::remove_reference<T>::type;

      edit_t (T = {}, limiter_type l = {});

      std::string get_text () const override;
      void set_text (const std::string&) override;

      void set (const type& t);
      type get () const;

      void set_limiter (limiter_type l);
      limiter_type get_limiter () const;

    private:
      T value;
      limiter_type limiter;
    };

    // --------------------------------------------------------------------------
    template<typename T = std::string,
             typename F = default_converter<T>,
             typename L = default_limiter<T>,
             text_origin_t origin = text_origin_t::vcenter_left,
             draw::frame::drawer frame = draw::frame::sunken_relief>
    class basic_edit : public edit_t<T, F, L> {
    public:
      typedef edit_t<T, F, L> super;
      typedef typename super::type type;
      typedef typename super::converter_type converter_type;
      typedef typename super::limiter_type limiter_type;

      basic_edit (T = {}, limiter_type l = {});
      basic_edit (const basic_edit& rhs);
      basic_edit (basic_edit&& rhs) noexcept;


      void paint (draw::graphics& graph);

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using edit_left = basic_edit<>;
    typedef edit_left edit;
    using edit_right = basic_edit<std::string,
                                  default_converter<std::string>,
                                  default_limiter<std::string>,
                                  text_origin_t::vcenter_right>;
    using edit_center = basic_edit<std::string,
                                   default_converter<std::string>,
                                   default_limiter<std::string>,
                                   text_origin_t::center>;

    // --------------------------------------------------------------------------
    template<text_origin_t origin,
             char C = '#',
             draw::frame::drawer frame = draw::frame::sunken_relief>
    class basic_pass : public edit_t<std::string> {
    public:
      typedef edit_t<std::string> super;

      basic_pass (const std::string& t = {});
      basic_pass (const basic_pass& rhs);
      basic_pass (basic_pass&& rhs) noexcept ;


      void paint (draw::graphics& graph);

      virtual std::string get_selected_text () const;

    private:
      void init ();
    };

    // --------------------------------------------------------------------------
    using pass_left = basic_pass<text_origin_t::vcenter_left>;
    typedef pass_left pass;
    using pass_right = basic_pass<text_origin_t::vcenter_right>;
    using pass_center = basic_pass<text_origin_t::center>;

    // --------------------------------------------------------------------------

  } // ctrl

} // gui

#include <gui/ctrl/edit.inl>
