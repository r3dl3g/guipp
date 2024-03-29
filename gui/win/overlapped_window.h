/**
 * @copyright (c) 2015-2021 Ing. Buero Rothfuss
 *                          Riedlinger Str. 8
 *                          70327 Stuttgart
 *                          Germany
 *                          http://www.rothfuss-web.de
 *
 * @author    <a href="mailto:armin@rothfuss-web.de">Armin Rothfuss</a>
 *
 * Project    gui++ lib
 *
 * @brief     window that can overlap windows of other applications
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


// --------------------------------------------------------------------------
//
// Common includes
//


// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/win/container.h>
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
    namespace detail {
      void GUIPP_WIN_EXPORT set_os_window (overlapped_window* w, os::window id);
    }

    class overlapped_context;

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT overlapped_window : public container {
    public:
      typedef container super;

      overlapped_window ();
      overlapped_window (const overlapped_window&);
      overlapped_window (overlapped_window&&);
      ~overlapped_window ();

      void destroy();

      bool is_valid () const;

      operator os::drawable() const;

      void set_visible (bool s = true); /// set window visible
      void enable (bool on = true);     /// enable of disable this window
      void disable ();                  /// disable this window

      void set_title (const std::string&);
      std::string get_title () const;

      bool is_minimized () const;
      bool is_maximized () const;
      bool is_top_most () const;
      bool is_fullscreen () const;

      void minimize ();
      void maximize ();
      void restore ();

      void set_top_most (bool toplevel);
      void set_fullscreen (bool fullscreen);

      void to_front ();
      void to_back ();

      void invalidate (const core::native_rect&) override;
      void invalidate ();
      void redraw (const core::native_rect&);

      core::point position () const override;
      core::size client_size () const override;
      core::native_point surface_position () const override;

      core::point surface_to_screen (const core::native_point&) const;
      core::native_point screen_to_surface (const core::point&) const;

      core::point get_current_pointer_pos () const;

      void shift_focus (bool backward = false);

      void set_focus_window (window* w);
      window* get_current_focus_window () const override;

      void set_cursor (const cursor&) override;

      void capture_pointer (window* w);
      void uncapture_pointer (window* w);

      bool handle_event (const core::event&, gui::os::event_result&) override;
      void add_event_mask (os::event_id mask) override;

      void remove (window*) override;

      os::window get_os_window () const;

    protected:
      void create (const class_info&,
                   overlapped_window&,
                   const core::rectangle& = core::rectangle::def,
                   bool adjust_size = true);

      void create (const class_info&,
                   const core::rectangle& = core::rectangle::def,
                   bool adjust_size = true);

      void create_internal (const class_info&,
                            os::window parent,
                            const core::rectangle&);

      void set_accept_focus (bool a);
      void set_mouse_window (window* w);

      void move_native (const core::point&) override;
      void resize_native (const core::size&) override;
      void geometry_native (const core::rectangle&) override;

      overlapped_context& get_context () const;

    private:
      friend void detail::set_os_window (overlapped_window*, os::window);
      void set_os_window (os::window);

    private:
      void init ();
      window* focus_window;
      window* capture_window;
      window* mouse_window;
      core::native_rect invalid_rect;
      std::vector<window*> capture_stack;

      os::window id;
      mutable std::unique_ptr<overlapped_context> surface;
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

      void create (const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    protected:
      void create (const class_info& cls,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT popup_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::popup_window_class<popup_window>;

      void create (overlapped_window& parent,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    protected:
      void create (const class_info& cls,
                   overlapped_window& parent,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT tooltip_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::tooltip_window_class<tooltip_window>;

      void create (overlapped_window& parent,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    protected:
      void create (const class_info& cls,
                   overlapped_window& parent,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    };

    // --------------------------------------------------------------------------
    class GUIPP_WIN_EXPORT dialog_window : public modal_window {
    public:
      typedef modal_window super;
      using clazz = cls::dialog_window_class<dialog_window>;

      void create (overlapped_window& parent,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    protected:
      void create (const class_info& cls,
                   overlapped_window& parent,
                   const core::rectangle& r = core::rectangle::def,
                   bool adjust_size = true);

    };

    // --------------------------------------------------------------------------
  } // namespace win

} // namespace gui

#include <gui/win/overlapped_window.inl>
