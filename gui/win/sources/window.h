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

      const window_class* get_window_class () const;

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

    protected:
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

    };

    // --------------------------------------------------------------------------
    class window_with_text : public window {
    public:
      void set_text (const std::string&);

      std::string get_text () const;

    protected:
      std::string text;
    };

    // --------------------------------------------------------------------------
    class client_window : public window {
    public:
      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def) {
        window::create(clazz, parent, r);
      }

      static const window_class clazz;
    };

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout>
    class layout_container : public container {
    public:
      typedef container super;
      typedef L layout_type;

      layout_container ()
        : layouter(this)
      {}

      void layout () {
        layouter.layout(size());
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

    window_class create_group_window_clazz (os::color);

    // --------------------------------------------------------------------------
    template<typename L = layout::standard_layout, os::color background = color::white>
    class group_window : public layout_container<L> {
    public:
      typedef layout_container<L> super;

      void create (const container& parent,
                   const core::rectangle& r = core::rectangle::def) {
        super::create(clazz, parent, r);
      }

    private:
      static window_class clazz;
    };

    template<typename L, os::color background>
    window_class group_window<L, background>::clazz(create_group_window_clazz(background));

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

      void end_modal ();
      void run_modal ();

    private:
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
    template<typename L = layout::standard_layout>
    class layout_main_window : public main_window {
    public:
      typedef main_window super;
      typedef L layout_type;

      layout_main_window ()
        : layouter(this)
      {}

      void layout () {
        layouter.layout(size());
      }

      inline layout_type& get_layout () {
        return layouter;
      }

      inline const layout_type& get_layout () const {
        return layouter;
      }

    protected:
      layout_type layouter;
    };

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
    template<typename L = layout::standard_layout>
    class layout_dialog_window : public dialog_window {
    public:
      typedef dialog_window super;
      typedef L layout_type;

      layout_dialog_window ()
        : layouter(this)
      {
        //register_event_handler(REGISTER_FUNCTION, win::size_event(core::bind_method(&layouter, &layout_type::layout)));
      }

      void layout () {
        layouter.layout(size());
      }

      inline layout_type& get_layout () {
        return layouter;
      }

      inline const layout_type& get_layout () const {
        return layouter;
      }

    protected:
      layout_type layouter;
    };

    // --------------------------------------------------------------------------
  } // win

} // gui
