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
 * @brief     C++ API: file tree/list view
 *
 * @file
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

  } // namespace fs

  namespace win {

    namespace path_tree {

      inline sys_fs::directory_iterator path_iterator (sys_fs::path const& n) {
#ifdef WIN32
        return sys_fs::directory_iterator(n);
#endif // WIN32
#ifdef X11
        return sys_fs::directory_iterator(n, sys_fs::directory_options::skip_permission_denied);
#endif // X11
      }

      inline bool path_info::has_sub_nodes (type const& n) {
        return sys_fs::is_directory(n);
      }

      inline auto path_info::make_reference(type const & n)->reference {
        return n;
      }

      inline auto path_info::dereference(reference const & r)->type const & {
        return r;
      }

      inline std::string path_info::label (type const& n) {
        return n.filename().string();
      }

      inline const draw::masked_bitmap& path_info::icon (type const&, bool has_children, bool is_open, bool selected) {
        return tree::standard_icon(has_children, is_open, selected);
      }

    } // path_tree

    // --------------------------------------------------------------------------
    template<typename T>
    inline file_list<T>::file_list (core::size::type item_size,
                                    os::color background,
                                    bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      super::set_drawer([&] (std::size_t idx,
                             const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             bool selected,
                             bool) {
        const sys_fs::path& path = current_dir[idx];
        win::paint::text_item(g, r, b, path.filename().string(), selected, text_origin::vcenter_left);
      });
    }

    template<typename T>
    inline void file_list<T>::set_path (const sys_fs::path& dir) {
      current_dir = T::sub_nodes(dir);
      super::set_count(current_dir.size());
      super::clear_selection(event_source::logic);
      super::set_scroll_pos(0);
      super::redraw_later();
    }

    template<typename T>
    inline sys_fs::path file_list<T>::get_selected_path () const {
      int selection = super::get_selection();
      if (selection > -1) {
        return current_dir[selection];
      }
      return sys_fs::path();
    }

    // --------------------------------------------------------------------------
    template<typename T>
    inline file_column_list<T>::file_column_list (core::size::type item_size,
                                                  os::color background,
                                                  bool grab_focus)
      : super(item_size, background, grab_focus)
    {
      detail::init_file_list_layout(super::get_column_layout());
      detail::init_file_list_header(super::header);
      super::set_drawer(detail::create_file_list_row_drawer());
      super::set_data([&](std::size_t i) {
        return detail::build_file_list_row(current_dir[i], (i == super::list.get_selection()));
      }, 0);
    }

    template<typename T>
    inline void file_column_list<T>::set_path (const sys_fs::path& dir) {
      current_dir = T::sub_nodes(dir);
      super::list.set_count(current_dir.size());
      super::list.clear_selection(event_source::logic);
      super::list.set_scroll_pos(0);
      super::redraw_later();
    }

    template<typename T>
    inline sys_fs::path file_column_list<T>::get_selected_path () const {
      int selection = super::list.get_selection();
      if (selection > -1) {
        return current_dir[selection];
      }
      return sys_fs::path();
    }

  } // win

  // --------------------------------------------------------------------------

} // gui
