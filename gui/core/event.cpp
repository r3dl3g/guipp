/**
 * @copyright (c) 2016-2017 Ing. Buero Rothfuss
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
 * @brief     C++ API: event
 *
 * @file
 */

// --------------------------------------------------------------------------
//
// Common includes
//
#include <map>
#include <iomanip>
#include <ostream>
#include <functional>

// --------------------------------------------------------------------------
//
// Library includes
//
#include <gui/core/event.h>

namespace gui {

  namespace core {

#ifdef GUIPP_X11
    Atom WM_LAYOUT_WINDOW = 0;

    namespace x11 {

      Atom WM_CREATE_WINDOW = 0;
      Atom WM_DELETE_WINDOW = 0;
      Atom WM_PROTOCOLS = 0;
      Atom WM_TAKE_FOCUS = 0;
      Atom GUI_LIB_WIN_PTR = 0;

      int init_messages () {
        init_atom(WM_CREATE_WINDOW, "WM_CREATE_WINDOW");
        init_atom(WM_DELETE_WINDOW, "WM_DELETE_WINDOW");
        init_atom(WM_PROTOCOLS, "WM_PROTOCOLS");
        init_atom(WM_TAKE_FOCUS, "WM_TAKE_FOCUS");
        init_atom(WM_LAYOUT_WINDOW, "WM_LAYOUT_WINDOW");
        init_atom(GUI_LIB_WIN_PTR, "GUI_LIB_WIN_PTR");
        return 1;
      }

      void init_atom (Atom& message, const char* name) {
        if (!message) {
          message = XInternAtom(core::global::get_instance(), name, False);
        }
      }
    } // namespace x11

#endif // GUIPP_X11

  } // namespace core

} // namespace gui

namespace std {

  typedef function<void(ostream&)> streamfkt;

  inline ostream& operator<< (ostream& out, const streamfkt& f) {
    f(out);
    return out;
  }

  namespace detail {

    typedef std::map<gui::os::event_id, const char*> message_map_t;

#define DEFINE_MESSAGE(a) {a, # a}

