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

      const draw::masked_bitmap& standard_icon (bool has_children, bool is_open, bool selected);

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
      template<typename I>
      class basic_tree : public vertical_list {
      public:
        typedef vertical_list super;

        typedef I tree_info;
        typedef typename tree_info::type type;
        typedef typename tree_info::reference reference;

        type root;

        basic_tree (core::size_type item_size = 20,
                    os::color background = color::white,
                    bool grab_focus = true);
        basic_tree (const basic_tree& rhs);
        basic_tree (basic_tree&& rhs);

        void init ();

        bool is_open (const reference ref) const;
        bool is_open (int idx) const;
        void set_open (int idx, bool o);

        void open_all ();
        void open_root ();
        void open_sub (const type& n);

        void toggle_node (int idx);
        void open_node (int idx);
        void close_node (int idx);

        void toggle_node (const reference);
        void open_node (const reference);
        void close_node (const reference);

        void select_node (const reference ref);

        void update_node_list ();
        void collect_children (const type& n, std::size_t depth = 0);

        bool is_valid_idx (int idx) const;

        const reference get_item (int idx) const;
        int get_parent_item_of (int idx);

        int size () const;

      protected:
        void draw_list_item (std::size_t idx,
                             const draw::graphics& g,
                             const core::rectangle& r,
                             const draw::brush& b,
                             bool selected,
                             bool hilited);

      private:
        struct depth_info {
          depth_info (reference ref, std::size_t depth);

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

        node (const std::string& label = std::string());
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
      template<typename I>
      struct range {
        typedef I iterator;

        range (const iterator& b, const iterator& e);
        range (iterator&& b, iterator&& e);

        iterator begin () const;
        iterator end () const;

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

        static bool has_sub_nodes (const node& n);
        static node_range sub_nodes (node const& n);
        static reference make_reference (node const& n);
        static type const& dereference (reference const& r);
        static std::string label (node const& n);
        static const draw::masked_bitmap& icon (type const&, bool has_children, bool is_open, bool selected);

      };

      // --------------------------------------------------------------------------
      // inlines
      template<typename I>
      basic_tree<I>::basic_tree (core::size_type item_size,
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
      void basic_tree<I>::init () {
        super::set_drawer(core::bind_method(this, &basic_tree::draw_list_item));
        super::register_event_handler(REGISTER_FUNCTION, selection_commit_event([&]() {
          toggle_node(super::get_selection());
        }));
        super::register_event_handler(REGISTER_FUNCTION, left_btn_down_event([&](os::key_state,
                                                                             const core::point & pt) {
          int idx = super::get_index_at_point(pt);
          if ((idx > -1) && (idx < size())) {
            const depth_info& i = nodes[idx];
            core::point_type x = core::point_type(i.depth * 16);
            if ((x <= pt.x()) && (x + 16 >= pt.x())) {
              toggle_node(idx);
            }
          }
        }));
        super::register_event_handler(REGISTER_FUNCTION, any_key_down_event([&](os::key_state,
                                                                                os::key_symbol key,
                                                                                const std::string &){
          switch (key) {
            case keys::left:
            case keys::numpad::left: {
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
            case keys::right:
            case keys::numpad::right:
              open_node(super::get_selection());
              break;
          }
        }));
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
        open_sub(root);
      }

      template<typename I>
      inline void basic_tree<I>::open_root () {
        open_nodes.clear();
        open_nodes.insert(tree_info::make_reference(root));
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
            super::set_selection(std::distance(nodes.begin(), i), event_source::logic);
          }
        }
      }

      template<typename I>
      void basic_tree<I>::update_node_list () {
        nodes.clear();
        collect_children(root);
        super::set_count(nodes.size());
        super::redraw_later();
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
      template<typename I>
      inline range<I>::range (const iterator& b, const iterator& e)
        : start(b)
        , finish(e)
      {}

      template<typename I>
      inline range<I>::range (iterator&& b, iterator&& e) {
        std::swap(start, b);
        std::swap(finish, e);
      }

      template<typename I>
      inline auto range<I>::begin() const->iterator {
        return start;
      }

      template<typename I>
      inline auto range<I>::end() const->iterator {
        return finish;
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

      // --------------------------------------------------------------------------

    } // tree

    typedef tree::basic_tree<tree::default_node_info> tree_view;

  } // win

} // gui
