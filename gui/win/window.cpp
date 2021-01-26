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


// --------------------------------------------------------------------------
//
// Common includes
//
#include <algorithm>
#include <map>
#include <set>

#ifdef GUIPP_X11
# include <X11/cursorfont.h>
#endif // GUIPP_X11
#ifdef GUIPP_QT
#include <QtWidgets/QApplication>
#include <QtGui/QScreen>
#endif // GUIPP_QT

// --------------------------------------------------------------------------
//
// Library includes
//
#include <util/ostreamfmt.h>
#include <logging/logger.h>
#include <gui/win/container.h>
#include <gui/win/window_event_proc.h>
#include <gui/win/window_event_handler.h>

#define NO_CAPTURExx


namespace gui {

  namespace win {

#ifdef GUIPP_X11

    namespace x11 {

# define XLIB_ERROR_CODE(a) case a: clog::fatal() << # a;break;

      bool check_return (int r) {
# ifndef NDEBUG
//        core::global::sync();
# endif
        switch (r) {
          case Success:
          case True:
          return true;
          XLIB_ERROR_CODE(BadValue)
          XLIB_ERROR_CODE(BadWindow)
          XLIB_ERROR_CODE(BadPixmap)
          XLIB_ERROR_CODE(BadAtom)
          XLIB_ERROR_CODE(BadCursor)
          XLIB_ERROR_CODE(BadFont)
          XLIB_ERROR_CODE(BadMatch)
          XLIB_ERROR_CODE(BadDrawable)
          XLIB_ERROR_CODE(BadAccess)
          XLIB_ERROR_CODE(BadAlloc)
          XLIB_ERROR_CODE(BadColor)
          XLIB_ERROR_CODE(BadGC)
          XLIB_ERROR_CODE(BadIDChoice)
          XLIB_ERROR_CODE(BadName)
          XLIB_ERROR_CODE(BadLength)
          XLIB_ERROR_CODE(BadImplementation)
        }
        return false;
      }

      bool check_status (Status s) {
        if (s) {
          return true;
        }
//      clog::fatal() << "xlib Status failed";
        return false;
      }

    } // namespace x11

#endif // GUIPP_X11

    struct log_hierarchy {
      explicit log_hierarchy (window* win)
        : win(win)
      {}

      window* win;
    };

    inline std::ostream& operator<< (std::ostream& out, const log_hierarchy& lh) {
      const window* w = lh.win;
      while (w) {
        out << " -> " << detail::get_os_window(*w);
        w = w->get_parent();
      }
      return out;
    }

    // --------------------------------------------------------------------------
    namespace hidden {
      std::map<std::string, class_info> window_class_info_map;
      std::vector<os::window> capture_stack;
    }

    // --------------------------------------------------------------------------
    window::window ()
      : id(0)
      , parent(nullptr)
    {
      init();
    }

    os::window get_desktop_window () {
#if GUIPP_X11
      return DefaultRootWindow(core::global::get_instance());
#else
      return NULL;
#endif

    }

    window::window (const window& rhs)
      : id(0)
      , flags(rhs.flags)
      , parent(nullptr)
    {
      init();
      if (rhs.is_valid()) {
        container* p = rhs.get_parent();
        create_internal(rhs.get_window_class(),
                        p ? detail::get_os_window(*p) : get_desktop_window(),
                        rhs.place());
        set_parent(*p);
      }
    }

    window::window (window&& rhs) noexcept
      : id(0)
      , flags(std::move(rhs.flags))
      , parent(nullptr)
    {
      init();
      std::swap(id, rhs.id);
      if (rhs.parent) {
        rhs.parent->remove_child(&rhs);
        std::swap(parent, rhs.parent);
        rhs.parent->add_child(this);
      }
    }

    void window::create (const class_info& type,
                         container& p,
                         const core::rectangle& r) {
      if (p.is_valid()) {
        create_internal(type, detail::get_os_window(p), r);
        set_parent(p);
      }
    }

    void window::create_internal (const class_info& type,
                                  os::window parent_id,
                                  const core::rectangle& r) {

      if (get_os_window()) {
        destroy();
      }

      id = create_window(type, r, parent_id, this);
#if defined(GUIPP_X11)
      send_client_message(this, core::x11::WM_CREATE_WINDOW);
#endif // GUIPP_X11
#if defined(GUIPP_QT)
      send_client_message(this, core::qt::WM_CREATE_WINDOW);
#endif // GUIPP_X11
    }

    window::operator os::drawable() const {
#ifdef GUIPP_QT
      return id;
#else
      return get_os_window();
#endif
    }

    core::point window::window_to_screen (const core::point& pt) const {
      return parent ? parent->client_to_screen(pt) : pt;
    }

    core::point window::screen_to_window (const core::point& pt) const {
      return parent ? parent->screen_to_client(pt) : pt;
    }

    container* window::get_root_window () const {
      if (parent) {
        return parent->get_root_window();
      }
      return (container*)this;
    }

    overlapped_window* window::get_overlapped_window () const {
      if (get_state().overlapped()) {
        return (overlapped_window*)this;
      }
      if (parent) {
        return parent->get_overlapped_window();
      } else {
        return (overlapped_window*)this;
      }
    }