    message_map_t& get_message_map () {
      static message_map_t message_map = {
#ifdef GUIPP_WIN

        DEFINE_MESSAGE(WM_CREATE),
        DEFINE_MESSAGE(WM_DESTROY),

        DEFINE_MESSAGE(WM_MOVE),
        DEFINE_MESSAGE(WM_SIZE),
        DEFINE_MESSAGE(WM_ENTERSIZEMOVE),
        DEFINE_MESSAGE(WM_EXITSIZEMOVE),
        DEFINE_MESSAGE(WM_MOVING),
        DEFINE_MESSAGE(WM_SIZING),
        DEFINE_MESSAGE(WM_GETMINMAXINFO),

        DEFINE_MESSAGE(WM_WINDOWPOSCHANGED),
        DEFINE_MESSAGE(WM_WINDOWPOSCHANGING),
        DEFINE_MESSAGE(WM_SHOWWINDOW),
        DEFINE_MESSAGE(WM_QUERYOPEN),

        DEFINE_MESSAGE(WM_ACTIVATE),
        DEFINE_MESSAGE(WM_ACTIVATEAPP),
        DEFINE_MESSAGE(WM_SETFOCUS),
        DEFINE_MESSAGE(WM_KILLFOCUS),
        DEFINE_MESSAGE(WM_ENABLE),

        DEFINE_MESSAGE(WM_CLOSE),
        DEFINE_MESSAGE(WM_QUERYENDSESSION),
        DEFINE_MESSAGE(WM_ENDSESSION),
        DEFINE_MESSAGE(WM_QUIT),

        DEFINE_MESSAGE(WM_PAINT),
        DEFINE_MESSAGE(WM_SETREDRAW),
        DEFINE_MESSAGE(WM_ERASEBKGND),
        DEFINE_MESSAGE(WM_PRINT),
        DEFINE_MESSAGE(WM_PRINTCLIENT),

        DEFINE_MESSAGE(WM_CHAR),
        DEFINE_MESSAGE(WM_DEADCHAR),
        DEFINE_MESSAGE(WM_KEYDOWN),
        DEFINE_MESSAGE(WM_KEYUP),

        DEFINE_MESSAGE(WM_SYSCHAR),
        DEFINE_MESSAGE(WM_SYSDEADCHAR),
        DEFINE_MESSAGE(WM_SYSKEYDOWN),
        DEFINE_MESSAGE(WM_SYSKEYUP),

        DEFINE_MESSAGE(WM_HOTKEY),

        DEFINE_MESSAGE(WM_MOUSEACTIVATE),
        DEFINE_MESSAGE(WM_MOUSEHOVER),
        DEFINE_MESSAGE(WM_MOUSELEAVE),
        DEFINE_MESSAGE(WM_MOUSEMOVE),
        DEFINE_MESSAGE(WM_MOUSEWHEEL),
        DEFINE_MESSAGE(WM_MOUSEHWHEEL),

        DEFINE_MESSAGE(WM_LBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_LBUTTONDOWN),
        DEFINE_MESSAGE(WM_LBUTTONUP),

        DEFINE_MESSAGE(WM_RBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_RBUTTONDOWN),
        DEFINE_MESSAGE(WM_RBUTTONUP),

        DEFINE_MESSAGE(WM_MBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_MBUTTONDOWN),
        DEFINE_MESSAGE(WM_MBUTTONUP),

        DEFINE_MESSAGE(WM_XBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_XBUTTONDOWN),
        DEFINE_MESSAGE(WM_XBUTTONUP),

        DEFINE_MESSAGE(WM_CUT),
        DEFINE_MESSAGE(WM_COPY),
        DEFINE_MESSAGE(WM_PASTE),
        DEFINE_MESSAGE(WM_CLEAR),
        DEFINE_MESSAGE(WM_UNDO),

        DEFINE_MESSAGE(WM_GETTEXT),
        DEFINE_MESSAGE(WM_GETTEXTLENGTH),
        DEFINE_MESSAGE(WM_SETTEXT),

        DEFINE_MESSAGE(WM_GETFONT),
        DEFINE_MESSAGE(WM_SETFONT),
        DEFINE_MESSAGE(WM_FONTCHANGE),

        DEFINE_MESSAGE(WM_GETICON),
        DEFINE_MESSAGE(WM_SETICON),
        DEFINE_MESSAGE(WM_PAINTICON),
        DEFINE_MESSAGE(WM_ICONERASEBKGND),

        DEFINE_MESSAGE(WM_GETHOTKEY),
        DEFINE_MESSAGE(WM_SETHOTKEY),

        DEFINE_MESSAGE(WM_SETCURSOR),

        DEFINE_MESSAGE(WM_HSCROLL),
        DEFINE_MESSAGE(WM_VSCROLL),

        DEFINE_MESSAGE(WM_HSCROLLCLIPBOARD),
        DEFINE_MESSAGE(WM_VSCROLLCLIPBOARD),

        DEFINE_MESSAGE(WM_DRAWITEM),
        DEFINE_MESSAGE(WM_CHARTOITEM),
        DEFINE_MESSAGE(WM_COMPAREITEM),
        DEFINE_MESSAGE(WM_DELETEITEM),
        DEFINE_MESSAGE(WM_MEASUREITEM),
        DEFINE_MESSAGE(WM_VKEYTOITEM),

        DEFINE_MESSAGE(WM_PAINTCLIPBOARD),
        DEFINE_MESSAGE(WM_DRAWCLIPBOARD),

        DEFINE_MESSAGE(WM_APPCOMMAND),
        DEFINE_MESSAGE(WM_ASKCBFORMATNAME),

        DEFINE_MESSAGE(WM_CANCELJOURNAL),
        DEFINE_MESSAGE(WM_CANCELMODE),
        DEFINE_MESSAGE(WM_CAPTURECHANGED),
        DEFINE_MESSAGE(WM_CHANGECBCHAIN),
        DEFINE_MESSAGE(WM_CHANGEUISTATE),
        DEFINE_MESSAGE(WM_CHILDACTIVATE),
        DEFINE_MESSAGE(WM_CLIPBOARDUPDATE),

        DEFINE_MESSAGE(WM_COMMAND),
        DEFINE_MESSAGE(WM_COMMNOTIFY),
        DEFINE_MESSAGE(WM_COMPACTING),
        DEFINE_MESSAGE(WM_CONTEXTMENU),
        DEFINE_MESSAGE(WM_COPYDATA),

        DEFINE_MESSAGE(WM_CTLCOLORBTN),
        DEFINE_MESSAGE(WM_CTLCOLORDLG),
        DEFINE_MESSAGE(WM_CTLCOLOREDIT),
        DEFINE_MESSAGE(WM_CTLCOLORLISTBOX),
        DEFINE_MESSAGE(WM_CTLCOLORMSGBOX),
        DEFINE_MESSAGE(WM_CTLCOLORSCROLLBAR),
        DEFINE_MESSAGE(WM_CTLCOLORSTATIC),

        DEFINE_MESSAGE(WM_DESTROYCLIPBOARD),
        DEFINE_MESSAGE(WM_DEVICECHANGE),
        DEFINE_MESSAGE(WM_DEVMODECHANGE),
        DEFINE_MESSAGE(WM_DISPLAYCHANGE),
#if(WINVER >= 0x0602)
        DEFINE_MESSAGE(WM_DPICHANGED),
#endif
        DEFINE_MESSAGE(WM_DROPFILES),
        DEFINE_MESSAGE(WM_DWMCOLORIZATIONCOLORCHANGED),
        DEFINE_MESSAGE(WM_DWMCOMPOSITIONCHANGED),
        DEFINE_MESSAGE(WM_DWMNCRENDERINGCHANGED),
        DEFINE_MESSAGE(WM_DWMSENDICONICLIVEPREVIEWBITMAP),
        DEFINE_MESSAGE(WM_DWMSENDICONICTHUMBNAIL),
        DEFINE_MESSAGE(WM_DWMWINDOWMAXIMIZEDCHANGE),

        DEFINE_MESSAGE(WM_ENTERIDLE),
        DEFINE_MESSAGE(WM_ENTERMENULOOP),
        DEFINE_MESSAGE(WM_EXITMENULOOP),

        DEFINE_MESSAGE(WM_GESTURE),
        DEFINE_MESSAGE(WM_GESTURENOTIFY),
        DEFINE_MESSAGE(WM_GETDLGCODE),
        DEFINE_MESSAGE(WM_GETOBJECT),
        DEFINE_MESSAGE(WM_GETTITLEBARINFOEX),
        DEFINE_MESSAGE(WM_HELP),

        DEFINE_MESSAGE(WM_IME_CHAR),
        DEFINE_MESSAGE(WM_IME_COMPOSITION),
        DEFINE_MESSAGE(WM_IME_COMPOSITIONFULL),
        DEFINE_MESSAGE(WM_IME_CONTROL),
        DEFINE_MESSAGE(WM_IME_ENDCOMPOSITION),
        DEFINE_MESSAGE(WM_IME_KEYDOWN),
        DEFINE_MESSAGE(WM_IME_KEYLAST),
        DEFINE_MESSAGE(WM_IME_KEYUP),
        DEFINE_MESSAGE(WM_IME_NOTIFY),
        DEFINE_MESSAGE(WM_IME_REQUEST),
        DEFINE_MESSAGE(WM_IME_SELECT),
        DEFINE_MESSAGE(WM_IME_SETCONTEXT),
        DEFINE_MESSAGE(WM_IME_STARTCOMPOSITION),

        DEFINE_MESSAGE(WM_INITDIALOG),
        DEFINE_MESSAGE(WM_INITMENU),
        DEFINE_MESSAGE(WM_INITMENUPOPUP),

        DEFINE_MESSAGE(WM_INPUT),
        DEFINE_MESSAGE(WM_INPUTLANGCHANGE),
        DEFINE_MESSAGE(WM_INPUTLANGCHANGEREQUEST),
        DEFINE_MESSAGE(WM_INPUT_DEVICE_CHANGE),

        DEFINE_MESSAGE(WM_MDIACTIVATE),
        DEFINE_MESSAGE(WM_MDICASCADE),
        DEFINE_MESSAGE(WM_MDICREATE),
        DEFINE_MESSAGE(WM_MDIDESTROY),
        DEFINE_MESSAGE(WM_MDIGETACTIVE),
        DEFINE_MESSAGE(WM_MDIICONARRANGE),
        DEFINE_MESSAGE(WM_MDIMAXIMIZE),
        DEFINE_MESSAGE(WM_MDINEXT),
        DEFINE_MESSAGE(WM_MDIREFRESHMENU),
        DEFINE_MESSAGE(WM_MDIRESTORE),
        DEFINE_MESSAGE(WM_MDISETMENU),
        DEFINE_MESSAGE(WM_MDITILE),

        DEFINE_MESSAGE(WM_MENUCHAR),
        DEFINE_MESSAGE(WM_MENUCOMMAND),
        DEFINE_MESSAGE(WM_MENUDRAG),
        DEFINE_MESSAGE(WM_MENUGETOBJECT),
        DEFINE_MESSAGE(WM_MENURBUTTONUP),
        DEFINE_MESSAGE(WM_MENUSELECT),

        DEFINE_MESSAGE(WM_NCACTIVATE),
        DEFINE_MESSAGE(WM_NCCALCSIZE),
        DEFINE_MESSAGE(WM_NCCREATE),
        DEFINE_MESSAGE(WM_NCDESTROY),
        DEFINE_MESSAGE(WM_NCHITTEST),
        DEFINE_MESSAGE(WM_NCLBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_NCLBUTTONDOWN),
        DEFINE_MESSAGE(WM_NCLBUTTONUP),
        DEFINE_MESSAGE(WM_NCMBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_NCMBUTTONDOWN),
        DEFINE_MESSAGE(WM_NCMBUTTONUP),
        DEFINE_MESSAGE(WM_NCMOUSEHOVER),
        DEFINE_MESSAGE(WM_NCMOUSELEAVE),
        DEFINE_MESSAGE(WM_NCMOUSEMOVE),
        DEFINE_MESSAGE(WM_NCPAINT),
#if(WINVER >= 0x0602)
        DEFINE_MESSAGE(WM_NCPOINTERDOWN),
        DEFINE_MESSAGE(WM_NCPOINTERUP),
        DEFINE_MESSAGE(WM_NCPOINTERUPDATE),
#endif
        DEFINE_MESSAGE(WM_NCRBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_NCRBUTTONDOWN),
        DEFINE_MESSAGE(WM_NCRBUTTONUP),
        DEFINE_MESSAGE(WM_NCXBUTTONDBLCLK),
        DEFINE_MESSAGE(WM_NCXBUTTONDOWN),
        DEFINE_MESSAGE(WM_NCXBUTTONUP),

        DEFINE_MESSAGE(WM_NEXTDLGCTL),
        DEFINE_MESSAGE(WM_NEXTMENU),
        DEFINE_MESSAGE(WM_NOTIFY),
        DEFINE_MESSAGE(WM_NOTIFYFORMAT),

        DEFINE_MESSAGE(WM_PALETTECHANGED),
        DEFINE_MESSAGE(WM_PALETTEISCHANGING),
        DEFINE_MESSAGE(WM_PARENTNOTIFY),
#if(WINVER >= 0x0602)
        DEFINE_MESSAGE(WM_POINTERACTIVATE),
        DEFINE_MESSAGE(WM_POINTERCAPTURECHANGED),
        DEFINE_MESSAGE(WM_POINTERDEVICECHANGE),
        DEFINE_MESSAGE(WM_POINTERDEVICEINRANGE),
        DEFINE_MESSAGE(WM_POINTERDEVICEOUTOFRANGE),
        DEFINE_MESSAGE(WM_POINTERDOWN),
        DEFINE_MESSAGE(WM_POINTERENTER),
        DEFINE_MESSAGE(WM_POINTERHWHEEL),
        DEFINE_MESSAGE(WM_POINTERLEAVE),
        DEFINE_MESSAGE(WM_POINTERUP),
        DEFINE_MESSAGE(WM_POINTERUPDATE),
        DEFINE_MESSAGE(WM_POINTERWHEEL),
#endif
        DEFINE_MESSAGE(WM_POWER),
        DEFINE_MESSAGE(WM_POWERBROADCAST),

        DEFINE_MESSAGE(WM_QUERYDRAGICON),
        DEFINE_MESSAGE(WM_QUERYNEWPALETTE),
        DEFINE_MESSAGE(WM_QUERYUISTATE),
        DEFINE_MESSAGE(WM_QUEUESYNC),

        DEFINE_MESSAGE(WM_RENDERALLFORMATS),
        DEFINE_MESSAGE(WM_RENDERFORMAT),

        DEFINE_MESSAGE(WM_SIZECLIPBOARD),
        DEFINE_MESSAGE(WM_SPOOLERSTATUS),
        DEFINE_MESSAGE(WM_STYLECHANGED),
        DEFINE_MESSAGE(WM_STYLECHANGING),
        DEFINE_MESSAGE(WM_SYNCPAINT),
        DEFINE_MESSAGE(WM_SYSCOLORCHANGE),
        DEFINE_MESSAGE(WM_SYSCOMMAND),

        DEFINE_MESSAGE(WM_TCARD),
        DEFINE_MESSAGE(WM_THEMECHANGED),
        DEFINE_MESSAGE(WM_TIMECHANGE),
        DEFINE_MESSAGE(WM_TIMER),
        DEFINE_MESSAGE(WM_TOUCH),
#if(WINVER >= 0x0602)
        DEFINE_MESSAGE(WM_TOUCHHITTESTING),
#endif
        DEFINE_MESSAGE(WM_UNICHAR),
        DEFINE_MESSAGE(WM_UNINITMENUPOPUP),
        DEFINE_MESSAGE(WM_UPDATEUISTATE),
        DEFINE_MESSAGE(WM_USERCHANGED),

        DEFINE_MESSAGE(WM_WININICHANGE),
        DEFINE_MESSAGE(WM_WTSSESSION_CHANGE),

        DEFINE_MESSAGE(BM_CLICK),
        DEFINE_MESSAGE(BM_GETCHECK),
        DEFINE_MESSAGE(BM_GETIMAGE),
        DEFINE_MESSAGE(BM_GETSTATE),
        DEFINE_MESSAGE(BM_SETCHECK),
        DEFINE_MESSAGE(BM_SETDONTCLICK),
        DEFINE_MESSAGE(BM_SETIMAGE),
        DEFINE_MESSAGE(BM_SETSTATE),
        DEFINE_MESSAGE(BM_SETSTYLE),

        DEFINE_MESSAGE(DM_GETDEFID),
#if(WINVER >= 0x0602)
        DEFINE_MESSAGE(DM_POINTERHITTEST),
#endif
        DEFINE_MESSAGE(DM_REPOSITION),
        DEFINE_MESSAGE(DM_SETDEFID),

        DEFINE_MESSAGE(EM_CANUNDO),
        DEFINE_MESSAGE(EM_CHARFROMPOS),
        DEFINE_MESSAGE(EM_EMPTYUNDOBUFFER),
        DEFINE_MESSAGE(EM_FMTLINES),
        DEFINE_MESSAGE(EM_GETFIRSTVISIBLELINE),
        DEFINE_MESSAGE(EM_GETHANDLE),
        DEFINE_MESSAGE(EM_GETIMESTATUS),
        DEFINE_MESSAGE(EM_GETLIMITTEXT),
        DEFINE_MESSAGE(EM_GETLINE),
        DEFINE_MESSAGE(EM_GETLINECOUNT),
        DEFINE_MESSAGE(EM_GETMARGINS),
        DEFINE_MESSAGE(EM_GETMODIFY),
        DEFINE_MESSAGE(EM_GETPASSWORDCHAR),
        DEFINE_MESSAGE(EM_GETRECT),
        DEFINE_MESSAGE(EM_GETSEL),
        DEFINE_MESSAGE(EM_GETTHUMB),
        DEFINE_MESSAGE(EM_GETWORDBREAKPROC),
        DEFINE_MESSAGE(EM_LIMITTEXT),
        DEFINE_MESSAGE(EM_LINEFROMCHAR),
        DEFINE_MESSAGE(EM_LINEINDEX),
        DEFINE_MESSAGE(EM_LINELENGTH),
        DEFINE_MESSAGE(EM_LINESCROLL),
        DEFINE_MESSAGE(EM_POSFROMCHAR),
        DEFINE_MESSAGE(EM_REPLACESEL),
        DEFINE_MESSAGE(EM_SCROLL),
        DEFINE_MESSAGE(EM_SCROLLCARET),
        DEFINE_MESSAGE(EM_SETHANDLE),
        DEFINE_MESSAGE(EM_SETIMESTATUS),
        DEFINE_MESSAGE(EM_SETMARGINS),
        DEFINE_MESSAGE(EM_SETMODIFY),
        DEFINE_MESSAGE(EM_SETPASSWORDCHAR),
        DEFINE_MESSAGE(EM_SETREADONLY),
        DEFINE_MESSAGE(EM_SETRECT),
        DEFINE_MESSAGE(EM_SETRECTNP),
        DEFINE_MESSAGE(EM_SETSEL),
        DEFINE_MESSAGE(EM_SETTABSTOPS),
        DEFINE_MESSAGE(EM_SETWORDBREAKPROC),

        DEFINE_MESSAGE(EM_UNDO),
        DEFINE_MESSAGE(LB_ADDFILE),
        DEFINE_MESSAGE(LB_ADDSTRING),
        DEFINE_MESSAGE(LB_DELETESTRING),
        DEFINE_MESSAGE(LB_DIR),
        DEFINE_MESSAGE(LB_FINDSTRING),
        DEFINE_MESSAGE(LB_FINDSTRINGEXACT),
        DEFINE_MESSAGE(LB_GETANCHORINDEX),
        DEFINE_MESSAGE(LB_GETCARETINDEX),
        DEFINE_MESSAGE(LB_GETCOUNT),
        DEFINE_MESSAGE(LB_GETCURSEL),
        DEFINE_MESSAGE(LB_GETHORIZONTALEXTENT),
        DEFINE_MESSAGE(LB_GETITEMDATA),
        DEFINE_MESSAGE(LB_GETITEMHEIGHT),
        DEFINE_MESSAGE(LB_GETITEMRECT),
        DEFINE_MESSAGE(LB_GETLISTBOXINFO),
        DEFINE_MESSAGE(LB_GETLOCALE),
        DEFINE_MESSAGE(LB_GETSEL),
        DEFINE_MESSAGE(LB_GETSELCOUNT),
        DEFINE_MESSAGE(LB_GETSELITEMS),
        DEFINE_MESSAGE(LB_GETTEXT),
        DEFINE_MESSAGE(LB_GETTEXTLEN),
        DEFINE_MESSAGE(LB_GETTOPINDEX),
        DEFINE_MESSAGE(LB_INITSTORAGE),
        DEFINE_MESSAGE(LB_INSERTSTRING),
        DEFINE_MESSAGE(LB_ITEMFROMPOINT),
        DEFINE_MESSAGE(LB_MSGMAX),
        DEFINE_MESSAGE(LB_MSGMAX),
        DEFINE_MESSAGE(LB_MSGMAX),
        DEFINE_MESSAGE(LB_MSGMAX),
        DEFINE_MESSAGE(LB_RESETCONTENT),
        DEFINE_MESSAGE(LB_SELECTSTRING),
        DEFINE_MESSAGE(LB_SELITEMRANGE),
        DEFINE_MESSAGE(LB_SELITEMRANGEEX),
        DEFINE_MESSAGE(LB_SETANCHORINDEX),
        DEFINE_MESSAGE(LB_SETCARETINDEX),
        DEFINE_MESSAGE(LB_SETCOLUMNWIDTH),
        DEFINE_MESSAGE(LB_SETCOUNT),
        DEFINE_MESSAGE(LB_SETCURSEL),
        DEFINE_MESSAGE(LB_SETHORIZONTALEXTENT),
        DEFINE_MESSAGE(LB_SETITEMDATA),
        DEFINE_MESSAGE(LB_SETITEMHEIGHT),
        DEFINE_MESSAGE(LB_SETLOCALE),
        DEFINE_MESSAGE(LB_SETSEL),
        DEFINE_MESSAGE(LB_SETTABSTOPS),
        DEFINE_MESSAGE(LB_SETTOPINDEX),

        DEFINE_MESSAGE(SBM_ENABLE_ARROWS),
        DEFINE_MESSAGE(SBM_GETPOS),
        DEFINE_MESSAGE(SBM_GETRANGE),
        DEFINE_MESSAGE(SBM_GETSCROLLBARINFO),
        DEFINE_MESSAGE(SBM_GETSCROLLINFO),
        DEFINE_MESSAGE(SBM_SETPOS),
        DEFINE_MESSAGE(SBM_SETRANGE),
        DEFINE_MESSAGE(SBM_SETRANGEREDRAW),
        DEFINE_MESSAGE(SBM_SETSCROLLINFO),
        DEFINE_MESSAGE(SW_ERASE)

# if defined(_WIN32_WCE) && (_WIN32_WCE >= 0x0400)
        , DEFINE_MESSAGE(LB_MULTIPLEADDSTRING)
# endif
#endif // GUIPP_WIN

#ifdef GUIPP_X11
        DEFINE_MESSAGE(KeyPress),
        DEFINE_MESSAGE(KeyRelease),
        DEFINE_MESSAGE(ButtonPress),
        DEFINE_MESSAGE(ButtonRelease),
        DEFINE_MESSAGE(MotionNotify),
        DEFINE_MESSAGE(EnterNotify),
        DEFINE_MESSAGE(LeaveNotify),
        DEFINE_MESSAGE(FocusIn),
        DEFINE_MESSAGE(FocusOut),
        DEFINE_MESSAGE(KeymapNotify),
        DEFINE_MESSAGE(Expose),
        DEFINE_MESSAGE(GraphicsExpose),
        DEFINE_MESSAGE(NoExpose),
        DEFINE_MESSAGE(VisibilityNotify),
        DEFINE_MESSAGE(CreateNotify),
        DEFINE_MESSAGE(DestroyNotify),
        DEFINE_MESSAGE(UnmapNotify),
        DEFINE_MESSAGE(MapNotify),
        DEFINE_MESSAGE(MapRequest),
        DEFINE_MESSAGE(ReparentNotify),
        DEFINE_MESSAGE(ConfigureNotify),
        DEFINE_MESSAGE(ConfigureRequest),
        DEFINE_MESSAGE(GravityNotify),
        DEFINE_MESSAGE(ResizeRequest),
        DEFINE_MESSAGE(CirculateNotify),
        DEFINE_MESSAGE(CirculateRequest),
        DEFINE_MESSAGE(PropertyNotify),
        DEFINE_MESSAGE(SelectionClear),
        DEFINE_MESSAGE(SelectionRequest),
        DEFINE_MESSAGE(SelectionNotify),
        DEFINE_MESSAGE(ColormapNotify),
        DEFINE_MESSAGE(ClientMessage),
        DEFINE_MESSAGE(MappingNotify),
        DEFINE_MESSAGE(GenericEvent)
#endif // GUIPP_X11

#ifdef GUIPP_QT
        DEFINE_MESSAGE(QEvent::None),
        DEFINE_MESSAGE(QEvent::Timer),
        DEFINE_MESSAGE(QEvent::MouseButtonPress),
        DEFINE_MESSAGE(QEvent::MouseButtonRelease),
        DEFINE_MESSAGE(QEvent::MouseButtonDblClick),
        DEFINE_MESSAGE(QEvent::MouseMove),
        DEFINE_MESSAGE(QEvent::KeyPress),
        DEFINE_MESSAGE(QEvent::KeyRelease),
        DEFINE_MESSAGE(QEvent::FocusIn),
        DEFINE_MESSAGE(QEvent::FocusOut),
        DEFINE_MESSAGE(QEvent::FocusAboutToChange),
        DEFINE_MESSAGE(QEvent::Enter),
        DEFINE_MESSAGE(QEvent::Leave),
        DEFINE_MESSAGE(QEvent::Paint),
        DEFINE_MESSAGE(QEvent::Move),
        DEFINE_MESSAGE(QEvent::Resize),
        DEFINE_MESSAGE(QEvent::Create),
        DEFINE_MESSAGE(QEvent::Destroy),
        DEFINE_MESSAGE(QEvent::Show),
        DEFINE_MESSAGE(QEvent::Hide),
        DEFINE_MESSAGE(QEvent::Close),
        DEFINE_MESSAGE(QEvent::Quit),
        DEFINE_MESSAGE(QEvent::ParentChange),
        DEFINE_MESSAGE(QEvent::ParentAboutToChange),
        DEFINE_MESSAGE(QEvent::ThreadChange),
        DEFINE_MESSAGE(QEvent::WindowActivate),
        DEFINE_MESSAGE(QEvent::WindowDeactivate),
        DEFINE_MESSAGE(QEvent::ShowToParent),
        DEFINE_MESSAGE(QEvent::HideToParent),
        DEFINE_MESSAGE(QEvent::Wheel),
        DEFINE_MESSAGE(QEvent::WindowTitleChange),
        DEFINE_MESSAGE(QEvent::WindowIconChange),
        DEFINE_MESSAGE(QEvent::ApplicationWindowIconChange),
        DEFINE_MESSAGE(QEvent::ApplicationFontChange),
        DEFINE_MESSAGE(QEvent::ApplicationLayoutDirectionChange),
        DEFINE_MESSAGE(QEvent::ApplicationPaletteChange),
        DEFINE_MESSAGE(QEvent::PaletteChange),
        DEFINE_MESSAGE(QEvent::Clipboard),
        DEFINE_MESSAGE(QEvent::Speech),
        DEFINE_MESSAGE(QEvent::MetaCall),
        DEFINE_MESSAGE(QEvent::SockAct),
        DEFINE_MESSAGE(QEvent::WinEventAct),
        DEFINE_MESSAGE(QEvent::DeferredDelete),
        DEFINE_MESSAGE(QEvent::DragEnter),
        DEFINE_MESSAGE(QEvent::DragMove),
        DEFINE_MESSAGE(QEvent::DragLeave),
        DEFINE_MESSAGE(QEvent::Drop),
        DEFINE_MESSAGE(QEvent::DragResponse),
        DEFINE_MESSAGE(QEvent::ChildAdded),
        DEFINE_MESSAGE(QEvent::ChildPolished),
        DEFINE_MESSAGE(QEvent::ChildRemoved),
        DEFINE_MESSAGE(QEvent::ShowWindowRequest),
        DEFINE_MESSAGE(QEvent::PolishRequest),
        DEFINE_MESSAGE(QEvent::Polish),
        DEFINE_MESSAGE(QEvent::LayoutRequest),
        DEFINE_MESSAGE(QEvent::UpdateRequest),
        DEFINE_MESSAGE(QEvent::UpdateLater),
        DEFINE_MESSAGE(QEvent::EmbeddingControl),
        DEFINE_MESSAGE(QEvent::ActivateControl),
        DEFINE_MESSAGE(QEvent::DeactivateControl),
        DEFINE_MESSAGE(QEvent::ContextMenu),
        DEFINE_MESSAGE(QEvent::InputMethod),
        DEFINE_MESSAGE(QEvent::TabletMove),
        DEFINE_MESSAGE(QEvent::LocaleChange),
        DEFINE_MESSAGE(QEvent::LanguageChange),
        DEFINE_MESSAGE(QEvent::LayoutDirectionChange),
        DEFINE_MESSAGE(QEvent::Style),
        DEFINE_MESSAGE(QEvent::TabletPress),
        DEFINE_MESSAGE(QEvent::TabletRelease),
        DEFINE_MESSAGE(QEvent::OkRequest),
        DEFINE_MESSAGE(QEvent::HelpRequest),
        DEFINE_MESSAGE(QEvent::IconDrag),
        DEFINE_MESSAGE(QEvent::FontChange),
        DEFINE_MESSAGE(QEvent::EnabledChange),
        DEFINE_MESSAGE(QEvent::ActivationChange),
        DEFINE_MESSAGE(QEvent::StyleChange),
        DEFINE_MESSAGE(QEvent::IconTextChange),
        DEFINE_MESSAGE(QEvent::ModifiedChange),
        DEFINE_MESSAGE(QEvent::MouseTrackingChange),
        DEFINE_MESSAGE(QEvent::WindowBlocked),
        DEFINE_MESSAGE(QEvent::WindowUnblocked),
        DEFINE_MESSAGE(QEvent::WindowStateChange),
        DEFINE_MESSAGE(QEvent::ReadOnlyChange),
        DEFINE_MESSAGE(QEvent::ToolTip),
        DEFINE_MESSAGE(QEvent::WhatsThis),
        DEFINE_MESSAGE(QEvent::StatusTip),
        DEFINE_MESSAGE(QEvent::ActionChanged),
        DEFINE_MESSAGE(QEvent::ActionAdded),
        DEFINE_MESSAGE(QEvent::ActionRemoved),
        DEFINE_MESSAGE(QEvent::FileOpen),
        DEFINE_MESSAGE(QEvent::Shortcut),
        DEFINE_MESSAGE(QEvent::ShortcutOverride),
        DEFINE_MESSAGE(QEvent::WhatsThisClicked),
        DEFINE_MESSAGE(QEvent::ToolBarChange),
        DEFINE_MESSAGE(QEvent::ApplicationActivate),
        DEFINE_MESSAGE(QEvent::ApplicationActivated),
        DEFINE_MESSAGE(QEvent::ApplicationDeactivate),
        DEFINE_MESSAGE(QEvent::ApplicationDeactivated),
        DEFINE_MESSAGE(QEvent::QueryWhatsThis),
        DEFINE_MESSAGE(QEvent::EnterWhatsThisMode),
        DEFINE_MESSAGE(QEvent::LeaveWhatsThisMode),
        DEFINE_MESSAGE(QEvent::ZOrderChange),
        DEFINE_MESSAGE(QEvent::HoverEnter),
        DEFINE_MESSAGE(QEvent::HoverLeave),
        DEFINE_MESSAGE(QEvent::HoverMove),
#ifdef QT_KEYPAD_NAVIGATION
        DEFINE_MESSAGE(QEvent::EnterEditFocus),
        DEFINE_MESSAGE(QEvent::LeaveEditFocus),
#endif
        DEFINE_MESSAGE(QEvent::AcceptDropsChange),
        DEFINE_MESSAGE(QEvent::ZeroTimerEvent),
        DEFINE_MESSAGE(QEvent::GraphicsSceneMouseMove),
        DEFINE_MESSAGE(QEvent::GraphicsSceneMousePress),
        DEFINE_MESSAGE(QEvent::GraphicsSceneMouseRelease),
        DEFINE_MESSAGE(QEvent::GraphicsSceneMouseDoubleClick),
        DEFINE_MESSAGE(QEvent::GraphicsSceneContextMenu),
        DEFINE_MESSAGE(QEvent::GraphicsSceneHoverEnter),
        DEFINE_MESSAGE(QEvent::GraphicsSceneHoverMove),
        DEFINE_MESSAGE(QEvent::GraphicsSceneHoverLeave),
        DEFINE_MESSAGE(QEvent::GraphicsSceneHelp),
        DEFINE_MESSAGE(QEvent::GraphicsSceneDragEnter),
        DEFINE_MESSAGE(QEvent::GraphicsSceneDragMove),
        DEFINE_MESSAGE(QEvent::GraphicsSceneDragLeave),
        DEFINE_MESSAGE(QEvent::GraphicsSceneDrop),
        DEFINE_MESSAGE(QEvent::GraphicsSceneWheel),
        DEFINE_MESSAGE(QEvent::KeyboardLayoutChange),
        DEFINE_MESSAGE(QEvent::DynamicPropertyChange),
        DEFINE_MESSAGE(QEvent::TabletEnterProximity),
        DEFINE_MESSAGE(QEvent::TabletLeaveProximity),
        DEFINE_MESSAGE(QEvent::NonClientAreaMouseMove),
        DEFINE_MESSAGE(QEvent::NonClientAreaMouseButtonPress),
        DEFINE_MESSAGE(QEvent::NonClientAreaMouseButtonRelease),
        DEFINE_MESSAGE(QEvent::NonClientAreaMouseButtonDblClick),
        DEFINE_MESSAGE(QEvent::MacSizeChange),
        DEFINE_MESSAGE(QEvent::ContentsRectChange),
        DEFINE_MESSAGE(QEvent::MacGLWindowChange),
        DEFINE_MESSAGE(QEvent::FutureCallOut),
        DEFINE_MESSAGE(QEvent::GraphicsSceneResize ),
        DEFINE_MESSAGE(QEvent::GraphicsSceneMove ),
        DEFINE_MESSAGE(QEvent::CursorChange),
        DEFINE_MESSAGE(QEvent::ToolTipChange),
        DEFINE_MESSAGE(QEvent::NetworkReplyUpdated),
        DEFINE_MESSAGE(QEvent::GrabMouse),
        DEFINE_MESSAGE(QEvent::UngrabMouse),
        DEFINE_MESSAGE(QEvent::GrabKeyboard),
        DEFINE_MESSAGE(QEvent::UngrabKeyboard),
        DEFINE_MESSAGE(QEvent::MacGLClearDrawable),
        DEFINE_MESSAGE(QEvent::StateMachineSignal),
        DEFINE_MESSAGE(QEvent::StateMachineWrapped),
        DEFINE_MESSAGE(QEvent::TouchBegin),
        DEFINE_MESSAGE(QEvent::TouchUpdate),
        DEFINE_MESSAGE(QEvent::TouchEnd),
#ifndef QT_NO_GESTURES
        DEFINE_MESSAGE(QEvent::NativeGesture),
#endif
        DEFINE_MESSAGE(QEvent::RequestSoftwareInputPanel),
        DEFINE_MESSAGE(QEvent::CloseSoftwareInputPanel),
        DEFINE_MESSAGE(QEvent::WinIdChange),
#ifndef QT_NO_GESTURES
        DEFINE_MESSAGE(QEvent::Gesture),
        DEFINE_MESSAGE(QEvent::GestureOverride),
#endif
        DEFINE_MESSAGE(QEvent::ScrollPrepare),
        DEFINE_MESSAGE(QEvent::Scroll),
        DEFINE_MESSAGE(QEvent::Expose),
        DEFINE_MESSAGE(QEvent::InputMethodQuery),
        DEFINE_MESSAGE(QEvent::OrientationChange),
        DEFINE_MESSAGE(QEvent::TouchCancel),
        DEFINE_MESSAGE(QEvent::ThemeChange),
        DEFINE_MESSAGE(QEvent::SockClose),
        DEFINE_MESSAGE(QEvent::PlatformPanel),
        DEFINE_MESSAGE(QEvent::StyleAnimationUpdate),
        DEFINE_MESSAGE(QEvent::ApplicationStateChange),
        DEFINE_MESSAGE(QEvent::WindowChangeInternal),
        DEFINE_MESSAGE(QEvent::ScreenChangeInternal),
        DEFINE_MESSAGE(QEvent::PlatformSurface),
        DEFINE_MESSAGE(QEvent::Pointer),
        DEFINE_MESSAGE(QEvent::TabletTrackingChange)
#endif // GUIPP_QT

      };
      return message_map;
      }

    } // detail


#ifdef GUIPP_X11

