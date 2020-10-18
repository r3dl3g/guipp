
#include <gui/win/container.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/attach_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>
#include <logging/core.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

struct drawing {

  drawing ()
    : win(nullptr)
  {}

  void create (gui::win::window& w, const gui::core::rectangle& r = gui::core::rectangle::def) {
    area = r;
    win = &w;
  }

  void paint (const gui::draw::graphics& g) {
    g.fill(gui::draw::rectangle(area), gui::color::light_gray);
    g.text(gui::draw::text_box(text(), area, gui::text_origin_t::center),
               gui::draw::font::system(),
               gui::color::dark_blue);
  }

  void set_text (const gui::ctrl::text_source& t) {
    text = t;
    if (win) win->invalidate();
  }

  void place (const gui::core::rectangle& r) {
    area = r;
    if (win) win->invalidate();
  }

  const gui::core::rectangle& place () const {
    return area;
  }

  gui::ctrl::text_source text;
  gui::core::rectangle area;
  gui::win::window* win;
};

template<typename T>
gui::layout::layout_function any (T& t) {
  return [&t] (const gui::core::rectangle& r) {
    t.place(r);
  };
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<attachable_layout<attach>> main;
  label_center first, second, third;//, fourth, fifth;
  drawing fourth, fifth;
  using right_layout = vertical_adaption<0, 2>;
  using middle_layout = horizontal_adaption<0, 2>;

  attachable_layout<right_layout> right(right_layout{lay(second), lay(third)});
  attachable_layout<middle_layout> middle(middle_layout{any(fourth), any(fifth)});

  attach& layout = main.get_layout();

  layout.attach_relative<What::top, 0, 10>(&first, &main);
  layout.attach_fix<What::bottom, Where::height, -10>(&first, &main);
  layout.attach_relative<What::left, 0, 10>(&first, &main);
  layout.attach_relative<What::right, make_relative(0.33), -10>(&first, &main);


  layout.attach_relative<What::top, 0, 10>(&right, &main);
  layout.attach_fix<What::bottom, Where::height, -10>(&right, &main);
  layout.attach_relative<What::left, make_relative(0.66), 10>(&right, &main);
  layout.attach_relative<What::right, make_relative(1.0), -10>(&right, &main);

  layout.attach_relative<What::top, 0, 10>(&middle, &main);
  layout.attach_fix<What::bottom, Where::height, -10>(&middle, &main);
  layout.attach_fix<What::left, Where::x2, 10>(&middle, &first);
  layout.attach_fix<What::right, Where::x, -10>(&middle, &right);

  first.set_text([&] () { return ostreamfmt("1.:(" << first.place() << ")"); });
  second.set_text([&] () { return ostreamfmt("2.:(" << second.place() << ")"); });
  third.set_text([&] () { return ostreamfmt("3.:(" << third.place() << ")"); });
  fourth.set_text([&] () { return ostreamfmt("4.:(" << fourth.place() << ")"); });
  fifth.set_text([&] () { return ostreamfmt("5.:(" << fifth.place() << ")"); });

  main.on_create([&] (window* parent, const rectangle& rect) {
    first.create(main);
    second.create(main);
    third.create(main);
    fourth.create(main);
    fifth.create(main);
  });
  main.on_paint(gui::draw::paint([&] (const gui::draw::graphics& g) {
    fourth.paint(g);
    fifth.paint(g);
  }));

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello All attach Layout");
  main.set_visible();

  return run_main_loop();
}
