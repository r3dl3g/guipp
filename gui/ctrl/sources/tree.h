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
#include "list.h"


namespace gui {

  namespace win {

    namespace paint {

      // --------------------------------------------------------------------------
      void tree_button (const draw::graphics& graph,
                        const core::rectangle& area,
                        bool is_open);

      void tree_node (const draw::graphics& graph,
                      const core::rectangle& area,
                      const draw::brush& background,
                      std::size_t depth,
                      const std::string& label,
                      const draw::masked_bitmap& icon,
                      bool has_children,
                      bool is_open,
                      bool selected,
                      bool hilited);

    } // paint

    namespace tree {

      // --------------------------------------------------------------------------
      const draw::masked_bitmap& open_folder_icon (bool selected);
      const draw::masked_bitmap& closed_folder_icon (bool selected);
      const draw::masked_bitmap& file_icon (bool selected);

      inline const draw::masked_bitmap& standard_icon (bool has_children, bool is_open, bool selected) {
        if (has_children) {
          return is_open ? open_folder_icon(selected) : closed_folder_icon(selected);
        }
        return file_icon(selected);
      }

      // --------------------------------------------------------------------------
      template<typename I>
      void tree_node_drawer (const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             const typename I::type& t,
                             std::size_t depth,
                             bool has_children,
                             bool is_open,
                             bool selected,
                             bool hilited) {
        paint::tree_node(g, r, b, depth,
                         I::label(t),
                         I::icon(t, has_children, is_open, selected),
                         has_children, is_open, selected, hilited);
      }

      // --------------------------------------------------------------------------
      template<typename I, int S = 20, os::color B = color::white>
      class tree : public vlist<S, B> {
      public:
        typedef vlist<S, B> super;

        typedef I tree_info;
        typedef typename tree_info::type type;
        typedef typename tree_info::reference reference;

        type root;

        tree () {
          super::set_drawer(core::bind_method(this, &tree::draw_list_item));
          super::register_event_handler(REGISTER_FUNCTION, selection_commit_event([&]() {
            toggle_node(super::get_selection());
          }));
          super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state,
                                                                                   const core::point& pt) {
            int idx = super::get_index_at_point(pt);
            if ((idx > -1) && (idx < size())) {
              const depth_info& i = nodes[idx];
              core::point_type x = core::point_type(i.depth * 16);
              if ((x <= pt.x()) && (x + 16 >= pt.x())) {
                toggle_node(idx);
              }
            }
          }));
          super::register_event_handler(REGISTER_FUNCTION, key_down_event([&](os::key_state,
                                                                              os::key_symbol key,
                                                                              const std::string&){
            switch (key) {
              case keys::left:
              case keys::numpad::left: {
                int idx = super::get_selection();
                if (is_valid(idx)) {
                  const reference ref = get_item(idx);
                  auto i = open_nodes.find(ref);
                  if (i != open_nodes.end()) {
                    open_nodes.erase(i);
                    update_node_list();
                  } else {
                    idx = get_parent_item_of(idx);
                    close_node(idx);
                    super::set_selection(idx);
                  }
                }
                break;
              }
              case keys::right:
              case keys::numpad::right:
                open_node(super::get_selection());
                break;
            }
          }));
        }

        bool is_open (const reference ref) const {
          return open_nodes.find(ref) != open_nodes.end();
        }

        bool is_open (int idx) const {
          if (is_valid(idx)) {
            const reference ref = get_item(idx);
            auto i = open_nodes.find(ref);
            return i != open_nodes.end();
          }
          return false;
        }

        void set_open (int idx, bool o) {
          if (is_valid(idx)) {
            const reference r = get_item(idx);
            if (o) {
              open_nodes.insert(r);
            } else {
              open_nodes.erase(r);
            }
          }
        }

        void open_all () {
          open_nodes.clear();
          open_sub(root);
        }

        void open_root () {
          open_nodes.clear();
          open_nodes.insert(tree_info::make_reference(root));
        }

        void open_sub (const type& n) {
          open_nodes.insert(tree_info::make_reference(n));
          for (const auto& i : tree_info::sub_nodes(n)) {
            open_sub(i);
          }
        }

        void toggle_node (int idx) {
          if (is_valid(idx)) {
            const reference ref = get_item(idx);
            auto i = open_nodes.find(ref);
            if (i != open_nodes.end()) {
              open_nodes.erase(i);
            } else {
              open_nodes.insert(ref);
            }
            update_node_list();
          }
        }

