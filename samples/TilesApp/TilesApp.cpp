

#include <gui/layout/layout_container.h>
#include <gui/layout/grid_layout.h>
#include <gui/ctrl/split_view.h>
#include <gui/ctrl/tile_view.h>
#include <gui/ctrl/look/table.h>

using namespace gui;
using namespace gui::win;
using namespace gui::layout;
using namespace gui::ctrl;
using namespace gui::draw;
using namespace gui::core;

template<std::size_t S, frame::drawer F = frame::raised_relief>
struct tile_drawer : public list_data {

  std::size_t size () const override { return S; }

  void draw_at (std::size_t idx, graphics& g, const core::rectangle& r, const brush& b, item_state s) const override {
    look::text_cell<std::size_t, F>(idx, g, r, text_origin_t::center, color::black, b.color(), s);
  }
};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {

  layout_main_window<grid_adaption<2, 2>> main;

  horizontal_scrollable_tile_view htileview;
  horizontal_scrollable_multi_tile_view htileview_m;
  vertical_scrollable_tile_view vtileview;
  vertical_scrollable_multi_tile_view vtileview_m;

  htileview->set_item_size({ 110, 33 });
  htileview->set_background(color::very_very_light_gray);
  htileview->set_border({ 10, 20 });
  htileview->set_spacing({ 5, 5 });
  htileview->set_data(tile_drawer<97>());

  htileview_m->set_item_size({ 110, 33 });
  htileview_m->set_background(color::very_very_light_gray);
  htileview_m->set_border({ 10, 20 });
  htileview_m->set_spacing({ 5, 5 });
  htileview_m->set_data(tile_drawer<97>());

  vtileview->set_item_size({ 330, 33 });
  vtileview->set_background(color::very_very_light_gray);
  vtileview->set_border({ 10, 20 });
  vtileview->set_spacing({ 5, 5 });
  vtileview->set_data(tile_drawer<97>());

  vtileview_m->set_item_size({ 330, 33 });
  vtileview_m->set_background(color::very_very_light_gray);
  vtileview_m->set_border({ 10, 20 });
  vtileview_m->set_spacing({ 5, 5 });
  vtileview_m->set_data(tile_drawer<97>());

  main.add({htileview, htileview_m, vtileview, vtileview_m});

  main.create({50, 50, 800, 600});
  main.on_destroy(&quit_main_loop);
  main.set_title("Tile View");
  main.set_visible();

  return run_main_loop();
}
