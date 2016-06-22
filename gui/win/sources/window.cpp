/**
* @copyright (c) 2015-2016 Ing. Buero Rothfuss
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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"


namespace gui {

  namespace win {

#ifdef X11
    namespace detail {

        typedef std::map<core::window_id, win::window*> window_map;
        window_map global_window_map;

    }
#endif // X11

    // --------------------------------------------------------------------------

    window::window()
      : id(0) {
    }

    window::~window() {
      destroy();
#ifdef X11
      detail::global_window_map.erase(id);
#endif // X11
    }

    void window::create(const window_class& type,
                        const window& parent,
                        const core::rectangle& place) {

      const core::point pos = place.position();
      const core::size sz = place.size();
#ifdef WIN32
      id = CreateWindowEx(type.get_ex_style(),            // window style
                          type.get_class_name().c_str(),  // address of registered class name
                          NULL,                           // address of window text
                          type.get_style(),               // window style
                          pos.x,                          // horizontal position of window
                          pos.y,                          // vertical position of window
                          sz.width,                       // window width
                          sz.height,                      // window height
                          parent.get_id(),                // handle of parent window
                          NULL,                           // handle of menu or child-window identifier
                          core::global::get_instance(),   // handle of application instance
                          (LPVOID)this);
#elif X11
      id = XCreateSimpleWindow(core::global::get_instance(), parent.id, pos.x, pos.y, sz.width, sz.height, type.get_style(), type.get_foreground(), type.get_background());
      detail::global_window_map[id] = this;

	    XSetWindowAttributes wa;
	    wa.event_mask = 0xFFFFFF;
	    XChangeWindowAttributes(core::global::get_instance(), id, CWEventMask, &wa);
#endif // X11
    }

    void window::create(const window_class& type,
                        const core::rectangle& place) {

      const core::point pos = place.position();
      const core::size sz = place.size();
#ifdef WIN32
      id = CreateWindowEx(type.get_ex_style(),            // window style
                          type.get_class_name().c_str(),  // address of registered class name
                          NULL,                           // address of window text
                          type.get_style(),               // window style
                          pos.x,                          // horizontal position of window
                          pos.y,                          // vertical position of window
                          sz.width,                       // window width
                          sz.height,                      // window height
                          NULL,                           // handle of parent window
                          NULL,                           // handle of menu or child-window identifier
                          core::global::get_instance(),   // handle of application instance
                          (LPVOID)this);
#elif X11
      id = XCreateSimpleWindow(core::global::get_instance(),
                               DefaultRootWindow(core::global::get_instance()),
                               pos.x, pos.y, sz.width, sz.height, type.get_style(), type.get_foreground(), type.get_background());
      detail::global_window_map[id] = this;

	    XSetWindowAttributes wa;
	    wa.event_mask = 0xFFFFFF;
	    XChangeWindowAttributes(core::global::get_instance(), id, CWEventMask, &wa);

#endif // X11
    }

    bool window::is_valid() const {
#ifdef WIN32
      return IsWindow(id) != FALSE;
#elif X11
      return detail::global_window_map[id] == this;
#endif // X11
    }

    bool window::is_visible() const {
#ifdef WIN32
      return is_valid() && IsWindowVisible(get_id());
#elif X11
        XWindowAttributes a;
        if (XGetWindowAttributes(core::global::get_instance(), id, &a)) {
            return a.map_state == IsViewable;
        }
        return false;
#endif // X11
    }

    bool window::is_enabled() const {
#ifdef WIN32
      return is_valid() && IsWindowEnabled(get_id());
#elif X11
      return is_valid();
#endif // X11
    }

    bool window::is_active() const {
#ifdef WIN32
      return is_valid() && (GetActiveWindow() == id);
#elif X11
      return is_valid();
#endif // X11
    }

    bool window::is_child() const {
#ifdef WIN32
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
#elif X11
      return false;
#endif // X11
    }

    bool window::is_popup() const {
#ifdef WIN32
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_POPUP) == WS_POPUP;
#elif X11
      return false;
#endif // X11
    }

    bool window::is_toplevel() const {
#ifdef WIN32
      return (GetWindowLong(get_id(), GWL_STYLE) & WS_CHILD) != WS_CHILD;
#elif X11
      return false;
#endif // X11
    }

    bool window::is_top_most() const {
#ifdef WIN32
      return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
#elif X11
      return false;
#endif // X11
    }

    bool window::is_minimized() const {
#ifdef WIN32
      return IsIconic(get_id()) != FALSE;
#elif X11
      return false;
#endif // X11
    }

    bool window::is_maximized() const {
#ifdef WIN32
      return IsZoomed(get_id()) != FALSE;
#elif X11
      return false;
#endif // X11
    }

    bool window::has_border() const {
#ifdef WIN32
      return (GetWindowLong(get_id(), GWL_STYLE) & (WS_BORDER | WS_DLGFRAME | WS_THICKFRAME) ? true : false);
#elif X11
      return false;
#endif // X11
    }

    void window::close() {
#ifdef WIN32
      CloseWindow(get_id());
#elif X11
#endif // X11
    }

    void window::destroy() {
#ifdef WIN32
      DestroyWindow(get_id());
#elif X11
#endif // X11
    }

    void window::quit() {
#ifdef WIN32
      PostQuitMessage(0);
#elif X11
#endif // X11
    }

    void window::setParent(const window& parent) {
#ifdef WIN32
      SetParent(get_id(), parent.get_id());
#elif X11
#endif // X11
    }

    window* window::getParent() const {
#ifdef WIN32
      return is_valid() ? window::get(GetParent(get_id())) : nullptr;
#elif X11
      return nullptr;
#endif // X11
    }

    bool window::is_parent_of(const window& child) const {
#ifdef WIN32
      return is_valid() && child.is_valid() && IsChild(get_id(), child.get_id()) != FALSE;
#elif X11
      return false;
#endif // X11
    }

    bool window::is_child_of(const window& parent) const {
#ifdef WIN32
      return is_valid() && parent.is_valid() && IsChild(parent.get_id(), get_id()) != FALSE;
#elif X11
      return false;
#endif // X11
    }

    void window::hide() {
#ifdef WIN32
      ShowWindow(get_id(), SW_HIDE);
#elif X11
      XUnmapWindow(core::global::get_instance(), id);
#endif // X11
    }

    void window::show() {
#ifdef WIN32
      ShowWindow(get_id(), SW_SHOWNA);
#elif X11
      XMapWindow(core::global::get_instance(), id);
#endif // X11
    }

    void window::minimize() {
#ifdef WIN32
      ShowWindow(get_id(), SW_MINIMIZE);
#elif X11
#endif // X11
    }

    void window::maximize() {
#ifdef WIN32
      ShowWindow(get_id(), SW_MAXIMIZE);
#elif X11
#endif // X11
    }

    void window::restore() {
#ifdef WIN32
      ShowWindow(get_id(), SW_RESTORE);
#elif X11
#endif // X11
    }

    void window::set_top_most(bool toplevel) {
#ifdef WIN32
      SetWindowPos(get_id(),
                   toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                   0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
#elif X11
#endif // X11
    }

    void window::enable(bool on) {
#ifdef WIN32
      EnableWindow(get_id(), on);
#elif X11
#endif // X11
    }

    void window::enableRedraw(bool on) {
#ifdef WIN32
      SendMessage(get_id(), WM_SETREDRAW, on, 0);
#elif X11
#endif // X11
    }

    void window::redraw_now() {
#ifdef WIN32
      RedrawWindow(get_id(), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
#elif X11
#endif // X11
    }

    void window::redraw_later() {
#ifdef WIN32
      InvalidateRect(get_id(), NULL, TRUE);
#elif X11
#endif // X11
    }

    void window::setText(const std::string& s) {
#ifdef WIN32
      SetWindowText(get_id(), s.c_str());
#elif X11
#endif // X11
    }

    std::string window::getText() const {
      std::string s;
#ifdef WIN32
      s.resize(GetWindowTextLength(get_id()) + 1);
      GetWindowText(get_id(), &s[0], (int)s.capacity());
#elif X11
#endif // X11
      return s;
    }

    core::size window::size() const {
#ifdef WIN32
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::size(r.right - r.left, r.bottom - r.top);
#elif X11
      return core::size();
#endif // X11
    }

    core::point window::position() const {
#ifdef WIN32
      RECT r;
      GetWindowRect(get_id(), &r);
      return screenToWindow({ r.left, r.top });
#elif X11
      return core::point();
#endif // X11
    }

    core::rectangle window::place() const {
      const core::rectangle pl = absolute_place();
      return core::rectangle(screenToWindow(pl.position()), pl.size());
    }

    core::rectangle window::absolute_place() const {
#ifdef WIN32
      RECT r;
      GetWindowRect(get_id(), &r);
      return core::rectangle(r);
#elif X11
      return core::rectangle();
#endif // X11
    }

    core::point window::absolute_position() const {
#ifdef WIN32
      RECT r;
      GetWindowRect(get_id(), &r);
      return{ r.left, r.top };
#elif X11
      return core::point();
#endif // X11
    }

    core::rectangle window::client_area() const {
#ifdef WIN32
      RECT r;
      GetClientRect(get_id(), &r);
      return core::rectangle(r);
#elif X11
      return core::rectangle();
#endif // X11
    }

    void window::move(const core::point& pt, bool repaint) {
      place(core::rectangle(pt, size()));
    }

    void window::resize(const core::size& sz, bool repaint) {
      place(core::rectangle(position(), sz));
    }

    void window::place(const core::rectangle& r, bool repaint) {
      const core::point pos = r.position();
      const core::size sz = r.size();
#ifdef WIN32
      MoveWindow(get_id(), pos.x, pos.y, sz.width, sz.height, repaint);
#elif X11
#endif // X11
    }

    core::point window::windowToScreen(const core::point& pt) const {
      window* p = getParent();
      return p ? p->clientToScreen(pt) : pt;
    }

    core::point window::screenToWindow(const core::point& pt) const {
      window* p = getParent();
      return p ? p->screenToClient(pt) : pt;
    }

    core::point window::clientToScreen(const core::point& pt) const {
#ifdef WIN32
      POINT Point = { pt.x, pt.y };
      ClientToScreen(get_id(), &Point);
      return core::point(Point.x, Point.y);
#elif X11
      return core::point();
#endif // X11
    }

    core::point window::screenToClient(const core::point& pt) const {
#ifdef WIN32
      POINT Point = { pt.x, pt.y };
      ScreenToClient(get_id(), &Point);
      return core::point(Point.x, Point.y);
#elif X11
      return core::point();
#endif // X11
    }

    window* window::get(core::window_id id) {
#ifdef WIN32
      return reinterpret_cast<window*>(GetWindowLongPtr(id, GWLP_USERDATA));
#endif // WIN32
#ifdef X11
      return detail::global_window_map[id];
#endif // X11
    }

  } // win

} // gui
