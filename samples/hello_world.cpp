

#include <gui/win/container.h>
#include <logging/core.h>
#include <util/string_util.h>
#include <iostream>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
struct graphics {
  void draw (const std::string& s) const {
    std::cout << s << std::endl;
  }
};

// --------------------------------------------------------------------------
template<typename T>
using item_type_drawer = void (*)(const T&, const graphics&);

// --------------------------------------------------------------------------
template<typename T>
void default_item_drawer (const T& t, const graphics& g) {
  g.draw(util::string::convert::from(t));
}

void hex_item_drawer (const int& t, const graphics& g) {
  g.draw(ostreamfmt(std::hex << t));
}

// --------------------------------------------------------------------------
//struct list_data {
//  typedef void item_index_drawer (std::size_t idx, const graphics&);
//  typedef std::size_t size_server ();

//  list_data ()
//  {}

//  list_data (std::function<size_server> sz_fn,
//               std::function<item_index_drawer> drw_fn)
//    : size_fn(sz_fn)
//    , draw_fn(drw_fn)
//  {}

//  std::size_t size () const {
//    return size_fn ? size_fn() : 0;
//  }

//  void draw_at (std::size_t idx, const graphics& g) const {
//    if (draw_fn) {
//      draw_fn(idx, g);
//    }
//  }

//protected:
//  std::function<size_server> size_fn;
//  std::function<item_index_drawer> draw_fn;
//};

//// --------------------------------------------------------------------------
//template<typename T>
//struct const_server : public list_data {

//  typedef list_data super;

//  const_server (std::initializer_list<T> args)
//    : super(util::bind_method(this, &const_server::my_size),
//            util::bind_method(this, &const_server::draw_at))
//    , data(args)
//  {}

//  const_server (const_server&& rhs)
//    : super(util::bind_method(this, &const_server::my_size),
//            util::bind_method(this, &const_server::draw_at))
//    , data(std::move(rhs.data))
//  {}

//  void draw_at (std::size_t idx, const graphics& g) const {
//    const T& t = data[idx];
//    default_item_drawer(t, g);
//  }

//private:
//  std::size_t my_size () const {
//    return data.size();
//  }

//  std::vector<T> data;
//};

//// --------------------------------------------------------------------------
//template<typename T>
//struct ref_server : public list_data {

//  typedef list_data super;

//  ref_server (const T& t)
//    : super(util::bind_method(this, &ref_server::size),
//            util::bind_method(this, &ref_server::draw_at))
//    , data(t)
//  {}

//  std::size_t size () const {
//    return data.size();
//  }

//  void draw_at (std::size_t idx, const graphics& g) const {
//    default_item_drawer(data[idx], g);
//  }

//private:
//  const T& data;
//};

//// --------------------------------------------------------------------------
//struct calc_server {

//  std::size_t size () const {
//    return 4;
//  }

//  void draw_at (std::size_t idx, const graphics& g) const {
//    const int t = idx * 10;
//    default_item_drawer(t, g);
//  }

//};


// --------------------------------------------------------------------------
template<item_type_drawer<int> D = default_item_drawer<int>>
struct calc_server : public list_data {

  std::size_t size () const override {
    return 4;
  }

  void draw_at (std::size_t idx, const graphics& g) const override {
    const int t = idx * 10;
    D(t, g);
  }

};

// --------------------------------------------------------------------------
struct item_list {

  item_list ()
  {}

  template<typename T>
  item_list (std::initializer_list<T> args)
    : server(new const_server<T>(args))
  {}

  item_list (const list_data* s)
    : server(s)
  {}

  void draw_all () {
    graphics g;
    g.draw("---");
    auto count = server->size();
    for (std::size_t i = 0; i < count; ++i) {
      draw_index(i, g);
    }
  }

  void draw_index (std::size_t idx, const graphics& g) {
    server->draw_at(idx, g);
  }

  std::unique_ptr<const list_data> server;

};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  item_list list(new calc_server<hex_item_drawer>());
  list.draw_all();

  item_list list2({ "Eins", "Zwei", "Drei", "Vier", "Fünf" });
  list2.draw_all();

  item_list list3({ 1, 2, 3, 4 });
  list3.draw_all();

  std::vector<int> ints({1, 2});
  item_list list4(new ref_server<std::vector<int>>(ints));
  list4.draw_all();
  ints.push_back(3);
  list4.draw_all();

//  item_list<bad_server> list4;
//  list4.draw_all();
  return 0;

  main_window  main;

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello world");
  main.set_visible();

  return run_main_loop();
}
