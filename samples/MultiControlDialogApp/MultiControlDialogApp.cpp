

#include <ctrl/std_dialogs.h>

//-----------------------------------------------------------------------------
namespace controls {

  //-----------------------------------------------------------------------------
  template<typename L, typename E, int N, int I, typename T>
  struct add {
    static void to (L& layout, std::array<E, N>& labels, T& controls) {
      layout.add(layout::lay(labels[I]));
      layout.add(layout::lay(std::get<I>(controls)));
      add<L, E, N, I + 1, T>::to(layout, labels, controls);
    }
  };

  template<typename L, typename E, int N, typename T>
  struct add<L, E, N, N, T> {
    static void to (L&, std::array<E, N>&, T&) {
    }
  };

  template<typename E>
  using init_function = std::function<void(E&e)>;

  //-----------------------------------------------------------------------------
  template<typename E, int N, int I, typename T, typename Initials>
  struct create {
    static void in (container& main, std::array<E, N>& labels, const std::vector<std::string>& message, T& ctrls, const Initials& initials) {
      labels[I].create(main, message[I]);
      std::get<I>(ctrls).create(main);
      std::get<I>(initials)(std::get<I>(ctrls));
      create<E, N, I + 1, T, Initials>::in(main, labels, message, ctrls, initials);
    }
  };

  template<typename E, int N, typename T, typename Initials>
  struct create<E, N, N, T, Initials> {
    static void in (container&, std::array<E, N>&, const std::vector<std::string>&, T&, const Initials&) {
    }
  };
}

//-----------------------------------------------------------------------------
template<typename T>
class /*GUIPP_CTRL_EXPORT*/ multi_control_dialog :
    public standard_dialog<win::group_window<layout::vertical_lineup<20, 15, 2>>> {
public:
  typedef layout::vertical_lineup<20, 15, 2> Layout;
  typedef win::group_window<Layout> content_view_type;
  typedef standard_dialog<content_view_type> super;
  using label_t = basic_label<text_origin_t::bottom_left,
                              draw::frame::no_frame>;

  using Controls = typename T::Controls;
  using Types = typename T::Types;
  using Initials = typename T::Initials;

  static constexpr size_t N = std::tuple_size<Types>();

  typedef void (action) (const Controls&);

  multi_control_dialog () {
    controls::add<Layout, label_t, N, 0, Controls>::to(content_view.get_layout(), labels, controls);
  }

  void create (win::overlapped_window& parent,
               const std::string& title,
               const std::vector<std::string>& message,
               const Initials& initial,
               const std::string& ok_label,
               const std::string& cancel_label,
               const core::rectangle& rect,
               std::function<action> action) {
    super::create(parent, title, rect, [&, action] (win::overlapped_window& dlg, int i) {
      if (i == 1) {
        action(controls);
      }
    }, {cancel_label, ok_label});
    controls::create<label_t, N, 0, Controls, Initials>::in(content_view, labels, message, controls, initial);
  }

  static void ask (win::overlapped_window& parent,
                   const std::string& title,
                   const std::vector<std::string>& message,
                   const Initials& initial,
                   const std::string& ok_label,
                   const std::string& cancel_label,
                   std::function<action> action) {
    multi_control_dialog dialog;
    dialog.create(parent, title, message, initial, ok_label, cancel_label,
                  core::rectangle(300, 200, 400, 85 + N * 44), action);
    dialog.show(parent);
  }

  std::array<label_t, N> labels;
  Controls controls;

};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::ctrl;
  using namespace gui::win;

  typedef controls::init_function<edit_left&> edit_init;
  typedef controls::init_function<drop_down_list&> drop_init;

  struct event_traits {
    typedef std::tuple<edit_left, edit_left, edit_left, drop_down_list> Controls;
    typedef std::tuple<edit_init, edit_init, edit_init, drop_init> Initials;
    typedef std::tuple<std::string, time_point, time_point, std::string> Types;
  };

  std::set<std::string> comments;
  for (const auto& ev : events) {
    comments.insert(ev->comment);
  }
  std::vector<std::string> data(comments.begin(), comments.end());

  event_traits::Initials inits = event_traits::Initials(
                                   [e] (edit_left& ctrl) {
                                     ctrl.set_text("Text");
                                   },
                                   [e] (edit_left& ctrl) {
                                     ctrl.set_text(ostreamfmt(e->begin));
                                   },
                                   [e] (edit_left& ctrl) {
                                     ctrl.set_text(ostreamfmt(e->end));
                                   },
                                   [&] (drop_down_list& ctrl) {
                                     ctrl.set_data(data);
                                   }
                                 );

  multi_control_dialog<event_traits>::ask(*this, "Edit event",
                                          std::vector<std::string>({"Id:", "Begin:", "End:", "Comment:"}),
                                          inits,
                                          "Okay", "Cancel",
                                          [&] (const event_traits::Controls& ctrls) {
    auto id = std::get<0>(ctrls).get_text();
    auto begin = std::get<1>(ctrls).get_text();
    auto end = std::get<2>(ctrls).get_text();
    auto comment = data[std::get<3>(ctrls).get_selection()];

    message_dialog::show(*this, "Info", ostreamfmt("Id:" << id << ",\nbegin:" << begin << ",\nend:" << end << ",\ncomment:" << comment), "Ok");
  });

  return 0;//run_main_loop();
}