    namespace {
      std::set<std::pair<const window*, os::event_id>> active_handler;
    }

    bool window::handle_event (const core::event& e, gui::os::event_result& result) {
      const auto keypair = std::make_pair(this, IF_QT_ELSE(e.type(), e.type));
      if (active_handler.find(keypair) != active_handler.end()) {
        clog::warn() << "already in handle_event for window: " << this << " " << e;
        return false;
      }
      active_handler.insert(keypair);
      if (any_key_up_event::match(e)) {
        os::key_symbol key = get_key_symbol(e);
        if (key == core::keys::tab) {
          os::key_state state = get_key_state(e);
          shift_focus(core::shift_key_bit_mask::is_set(state));
        }
      } else if (lost_focus_event::match(e)) {
        focus_lost();
      }

//      clog::trace() << "handle_event: for window: " << this << " " << e;
      bool res = false;
      if (is_enabled() || paint_event::match(e)) {
        res = events.handle_event(e, result);
      }
      active_handler.erase(keypair);
      return res;
    }

    void window::shift_focus (bool backward) {
      if (parent) {
        parent->shift_focus(this, backward);
      }
    }

    void window::focus_lost () {
      set_state().focused(false);
      invalidate();
    }

    bool window::can_accept_focus () const {
      return is_focus_accepting() && is_enabled() && is_visible();
    }

    void window::set_accept_focus (bool a) {
      if (is_valid()) {
#ifdef GUIPP_QT
        get_os_window()->setFocusPolicy(a ? Qt::WheelFocus : Qt::NoFocus);
#endif //GUIPP_QT
      }
      set_state().accept_focus(a);
    }

    bool window::is_focus_accepting () const {
      return get_state().focus_accepting();
    }

    const class_info& window::get_window_class () const {
      return hidden::window_class_info_map[get_class_name()];
    }

    void window::notify_event_double (os::message_type message, double d1) {
      long l1 = static_cast<long>(d1 * 1000000.0);
      send_client_message(this, message, l1, 0);
    }

    void window::set_parent (container& p) {
      if (parent) {
        parent->remove_child(this);
        parent = nullptr;
      }
      if (is_valid() && p.is_valid()) {
        parent = &p;
        p.add_child(this);
      }
    }

    container* window::get_parent () const {
      return parent;
    }

    bool window::is_child_of (const container& p) const {
      return (parent == &p);
    }

    // --------------------------------------------------------------------------

#ifdef GUIPP_WIN

    struct disable_wheel_hook {
      disable_wheel_hook ()
        : hook_id(0)
      {}

      void enable ();
      void disable ();

    private:
      HHOOK hook_id;
    };

    disable_wheel_hook s_wheel_hook;

    LRESULT CALLBACK mouseInputHook (int nCode, WPARAM wParam, LPARAM lParam) {
      //"if nCode is less than zero, the hook procedure must pass the message to the CallNextHookEx function
      //without further processing and should return the value returned by CallNextHookEx"
      if ((nCode >= 0) && ((wParam == WM_MOUSEWHEEL || wParam == WM_MOUSEHWHEEL))) {
        return -1;
      }

      return ::CallNextHookEx(nullptr, nCode, wParam, lParam);
    }

    void disable_wheel_hook::enable () {
      if (!hook_id) {
        hook_id = SetWindowsHookEx(WH_MOUSE_LL, mouseInputHook, NULL, 0);
      }
    }

    void disable_wheel_hook::disable () {
      if (hook_id) {
        UnhookWindowsHookEx(hook_id);
        hook_id = 0;
      }
    }

    // --------------------------------------------------------------------------
    void window::init ()
    {}

    window::~window () {
      destroy();
    }

    void window::destroy () {
      if (get_os_window()) {
        DestroyWindow(get_os_window());
        id = 0;
      }
    }

    void window::close () {
      if (get_os_window()) {
        CloseWindow(get_os_window());
      }
    }

    void window::enable (bool on) {
      if (set_state().enable(on)) {
        if (is_valid()) {
          gui::os::style ws = get_window_class().get_style();
          if ((ws & WS_POPUP) != WS_POPUP) {
            // For WS_POPUP EnableWindow(, false) causes an empty window.
            EnableWindow(get_os_window(), on);
          }
          invalidate();
        }
      }
    }

    bool window::is_valid () const {
      return (get_os_window() != 0) && (IsWindow(get_os_window()) != FALSE);
    }

    bool window::is_visible () const {
      return is_valid() && IsWindowVisible(get_os_window());
    }

    bool window::is_focused () const {
      return is_valid() && (GetFocus() == get_os_window());
    }

