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
 * @brief     C++ API: tree control
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
      inline basic_tree<I>::basic_tree (const basic_tree& rhs)
        : super(rhs)
        , nodes(rhs.nodes)
      {
        init();
      }

      template<typename I>
      inline basic_tree<I>::basic_tree (basic_tree&& rhs)
        : super(std::move(rhs))
        , nodes(std::move(rhs.nodes))
        , open_nodes(std::move(rhs.open_nodes))
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
      auto basic_tree<I>::get_roots () const -> const std::vector<type>& {
        return roots;
      }

      template<typename I>
      void basic_tree<I>::set_roots (const std::vector<type>& new_roots) {
        roots = new_roots;
      }

      template<typename I>
      void basic_tree<I>::init () {
        set_root(type());
        super::set_drawer(basepp::bind_method(this, &basic_tree::draw_list_item));
        super::on_selection_commit([&]() {
          toggle_node(super::get_selection());
        });
        super::on_left_btn_down([&](os::key_state, const core::point & pt) {
          int idx = super::get_index_at_point(pt);
          if ((idx > -1) && (idx < size())) {
            const depth_info& i = nodes[idx];
            core::point::type x = core::point::type(i.depth * 16);
            if ((x <= pt.x()) && (x + 16 >= pt.x())) {
              toggle_node(idx);
            }
          }
        });
        super::on_any_key_down([&](os::key_state, os::key_symbol key, const std::string &){
          switch (key) {
            case win::keys::left:
            case win::keys::numpad::left: {
              int idx = super::get_selection();
              if (is_valid_idx(idx)) {
                const reference ref = get_item(idx);
                auto i = open_nodes.find(ref);
                if (i != open_nodes.end()) {
                  open_nodes.erase(i);
                  update_node_list();
                } else {
                  idx = get_parent_item_of(idx);
                  close_node(idx);
                  super::set_selection(idx, event_source::keyboard);
                }
              }
              break;
            }
            case win::keys::right:
            case win::keys::numpad::right:
              open_node(super::get_selection());
              break;
          }
        });
      }

      template<typename I>
      inline bool basic_tree<I>::is_open (const reference ref) const {
        return open_nodes.find(ref) != open_nodes.end();
      }

      template<typename I>
      inline bool basic_tree<I>::is_open (int idx) const {
        if (is_valid_idx(idx)) {
          auto i = open_nodes.find(get_item(idx));
          return i != open_nodes.end();
        }
        return false;
      }

      template<typename I>
      inline void basic_tree<I>::set_open (int idx, bool o) {
        if (is_valid_idx(idx)) {
          const reference r = get_item(idx);
          if (o) {
            open_nodes.insert(r);
          } else {
            open_nodes.erase(r);
          }
        }
      }

      template<typename I>
      inline void basic_tree<I>::open_all () {
        open_nodes.clear();
        for (const auto& root : roots) {
          open_sub(root);
        }
      }

      template<typename I>
      inline void basic_tree<I>::open_root () {
        open_nodes.clear();
        for (const auto& root : roots) {
          open_nodes.insert(tree_info::make_reference(root));
        }
      }

      template<typename I>
      inline void basic_tree<I>::open_sub (const type& n) {
        open_nodes.insert(tree_info::make_reference(n));
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
        auto i = open_nodes.find(ref);
        if (i != open_nodes.end()) {
          open_nodes.erase(i);
        } else {
          open_nodes.emplace(ref);
        }
        update_node_list();
      }

      template<typename I>
      void basic_tree<I>::open_node (const reference ref) {
        auto i = open_nodes.find(ref);
        if (i == open_nodes.end()) {
          open_nodes.emplace(ref);
          update_node_list();
        }
      }

      template<typename I>
      void basic_tree<I>::add_open_node (const reference ref) {
        auto i = open_nodes.find(ref);
        if (i == open_nodes.end()) {
          open_nodes.emplace(ref);
        }
      }

      template<typename I>
      void basic_tree<I>::close_node (const reference ref) {
        auto i = open_nodes.find(ref);
        if (i != open_nodes.end()) {
          open_nodes.erase(i);
          update_node_list();
        }
      }

      template<typename I>
      void basic_tree<I>::select_node (const reference ref) {
        for (auto i = nodes.begin(), e = nodes.end(); i != e; ++i) {
          if (i->ref == ref) {
            super::set_selection(static_cast<int>(std::distance(nodes.begin(), i)), event_source::logic);
            return;
          }
        }
      }

      template<typename I>
      void basic_tree<I>::update_node_list () {
        nodes.clear();
        for (const auto& root : roots) {
          collect_children(root);
        }
        super::set_count(nodes.size());
        super::invalidate();
      }

      template<typename I>
      void basic_tree<I>::collect_children (const type& n, std::size_t depth) {
        nodes.emplace_back(depth_info(tree_info::make_reference(n), depth));
        if (is_open(tree_info::make_reference(n))) {
          for (const auto& i : tree_info::sub_nodes(n)) {
            collect_children(i, depth + 1);
          }
        }
      }

      template<typename I>
      inline auto basic_tree<I>::get_item(int idx) const->const reference {
        return nodes[idx].ref;
      }

      template<typename I>
      inline int basic_tree<I>::size () const {
        return static_cast<int>(nodes.size());
      }

      template<typename I>
      inline bool basic_tree<I>::is_valid_idx (int idx) const {
        return (idx > -1) && (idx < size());
      }

      template<typename I>
      int basic_tree<I>::get_parent_item_of (int idx) {
        if (is_valid_idx(idx)) {
          const std::size_t depth = nodes[idx].depth;
          do {
            --idx;
          } while ((idx > -1) && (nodes[idx].depth >= depth));
          return idx;
        }
        return -1;
      }

      template<typename I>
      inline void basic_tree<I>::draw_list_item (std::size_t idx,
                                                 const draw::graphics& g,
                                                 const core::rectangle& r,
                                                 const draw::brush& b,
                                                 bool selected,
                                                 bool hilited) {
        const depth_info& i = nodes[idx];
        const type& n = tree_info::dereference(i.ref);
        tree_node_drawer<I>(g, r, b, n, i.depth,
                            tree_info::has_sub_nodes(n), is_open(i.ref),
                            selected, hilited);
      }

      template<typename I>
      inline basic_tree<I>::depth_info::depth_info (reference ref, std::size_t depth)
        : ref(ref)
        , depth(depth)
      {}

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

      inline node::node (const std::string& label, std::initializer_list<node> nodes)
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

      inline void node::add_nodes (std::initializer_list<node> nodes) {
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
        return node_range(n.begin(), n.end());
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

      inline const draw::masked_bitmap& default_node_info::icon (type const&, bool has_children, bool is_open, bool selected) {
        return standard_icon(has_children, is_open, selected);
      }

    } // tree

  } // ctrl

} // gui
