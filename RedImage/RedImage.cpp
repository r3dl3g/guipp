
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/tile_view.h>
#include <gui/ctrl/scroll_view.h>
#include <gui/layout/grid_layout.h>
#include <gui/layout/lineup_layout.h>
#include <gui/layout/attach_layout.h>
#include <gui/layout/dynamic_border_layout.h>
#include <util/string_util.h>
#include <util/blocking_queue.h>
#include <logging/file_logger.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

#include "redimage_settings.h"
#include "image_view.h"
#include "side_bar.h"
#include "color_filter.h"
#include "thumb_view.h"
#include "status_bar.h"
#include "over_view.h"
#include "background_worker.h"
#include "cvmat2pixmap.h"

#include <iomanip>
#include <functional>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;

namespace xml = boost::property_tree::xml_parser;


// --------------------------------------------------------------------------
pixmap create_text_pixmap (const std::string& str,
                           const core::rectangle& rect,
                           const os::color color) {
  static font f = font::menu().with_thickness(font::bold);

  pixmap img(rect.size());
  graphics g(img);
  text_box box(str, rect, text_origin_t::center);
  g.clear(color::black);
  g.text(box, f, color);
  return img;
}
// --------------------------------------------------------------------------
masked_bitmap create_rect_pixmap (core::rectangle rect,
                           bool filled) {
  pixmap img(rect.size());
  graphics g(img);
  g.clear(color::menuColor());
  g.frame(draw::rectangle(rect), color::menuTextColor());
  if (filled) {
    rect.shrink({2.0F, 2.0F});
    g.fill(draw::rectangle(rect), color::menuTextColor());
  }
  return {img, bitmap()};
}
// --------------------------------------------------------------------------
class RedImage : public layout_main_window<gui::layout::dynamic_border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::dynamic_border_layout<>, float, float, float, float> super;

  RedImage ();

  void onCreated (window*, const core::rectangle&);
  void open_image ();
  void load_image (const sys_fs::path&);
  void quit ();
  void check_for_modification ();

  void show_raw ();
  void show_hsv ();

  void show_image (int i);
  void calc_image ();
  void reset_color_range (int i);
  void calc_image_and_show ();

  cv::Vec3b get_hsv_at (const core::size& win_size, const core::point& pt);
  void set_hsv_for (const cv::Vec3b& hsv, int i);

  cv::Vec3b get_bgr_at (const core::size& win_size, const core::point& pt);

  data::hsv_range get_hsv_range_at (const core::size& win_size, const core::point& pt, const core::size& sz);
  void set_hsv_range_for (const data::hsv_range& hsv, int i);

  void calc_all_and_show ();
  void add_filter ();
  void remove_filter (view::color_key_group* grp);

  void show_full_image (int i);
  void show_filter_view ();
  void show_folder_view ();
  void show_mask_view ();
  void show_quad_view ();

  void reset_current_color_range ();

  void load ();
  void save ();
  void saveAs ();
  void read_settings ();

  void toggle_learning ();
  void toggle_adjust_brightness ();
  void toggle_values ();

  void calc_title ();
  void calc_status ();
  void show_hsv_value (const cv::Vec3b& hsv);
  void show_bgr_value (const cv::Vec3b& rgb);
  void show_values_at (const core::size& sz, const core::point& pt);

  void open_folder ();

  void show_next ();
  void show_prev ();

  void calc_folder ();

  void init_sidebar_filter (int i);
  void init_sidebar ();

  void undo ();

private:
  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu view_sub_menu;
  popup_menu help_sub_menu;

  masked_bitmap hook_icon;
  masked_bitmap cross_icon;

  view::status_bar status;

  view::thumb_view folder_view;
  view::image_view full_image_view;
  view::over_view<4, 3> filter_view;
  view::over_view<4, 3> mask_view;
  view::over_view<2, 2> quad_view;

  window* current_view;

  int curent_full_image_view;

  typedef ctrl::scroll_view<> side_scroll_t;

  side_scroll_t side_scroll;
  view::side_bar filter_list;

  bool learning_mode;
  bool adjust_brightness;

  std::thread background_thread;
  volatile bool is_active;

  cv::Mat raw_image;
  cv::Mat hsv_image;

  float portions[3];

  enum {
    IMAGE_COUNT = 11,
    FILTER_HIGH = view::side_bar::layout_type::dimension + view::side_bar::layout_type::gap + view::side_bar::layout_type::separatior_width,
    FILTER_WIDTH = 275 - scroll_bar::scroll_bar_width
  };
  cv::Mat image[IMAGE_COUNT];

  data::hsv_range old_range;
  int undo_index;

  data::redimage_settings settings;
  data::redimage_settings old_settings;
  sys_fs::path settings_path;
  sys_fs::path image_path;

  std::function<void()> restore_last_view;
};