    bool window::is_child () const {
      return is_valid() && (GetWindowLong(get_os_window(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::is_popup () const {
      return is_valid() && (GetWindowLong(get_os_window(), GWL_STYLE) & WS_POPUP) == WS_POPUP;
    }

    bool window::is_toplevel () const {
      return is_valid() && (GetWindowLong(get_os_window(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
    }

    bool window::has_border () const {
      return is_valid() && (GetWindowLong(get_os_window(), GWL_STYLE) & (WS_BORDER | WS_DLGFRAME | WS_THICKFRAME) ? true : false);
    }

    void window::set_visible (bool s) {
      if (is_valid()) {
        ShowWindow(get_os_window(), s ? SW_SHOWNA : SW_HIDE);
      }
    }

    void window::take_focus (bool) {
      if (is_valid()) {
        set_state().focused(true);
        SetFocus(get_os_window());
        invalidate();
      }
    }

    void window::to_front () {
      if (is_valid()) {
        SetWindowPos(get_os_window(), HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
      }
    }

    void window::to_back () {
      if (is_valid()) {
        SetWindowPos(get_os_window(), HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
      }
    }

    void window::redraw () const {
      if (is_valid()) {
        clog::trace() << "redraw: " << get_os_window();
        RedrawWindow(get_os_window(), nullptr, nullptr, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW | RDW_ERASENOW);
      }
    }

    void window::invalidate () const {
      if (is_valid()) {
        clog::trace() << "invalidate: " << get_os_window();
        InvalidateRect(get_os_window(), nullptr, TRUE);
      }
    }

    core::size window::size () const {
      RECT r;
      GetWindowRect(get_os_window(), &r);
      return core::size(r);
    }

    core::point window::position () const {
      RECT r;
      GetWindowRect(get_os_window(), &r);
      return screen_to_window(core::point(os::point {r.left, r.top}));
    }

    core::rectangle window::place () const {
      const core::rectangle pl = absolute_place();
      return core::rectangle(screen_to_window(pl.position()), pl.size());
    }

    core::rectangle window::absolute_place () const {
      RECT r;
      GetWindowRect(get_os_window(), &r);
      return core::rectangle(r);
    }

    core::point window::absolute_position () const {
      RECT r;
      GetWindowRect(get_os_window(), &r);
      return core::point(r);
    }

    core::size window::client_size () const {
      RECT r;
      GetClientRect(get_os_window(), &r);
      return core::size(r);
    }

    core::rectangle window::client_area () const {
      RECT r;
      GetClientRect(get_os_window(), &r);
      return core::rectangle(r);
    }

    void window::move (const core::point& pt, bool repaint) {
      if (is_valid()) {
        SetWindowPos(get_os_window(), nullptr, pt.os_x(), pt.os_y(), 0, 0, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
        if (repaint) {
          invalidate();
        }
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      if (is_valid()) {
        SetWindowPos(get_os_window(), nullptr, 0, 0, sz.os_width(), sz.os_height(), SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOMOVE | SWP_NOZORDER);
        send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(sz));
        if (repaint) {
          invalidate();
        }
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      if (is_valid()) {
        MoveWindow(get_os_window(), r.os_x(), r.os_y(), r.os_width(), r.os_height(), repaint);
        send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(r.size()));
      }
    }

    core::point window::client_to_screen (const core::point& pt) const {
      POINT Point = pt;
      ClientToScreen(get_os_window(), &Point);
      return core::point(Point);
    }

    core::point window::screen_to_client (const core::point& pt) const {
      POINT Point = pt;
      ScreenToClient(get_os_window(), &Point);
      return core::point(Point);
    }

    void window::set_cursor (const os::cursor& c) {
      if (is_valid()) {
        SetClassLongPtr(get_os_window(), GCLP_HCURSOR, reinterpret_cast<LONG_PTR>(c));
      }
    }

    void window::capture_pointer () {
      if (is_valid()) {
        clog::trace() << "capture_pointer:" << get_os_window();
        hidden::capture_stack.push_back(get_os_window());
        SetCapture(get_os_window());
        s_wheel_hook.enable();
      }
    }

    void window::uncapture_pointer () {
      if (!hidden::capture_stack.empty()) {
        if (hidden::capture_stack.back() != get_os_window()) {
          clog::fatal() << "uncapture_pointer:" << get_os_window() << " differs from stack back:(" << hidden::capture_stack.back() << ")";
        } else {
          clog::trace() << "uncapture_pointer:" << get_os_window();
        }
        ReleaseCapture();
        hidden::capture_stack.pop_back();
        if (!hidden::capture_stack.empty()) {
          clog::trace() << "re-capture_pointer:" << hidden::capture_stack.back();
          SetCapture(hidden::capture_stack.back());
        } else {
          s_wheel_hook.disable();
        }
      }
    }

# ifdef WIN32_DEPRECATED
    os::style window::get_style (os::style mask) const {
      LONG old_style = GetWindowLong(get_os_window(), GWL_STYLE);
      os::style new_style = old_style & mask;
      return new_style;
    }

    void window::set_style (os::style style, bool enable) {
      LONG new_style = enable ? get_style() | style : get_style(~style);
      SetWindowLong(get_os_window(), GWL_STYLE, new_style);
      redraw();
    }

# endif // WIN32_DEPRECATED

    os::window window::create_window (const class_info& type,
                                      const core::rectangle& r,
                                      os::window parent_id,
                                      window* data) {
      auto display = core::global::get_instance();

      os::brush back = ((type.get_background() > 0) && (type.get_background() < 20))
                       ? reinterpret_cast<os::brush>(static_cast<LPARAM>(type.get_background()))
                       : (color::extract<color::part::alpha>(type.get_background()) == 0xff)
                       ? NULL : CreateSolidBrush(type.get_background());

      std::string name = type.get_class_name().substr(0, 255);
      WNDCLASS wc = {
        /* Register the window class. */
        type.get_class_style(),
        win32::WindowEventProc,
        0,
        0,
        display,
        nullptr,
        type.get_cursor(),
        back,
        nullptr,
        name.c_str()
      };

      ATOM cls_id = RegisterClass(&wc);
      if (cls_id) {
        hidden::window_class_info_map[type.get_class_name()] = type;
      } else {
        auto error_no = GetLastError();
        if (error_no != 1410) {
          clog::trace() << getLastErrorText() << " class name length: " << name.length();
        }
      }
      
      os::window id = CreateWindowEx(type.get_ex_style(),
                                     name.c_str(),
                                     nullptr,         // address of window text
                                     type.get_style(),  // window style
                                     r.os_x(),        // horizontal position of window
                                     r.os_y(),        // vertical position of window
                                     r.os_width(),    // window width
                                     r.os_height(),   // window height
                                     parent_id,       // handle of parent window
                                     NULL,            // handle of menu or child-window identifier
                                     display,         // handle of application instance
                                     data);
      SetWindowLongPtr(id, GWLP_USERDATA, (LONG_PTR)data);

      return id;
    }

    void window::notify_event (os::message_type message, long l1, long l2) {
      send_client_message(this, message, l1, l2);
    }

    std::string window::get_class_name () const {
      char class_name[256];
      GetClassName(id, class_name, 256);
      return std::string(class_name);
    }

    core::size window::screen_size () {
      return core::size(gui::os::size{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) });
    }

    core::rectangle window::screen_area () {
      return core::rectangle(screen_size());
    }

#endif // GUIPP_WIN    

    // --------------------------------------------------------------------------

#ifdef GUIPP_X11
    namespace hidden {
      std::map<os::window, std::string> window_class_map;
    }

    window::~window () {
      destroy();
      x11::unprepare_win(this);
    }

    void window::init () {
      static int initialized = core::x11::init_messages();
      (void)initialized;
      x11::prepare_win_for_event(this, KeyPressMask);
    }

    void window::destroy () {
      if (get_os_window()) {
        hidden::window_class_map.erase(get_os_window());
        x11::validate_window(get_os_window());
        x11::check_return(XDestroyWindow(core::global::get_instance(), get_os_window()));
        detail::unset_os_window(get_os_window());
        id = 0;
      }
    }

    void window::close () {
      send_client_message(this, core::x11::WM_PROTOCOLS, core::x11::WM_DELETE_WINDOW);
    }

    void window::enable (bool on) {
      if (set_state().enable(on)) {
        if (is_valid()) {
          if (get_window_class().get_cursor()) {
            unsigned long mask = CWCursor;
            XSetWindowAttributes wa = {0};
            wa.cursor = on ? get_window_class().get_cursor()
                           : (os::cursor)win::cursor::arrow();
            x11::check_return(XChangeWindowAttributes(core::global::get_instance(), get_os_window(), mask, &wa));
          }
          invalidate();
        }
      }
    }

    bool window::is_valid () const {
      return (get_os_window() != 0) && (detail::get_window(get_os_window()) == this);
    }

    bool window::is_focused () const {
      Window focus = 0;
      int revert_to = 0;
      if (is_valid()) {
        if (x11::check_return(XGetInputFocus(core::global::get_instance(), &focus, &revert_to))) {
          return focus == get_os_window();
        }
      }
      return false;
    }

    bool window::has_border () const {
      XWindowAttributes a = {0};
      return (x11::check_status(XGetWindowAttributes(core::global::get_instance(), get_os_window(), &a)) &&
              (a.border_width > 0));
    }

    bool is_window_visible (os::window id) {
      XWindowAttributes a = {0};
      int result = XGetWindowAttributes(core::global::get_instance(), id, &a);
      return (x11::check_status(result) && (a.map_state == IsViewable));
    }

    bool window::is_visible () const {
      if (is_valid()) {
//        return get_state().visible();
        return is_window_visible(get_os_window());
      }
      return false;
    }

    void window::set_visible (bool s) {
      if (is_valid()) {
        bool current = is_visible();
        if (current != s) {
          if (s) {
            clog::trace() << "Show window:" << *this;
            x11::check_return(XMapWindow(core::global::get_instance(), get_os_window()));
          } else {
            clog::trace() << "Hide window:" << *this;
            x11::check_return(XUnmapWindow(core::global::get_instance(), get_os_window()));
          }
          set_state().visible(is_window_visible(get_os_window()));
        }
      }
    }

    void window::take_focus (bool) {
      set_state().focused(true);
      x11::check_return(XSetInputFocus(core::global::get_instance(), get_os_window(),
                                       RevertToParent, CurrentTime));
      invalidate();
    }

    void window::to_front () {
      x11::check_return(XRaiseWindow(core::global::get_instance(), get_os_window()));
    }

    void window::to_back () {
      x11::check_return(XLowerWindow(core::global::get_instance(), get_os_window()));
    }

    void window::redraw () const {

      if (get_state().redraw_disabled() || !is_visible()) {
        return;
      }

      XEvent event;

      XExposeEvent& e = event.xexpose;
      e.type = Expose;
      e.serial = 0;
      e.send_event = true;
      e.display = core::global::get_instance();
      e.window = get_os_window();
      auto p = place();
      e.x = p.os_x();
      e.y = p.os_y();
      e.width = p.os_width();
      e.height = p.os_height();
      e.count = 0;
      gui::os::event_result result;

      clog::trace() << "redraw: " << event;
      events.handle_event(event, result);

      core::global::sync();
    }

    void window::invalidate () const {
      if (get_os_window() && is_visible()) {
        x11::invalidate_window(get_os_window());
      }
    }

    core::size window::size () const {
      Window root = 0;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      Window wid = get_os_window();
      if (wid && x11::check_status(XGetGeometry(core::global::get_instance(), wid,
                                                &root, &x, &y, &width, &height,
                                                &border_width, &depth))) {
        return core::global::scale_from_native(core::native_size{width, height});
      }
      return core::size::zero;
    }

    core::point window::position () const {
      Window root = 0;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      Window wid = get_os_window();
      if (wid && x11::check_return(XGetGeometry(core::global::get_instance(), wid,
                                                &root, &x, &y, &width, &height,
                                                &border_width, &depth))) {
        return core::global::scale_from_native(core::native_point{x, y});
      }
      return core::point::undefined;
    }

    core::rectangle window::place () const {
      Window root = 0;
      int x = 0, y = 0;
      unsigned int width = 0, height = 0;
      unsigned int border_width = 0;
      unsigned int depth = 0;
      Window wid = get_os_window();
      if (wid && x11::check_return(XGetGeometry(core::global::get_instance(), wid,
                                                &root, &x, &y, &width, &height,
                                                &border_width, &depth))) {
        return core::global::scale_from_native(core::native_rect{x, y, width, height});
      }
      return core::rectangle::def;
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point window::absolute_position () const {
      return client_to_screen(core::point::zero);
    }

    core::size window::client_size () const {
      return size(); // - core::size(1, 1);
    }

    core::rectangle window::client_area () const {
      return core::rectangle(client_size());
    }

    void window::move (const core::point& pt, bool repaint) {
      if (position() != pt) {
        auto ox = pt.os_x();
        auto oy = pt.os_y();
        x11::check_return(XMoveWindow(core::global::get_instance(), get_os_window(), ox, oy));
        if (repaint) {
          invalidate();
        }
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      if (sz.empty()) {
        if (is_visible()) {
          set_visible(false);
        }
      } else {
        if (size() != sz) {
          if (!is_visible()) {
            set_visible();
          }
          x11::check_return(XResizeWindow(core::global::get_instance(),
                                          get_os_window(), sz.os_width(), sz.os_height()));
          send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(sz));
          if (repaint) {
            invalidate();
          }
        }
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      if (r.empty()) {
        if (is_visible()) {
          set_visible(false);
        }
      } else {
        if (!is_visible()) {
          set_visible();
        }
        const auto current = place();
        if (current != r) {

          x11::check_return(XMoveResizeWindow(core::global::get_instance(),
                                              get_os_window(), r.os_x(), r.os_y(),
                                              r.os_width(), r.os_height()));
          if (current.size() != r.size()) {
            send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(r.size()));
            if (repaint) {
              invalidate();
            }
          }
        }
      }
    }

    core::point window::client_to_screen (const core::point& pt) const {
      int x, y;
      Window child_return;
      auto display = core::global::get_instance();
      x11::check_return(XTranslateCoordinates(display,
                                              get_os_window(),
                                              DefaultRootWindow(display),
                                              core::global::scale_to_native<int>(pt.x()),
                                              core::global::scale_to_native<int>(pt.y()),
                                              &x,
                                              &y,
                                              &child_return));
      return {core::global::scale_from_native<core::point::type>(x), core::global::scale_from_native<core::point::type>(y)};
    }

    core::point window::screen_to_client (const core::point& pt) const {
      int x, y;
      Window child_return;
      auto display = core::global::get_instance();
      x11::check_return(XTranslateCoordinates(display,
                                              DefaultRootWindow(display),
                                              get_os_window(),
                                              core::global::scale_to_native<int>(pt.x()),
                                              core::global::scale_to_native<int>(pt.y()),
                                              &x,
                                              &y,
                                              &child_return));
      return {core::global::scale_from_native<core::point::type>(x), core::global::scale_from_native<core::point::type>(y)};
    }

    void window::set_cursor (const os::cursor& c) {
      XDefineCursor(core::global::get_instance(), get_os_window(), c);
    }

    void window::capture_pointer () {
#ifndef NO_CAPTURE
      clog::trace() << "capture_pointer:" << get_os_window();
      hidden::capture_stack.push_back(get_os_window());
      x11::check_return(XGrabPointer(core::global::get_instance(), get_os_window(),
                                     False,
                                     ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                     GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
#endif // NO_CAPTURE
    }

    void window::uncapture_pointer () {
#ifndef NO_CAPTURE
      if (!hidden::capture_stack.empty()) {
        if (hidden::capture_stack.back() != get_os_window()) {
          clog::fatal() << "uncapture_pointer:" << get_os_window() << " differs from stack back:(" << hidden::capture_stack.back() << ")";
        } else {
          clog::trace() << "uncapture_pointer:" << get_os_window();
        }
        x11::check_return(XUngrabPointer(core::global::get_instance(), CurrentTime));
        hidden::capture_stack.pop_back();
        if (!hidden::capture_stack.empty()) {
          clog::trace() << "re-capture_pointer:" << hidden::capture_stack.back();
          x11::check_return(XGrabPointer(core::global::get_instance(), hidden::capture_stack.back(),
                                         False,
                                         ButtonPressMask | ButtonReleaseMask | PointerMotionMask | EnterWindowMask | LeaveWindowMask,
                                         GrabModeAsync, GrabModeAsync, None, None, CurrentTime));
        }
      }
#endif // NO_CAPTURE
    }

    os::window window::create_window (const class_info& type,
                                      const core::rectangle& r,
                                      os::window parent_id,
                                      window* data) {
      auto display = core::global::get_instance();
      auto screen = core::global::x11::get_screen();

      unsigned long mask = 0;
      XSetWindowAttributes wa;

      XVisualInfo vinfo;
      XMatchVisualInfo(display, screen, 24, TrueColor, &vinfo);

      if (type.get_background() == color::transparent) {
        mask |= CWBackPixmap;
        wa.background_pixmap = None;
      } else {
        wa.background_pixel = type.get_background();
        wa.border_pixel = 0;
        mask |= (CWBackPixel | CWBorderPixel);
      }

      auto visual = vinfo.visual;
      auto depth = vinfo.depth;

      if (type.get_cursor()) {
        mask |= CWCursor;
        wa.cursor = type.get_cursor();
      }

      mask |= (CWBitGravity | CWBackingStore | CWColormap);
      wa.bit_gravity = ForgetGravity;
      wa.backing_store = NotUseful;

      mask |= CWColormap;
      wa.colormap = XCreateColormap(display, DefaultRootWindow(display), visual, AllocNone);

      os::window id = XCreateWindow(display,
                                    parent_id,
                                    r.os_x(),
                                    r.os_y(),
                                    std::max<gui::os::size_type>(r.os_width(), 1),
                                    std::max<gui::os::size_type>(r.os_height(), 1),
                                    0,
                                    depth,
                                    type.get_class_style(),
                                    visual,
                                    mask,
                                    &wa);

      detail::set_os_window(id, data);
      data->id = id;

      x11::prepare_win_for_event(data, KeyReleaseMask);

      if (0 == hidden::window_class_info_map.count(type.get_class_name())) {
        hidden::window_class_info_map[type.get_class_name()] = type;
      }
      hidden::window_class_map[id] = type.get_class_name();

      return id;
    }

    std::string window::get_class_name () const {
      return hidden::window_class_map[get_os_window()];
    }

    void window::notify_event (os::message_type message, long l1, long l2) {
      XEvent event;
      XClientMessageEvent& client = event.xclient;

      client.type = ClientMessage;
      client.serial = 0;
      client.send_event = True;
      client.display = core::global::get_instance();
      client.window = get_os_window();
      client.message_type = message;
      client.format = 32;
      client.data.l[0] = l1;
      client.data.l[1] = l2;
      client.data.l[2] = 0;
      client.data.l[3] = 0;
      client.data.l[4] = 0;

      gui::os::event_result result = 0;
      handle_event(event, result);
    }

    core::size window::screen_size () {
      auto dpy = core::global::get_instance();
      auto scr = core::global::x11::get_screen();
      auto width = static_cast<gui::os::size_type>(DisplayWidth(dpy, scr));
      auto height = static_cast<gui::os::size_type>(DisplayHeight(dpy, scr));
      return core::size(gui::os::size{ width, height });
    }

    core::rectangle window::screen_area () {
      return core::rectangle(screen_size());
    }

#endif // GUIPP_X11

#ifdef GUIPP_QT
    // --------------------------------------------------------------------------
    namespace hidden {
      std::map<os::window, std::string> window_class_map;
    }

    void window::init ()
    {}

    window::~window () {
      destroy();
    }

    void window::destroy () {
      if (is_valid()) {
//        send_client_message(this, core::qt::WM_DESTROY_WINDOW);
        hidden::window_class_map.erase(id);
        id->set_window(nullptr);
        id->deleteLater();
        id = nullptr;
      }
    }

    void window::close () {
      if (is_valid()) {
        send_client_message(this, QEvent::Close);
        id->close();
      }
    }

    void window::enable (bool on) {
      if (set_state().enable(on)) {
        if (is_valid()) {
          get_os_window()->setEnabled(on);
          invalidate();
        }
      }
    }

    bool window::is_valid () const {
      return (get_os_window() != 0);
    }

    bool window::is_visible () const {
      return is_valid() && get_os_window()->isVisible();
    }

    bool window::is_focused () const {
      return is_valid() && get_os_window()->hasFocus();
    }

    bool window::is_child () const {
      return is_valid() && !get_os_window()->isTopLevel();
    }

    bool window::is_popup () const {
      return is_valid() && (get_os_window()->isWindow() && (get_os_window()->windowType() == Qt::Popup));
    }

    bool window::is_toplevel () const {
      return is_valid() && get_os_window()->isTopLevel();
    }

    bool window::has_border () const {
      return is_valid() && (get_os_window()->frameSize() != get_os_window()->size());
    }

    void window::set_visible (bool s) {
      if (is_valid()) {
        get_os_window()->setVisible(s);
      }
    }

    void window::take_focus (bool) {
      if (is_valid()) {
        auto win = get_os_window();
        set_state().focused(true);
        win->setFocus();
        invalidate();
      }
    }

    void window::to_front () {
      if (is_valid()) {
        get_os_window()->stackUnder(get_os_window()->parentWidget());
      }
    }

    void window::to_back () {
      if (is_valid()) {
        get_os_window()->lower();
      }
    }

    void window::redraw () const {
      if (is_valid()) {
        clog::trace() << "redraw: " << get_os_window();
        get_os_window()->repaint();
      }
    }

    void window::invalidate () const {
      if (is_valid()) {
        clog::trace() << "invalidate: " << get_os_window();
        get_os_window()->update();
      }
    }

    core::size window::size () const {
      return is_valid() ? core::size(get_os_window()->frameSize())
                        : core::size::zero;
    }

    core::point window::position () const {
      return is_valid() ? core::point(get_os_window()->pos())
                        : core::point::zero;
    }

    core::rectangle window::place () const {
      return is_valid() ? core::rectangle(get_os_window()->pos(), get_os_window()->frameSize())
                        : core::rectangle::zero;
    }

    core::rectangle window::absolute_place () const {
      return core::rectangle(absolute_position(), size());
    }

    core::point window::absolute_position () const {
      if (is_valid()) {
        auto w = get_os_window();
        return core::point(w->mapToGlobal(w->mapFromParent(w->pos())));
      }
      return core::point::zero;
    }

    core::size window::client_size () const {
      return is_valid() ? core::size(get_os_window()->size())
                        : core::size::zero;
    }

    core::rectangle window::client_area () const {
      return core::rectangle(client_size());
    }

    void window::move (const core::point& pt, bool repaint) {
      if (is_valid()) {
        get_os_window()->move(pt.os_x(), pt.os_y());
        if (repaint) {
          invalidate();
        }
      }
    }

    void window::resize (const core::size& sz, bool repaint) {
      if (is_valid()) {
        get_os_window()->resize(sz.os_width(), sz.os_height());
        send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(sz));
        if (repaint) {
          invalidate();
        }
      }
    }

    void window::place (const core::rectangle& r, bool repaint) {
      if (is_valid()) {
        get_os_window()->setGeometry(r.os());
        if (repaint) {
          invalidate();
        }
        send_client_message(this, core::WM_LAYOUT_WINDOW, core::rectangle(r.size()));
      }
    }

    core::point window::client_to_screen (const core::point& pt) const {
      return is_valid() ? core::point(get_os_window()->mapToGlobal(pt.os()))
                        : core::point::zero;
    }

    core::point window::screen_to_client (const core::point& pt) const {
      return is_valid() ? core::point(get_os_window()->mapFromGlobal(pt.os()))
                        : core::point::zero;
    }

    void window::set_cursor (const os::cursor& c) {
      if (is_valid()) {
        get_os_window()->setCursor(c);
      }
    }

    void window::capture_pointer () {
#ifndef NO_CAPTURE
      if (is_valid()) {
        clog::trace() << "capture_pointer:" << get_os_window();
        hidden::capture_stack.push_back(get_os_window());
        get_os_window()->grabMouse();
      }
#endif // NO_CAPTURE
    }

    void window::uncapture_pointer () {
#ifndef NO_CAPTURE
      if (!hidden::capture_stack.empty()) {
        if (hidden::capture_stack.back() != get_os_window()) {
          clog::fatal() << "uncapture_pointer:" << get_os_window() << " differs from stack back:(" << hidden::capture_stack.back() << ")";
        } else {
          clog::trace() << "uncapture_pointer:" << get_os_window();
        }
        get_os_window()->releaseMouse();
        hidden::capture_stack.pop_back();
        if (!hidden::capture_stack.empty()) {
          clog::trace() << "re-capture_pointer:" << hidden::capture_stack.back();
          hidden::capture_stack.back()->grabMouse();
        }
      }
#endif // NO_CAPTURE
    }

    os::window window::create_window (const class_info& type,
                                      const core::rectangle& r,
                                      os::window parent_id,
                                      window* data) {
      os::window id = new os::qt::Widget(parent_id, type.get_style(), data);
      Qt::WindowFlags style = id->windowFlags();
      //clog::debug() << "Expected style: " << std::hex << type.get_style() << ", current style: " << std::hex << style;

      id->setGeometry(r.os());
      id->setCursor(type.get_cursor());

      QPalette pal = id->palette();
      pal.setColor(QPalette::Window, QColor(type.get_background()));
      id->setAutoFillBackground(true);
      id->setPalette(pal);
      id->setFocusPolicy(data->can_accept_focus() ? Qt::WheelFocus : Qt::NoFocus);

      hidden::window_class_map[id] = type.get_class_name();

      //type.get_ex_style(),
      //type.get_style(),  // window style

      return id;
    }

    void window::notify_event (os::message_type message, long l1, long l2) {
      send_client_message(this, message, l1, l2);
    }

    std::string window::get_class_name () const {
      return hidden::window_class_map[get_os_window()];
    }

    core::size window::screen_size () {
      return core::size(core::global::get_instance()->primaryScreen()->availableSize());
    }

    core::rectangle window::screen_area () {
      return core::rectangle(core::global::get_instance()->primaryScreen()->availableGeometry());
    }

#endif // GUIPP_QT

#ifdef GUIPP_COCOA

#ifdef __OBJC__
    - (void)editColor:(NSColor *)color locatedAtScreenRect:(NSRect)rect {
      
      // code unrelated to event monitoring deleted here.....
      
      // Start watching events to figure out when to close the window
      auto _eventMonitor = [NSEvent addLocalMonitorForEventsMatchingMask:
                       (NSLeftMouseDownMask | NSRightMouseDownMask | NSOtherMouseDownMask | NSKeyDownMask)
                                                            handler:^(NSEvent *incomingEvent) {
                       NSEvent *result = incomingEvent;
                       NSWindow *targetWindowForEvent = [incomingEvent window];
                       if (targetWindowForEvent != _window) {
                       [self _closeAndSendAction:NO];
                       } else if ([incomingEvent type] == NSKeyDown) {
                       if ([incomingEvent keyCode] == 53) {
                       // Escape
                       [self _closeAndSendAction:NO];
                       result = nil; // Don't process the event
                       } else if ([incomingEvent keyCode] == 36) {
                       // Enter
                       [self _closeAndSendAction:YES];
                       result = nil;
                       }
                       }
                       return result;
                       }];
    }
    
    void send_event () {
      CGPoint newloc;
      CGEventRef eventRef;
      newloc.x = x;
      newloc.y = y;
      
      eventRef = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, newloc,
                                         kCGMouseButtonCenter);
      //Apparently, a bug in xcode requires this next line
      CGEventSetType(eventRef, kCGEventMouseMoved);
      CGEventPost(kCGSessionEventTap, eventRef);
      CFRelease(eventRef);
      
      return 0;
    }
    
    void attachEventHandlers () {
      
      //create our event type spec for the keyup
      EventTypeSpec eventType;
      eventType.eventClass = kEventClassKeyboard;
      eventType.eventKind = kEventRawKeyUp;
      
      //create a callback for our event to fire in
      EventHandlerUPP handlerFunction = NewEventHandlerUPP(globalKeyPress);
      
      //install the event handler
      OSStatus err = InstallEventHandler(GetEventMonitorTarget(), handlerFunction, 1, &eventType, self, NULL);
      
      //error checking
      if( err )
      {
        //TODO: need an alert sheet here
        NSLog(@"Error registering keyboard handler...%d", err);
      }
      
      //create our event type spec for the mouse events
      EventTypeSpec eventTypeM;
      eventTypeM.eventClass = kEventClassMouse;
      eventTypeM.eventKind = kEventMouseUp;
      
      //create a callback for our event to fire in
      EventHandlerUPP handlerFunctionM = NewEventHandlerUPP(globalMousePress);
      
      //install the event handler
      OSStatus errM = InstallEventHandler(GetEventMonitorTarget(), handlerFunctionM, 1, &eventTypeM, self, NULL);
      
      //error checking
      if( errM )
      {
        //TODO: need an alert sheet here
        NSLog(@"Error registering mouse handler...%d", err);
      }
    }

#endif // __OBJC__
    
#endif // GUIPP_COCOA
    
    // --------------------------------------------------------------------------

  } // win

#ifdef GUIPP_QT

  namespace os {

    namespace qt {

      Widget::Widget (Widget* parent, os::style s, win::window* w)
        : QWidget(parent, s)
        , win(w)
      {}

      Widget::~Widget () {
        if (win) {
          win::detail::set_os_window(win, nullptr);
        }
      }

      win::window* Widget::get_window () const {
        return win;
      }

      void Widget::set_window (win::window* w) {
        win = w;
      }

      Widget* Widget::get_parent () const {
        return static_cast<Widget*>(parentWidget());
      }

      bool Widget::event (QEvent* e) {
        gui::os::event_result result;
        gui::core::event ev = {this, e};
        clog::trace() << "Widget received event: " << ev;
        if (win && win->handle_event(ev, result)) {
          return true;
        }
        return QWidget::event(e);
      }

    } // namespace qt

  } // os

#endif // GUIPP_QT

} // gui
