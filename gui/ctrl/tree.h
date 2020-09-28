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

    namespace paint {

      // --------------------------------------------------------------------------
      GUIPP_CTRL_EXPORT void tree_button (const draw::graphics& graph,
                                          const core::rectangle& area,
                                          bool is_open);

      GUIPP_CTRL_EXPORT void tree_node (const draw::graphics& graph,
                                        const core::rectangle& area,
                                        const draw::brush& background,
                                        std::size_t depth,
                                        const std::string& label,
                                        const draw::pixmap& icon,
                                        bool has_children,
                                        bool is_open,
                                        item_state state);

    } // paint

    namespace tree {

      // --------------------------------------------------------------------------
      GUIPP_CTRL_EXPORT const draw::pixmap& open_folder_icon (bool selected);
      GUIPP_CTRL_EXPORT const draw::pixmap& closed_folder_icon (bool selected);
      GUIPP_CTRL_EXPORT const draw::pixmap& file_icon (bool selected);

      GUIPP_CTRL_EXPORT const draw::pixmap& standard_icon (bool has_children, bool is_open, bool selected);

      // --------------------------------------------------------------------------
      template<typename I>
      void tree_node_drawer (const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             const typename I::type& t,
                             std::size_t depth,
                             bool has_children,
                             bool is_open,
                             item_state state) {
        paint::tree_node(g, r, b, depth,
                         I::label(t),
                         I::icon(t, has_children, is_open, item_state::selected == state),
                         has_children, is_open, state);
      }

      // --------------------------------------------------------------------------
      template<typename I>
      class basic_tree : public vertical_list {
      public:
        typedef vertical_list super;

        typedef I tree_info;
        typedef typename tree_info::type type;
        typedef typename tree_info::reference reference;
        typedef std::vector<type> roots_list;

        void set_root (const type& root);
        void set_root (type&& root);

        const type& get_root () const;

        void add_root (const type& root);
        void add_root (type&& root);

        const roots_list& get_roots () const;
        void set_roots (const roots_list&);

        basic_tree (core::size::type item_size = list_defaults<>::item_size,
                    os::color background = color::white,
                    bool grab_focus = true);
        basic_tree (basic_tree&& rhs);

        void init ();

        bool is_open (const reference) const;
        bool is_open (int idx) const;
        void set_open (int idx, bool o);

        void open_all ();
        void open_root ();
        void open_sub (const type&);

        void toggle_node (int idx);
        void open_node (int idx);
        void close_node (int idx);

        void toggle_node (const reference);
        void open_node (const reference);
        void close_node (const reference);
        void add_open_node (const reference);

        void select_node (const reference);

        void update_node_list ();
        void collect_children (const type& n, std::size_t depth = 0);

        bool is_valid_idx (int idx) const;

        const reference get_item (int idx) const;
        int get_parent_item_of (int idx);

        int size () const;

      private:
        roots_list roots;

        struct depth_info {
          depth_info (reference ref, std::size_t depth);

          reference ref;
          std::size_t depth;
        };

        typedef std::vector<depth_info> node_list;

        typedef std::set<reference> node_set;

        struct tree_data : public list_data {

          std::size_t size () const override;

          void draw_at (std::size_t idx,
                        const draw::graphics& g,
                        const core::rectangle& place,
                        const draw::brush& background,
                        item_state state) const override;

          bool is_open (const reference ref) const;

          const list_data& operator ()() const {
            return *this;
          }

          node_list nodes;
          node_set open_nodes;
        } data;
      };

      // --------------------------------------------------------------------------
      struct node {
        typedef std::vector<node> node_list;
        typedef node_list::const_iterator iterator;

        node (const std::string& label = std::string());
        node (const char* label);
        node (std::string&& label);
        node (const std::string& label, std::initializer_list<node> nodes);

        void add_node (const node& node);
        void add_node (node&& node);
        void add_nodes (std::initializer_list<node> nodes);

        const node_list& nodes () const;

        iterator begin () const;
        iterator end () const;

        std::string label;
        node_list sub_nodes;
      };

      // --------------------------------------------------------------------------
      struct default_node_info {
        typedef node type;
        typedef node::iterator iterator;
        typedef const node* reference;
        typedef core::range<iterator> node_range;

        static bool has_sub_nodes (const node& n);
        static node_range sub_nodes (node const& n);
        static reference make_reference (node const& n);
        static type const& dereference (reference const& r);
        static std::string label (node const& n);
        static const draw::pixmap& icon (type const&, bool has_children, bool is_open, bool selected);

      };

    } // tree

    typedef tree::basic_tree<tree::default_node_info> tree_view;

  } // ctrl

} // gui

#include <gui/ctrl/tree.inl>