// --------------------------------------------------------------------------
RedImage::RedImage ()
  : super(20, 24, 277, 0)
  , current_view(nullptr)
  , curent_full_image_view(-1)
  , learning_mode(false)
  , adjust_brightness(true)
  , is_active(true)
  , undo_index(-1)
{
  for (auto& p : portions) {
    p = 0.0F;
  }
  on_create(util::bind_method(this, &RedImage::onCreated));
  on_destroy(&win::quit_main_loop);
  on_close(util::bind_method(this, &RedImage::quit));
  status.side_bar_toggle.set_checked(true);
  status.side_bar_toggle.on_clicked([&] () {
    toggle_values();
  });
}
// --------------------------------------------------------------------------
void RedImage::onCreated (win::window*, const core::rectangle&) {
  menu.data.add_entries({
    main_menu_entry("File", 'F', [&]() {
      file_sub_menu.popup(menu);
    }),
    main_menu_entry("Edit", 'E', [&]() {
      edit_sub_menu.popup(menu);
    }),
    main_menu_entry("View", 'V', [&]() {
      view_sub_menu.popup(menu);
    }),
    main_menu_entry("Help", 'H', [&]() {
      help_sub_menu.popup(menu);
    })
  });

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'o', util::bind_method(this, &RedImage::load), hot_key(keys::o, state::control), false),
    menu_entry("Save", 's', util::bind_method(this, &RedImage::save), hot_key(keys::s, state::control), false),
    menu_entry("SaveAs", 'a', util::bind_method(this, &RedImage::saveAs), hot_key(keys::s, state::control|state::shift), false),
    menu_entry("Open image", 'i', util::bind_method(this, &RedImage::open_image), hot_key(keys::i, state::control), true),
    menu_entry("Open directory", 'd', util::bind_method(this, &RedImage::open_folder), hot_key(keys::d, state::control), false),
    menu_entry("Exit", 'x', util::bind_method(this, &RedImage::quit), hot_key(keys::f4, state::alt), true)
  });

  const float icn_sz = core::global::scale<float>(12);
  core::rectangle icon_rect(0, 0, icn_sz, icn_sz);

  hook_icon = create_rect_pixmap(icon_rect, true);//create_text_pixmap("\u2612", icon_rect, color::dark_red); //
  cross_icon = create_rect_pixmap(icon_rect, false);//create_text_pixmap("\u2610", icon_rect, color::dark_green); //

  edit_sub_menu.data.add_entries({
    menu_entry("Leaning mode", 'l', util::bind_method(this, &RedImage::toggle_learning), hot_key(keys::t, state::control), false, cross_icon),
    menu_entry("Adjust Brightness", 'n', util::bind_method(this, &RedImage::toggle_adjust_brightness), hot_key(keys::n, state::control), false, hook_icon),
    menu_entry("Calc all", 'a', util::bind_method(this, &RedImage::calc_all_and_show), hot_key(keys::a, state::control), false),
    menu_entry("Original", 'o', util::bind_method(this, &RedImage::show_raw), hot_key(keys::g, state::control|state::alt), true),
    menu_entry("HSV Image", 'h', util::bind_method(this, &RedImage::show_hsv), hot_key(keys::h, state::control|state::alt), false),
    //menu_entry("Lab Image", 'l', util::bind_method(this, &RedImage::show_lab), hot_key(keys::b, state::control|state::alt), false),
    menu_entry("Reset range", 'r', util::bind_method(this, &RedImage::reset_current_color_range), hot_key(keys::r, state::control), true),
    menu_entry("Add filter", 'f', util::bind_method(this, &RedImage::add_filter), hot_key(keys::insert, state::control), true),
    menu_entry("Undo", 'u', util::bind_method(this, &RedImage::undo), hot_key(keys::z, state::control), true),
  });

  view_sub_menu.data.add_entries({
    menu_entry("Filter View", 'r', util::bind_method(this, &RedImage::show_filter_view), hot_key(keys::f, state::control), false),
    menu_entry("Directory View", 'd', util::bind_method(this, &RedImage::show_folder_view), hot_key(keys::g, state::control), false),
    menu_entry("Masks View", 'm', util::bind_method(this, &RedImage::show_mask_view), hot_key(keys::m, state::control), false),
    menu_entry("Main View", 'a', util::bind_method(this, &RedImage::show_quad_view), hot_key(keys::h, state::control), false),
    menu_entry("Toggle Values", 't', util::bind_method(this, &RedImage::toggle_values), hot_key(keys::v, state::control), false, hook_icon),
    menu_entry("Full View Image 1", '1', [&] () { RedImage::show_full_image(0); }, hot_key('1', state::control), true),
    menu_entry("Full View Image 2", '2', [&] () { RedImage::show_full_image(1); }, hot_key('2', state::control), false),
    menu_entry("Full View Image 3", '3', [&] () { RedImage::show_full_image(2); }, hot_key('3', state::control), false),
    menu_entry("Full View Image 4", '4', [&] () { RedImage::show_full_image(3); }, hot_key('4', state::control), false),
    menu_entry("Full View Image 5", '5', [&] () { RedImage::show_full_image(4); }, hot_key('5', state::control), false),
    menu_entry("Full View Image 6", '6', [&] () { RedImage::show_full_image(5); }, hot_key('6', state::control), false),
    menu_entry("Full View Image 7", '7', [&] () { RedImage::show_full_image(6); }, hot_key('7', state::control), false),
    menu_entry("Full View Image 8", '8', [&] () { RedImage::show_full_image(7); }, hot_key('8', state::control), false),
    menu_entry("Full View Image 9", '9', [&] () { RedImage::show_full_image(8); }, hot_key('9', state::control), false),
    menu_entry("Next Image", 'n', util::bind_method(this, &RedImage::show_next), hot_key(keys::right, state::control), true),
    menu_entry("Previous Image", 'P', util::bind_method(this, &RedImage::show_prev), hot_key(keys::left, state::control), false),
  });

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(*this, "About RedImage", "RedImage Version 0.9.0", "Ok");
    })
  );

  menu.create(*this);

  file_sub_menu.data.register_hot_keys(this);
  edit_sub_menu.data.register_hot_keys(this);
  view_sub_menu.data.register_hot_keys(this);

  side_scroll.create(*this);
  filter_list.create(side_scroll, core::rectangle(core::size(FILTER_WIDTH, FILTER_HIGH * 2)));

  status.create(*this);
  mask_view.create(*this);
  full_image_view.create(*this);
  folder_view.create(*this);
  quad_view.create(*this);
  filter_view.create(*this);

  int i = 0;
  for(view::image_view& v : filter_view.image_views) {
    v.on_left_btn_dblclk([&, i] (os::key_state, const core::point) {
      show_full_image(i);
    });
    v.on_mouse_move([&] (os::key_state, const core::point& pt) {
      show_values_at(v.size(), pt);
    });
    ++i;
  }

  full_image_view.on_left_btn_dblclk([&] (os::key_state, const core::point) {
    if (restore_last_view) {
      restore_last_view();
    }
  });
  full_image_view.on_left_btn_down([&] (os::key_state, const core::point& pt) {
    if (learning_mode) {
      if (curent_full_image_view > 2) {
        set_hsv_range_for(get_hsv_range_at(full_image_view.size(), pt, core::size(2, 2)), curent_full_image_view - 3);
      }
    }
  });
  full_image_view.on_mouse_move([&] (os::key_state, const core::point& pt) {
    show_values_at(full_image_view.size(), pt);
  });
  i = 0;
  for(view::image_view& v : quad_view.image_views) {
    v.on_left_btn_dblclk([&, i] (os::key_state, const core::point) {
      show_full_image(i);
    });
    v.on_mouse_move([&] (os::key_state, const core::point& pt) {
      show_values_at(v.size(), pt);
    });
    ++i;
  }

  folder_view.on_selection_commit([&] () {
    auto idx = folder_view.get_selection();
    if (idx < folder_view.list.size()) {
      const auto& info = folder_view.list[idx];
      show_quad_view();
      load_image(info.filename);
    }
  });

  get_layout().set_center_top_bottom_left_right(
    [&] (const core::rectangle& r) {
      filter_view.place(r);
      folder_view.place(r);
      quad_view.place(r);
      mask_view.place(r);
      full_image_view.place(r);
    },
    layout::lay(menu), layout::lay(status), layout::lay(side_scroll), nullptr);

  for (int i = 3; i < filter_view.count; ++i) {
    filter_view.image_views[i].on_left_btn_down([&, i] (os::key_state, const core::point& pt) {
      if (learning_mode) {
        set_hsv_range_for(get_hsv_range_at(filter_view.image_views[i].size(), pt, core::size(2, 2)), i - 3);
      }
    });
  }
  init_sidebar();

  set_children_visible();
  filter_view.to_front();

  read_settings();
  old_settings = settings;
}
//-----------------------------------------------------------------------------
void RedImage::init_sidebar () {
  filter_list.resize(core::size(FILTER_WIDTH, FILTER_HIGH * filter_list.color_keys.size()));
  for (int i = 0; i < filter_list.color_keys.size(); ++i) {
    init_sidebar_filter(i);
  }
}
//-----------------------------------------------------------------------------
void RedImage::init_sidebar_filter (int i) {
  filter_list.color_keys[i]->on_content_changed([&] () {
    calc_image_and_show();
  });
}
//-----------------------------------------------------------------------------
void RedImage::add_filter () {
  gui::ctrl::input_dialog::ask(*this, "New Filter", "Please enter name of new filter",
                               "Filter", "Ok", "Cancel", [&] (const std::string& name) {
    filter_list.add(name);
    init_sidebar_filter(filter_list.color_keys.size() - 1);
    filter_list.resize(core::size(FILTER_WIDTH, FILTER_HIGH * filter_list.color_keys.size()));
  });
}
//-----------------------------------------------------------------------------
void RedImage::remove_filter (view::color_key_group* grp) {
  auto& vec = filter_list.color_keys;
  vec.erase(std::remove_if(vec.begin(), vec.end(), [=](const std::unique_ptr<view::color_key_group>& i){ return i.get() == grp;}), vec.end());
}
//-----------------------------------------------------------------------------
data::hsv_range RedImage::get_hsv_range_at (const core::size& win_size, const core::point& pt, const core::size& sz) {
  if (hsv_image.empty()) {
    return data::hsv_range();
  }
  const auto img_size = hsv_image.size();
  const int x = (int)(pt.x() / win_size.width() * (float)img_size.width);
  const int y = (int)(pt.y() / win_size.height() * (float)img_size.height);
  const int width = (int)(sz.width() / 2);
  const int height = (int)(sz.height() / 2);
  const int min_x = std::max(0, x - width);
  const int min_y = std::max(0, y - height);
  const int max_x = std::max(min_x + width, x + width);
  const int max_y = std::max(min_y + height, y + height);

  cv::Mat sub(hsv_image, cv::Rect(min_x, min_y, max_x - min_x + 1, max_y - min_y + 1));
  cv::Vec3b init = sub.at<cv::Vec3b>(cv::Point(0, 0));
  data::range h(init[0], init[0]), s(init[1], init[1]), v(init[2], init[2]);

  for (cv::MatConstIterator_<cv::Vec3b> i = sub.begin<cv::Vec3b>(), e = sub.end<cv::Vec3b>(); i != e; ++i) {
    const cv::Vec3b& hsv = *i;
    h.extend(hsv[0]);
    s.extend(hsv[1]);
    v.extend(hsv[2]);
  }
  return data::hsv_range(h, s, v);
}
//-----------------------------------------------------------------------------
void RedImage::undo () {
  if (undo_index > -1) {
    filter_list.color_keys[undo_index]->set(old_range);
    undo_index = -1;
    calc_image_and_show();
  }
}
//-----------------------------------------------------------------------------
void RedImage::set_hsv_range_for (const data::hsv_range& hsv, int i) {
  if ((i > -1) && (i < filter_list.color_keys.size())) {
    old_range = filter_list.color_keys[i]->get();
    undo_index = i;
    filter_list.color_keys[i]->add(hsv);
    calc_image_and_show();
  }
}
//-----------------------------------------------------------------------------
cv::Vec3b RedImage::get_hsv_at (const core::size& win_size, const core::point& pt) {
  if (hsv_image.empty()) {
    return cv::Vec3b();
  }
  const auto img_size = hsv_image.size();
  const int x = (int)(pt.x() / win_size.width() * (float)img_size.width);
  const int y = (int)(pt.y() / win_size.height() * (float)img_size.height);
  if ((x < img_size.width) && (y < img_size.height)) {
    return hsv_image.at<cv::Vec3b>(cv::Point(x, y));
  }
  return cv::Vec3b();
}
//-----------------------------------------------------------------------------
cv::Vec3b RedImage::get_bgr_at (const core::size& win_size, const core::point& pt) {
  if (raw_image.empty()) {
    return cv::Vec3b();
  }
  const auto img_size = raw_image.size();
  const int x = (int)(pt.x() / win_size.width() * (float)img_size.width);
  const int y = (int)(pt.y() / win_size.height() * (float)img_size.height);
  if ((x < img_size.width) && (y < img_size.height)) {
    return raw_image.at<cv::Vec3b>(cv::Point(x, y));
  }
  return cv::Vec3b();
}
//-----------------------------------------------------------------------------
void RedImage::set_hsv_for (const cv::Vec3b& hsv, int i) {
  filter_list.color_keys[i]->set_hsv(hsv);
  calc_image_and_show();
}
//-----------------------------------------------------------------------------
void RedImage::show_full_image (int i) {
  cv::Mat src = (i == 0 ? raw_image : image[i - 1]);
  if (!src.empty()) {
    full_image_view.to_front();
    current_view = &full_image_view;

    curent_full_image_view = i;
    full_image_view.set_image_and_scale(src);
  }
}
//-----------------------------------------------------------------------------
void RedImage::show_filter_view () {
  filter_view.to_front();
  current_view = &filter_view;
  curent_full_image_view = -1;
  restore_last_view = [&] () { show_filter_view(); };
}
//-----------------------------------------------------------------------------
void RedImage::show_folder_view () {
  folder_view.to_front();
  current_view = &folder_view;
  restore_last_view = [&] () { show_folder_view(); };
}
//-----------------------------------------------------------------------------
void RedImage::show_mask_view () {
  mask_view.to_front();
  current_view = &mask_view;
  curent_full_image_view = -1;
  restore_last_view = [&] () { show_mask_view(); };
}
//-----------------------------------------------------------------------------
void RedImage::show_quad_view () {
  quad_view.to_front();
  current_view = &quad_view;
  curent_full_image_view = -1;
  restore_last_view = [&] () { show_quad_view(); };
}
//-----------------------------------------------------------------------------
void RedImage::toggle_learning () {
  learning_mode = !learning_mode;
  edit_sub_menu.data[0].set_icon(learning_mode ? hook_icon : cross_icon);
  calc_status();
}
//-----------------------------------------------------------------------------
void RedImage::toggle_adjust_brightness () {
  settings.normalize(!settings.normalize());
  edit_sub_menu.data[1].set_icon(settings.normalize() ? hook_icon : cross_icon);
}
//-----------------------------------------------------------------------------
void RedImage::toggle_values () {
  bool off = (get_layout().get_left_width() == 0);
  get_layout().set_left_width(off ? 277 : 0);
  layout();
  view_sub_menu.data[4].set_icon(off ? hook_icon : cross_icon);
  status.side_bar_toggle.set_checked(off);
}
//-----------------------------------------------------------------------------
void RedImage::show_raw () {
  filter_view.image_views[0].set_image_and_scale(raw_image);
}
//-----------------------------------------------------------------------------
void RedImage::show_hsv () {
  filter_view.image_views[0].set_image_and_scale(hsv_image);
}
//-----------------------------------------------------------------------------
void RedImage::show_image (int i) {
  filter_view.image_views[i + 1].set_image_and_scale(image[i]);
}
//-----------------------------------------------------------------------------
void calc_image (const cv::Mat& hsv_image,
                 const view::side_bar::color_key_group_list& filter,
                 float portions[3]) {
  cv::Mat off_mask, bad_mask, good_mask;
  const view::color_key_group* bad_grp = nullptr;
  int idx = 0;
  for (auto& f : filter) {
    cv::Mat mask = f->calc_mask(hsv_image);
    if (0 == idx) {
      bad_mask = mask;
    } else {
      if (1 == idx) {
        off_mask = mask;
      } else {
        off_mask |= mask;
      }
    }
    ++idx;
  }

  bad_mask &= ~off_mask;
  good_mask = ~(bad_mask | off_mask);

  portions[0] = proc::color_filter::calc_portion(good_mask);
  portions[1] = proc::color_filter::calc_portion(off_mask);
  portions[2] = proc::color_filter::calc_portion(bad_mask);

}
//-----------------------------------------------------------------------------
void RedImage::calc_image () {
  if (hsv_image.empty()) {
    return;
  }

  cv::Mat off_mask;
  view::color_key_group* bad_grp = nullptr;
  int idx = 0;
  for (auto& f : filter_list.color_keys) {
    view::color_key_group* grp = f.get();
    grp->mask = grp->calc_mask(hsv_image);
    if (0 == idx) {
      bad_grp = grp;
    } else {
      if (1 == idx) {
        off_mask = grp->mask.clone();
      } else {
        off_mask |= grp->mask;
      }
      grp->image.setTo(cv::Scalar(0, 0, 0));
      raw_image.copyTo(grp->image, grp->mask);
      image[idx + 2].setTo(cv::Scalar(0, 0, 0));
      raw_image.copyTo(image[idx + 2], learning_mode ? ~grp->mask : grp->mask);
      mask_view.image_views[idx + 2 + 1].set_image_and_scale(grp->mask);
    }
    ++idx;
  }

  bad_grp->mask &= ~off_mask;
  raw_image.copyTo(bad_grp->image, bad_grp->mask);

  cv::Mat good_mask = ~(bad_grp->mask | off_mask);

  portions[0] = proc::color_filter::calc_portion(good_mask);
  portions[1] = proc::color_filter::calc_portion(off_mask);
  portions[2] = proc::color_filter::calc_portion(bad_grp->mask);

  image[0].setTo(cv::Scalar(0, 0, 0));
  image[1].setTo(cv::Scalar(0, 0, 0));
  image[2].setTo(cv::Scalar(0, 0, 0));

  raw_image.copyTo(image[0], off_mask);
  raw_image.copyTo(image[1], good_mask);
  raw_image.copyTo(image[2], bad_grp->mask);

  mask_view.image_views[1].set_image_and_scale(off_mask);
  mask_view.image_views[2].set_image_and_scale(good_mask);
  mask_view.image_views[3].set_image_and_scale(bad_grp->mask);

  quad_view.image_views[0].set_image_and_scale(raw_image);
  quad_view.image_views[1].set_image_and_scale(image[0]);
  quad_view.image_views[2].set_image_and_scale(image[1]);
  quad_view.image_views[3].set_image_and_scale(image[2]);

}
//-----------------------------------------------------------------------------
void RedImage::calc_image_and_show () {
  if (hsv_image.empty()) {
    return;
  }
  calc_image();

  for (int i = 0; i < IMAGE_COUNT; ++i) {
    show_image(i);
  }

  calc_status();
  if (curent_full_image_view > -1) {
    show_full_image(curent_full_image_view);
  }
}
//-----------------------------------------------------------------------------
void RedImage::calc_all_and_show () {
  if (current_view == &folder_view) {
    calc_folder();
  } else {
    calc_image_and_show();
  }
}
//-----------------------------------------------------------------------------
void RedImage::reset_current_color_range () {
  if (curent_full_image_view > 2) {
    reset_color_range(curent_full_image_view - 3);
  }
}
//-----------------------------------------------------------------------------
void RedImage::reset_color_range (int i) {
  filter_list.color_keys[i]->reset();
  calc_image_and_show();
}
//-----------------------------------------------------------------------------
void RedImage::check_for_modification () {
  read_settings();
  if (old_settings != settings) {
    clog::debug() << "Old:" << old_settings << " New:" << settings;
    yes_no_dialog::ask(*this, "Question!", "Settings have changed! Do you realy want to save before exit?", "Yes", "No", [&] (bool yes) {
      if (yes) {
        save();
      }
    });
  }
}
//-----------------------------------------------------------------------------
void RedImage::quit () {
  check_for_modification();
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      is_active = false;
      if (background_thread.joinable()) {
        background_thread.join();
      }
      win::quit_main_loop(this);
    } else {
      take_focus();
    }
  });
}
//-----------------------------------------------------------------------------
bool is_jpeg (const sys_fs::path& i) {
  auto ext = i.extension();
  return (ext == ".jpg") || (ext == ".jpeg") || (ext == ".JPG") || (ext == ".JPEG");
}
//-----------------------------------------------------------------------------
bool is_not_jpeg_or_dir (const sys_fs::directory_entry& i) {
  return !is_jpeg(i.path()) && !(i.status().type() == sys_fs::file_type::directory);
}
//-----------------------------------------------------------------------------
bool is_not_xml (const sys_fs::directory_entry& i) {
  auto ext = i.path().extension();
  return !(ext == ".xml") && !(ext == ".XML") && !(i.status().type() == sys_fs::file_type::directory);
}
//-----------------------------------------------------------------------------
void RedImage::open_image () {
  auto old_path = sys_fs::current_path();
  if (!settings.last_path().empty() && sys_fs::exists(settings.last_path())) {
    sys_fs::current_path(settings.last_path());
  }
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    load_image(file);
  }, is_not_jpeg_or_dir);
  sys_fs::current_path(old_path);
}
// --------------------------------------------------------------------------
void load_image (const sys_fs::path& file, cv::Mat& raw, cv::Mat& hsv, bool adjust_brightness) {
  cv::Mat srcImage = cv::imread(file.string(), cv::IMREAD_COLOR);
  cv::Mat scaledImage;
  cv::resize(srcImage, scaledImage, cv::Size(1500, 1000), 0, 0, cv::INTER_LINEAR);
  if (adjust_brightness) {
    cv::Scalar mean = cv::mean(scaledImage);
    const int mid = ((int)mean[0]+(int)mean[1]+(int)mean[2]) / 3;
    const int diff = 128 - mid;
    raw = scaledImage + cv::Scalar(diff, diff, diff);
  } else {
    raw = scaledImage;
  }
  //cv::normalize(scaledImage, raw, 0, 255, cv::NORM_L2, -1);
  cv::cvtColor(raw, hsv, cv::COLOR_BGR2HSV);
}
// --------------------------------------------------------------------------
void RedImage::load_image (const sys_fs::path& file) {
  if (sys_fs::exists(file)) {
    image_path = file;
    for (auto& p : portions) {
      p = 0.0F;
    }
    calc_title();
    settings.last_path(file.parent_path().string());

    ::load_image(file, raw_image, hsv_image, settings.normalize());

    raw_image.copyTo(image[0]);
    raw_image.copyTo(image[1]);

    show_raw();
    calc_all_and_show();
  }
}
// --------------------------------------------------------------------------
void RedImage::load () {
  check_for_modification();
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    settings_path = file;
    for (auto& p : portions) {
      p = 0.0F;
    }
    for (auto& im : this->image) {
      im = cv::Mat();
    }
    calc_title();
    calc_status();

    using boost::property_tree::ptree;

    ptree xml_main;
    try {
      xml::read_xml(settings_path.string(), xml_main, xml::no_comments);
      auto opt = xml_main.get_child_optional("redimage");
      if (opt) {
        settings.read(opt.get());
        old_settings = settings;
        filter_list.set(settings.colors());
        init_sidebar();
        edit_sub_menu.data[1].set_icon(settings.normalize() ? hook_icon : cross_icon);
      }
    } catch (std::exception& ex) {
      clog::warn() << "Exception while reading redimage.xml:" << ex.what();
    }
  }, is_not_xml);
}
// --------------------------------------------------------------------------
void RedImage::read_settings () {
  settings.colors(filter_list.get());
}
// --------------------------------------------------------------------------
void RedImage::save () {
  if (settings_path.empty()) {
    saveAs();
  } else {
    read_settings();

    using boost::property_tree::ptree;

    ptree main;
    settings.write(main);
    ptree xml_main;
    xml_main.put_child("redimage", main);

    boost::property_tree::xml_writer_settings<ptree::key_type> xml_settings('\t', 1);
    xml::write_xml(settings_path.string(), xml_main, std::locale(), xml_settings);

    old_settings = settings;
  }
}
// --------------------------------------------------------------------------
void RedImage::saveAs () {
  file_save_dialog::show(*this, "Save File As", "Sorte", "Filename", "Save", "Cancel", [&] (const sys_fs::path& file) {
     settings_path = file;
     calc_title();
     save();
  });
}
// --------------------------------------------------------------------------
void RedImage::calc_title () {
  std::ostringstream buffer;
  buffer << "RedImage";
  if (!settings_path.empty()) {
    buffer << " - " << settings_path.filename();
  }
  if (!image_path.empty()) {
    buffer << " - " << image_path.filename();
  }
  set_title(buffer.str());
}
// --------------------------------------------------------------------------
void RedImage::calc_status () {
  status.labels[0].set_text(learning_mode ? "Learning Mode" : "");

  std::ostringstream buffer;
  if (portions[2] != 0.0F) {
    buffer << "Bad: " << std::fixed << std::setprecision(2) << (portions[2] * 100.0F) << "%";
  }
  if (portions[1] != 0.0F) {
    if (portions[2] != 0.0F) {
      buffer << " - ";
    }
    buffer << "Off: " << std::fixed << std::setprecision(2) << (portions[1] * 100.0F) << "%";
  }
  if (portions[0] != 0.0F) {
    if ((portions[1] != 0.0F) || (portions[2] != 0.0F)) {
      buffer << " - ";
    }
    buffer << "Good: " << std::fixed << std::setprecision(2) << (portions[0] * 100.0F) << "%";
//           << "% - Quality: " << std::fixed << std::setprecision(2) << (portions[2] * 100.0F) << "%";
  }
  status.labels[1].set_text(buffer.str());
}
//-----------------------------------------------------------------------------
void RedImage::show_values_at (const core::size& sz, const core::point& pt) {
  const auto hsv = get_hsv_at(sz, pt);
  show_hsv_value(hsv);
  show_bgr_value(get_bgr_at(sz, pt));
  for (auto& f : filter_list.color_keys) {
    f.get()->check_hsv(hsv);
  }
}
//-----------------------------------------------------------------------------
void RedImage::show_hsv_value (const cv::Vec3b& hsv) {
  std::ostringstream buffer;
  buffer << "HSV: " << std::setw(3) << (int)hsv[0]
         << ":" << std::setw(3) << (int)hsv[1]
         << ":" << std::setw(3) << (int)hsv[2];
  status.labels[2].set_text(buffer.str());
}
//-----------------------------------------------------------------------------
void RedImage::show_bgr_value (const cv::Vec3b& rgb) {
  std::ostringstream buffer;
  buffer << "RGB: " << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int)rgb[2]
         << ":" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int)rgb[1]
         << ":" << std::setw(2) << std::setfill('0') << std::uppercase << std::hex << (int)rgb[0];
  status.labels[3].set_text(buffer.str());
  status.color.set_bgr_color(rgb);
}
// --------------------------------------------------------------------------
void RedImage::calc_folder () {
  typedef background_worker<int> worker;
  worker calculation_threads;

  for (int i = 0; i < folder_view.list.size(); ++i) {
    calculation_threads.add(i);
  }

  const bool adjust_brightness = settings.normalize();
  calculation_threads.start([=] (worker::queue& queue) {

    int idx = -1;
    while (queue.try_dequeue(idx)) {

      if (!is_active) {
        return;
      }

      image_info info = folder_view.list[idx];

      cv::Mat raw, hsv;
      ::load_image(info.filename, raw, hsv, adjust_brightness);

      float portions[3];
      ::calc_image(hsv, filter_list.color_keys, portions);

      info.bad = portions[2] * 100.0F;
      info.off = portions[1] * 100.0F;
      info.good = portions[0] * 100.0F;
      info.quality = info.bad;// / info.good * 100.0F;

      // thumbnail
      auto native_view_size = core::global::scale(folder_view.get_item_size() - core::size(4, 4));
      cv::Size thumb_size(native_view_size.width() / 2, native_view_size.height());
      cv::Mat thumb(thumb_size, raw.type());
      cv::resize(raw, thumb, thumb_size, 0, 0, cv::INTER_LINEAR);
      info.image = cvMat2pixmap(thumb);

      win::run_on_main([&, info, idx] () {
        folder_view.list[idx] = info;
        folder_view.update_list();
      });
    }
  });

  calculation_threads.join();
}
// --------------------------------------------------------------------------
void RedImage::open_folder () {
  auto old_path = sys_fs::current_path();
  if (!settings.last_path().empty() && sys_fs::exists(settings.last_path())) {
    sys_fs::current_path(settings.last_path());
  }
  dir_open_dialog::show(*this, "Choose folder", "Open", "Cancel", [&] (const sys_fs::path& folder) {
    if (sys_fs::exists(folder) && sys_fs::is_directory(folder)) {
      folder_view.list.clear();
      show_folder_view();
      std::vector<sys_fs::path> list;
      std::copy_if(sys_fs::directory_iterator(folder
#ifdef X11
          , sys_fs::directory_options::skip_permission_denied
#endif // X11
                                           ), sys_fs::directory_iterator(), std::back_inserter(list), is_jpeg);
      std::sort(list.begin(), list.end());
      for (const auto& i : list) {
        folder_view.list.emplace_back(i);
      }
      folder_view.update_list();
      if (background_thread.joinable()) {
        is_active = false;
        background_thread.join();
      }
      is_active = true;
      background_thread = std::thread([&] () {
        calc_folder();
      });
    }
  });
  sys_fs::current_path(old_path);
}
// --------------------------------------------------------------------------
void RedImage::show_next () {
  if (!folder_view.list.empty()) {
    const int idx = (folder_view.get_selection() + 1) % folder_view.list.size();
    folder_view.set_selection(idx, event_source::logic);
    const auto& info = folder_view.list[idx];
    load_image(info.filename);
  }
}
// --------------------------------------------------------------------------
void RedImage::show_prev () {
  if (!folder_view.list.empty()) {
    const int idx = (folder_view.get_selection() > 0 ? folder_view.get_selection() : (int)folder_view.list.size()) - 1;
    folder_view.set_selection(idx, event_source::logic);
    const auto& info = folder_view.list[idx];
    load_image(info.filename);
  }
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  RedImage main;

  logging::file_logger l("RedImage.log", logging::level::debug, logging::core::get_standard_formatter());

  main.create({50, 50, 800, 600});
  main.calc_title();
  main.set_visible();
  main.maximize();

  return run_main_loop();
}
