/**
* @copyright (c) 2015-2017 Ing. Buero Rothfuss
*                          Riedlinger Str. 8
*                          70327 Stuttgart
*                          Germany
*                          http://www.rothfuss-web.de
*
* @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
*
* Project    standard lib
*
* Customer   -
*
* @brief     C++ API: basic window
*
* @file
*/

#pragma once

// --------------------------------------------------------------------------
//
// Common includes
//
#include <cstddef>
#include <memory>


// --------------------------------------------------------------------------
//
// Library includes
//
#include "event_container.h"
#include "gui_types.h"
#include "window_class.h"
#include "window_event_handler.h"
#include "layout.h"


namespace gui {

  // --------------------------------------------------------------------------
  typedef std::string (text_source_fn)();
  typedef std::function<text_source_fn> text_source;

  // --------------------------------------------------------------------------
  struct const_text {
    const_text ()
    {}

    const_text (const std::string& text)
      :text(text)
    {}

    std::string operator() () const {
      return text;
    }

  private:
    std::string text;
  };

  // --------------------------------------------------------------------------
  namespace win {

    class window;

    namespace detail {
      void set_id (window* w,
                   os::window id);

#ifdef X11
      extern Atom WM_CREATE_WINDOW;
#endif // X11
    }

    // --------------------------------------------------------------------------
    class container;

    // --------------------------------------------------------------------------
    class window {
    public:
      typedef core::event_container::event_handler_function event_handler_function;

      window ();

      ~window ();

      inline os::window get_id () const {
        return id;
      }

      inline operator os::drawable () const {
        return id;
      }

      bool is_valid () const;

      bool is_visible () const;

      bool is_enabled () const;

      bool has_focus () const;

      bool is_child () const;

      bool is_popup () const;

      bool is_toplevel () const;

      bool has_border () const;

      void destroy ();

      void quit ();

      void set_parent (const container& parent);

      container* get_parent () const;

      container* get_root () const;

      bool is_child_of (const container& parent) const;

      void set_visible (bool s = true);

      void enable (bool on = true);

      inline void disable () {
        enable(false);
      }

      void take_focus ();

      void shift_focus (bool backward = false);

      void to_front ();

      void to_back ();

      void enable_redraw (bool on = true);

      void redraw_now ();

      void redraw_later ();

      core::size size () const;

      core::point position () const;

      core::rectangle absolute_place () const;

      core::rectangle place () const;

      core::point absolute_position () const;

      core::size client_size () const;

      core::rectangle client_area () const;

      void move (const core::point&,
                 bool repaint = true);

      void resize (const core::size&,
                   bool repaint = true);

      void place (const core::rectangle&,
                  bool repaint = true);

      core::point window_to_screen (const core::point&) const;
      core::point screen_to_window (const core::point&) const;

      core::point client_to_screen (const core::point&) const;
      core::point screen_to_client (const core::point&) const;

      const window_class& get_window_class () const;

      void capture_pointer ();
      void uncapture_pointer ();

      void register_event_handler (char const name[], const event_handler_function& f, os::event_id mask);
      void register_event_handler (char const name[], event_handler_function&& f, os::event_id mask);

      void unregister_event_handler (const event_handler_function& f);

      template<typename H>
      void register_event_handler (char const name[], const H& h) {
        register_event_handler(name, h, h.mask);
      }

      template<typename H>
      void register_event_handler (char const name[], H&& h) {
        register_event_handler(name, std::move(h), h.mask);
      }

      template<typename T>
      void register_event_handler (char const name[], T* t, bool(T::*method)(const core::event&, os::event_result& result), os::event_id mask) {
        register_event_handler(name, core::bind_method(t, method), mask);
      }

      bool handle_event (const core::event&, os::event_result&);

      void prepare_for_event (os::event_id mask);

      bool accept_focus () const;

      inline void set_accept_focus (bool a) {
        focus_accepting = a;
      }

      inline bool is_focus_accepting () const {
        return focus_accepting;
      }

    protected:
      window (const window&);
      window (window&&);

      window& operator=(const window&) = delete;
      window& operator=(window&&) = delete;

      void create (const window_class&,
                   const container&,
                   const core::rectangle& = core::rectangle::def);

#ifdef WIN32
      void set_style (os::style mask, bool);
      os::style get_style (os::style mask =
                                     std::numeric_limits<os::style>::max()) const;
#endif // WIN32

      void create (const window_class&,
                   os::window parent,
                   const core::rectangle&);

    private:
      friend void detail::set_id (window*, os::window);

      void init ();

      os::window id;
      const window_class* cls;
      core::event_container events;

      bool focus_accepting;

#ifdef X11
      bool redraw_disabled;
      bool window_disabled;
#endif // X11
    };

