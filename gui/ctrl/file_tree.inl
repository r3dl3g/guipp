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
 * @brief     file tree and list control
 *
 * @license   MIT license. See accompanying file LICENSE.
 */

#pragma once


namespace gui {

  namespace fs {

    inline const sys_fs::directory_entry& filtered_iterator::operator* () const {
      return *iterator;
    }

    inline const sys_fs::directory_entry* filtered_iterator::operator-> () const {
      return &(*iterator);
    }

    inline bool operator== (const filtered_iterator& lhs,
                            const filtered_iterator& rhs) {
      return lhs.iterator == rhs.iterator;
    }

    inline bool operator!= (const filtered_iterator& lhs,
                            const filtered_iterator& rhs) {
      return lhs.iterator != rhs.iterator;
    }

    inline bool file_info::operator== (const file_info& rhs) const {
      return path == rhs.path;
    }

    inline bool file_info::operator!= (const file_info& rhs) const {
      return path != rhs.path;
    }

    inline bool file_info::operator< (const file_info& rhs) const {
      return path < rhs.path;
    }

    inline bool file_info::operator<= (const file_info& rhs) const {
      return path <= rhs.path;
    }

    inline bool file_info::operator> (const file_info& rhs) const {
      return path > rhs.path;
    }

    inline bool file_info::operator>= (const file_info& rhs) const {
      return path >= rhs.path;
    }

  } // namespace fs

  namespace ctrl {

    namespace path_tree {

      inline sys_fs::directory_iterator path_iterator (fs::file_info const& n) {
#if defined(WIN32) || defined(USE_BOOST)
        return sys_fs::directory_iterator(n.path);
#elif defined(unix) || defined(__APPLE__)
        return sys_fs::directory_iterator(n.path, sys_fs::directory_options::skip_permission_denied);
#else
# error Undefined system: sys_fs::directory_iterator path_iterator (fs::file_info const& n)
#endif // GUIPP_QT
      }

      inline bool path_info::has_sub_nodes (type const& n) {
        return n.is_directory();
      }

      inline auto path_info::make_reference(type const & n) -> reference {
        return n;
      }

      inline auto path_info::dereference(reference const & r) -> type const& {
        return r;
      }

      inline std::string path_info::label (type const& n) {
        return n.filename();
      }

      inline gui::tree::icon_drawer path_info::icon (type const&, bool has_children, bool is_open, bool selected) {
        return gui::tree::standard_icon_drawer(has_children, is_open, selected);
      }

    } // path_tree

    // --------------------------------------------------------------------------
    template<typename T, file_item_drawer D, typename S>
    inline file_list<T, D, S>::file_list (core::size::type item_size,
                                       os::color background,
                                       bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      init();
    }

    template<typename T, file_item_drawer D, typename S>
    inline file_list<T, D, S>::file_list (const file_list& rhs)
      : super(rhs)
      , current_dir(rhs.current_dir)
      , current_path(rhs.current_path)
    {
      init();
    }

    template<typename T, file_item_drawer D, typename S>
    inline file_list<T, D, S>::file_list (file_list&& rhs) noexcept
      : super(std::move(rhs))
      , current_dir(std::move(rhs.current_dir))
      , current_path(std::move(rhs.current_path))
    {
      init();
    }

    template<typename T, file_item_drawer D, typename S>
    inline void file_list<T, D, S>::init () {
      super::set_data(indirect_list_data<fs::file_info, D, std::vector<fs::file_info>>(current_dir));
    }

    template<typename T, file_item_drawer D, typename S>
    inline void file_list<T, D, S>::set_path (const sys_fs::path& dir, std::function<fs::filter_fn> filter) {
      current_path = dir;
      current_dir = T::sub_nodes(current_path, filter);
      super::clear_selection(event_source::logic);
      super::set_scroll_pos(core::point::zero);
      super::invalidate();
    }

    template<typename T, file_item_drawer D, typename S>
    inline sys_fs::path file_list<T, D, S>::get_selected_path () const {
      int selection = super::get_selection().get_first_index();
      if (selection > -1) {
        return current_dir[selection].path;
      }
      return current_path;
    }

    template<typename T, file_item_drawer D, typename S>
    const sys_fs::path& file_list<T, D, S>::get_current_path () const {
      return current_path;
    }

