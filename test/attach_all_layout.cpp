
#include <gui/win/container.h>
#include <gui/layout/layout_container.h>
#include <gui/layout/border_layout.h>
#include <gui/layout/attach_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/label.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::win;

  using namespace gui::layout;
  using namespace gui::ctrl;
  using namespace gui::core;

  layout_main_window<attachable_layout<attach_all>> main;
  label_center first, second, third, fourth, fifth;
  using right_layout = vertical_adaption<0, 2>;
  using middle_layout = horizontal_adaption<0, 2>;

  attachable_layout<right_layout> right(right_layout{win(second), win(third)});
  attachable_layout<middle_layout> middle(middle_layout{win(fourth), win(fifth)});

  attach_all& layout = main.get_layout();

  namespace att = gui::layout::attach;
  using rlay = att::lay<right_layout>;
  using mlay = att::lay<middle_layout>;

  layout.attach_relative<What::top, 0, 10>(attach_win(first), attach_win(main));
  layout.attach_fix<What::bottom, Where::height, -10>(att::win(first), att::win(main));
  layout.attach_relative<What::left, 0, 10>(att::win(first), att::win(main));
  layout.attach_relative<What::right, make_relative(0.33), -10>(att::win(first), att::win(main));


  layout.attach_relative<What::top, 0, 10>(rlay(right), att::win(main));
  layout.attach_fix<What::bottom, Where::height, -10>(rlay(right), att::win(main));
  layout.attach_relative<What::left, make_relative(0.66), 10>(rlay(right), att::win(main));
  layout.attach_relative<What::right, make_relative(1.0), -10>(rlay(right), att::win(main));

  layout.attach_relative<What::top, 0, 10>(mlay(middle), att::win(main));
  layout.attach_fix<What::bottom, Where::height, -10>(mlay(middle), att::win(main));
  layout.attach_fix<What::left, Where::x2, 10>(mlay(middle), att::win(first));
  layout.attach_fix<What::right, Where::x, -10>(mlay(middle), rlay(right));

  main.on_create([&] (window* parent, const rectangle& rect) {
    first.create(main);
    second.create(main);
    third.create(main);
    fourth.create(main);
    fifth.create(main);

    first.set_text([&] () { return ostreamfmt("1.:(" << first.place() << ")"); });
    second.set_text([&] () { return ostreamfmt("2.:(" << second.place() << ")"); });
    third.set_text([&] () { return ostreamfmt("3.:(" << third.place() << ")"); });
    fourth.set_text([&] () { return ostreamfmt("4.:(" << fourth.place() << ")"); });
    fifth.set_text([&] () { return ostreamfmt("5.:(" << fifth.place() << ")"); });

    main.layout();
  });

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Hello All attach Layout");
  main.set_visible();

  return run_main_loop();
}