        void open_node (int idx) {
          if (is_valid(idx)) {
            const reference ref = get_item(idx);
            auto i = open_nodes.find(ref);
            if (i == open_nodes.end()) {
              open_nodes.insert(ref);
              update_node_list();
            }
          }
        }

        void close_node (int idx) {
          if (is_valid(idx)) {
            const reference ref = get_item(idx);
            auto i = open_nodes.find(ref);
            if (i != open_nodes.end()) {
              open_nodes.erase(i);
              update_node_list();
            }
          }
        }

        void update_node_list () {
          nodes.clear();
          collect_children(root);
          super::set_count(nodes.size());
          super::redraw_later();
        }

        void collect_children (const type& n, std::size_t depth = 0) {
          nodes.emplace_back(depth_info(tree_info::make_reference(n), depth));
          if (is_open(tree_info::make_reference(n))) {
            for (const auto& i : tree_info::sub_nodes(n)) {
              collect_children(i, depth + 1);
            }
          }
        }

        inline const reference get_item (int idx) const {
          return nodes[idx].ref;
        }

        inline int size () const {
          return static_cast<int>(nodes.size());
        }

        inline bool is_valid (int idx) const {
          return (idx > -1) && (idx < size());
        }

        int get_parent_item_of (int idx) {
          if (is_valid(idx)) {
            const std::size_t depth = nodes[idx].depth;
            do {
              --idx;
            } while ((idx > -1) && (nodes[idx].depth >= depth));
            return idx;
          }
          return -1;
        }

      protected:
        void draw_list_item (std::size_t idx,
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

      private:
        struct depth_info {
          depth_info (reference ref, std::size_t depth)
            : ref(ref)
            , depth(depth)
          {}

          reference ref;
          std::size_t depth;
        };

        typedef std::vector<depth_info> node_list;
        node_list nodes;

        typedef std::set<reference> node_set;
        node_set open_nodes;
      };

      // --------------------------------------------------------------------------
      struct node {
        typedef std::vector<node> node_list;
        typedef node_list::const_iterator iterator;

        node (const std::string& label = std::string())
          : label(label)
        {}

        node (const node& rhs)
          : label(rhs.label)
          , sub_nodes(rhs.sub_nodes)
        {}

        node (node&& rhs) {
          std::swap(label, rhs.label);
          std::swap(sub_nodes, rhs.sub_nodes);
        }

        node (std::string&& label)
          : label(std::move(label))
        {}

        node (const std::string& label, std::initializer_list<node> nodes)
          : label(label)
        {
          add_nodes(nodes);
        }

        void add_node (const node& node) {
          sub_nodes.emplace_back(node);
        }

        void add_node (node&& node) {
          sub_nodes.emplace_back(std::move(node));
        }

        void add_nodes (std::initializer_list<node> nodes) {
          for (auto& i : nodes) {
            add_node(i);
          }
        }

        const node_list& nodes () const {
          return sub_nodes;
        }

        iterator begin () const {
          return sub_nodes.begin();
        }

        iterator end () const {
          return sub_nodes.end();
        }

        std::string label;
        node_list sub_nodes;
      };

      // --------------------------------------------------------------------------
      template<typename I>
      struct range {
        typedef I iterator;

        range (const iterator& b, const iterator& e)
          : start(b)
          , finish(e)
        {}

        range (iterator&& b, iterator&& e) {
          std::swap(start, b);
          std::swap(finish, e);
        }

        range (const range& rhs)
          : start(rhs.start)
          , finish(rhs.finish)
        {}

        range (range&& rhs) {
          std::swap(start, rhs.start);
          std::swap(finish, rhs.finish);
        }

        iterator begin () const { return start; }
        iterator end () const { return finish; }

      private:
        iterator start;
        iterator finish;
      };

      // --------------------------------------------------------------------------
      struct default_node_info {
        typedef node type;
        typedef node::iterator iterator;
        typedef const node* reference;
        typedef range<iterator> node_range;

        static bool has_sub_nodes (const node& n) {
          return !n.nodes().empty();
        }

        static node_range sub_nodes (node const& n) {
          return node_range(n.begin(), n.end());
        }

        static reference make_reference (node const& n) {
          return &n;
        }

        static type const& dereference (reference const& r) {
          return *r;
        }

        static std::string label (node const& n) {
          return n.label;
        }

        static const draw::masked_bitmap& icon (type const&, bool has_children, bool is_open, bool selected) {
          return standard_icon(has_children, is_open, selected);
        }
      };

      // --------------------------------------------------------------------------

    } // tree

    template<int S = 20, os::color B = color::white>
    using tree_view = tree::tree<tree::default_node_info, S, B>;

  } // win

} // gui
