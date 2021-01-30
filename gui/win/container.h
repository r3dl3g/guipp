/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
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
#include <set>


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/window.h>
#ifdef GUIPP_WIN
# include <gui/win/container_class_win32.h>
#elif GUIPP_X11
# include <gui/win/container_class_x11.h>
#elif GUIPP_QT
# include <gui/win/container_class_qt.h>
# include <QtCore/QEventLoop>
#endif // GUIPP_QT


namespace gui {

  // --------------------------------------------------------------------------
  namespace win {

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT container : public window {
    public:
      typedef window super;
      typedef std::vector<window*> window_list_t;

      container ();

      bool is_parent_of (const window& child) const;
      bool is_sub_window (const window* child) const;

      void set_children_visible (bool = true);

      window_list_t get_children () const;
      void collect_children (window_list_t&) const;

      void add_child (window*);
      void remove_child (window*);

      void to_front (window*);
      void to_back (window*);

    protected:
      using window::create;

      container (const container&);
      container (container&&) noexcept ;

    private:
      void init ();

      std::set<window*> children;
    };

    // --------------------------------------------------------------------------
    namespace detail {

      void set_os_window (overlapped_window* w, os::window id);
      os::window get_os_window (const overlapped_window&);

    }

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT overlapped_window : public container {
    public:
      typedef container super;

      overlapped_window ();
      overlapped_window (const overlapped_window&);
      overlapped_window (overlapped_window&&);
      ~overlapped_window ();

      void close ();
      bool is_valid () const;

      operator os::drawable() const;

      void set_visible (bool s = true); /// set window visible
      void enable (bool on = true);     /// enable of disable this window
      void disable ();                  /// disable this window

      void set_title (const std::string&);
      std::string get_title () const;

      bool is_top_most () const;
      bool is_minimized () const;
      bool is_maximized () const;

      void minimize ();
      void maximize ();
      void restore ();

      void set_top_most (bool toplevel);

      void to_front ();
      void to_back ();

      void invalidate (const core::rectangle&) const;
      void invalidate () const;
      void redraw (const core::rectangle&) const;

      void shift_focus (bool backward = false);

      void set_focus_window (window* w);
      window* get_focus_window () const;

      void set_cursor (const os::cursor&);

      void capture_pointer (window* w);
      void uncapture_pointer (window* w);

      bool handle_event (const core::event&, gui::os::event_result&) const override;

    protected:
      void create (const class_info&,
                   overlapped_window&,
                   const core::rectangle& = core::rectangle::def);

      void create (const class_info&,
                   const core::rectangle& = core::rectangle::def);

      void create_internal (const class_info&,
                            os::window parent,
                            const core::rectangle&);

      void destroy ();

      void set_accept_focus (bool a);

      void move_native (const core::point&) override;
      void resize_native (const core::size&) override;
      void place_native (const core::rectangle&) override;

    private:
      friend void detail::set_os_window (overlapped_window*, os::window);
      friend os::window detail::get_os_window (const overlapped_window&);

      os::window get_os_window () const;
      void set_os_window (os::window);

    private:
      void init ();
      window* focus_window;
      window* capture_window;

      os::window id;
    };

    // --------------------------------------------------------------------------
    struct hot_key_action {
      core::hot_key hk;
      core::hot_key::call fn;
    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT modal_window : public overlapped_window {
    public:
      typedef overlapped_window super;

      modal_window ();
      modal_window (const modal_window&);
      modal_window (modal_window&&) noexcept ;

      void end_modal ();
      void run_modal (overlapped_window&);
      void run_modal (overlapped_window&, const std::vector<hot_key_action>& hot_keys);

    private:
      void init ();

      volatile bool is_modal;
#ifdef GUIPP_QT
      QEventLoop event_loop;
#endif // GUIPP_QT
    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT main_window : public overlapped_window {
    public:
      typedef overlapped_window super;
      using clazz = cls::main_window_class<main_window>;

      void create (const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT popup_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::popup_window_class<popup_window>;

      void create (overlapped_window& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, overlapped_window& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT dialog_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::dialog_window_class<dialog_window>;

      void create (overlapped_window& parent, const core::rectangle& r = core::rectangle::def);

    protected:
      void create (const class_info& cls, overlapped_window& parent, const core::rectangle& r = core::rectangle::def);

    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#include <gui/win/container.inl>