    // --------------------------------------------------------------------------
    class window_with_text : public window {
    public:
      typedef window super;

      window_with_text (const std::string& = std::string());
      window_with_text (const text_source&);

      window_with_text (const window_with_text& rhs);
      window_with_text (window_with_text&& rhs);

      void set_text (const text_source&);
      void set_text (const std::string&);

      std::string get_text () const;

    protected:
      text_source text;
    };

    // --------------------------------------------------------------------------
    class client_window : public window {
    public:
      typedef window super;

      client_window ()
      {}

      client_window (const client_window& rhs)
        : super(rhs)
      {}

      client_window (client_window&& rhs)
        : super(rhs)
      {}

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def) {
        window::create(clazz, parent, r);
      }

      static const window_class clazz;
    };

    // --------------------------------------------------------------------------
    class container : public window {
    public:
      typedef window super;

      container ();

      bool is_parent_of (const window& parent) const;

      bool is_sub_window (const window* child) const;

      void set_children_visible (bool = true);

      std::vector<window*> get_children () const;

      void shift_focus (window&, bool backward = false);
      void forward_focus (bool backward = false);

    protected:
      container (const container&);
      container (container&&);

    private:
      void init ();
    };

    namespace detail {

      // --------------------------------------------------------------------------
      template<typename B, typename L = layout::standard_layout, typename... Args>
      class layout_base : public B {
      public:
        typedef B super;
        typedef L layout_type;

        layout_base (const Args&... args)
          : layouter(this, args...)
        {}

        layout_base (const layout_base& rhs)
          : super(rhs)
          , layouter(this, rhs.layouter)
        {}

        layout_base (layout_base&& rhs)
          : super(std::move(rhs))
          , layouter(this, std::move(rhs.layouter))
        {}

        void layout () {
          layouter.layout(super::size());
        }

        inline layout_type& get_layout() {
          return layouter;
        }

        inline const layout_type& get_layout() const {
          return layouter;
        }

      private:
        layout_type layouter;
      };

    } // detail

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_container = detail::layout_base<container, L, Args...>;

    window_class create_group_window_clazz (os::color);

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, os::color background = color::white, typename... Args>
    class group_window : public layout_container<L, Args...> {
    public:
      typedef layout_container<L, Args...> super;

      group_window (const Args&... args)
        : super(args...)
      {}

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    private:
      static window_class clazz;
    };

    template<typename L, os::color background, typename... Args>
    window_class group_window<L, background, Args...>::clazz(create_group_window_clazz(background));

    // --------------------------------------------------------------------------
    class overlapped_window : public container {
    public:
      typedef container super;

      void set_title (const std::string&);

      std::string get_title () const;

      bool is_top_most () const;

      bool is_minimized () const;

      bool is_maximized () const;

      void minimize ();

      void maximize ();

      void restore ();

      void set_top_most (bool toplevel);

      void create (const window_class&,
                   const window&,
                   const core::rectangle& = core::rectangle::def);

      void create (const window_class&,
                   const core::rectangle& = core::rectangle::def);
    };

    // --------------------------------------------------------------------------
    class modal_window : public overlapped_window {
    public:
      typedef overlapped_window super;

      modal_window ();
      modal_window (const modal_window&);
      modal_window (modal_window&&);

      void end_modal ();
      void run_modal ();

    private:
      void init ();

      volatile bool is_modal;
    };

    namespace detail {

      // --------------------------------------------------------------------------
      class main_window_class : public window_class {
      public:
        main_window_class ();

        void prepare (window*, os::window) const override;
      };

      // --------------------------------------------------------------------------
      class popup_window_class : public window_class {
      public:
        popup_window_class ();

        void prepare (window*, os::window) const override;
      };

      // --------------------------------------------------------------------------
      class dialog_window_class : public window_class {
      public:
        dialog_window_class ();

        void prepare (window*, os::window) const override;
      };

    }

    // --------------------------------------------------------------------------
    class main_window : public overlapped_window {
      public:
        typedef overlapped_window super;

      void create (const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, r);
      }

    protected:
      static detail::main_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_main_window = detail::layout_base<main_window, L, Args...>;

    // --------------------------------------------------------------------------
    class popup_window : public modal_window {
    public:
      typedef modal_window super;

      void create (const window& parent, const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    protected:
      static detail::popup_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_popup_window = detail::layout_base<popup_window, L, Args...>;

    // --------------------------------------------------------------------------
    class dialog_window : public modal_window {
    public:
      typedef modal_window super;

      void create (const window& parent, const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    protected:
      static detail::dialog_window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, typename... Args>
    using layout_dialog_window = detail::layout_base<dialog_window, L, Args...>;

    // --------------------------------------------------------------------------
  } // win

} // gui