    // --------------------------------------------------------------------------
    template<typename T, typename S>
    inline file_column_list<T, S>::file_column_list (core::size::type item_size,
                                                  os::color background,
                                                  bool grab_focus)
      : super(item_size, background, grab_focus)
      , order(sort_order::none)
    {
      init();
    }

    template<typename T, typename S>
    inline file_column_list<T, S>::file_column_list (const file_column_list& rhs)
      : super(rhs)
      , mouse_down_point(rhs.mouse_down_point)
      , order(rhs.order)
    {
      init();
    }

    template<typename T, typename S>
    inline file_column_list<T, S>::file_column_list (file_column_list&& rhs) noexcept
      : super(std::move(rhs))
      , mouse_down_point(std::move(rhs.mouse_down_point))
      , order(std::move(rhs.order))
    {
      init();
    }

    template<typename T, typename S>
    void file_column_list<T, S>::init () {
      detail::init_file_list_layout(super::get_column_layout(), super::list.view.get_item_size());
      init_file_list_header(super::header);
      super::set_data(detail::file_list_row_data(current_dir, super::list));
      super::header.on_left_btn_down(util::bind_method(this, &file_column_list::handle_header_mouse_down));
      super::header.on_left_btn_up(util::bind_method(this, &file_column_list::handle_header_mouse_up));
    }

    template<typename T, typename S>
    inline void file_column_list<T, S>::handle_header_mouse_down (os::key_state, const core::native_point& npt) {
      mouse_down_point = super::surface_to_client(npt);
    }

    template<typename T, typename S>
    inline void file_column_list<T, S>::handle_header_mouse_up (os::key_state, const core::native_point& npt) {
      auto pt = super::surface_to_client(npt);
      if (mouse_down_point == pt) {
        int i = super::get_column_layout().index_at(pt.x());
        const auto idx = static_cast<unsigned short>(order);
        if (idx == i * 2) { // down
          sort_by(sort_order(i * 2 - 1)); // up
        } else {
          sort_by(sort_order(i * 2));     // down
        }
      }
      mouse_down_point = core::point::undefined;
    }

    template<typename T, typename S>
    inline void file_column_list<T, S>::set_path (const sys_fs::path& dir, std::function<fs::filter_fn> filter) {
      current_dir = T::sub_nodes(dir, filter);
      super::list->clear_selection(event_source::logic);
      super::list->set_scroll_pos(core::point::zero);
      if (order == sort_order::none) {
        order = sort_order::name_up;
      }
      if (order != sort_order::name_up) {
        sort_list_by(current_dir, order);
      }
      super::list.invalidate();
      super::header.invalidate();
    }

    template<typename T, typename S>
    inline sys_fs::path file_column_list<T, S>::get_selected_path () const {
      if (super::list->has_selection()) {
        return current_dir[super::list->get_selection().get_first_index()].path;
      }
      return sys_fs::path();
    }

    template<typename T, typename S>
    void file_column_list<T, S>::sort_by (sort_order new_order) {
      if (order != new_order) {
        order = new_order;
        sort_list_by(current_dir, order);
        super::list.invalidate();
        super::header.invalidate();
      }
    }

    template<typename T, typename S>
    sort_order file_column_list<T, S>::get_sort_order () const {
      return order;
    }

    template<typename T, typename S>
    void file_column_list<T, S>::init_file_list_header (column_list_header<layout::weight_column_list_layout>& header) {
      header.set_cell_drawer([&] (std::size_t i, draw::graphics& g, const core::rectangle& r, const draw::brush& background) {
        static std::string title[] = {"", "Name", "Size", "Changed"};
        g.fill(draw::rectangle(r), background);
        draw::frame::raised_relief(g, r);
        g.text(draw::text_box(title[i], r, text_origin_t::center), draw::font::system(), color::windowTextColor());
        if (order != sort_order::none) {
          core::rectangle s = r.right_width(r.height()).shrinked({7, 8});
          const auto idx = static_cast<unsigned short>(order);
          if (idx == i * 2) {
            draw_arrow_down(g, s, color::black);
          } else if (idx == i * 2 - 1) {
            draw_arrow_up(g, s, color::black);
          }
        }
      });
    }


  } // ctrl

  // --------------------------------------------------------------------------

} // gui
