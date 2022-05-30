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
 * @brief     tree view control
 *
 * @license   MIT license. See accompanying file LICENSE.
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
#include <gui/ctrl/list.h>


namespace gui {

  namespace ctrl {

    namespace tree {

      template<typename I>
      basic_tree<I>::basic_tree (core::size::type item_size,
                                 os::color background,
                                 bool grab_focus)
        : super(item_size, background, grab_focus)
      {
        init();
      }

      template<typename I>
      inline basic_tree<I>::basic_tree (basic_tree&& rhs) noexcept
        : super(std::move(rhs))
        , data(std::move(rhs.data))
      {
        init();
      }

      template<typename I>
      void basic_tree<I>::set_root (type&& root) {
        roots.clear();
        roots.emplace_back(root);
      }

      template<typename I>
      void basic_tree<I>::set_root (const type& root) {
        roots.clear();
        roots.emplace_back(root);
      }

      template<typename I>
      auto basic_tree<I>::get_root () const -> const type& {
        return roots[0];
      }

      template<typename I>
      void basic_tree<I>::add_root (const type& root) {
        roots.emplace_back(root);
      }

      template<typename I>
      void basic_tree<I>::add_root (type&& root) {
        roots.emplace_back(root);
      }

      template<typename I>
      auto basic_tree<I>::get_roots () const -> const roots_list_type& {
        return roots;
      }

      template<typename I>
      void basic_tree<I>::set_roots (const roots_list_type& new_roots) {
        roots = new_roots;
      }

      template<typename I>
      void basic_tree<I>::init () {
        set_root(type());
        super::set_data(data);
        super::on_selection_commit([&]() {
          toggle_node(super::get_selection().get_first_index());
        });
        super::on_left_btn_down([&](os::key_state, const core::native_point& pt_) {
          core::point pt = super::surface_to_client(pt_);
          int idx = super::get_index_at_point(pt);
          if ((idx > -1) && (idx < size())) {
            const depth_info& i = data.nodes[idx];
            core::point::type x = core::point::type(i.depth * super::get_item_size());
            if ((x <= pt.x()) && (x + super::get_item_size() >= pt.x())) {
              toggle_node(idx);
            }
          }
        });
        super::on_any_key_down([&](os::key_state state, os::key_symbol key, const std::string &){
          if (state != core::state::none) {
            return;
          }
          switch (key) {
            case core::keys::left:
            case core::keys::numpad::left: {
              int idx = super::get_selection().get_first_index();
              if (is_valid_idx(idx)) {
                const reference ref = get_item(idx);
                auto i = data.open_nodes.find(ref);
                if (i != data.open_nodes.end()) {
                  data.open_nodes.erase(i);
                  update_node_list();
                } else {
                  idx = get_parent_item_of(idx);
                  close_node(idx);
                  super::set_selection(idx, event_source::keyboard);
                }
              }
              break;
            }
            case core::keys::right:
            case core::keys::numpad::right:
              open_node(super::get_selection().get_first_index());
              break;
          }
        });
      }

      template<typename I>
      inline bool basic_tree<I>::is_open (const reference ref) const {
        return data.is_open(ref);
      }

      template<typename I>
      inline bool basic_tree<I>::is_open (int idx) const {
        if (is_valid_idx(idx)) {
          auto i = data.open_nodes.find(get_item(idx));
          return i != data.open_nodes.end();
        }
        return false;
      }

      template<typename I>
      inline void basic_tree<I>::set_open (int idx, bool o) {
        if (is_valid_idx(idx)) {
          const reference r = get_item(idx);
          if (o) {
            data.open_nodes.insert(r);
          } else {
            data.open_nodes.erase(r);
          }
        }
      }

      template<typename I>
      inline void basic_tree<I>::open_all () {
        data.open_nodes.clear();
        for (const auto& root : roots) {
          open_sub(root);
        }
      }

      template<typename I>
      inline void basic_tree<I>::open_root () {
        data.open_nodes.clear();
        for (const auto& root : roots) {
          data.open_nodes.insert(tree_info::make_reference(root));
        }
      }

      template<typename I>
      inline void basic_tree<I>::open_sub (const type& n) {
        data.open_nodes.insert(tree_info::make_reference(n));
        for (const auto& i : tree_info::sub_nodes(n)) {
          open_sub(i);
        }
      }

      template<typename I>
      void basic_tree<I>::toggle_node (int idx) {
        if (is_valid_idx(idx)) {
          const reference ref = get_item(idx);
          toggle_node(ref);
        }
      }

      template<typename I>
      void basic_tree<I>::open_node (int idx) {
        if (is_valid_idx(idx)) {
          const reference ref = get_item(idx);
          open_node(ref);
        }
      }

      template<typename I>
      void basic_tree<I>::close_node (int idx) {
        if (is_valid_idx(idx)) {
          const reference ref = get_item(idx);
          close_node(ref);
        }
      }

      template<typename I>
      void basic_tree<I>::toggle_node (const reference ref) {
        auto i = data.open_nodes.find(ref);
        if (i != data.open_nodes.end()) {
          data.open_nodes.erase(i);
        } else {
          data.open_nodes.emplace(ref);
        }
        update_node_list();
      }