  template<typename T>
  inline streamfkt position (const T& t) {
    return [&] (ostream& out) { out << " pos: (" << t.x << ", " << t.y << ")"; };
  }

  template<typename T>
  inline streamfkt size (const T& t) {
    return [&] (ostream& out) { out << " size: (" << t.width << ", " << t.height << ")"; };
  }

  template<typename T>
  inline streamfkt place (const T& t) {
    return [&] (ostream& out) { out << " area: (" << t.x << ", " << t.y << ")-(" << t.width << ", " << t.height << ")"; };
  }

  template<typename T>
  inline streamfkt count (const T& t) {
    return [&] (ostream& out) { out << " count: " << t.count; };
  }

  template<typename T>
  inline streamfkt state (const T& t) {
    return [&] (ostream& out) { out << " state: " << t.state; };
  }

  template<typename T>
  inline streamfkt window (const T& t) {
    return [&] (ostream& out) { out << " window: " << t.window; };
  }

  template<typename T>
  inline streamfkt drawable (const T& t) {
    return [&] (ostream& out) { out << " drawable: " << t.drawable; };
  }

  std::ostream& operator<< (std::ostream& out, const gui::core::event& e) {
    const char* msg = detail::get_message_map()[e.type];
    if (msg) {
      out << msg;
    } else {
      out << "Unknown Event Type " << e.type;
    }
    switch (e.type) {
      case KeyPress:
      case KeyRelease:
        out << window(e.xkey) << " keycode:" << e.xkey.keycode << state(e.xkey);
      break;
      case ButtonPress:
      case ButtonRelease:
        out << window(e.xbutton) << " button:" << e.xbutton.button << state(e.xbutton) << position(e.xbutton);
      break;
      case MotionNotify:
        out << window(e.xmotion) << state(e.xmotion) << position(e.xmotion);
      break;
      case EnterNotify:
      case LeaveNotify:
        out << window(e.xcrossing) << position(e.xcrossing);
      break;
      case FocusIn:
      case FocusOut:
        out << window(e.xfocus) << " mode:" << e.xfocus.mode << " detail:" << e.xfocus.detail;
      break;
      case KeymapNotify:
        out << window(e.xkeymap) << " key_vector:'" << e.xkeymap.key_vector << "'";
      break;
      case Expose:
        out << window(e.xexpose) << place(e.xexpose) << count(e.xexpose);
      break;
      case GraphicsExpose:
        out << drawable(e.xgraphicsexpose) << place(e.xgraphicsexpose) << count(e.xgraphicsexpose);
      break;
      case NoExpose:
        out << drawable(e.xnoexpose);
      break;
      case VisibilityNotify:
        out << window(e.xvisibility) << state(e.xvisibility);
      break;
      case CreateNotify:
        out << window(e.xcreatewindow) << place(e.xcreatewindow);
      break;
      case DestroyNotify:
        out << window(e.xdestroywindow);
      break;
      case UnmapNotify:
        out << window(e.xunmap);
      break;
      case MapNotify:
        out << window(e.xmap);
      break;
      case MapRequest:
        out << window(e.xmaprequest);
      break;
      case ReparentNotify:
        out << window(e.xreparent) << position(e.xreparent);
      break;
      case ConfigureNotify:
        out << window(e.xconfigure) << place(e.xconfigure);
      break;
      case ConfigureRequest:
        out << window(e.xconfigurerequest);
      break;
      case GravityNotify:
        out << window(e.xgravity) << position(e.xgravity);
      break;
      case ResizeRequest:
        out << window(e.xresizerequest) << size(e.xresizerequest);
      break;
      case CirculateNotify:
        out << window(e.xcirculate)<< " place:" << e.xcirculate.place;
      break;
      case CirculateRequest:
        out << window(e.xcirculaterequest) << " place:" << e.xcirculaterequest.place;
      break;
      case PropertyNotify:
        out << window(e.xproperty) << state(e.xproperty);
      break;
      case SelectionClear:
        out << window(e.xselectionclear);
      break;
      case SelectionRequest:
        out << " owner:" << e.xselectionrequest.owner;
      break;
      case SelectionNotify:
        out << " requestor:" << e.xselection.requestor;
      break;
      case ColormapNotify:
        out << window(e.xcolormap);
      break;
      case ClientMessage: {
        char* atomName = XGetAtomName(gui::core::global::get_instance(), e.xclient.message_type);
        out << " " << atomName << " " << window(e.xclient);
        XFree(atomName);
      }
      break;
      case MappingNotify:
        out << window(e.xmapping) << count(e.xmapping);
      break;
      case GenericEvent:
        out << " extension:" << e.xgeneric.extension << " evtype:" << e.xgeneric.evtype;
      break;
    }
    return out;
  }
#endif // GUIPP_X11

#ifdef GUIPP_WIN
  std::ostream& operator<< (std::ostream& out, const gui::core::event& e) {
    const char* msg = detail::get_message_map()[e.type];
    if (msg) {
      out << msg;
    } else if ((e.type >= WM_USER) && (e.type < WM_APP)) {
      out << "WM_USER+" << (e.type - WM_USER);
    } else if ((e.type >= WM_APP) && (e.type < 0xC000)) {
      out << "WM_APP+" << (e.type - WM_APP);
    } else {
      out << "0x" << std::hex << e.type;
    }
    out << " window: " << e.id;
    return out;
  }
#endif // GUIPP_WIN

#ifdef GUIPP_QT
  std::ostream& operator<< (std::ostream& out, const gui::core::event& e) {
    const char* msg = detail::get_message_map()[e.type()];
    if (msg) {
      out << msg << " [" << e.id << "]";
    } else {
      out << "0x" << std::hex << e.type() << " [" << e.id << "]";
    }
    return out;
  }
#endif // GUIPP_QT

}
