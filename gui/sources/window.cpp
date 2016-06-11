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

// --------------------------------------------------------------------------
//
// Library includes
//
#include "window.h"


namespace gui {

  window::window()
    : id(0)
  {}

  window::~window() {
    destroy();
    for (auto i : event_handler_list) {
      delete i;
    }
  }

  void window::create(const window_class& type,
                      const window& parent,
                      const rectangle& place) {

    id = CreateWindowEx(type.ex_style,          // window style
                       type.class_name.c_str(), // address of registered class name
                       NULL,                    // address of window text
                       type.style,              // window style
                       place.position.x,        // horizontal position of window
                       place.position.y,        // vertical position of window
                       place.size.width,        // window width
                       place.size.height,       // window height
                       parent.get_id(),         // handle of parent window
                       NULL,                    // handle of menu or child-window identifier
                       type.instance,           // handle of application instance
                       (LPVOID)this);
  }

  void window::create(const window_class& type,
                      const rectangle& place) {

    id = CreateWindowEx(type.ex_style,          // window style
                        type.class_name.c_str(), // address of registered class name
                        NULL,                    // address of window text
                        type.style,              // window style
                        place.position.x,        // horizontal position of window
                        place.position.y,        // vertical position of window
                        place.size.width,        // window width
                        place.size.height,       // window height
                        NULL,                    // handle of parent window
                        NULL,                    // handle of menu or child-window identifier
                        type.instance,           // handle of application instance
                        (LPVOID)this);
  }

  bool window::is_valid() const {
    return IsWindow(id) != FALSE;
  }

  bool window::is_visible() const {
    return is_valid() && IsWindowVisible(get_id());
  }

  bool window::is_enabled() const {
    return is_valid() && IsWindowEnabled(get_id());
  }

  bool window::is_active() const {
    return is_valid() && (GetActiveWindow() == id);
  }

  bool window::is_popup() const {
    return (GetWindowLong(get_id(), GWL_STYLE) & WS_POPUP) == WS_POPUP;
  }

  bool window::is_overlapped() const {
    return (GetWindowLong(get_id(), GWL_STYLE) & (WS_CHILD | WS_POPUP)) == 0;
  }

  bool window::is_toplevel() const {
    return (GetWindowLong(get_id(), GWL_EXSTYLE) & WS_EX_TOPMOST) == WS_EX_TOPMOST;
  }

  bool window::is_minimized() const {
    return IsIconic(get_id()) != FALSE;
  }

  bool window::is_maximized() const {
    return IsZoomed(get_id()) != FALSE;
  }

  bool window::has_border() const {
    return (GetWindowLong(get_id(), GWL_STYLE) & (WS_BORDER | WS_DLGFRAME | WS_THICKFRAME) ? true : false);
  }

  void window::close() {
    CloseWindow(get_id());
  }

  void window::destroy() {
    DestroyWindow(get_id());
  }

  void window::quit() {
    PostQuitMessage(0);
  }

  void window::setParent(const window& parent) {
    SetParent(get_id(), parent.get_id());
  }

  window* window::getParent() const {
    return is_valid() ? window::get(GetParent(get_id())) : nullptr;
  }

  bool window::is_parent_of(const window& child) const {
    return is_valid() && child.is_valid() && IsChild(get_id(), child.get_id()) != FALSE;
  }

  bool window::is_child_of(const window& parent) const {
    return is_valid() && parent.is_valid() && IsChild(parent.get_id(), get_id()) != FALSE;
  }

  void window::hide() {
    ShowWindow(get_id(), SW_HIDE);
  }
  
  void window::show() {
    ShowWindow(get_id(), SW_SHOWNA);
  }

  void window::minimize() {
    ShowWindow(get_id(), SW_MINIMIZE);
  }

  void window::maximize() {
    ShowWindow(get_id(), SW_MAXIMIZE);
  }

  void window::restore() {
    ShowWindow(get_id(), SW_RESTORE);
  }

  void window::set_toplevel(bool toplevel) {
    SetWindowPos(get_id(), toplevel ? HWND_TOPMOST : HWND_NOTOPMOST,
                 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
  }

  void window::enable(bool on) {
    EnableWindow(get_id(), on);
  }

  void window::enableRedraw(bool on) {
    SendMessage(get_id(), WM_SETREDRAW, on, 0);
  }

  void window::redraw_now() {
    UpdateWindow(get_id());
  }

  void window::redraw_later() {
    InvalidateRect(get_id(), NULL, TRUE);
  }

  void window::setText(const std::string& s) {
    SetWindowText(get_id(), s.c_str());
  }

  std::string window::getText() const {
    std::string s;
    s.resize(GetWindowTextLength(get_id()) + 1);
    GetWindowText(get_id(), &s[0], (int)s.capacity());
    return s;
  }

  size window::size() const {
    RECT r;
    GetWindowRect(get_id(), &r);
    return gui::size(r.right - r.left, r.bottom - r.top);
  }

  position window::position() const {
    RECT r;
    GetWindowRect(get_id(), &r);
    return gui::position(r.left, r.top);
  }

  rectangle window::place() const {
    RECT r;
    GetWindowRect(get_id(), &r);
    return gui::rectangle(r.left, r.top, r.right - r.left, r.bottom - r.top);
  }

  position window::absolute_position() const {
    return windowToScreen(position());
  }

  rectangle window::client_area() const {
    RECT r;
    GetClientRect(get_id(), &r);
    return gui::rectangle(r.left, r.top, r.right - r.left, r.bottom - r.top);
  }

  void window::move(const gui::position& pt, bool repaint) {
    SetWindowPos(get_id(), 0, pt.x, pt.y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER | (repaint ? 0 : SWP_NOREDRAW));
  }
  
  void window::resize(const gui::size& sz, bool repaint) {
    SetWindowPos(get_id(), 0, 0, 0, sz.width, sz.height, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER | (repaint ? 0 : SWP_NOREDRAW));
  }
  
  void window::place(const gui::rectangle& r, bool repaint) {
    MoveWindow(get_id(), r.position.x, r.position.y, r.size.width, r.size.height, repaint);
  }

  gui::position window::windowToScreen(const gui::position& pt) const {
    POINT Point = { pt.x, pt.y };
    ClientToScreen(get_id(), &Point);
    return gui::position(Point.x, Point.y);
  }
  
  gui::position window::screenToWindow(const gui::position& pt) const {
    POINT Point = { pt.x, pt.y };
    ScreenToClient(get_id(), &Point);
    return gui::position(Point.x, Point.y);
  }

  void window::register_event_handler(event_handler* handler) {
    event_handler_list.push_back(handler);
  }

  void window::unregister_event_handler(event_handler* handler) {
    auto end = event_handler_list.end();
    auto idx = std::remove(event_handler_list.begin(), end, handler);
    event_handler_list.erase(idx, end);
  }

  window* window::get(window_id id) {
    return reinterpret_cast<window*>(GetWindowLongPtr(id, GWLP_USERDATA));
  }

  bool window::handle_event(const window_event& e, event_result& resultValue) {
    bool result = false;
    for (auto i : event_handler_list) {
      result |= i->handle_event(e, resultValue);
    }
    return result;
  }

}
