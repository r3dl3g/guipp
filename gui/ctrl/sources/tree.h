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

      void tree_button (const draw::graphics& graph,
                        const core::rectangle& area,
                        bool is_open);

      void tree_node (const draw::graphics& graph,
                      const core::rectangle& area,
                      const draw::brush& background,
                      int depth,
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

      // --------------------------------------------------------------------------
      template<typename T>
      bool has_sub_nodes (const T& n);

      template<typename T>
      struct node_info {
        typedef T type;
        typedef T iterator;
        typedef T reference;
      };

      template<typename T>
      typename node_info<T>::iterator begin (T const&);

      template<typename T>
      typename node_info<T>::iterator end (T const&);

      template<typename T>
      typename node_info<T>::reference make_reference (T const&);

      template<typename T>
      typename node_info<T>::type const& dereference (typename node_info<T>::reference const&);

      template<typename T>
      std::string get_label (T const& n);

      template<typename T>
      const draw::masked_bitmap& get_icon (T const&, bool has_children, bool is_open, bool selected) {
        if (has_children) {
          return is_open ? open_folder_icon(selected) : closed_folder_icon(selected);
        }
        return file_icon(selected);
      }

      // --------------------------------------------------------------------------
      template<typename T>
      void tree_node_drawer (const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             const T& t,
                             int depth,
                             bool has_children,
                             bool is_open,
                             bool selected,
                             bool hilited) {
        paint::tree_node(g, r, b, depth,
                         get_label<T>(t),
                         get_icon<T>(t, has_children, is_open, selected),
                         has_children, is_open, selected, hilited);
      }

      // --------------------------------------------------------------------------
      template<typename T, int S = 20, os::color B = color::white>
      class tree : public vlist<S, B> {
      public:
        typedef vlist<S, B> super;

        typedef typename node_info<T>::type type;
        typedef typename node_info<T>::reference reference;

        type root;

        tree () {
          super::set_drawer(core::bind_method(this, &tree::draw_list_item));
          super::register_event_handler(selection_commit_event([&]() {
            toggle_node(super::get_selection());
          }));
          super::register_event_handler(left_btn_down_event([&](os::key_state, const core::point& pt) {
            int idx = super::get_index_at_point(pt);
            if (idx > -1) {
              const depth_info& i = nodes[idx];
              core::point_type x = core::point_type(i.depth * 16);
              if ((x <= pt.x()) && (x + 16 >= pt.x())) {
                toggle_node(idx);
              }
            }
          }));
        }

        bool is_open (const reference n) {
          return open_nodes.find(n) != open_nodes.end();
        }

        void set_open (const reference n, bool o) {
          if (o) {
            open_nodes.insert(n);
          } else {
            open_nodes.erase(n);
          }
        }

        void open_all () {
          open_nodes.clear();
          open_sub(root);
        }

        void open_sub (const type& n) {
          open_nodes.insert(make_reference<T>(n));
          for (auto i = begin(n), e = end(n); i != e; ++i) {
            open_sub(*i);
          }
        }

        void toggle_node (int idx) {
          if (idx > -1) {
            const reference node = nodes[idx].node;
            set_open(node, !is_open(node));
            update_node_list();
          }
        }

        void update_node_list () {
          nodes.clear();
          collect_children(root);
          super::set_count(nodes.size());
          super::redraw_later();
        }

        void collect_children (const type& n, int depth = 0) {
          nodes.emplace_back(depth_info(make_reference<T>(n), depth));
          if (is_open(make_reference<T>(n))) {
            for (auto i = begin(n), e = end(n); i != e; ++i) {
              collect_children(*i, depth + 1);
            }
          }
        }

        void draw_list_item (int idx,
                             const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             bool selected,
                             bool hilited) {
          const depth_info& i = nodes[idx];
          const type& n = dereference<T>(i.node);
          tree_node_drawer<type>(g, r, b, n, i.depth,
                                 has_sub_nodes(n), is_open(i.node),
                                 selected, hilited);
        }

      private:
        struct depth_info {
          depth_info(reference node, int depth)
            : node(node)
            , depth(depth)
          {}

          reference node;
          int depth;
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
      template<>
      inline bool has_sub_nodes<node> (const node& n) {
        return !n.nodes().empty();
      }

      template<>
      struct node_info<node> {
        typedef node type;
        typedef node::iterator iterator;
        typedef const node* reference;
      };

      template<>
      inline node::iterator begin<node> (node const& n) {
        return n.begin();
      }

      template<>
      inline node::iterator end<node> (node const& n) {
        return n.end();
      }

      template<>
      inline node_info<node>::reference make_reference<node> (node const& n) {
        return &n;
      }

      template<>
      inline const node& dereference<node> (node_info<node>::reference const& r) {
        return *r;
      }

      template<>
      inline std::string get_label<node> (node const& n) {
        return n.label;
      }

    } // tree

    template<int S = 20, os::color B = color::white>
    using tree_view = tree::tree<tree::node, S, B>;

  } // win

} // gui