      template<typename I>
      void basic_tree<I>::open_node (const reference ref) {
        auto i = data.open_nodes.find(ref);
        if (i == data.open_nodes.end()) {
          data.open_nodes.emplace(ref);
          update_node_list();
        }
      }

      template<typename I>
      void basic_tree<I>::add_open_node (const reference ref) {
        auto i = data.open_nodes.find(ref);
        if (i == data.open_nodes.end()) {
          data.open_nodes.emplace(ref);
        }
      }

      template<typename I>
      void basic_tree<I>::close_node (const reference ref) {
        auto i = data.open_nodes.find(ref);
        if (i != data.open_nodes.end()) {
          data.open_nodes.erase(i);
          update_node_list();
        }
      }

      template<typename I>
      void basic_tree<I>::select_node (const reference ref) {
        for (auto i = data.nodes.begin(), e = data.nodes.end(); i != e; ++i) {
          if (i->ref == ref) {
            super::set_selection(static_cast<int>(std::distance(data.nodes.begin(), i)), event_source::logic);
            return;
          }
        }
      }

      template<typename I>
      void basic_tree<I>::update_node_list () {
        data.nodes.clear();
        for (const auto& root : roots) {
          collect_children(root);
        }
        super::set_data(data);
        super::invalidate();
        super::notify_content_changed();
      }

      template<typename I>
      void basic_tree<I>::collect_children (const type& n, std::size_t depth) {
        auto ref = tree_info::make_reference(n);
        data.nodes.emplace_back(depth_info(ref, depth));
        if (is_open(ref)) {
          for (const auto& i : tree_info::sub_nodes(n)) {
            collect_children(i, depth + 1);
          }
        }
      }

      template<typename I>
      inline auto basic_tree<I>::get_item (int idx) const -> reference {
        return data.nodes[idx].ref;
      }

      template<typename I>
      inline int basic_tree<I>::size () const {
        return static_cast<int>(data.size());
      }

      template<typename I>
      inline bool basic_tree<I>::is_valid_idx (int idx) const {
        return (idx > -1) && (idx < size());
      }

      template<typename I>
      int basic_tree<I>::get_parent_item_of (int idx) {
        if (is_valid_idx(idx)) {
          const std::size_t depth = data.nodes[idx].depth;
          do {
            --idx;
          } while ((idx > -1) && (data.nodes[idx].depth >= depth));
          return idx;
        }
        return -1;
      }

      template<typename I>
      inline basic_tree<I>::depth_info::depth_info (reference ref, std::size_t depth)
        : ref(ref)
        , depth(depth)
      {}

      template<typename I>
      std::size_t basic_tree<I>::tree_data::size () const {
        return nodes.size();
      }

      template<typename I>
      void basic_tree<I>::tree_data::draw_at (std::size_t idx,
                                              draw::graphics& g,
                                              const core::rectangle& r,
                                              const draw::brush& b,
                                              item_state state) const {
        const depth_info& i = nodes[idx];
        const type& n = tree_info::dereference(i.ref);
        tree_node_drawer<I>(g, r, b, n, i.depth,
                            tree_info::has_sub_nodes(n), is_open(i.ref),
                            state);
      }

      template<typename I>
      inline bool basic_tree<I>::tree_data::is_open (const reference ref) const {
        return open_nodes.find(ref) != open_nodes.end();
      }

      // --------------------------------------------------------------------------
      inline node::node (const std::string& label)
        : label(label)
      {}

      inline node::node (const char* label)
        : label(label)
      {}

      inline node::node (std::string&& label)
        : label(std::move(label))
      {}

      inline node::node (const std::string& label, const node_list& nodes)
        : label(label)
      {
        add_nodes(nodes);
      }

      inline void node::add_node (const node& node) {
        sub_nodes.emplace_back(node);
      }

      inline void node::add_node (node&& node) {
        sub_nodes.emplace_back(std::move(node));
      }

      inline void node::add_nodes (const node_list& nodes) {
        for (auto& i : nodes) {
          add_node(i);
        }
      }

      inline auto node::nodes() const->const node_list& {
        return sub_nodes;
      }

      inline auto node::begin() const->iterator {
        return sub_nodes.begin();
      }

      inline auto node::end() const->iterator {
        return sub_nodes.end();
      }

      // --------------------------------------------------------------------------
      inline bool default_node_info::has_sub_nodes (const node& n) {
        return !n.nodes().empty();
      }

      inline auto default_node_info::sub_nodes(node const & n)->node_range {
        return {n.begin(), n.end()};
      }

      inline auto default_node_info::make_reference(node const & n)->reference {
        return &n;
      }

      inline auto default_node_info::dereference(reference const & r)->type const & {
        return *r;
      }

      inline std::string default_node_info::label (node const& n) {
        return n.label;
      }

      inline gui::tree::icon_drawer default_node_info::icon (type const&, bool has_children, bool is_open, bool selected) {
        return gui::tree::standard_icon_drawer(has_children, is_open, selected);
      }

    } // tree

  } // ctrl

} // gui
