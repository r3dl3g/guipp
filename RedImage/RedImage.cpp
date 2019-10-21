
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/tile_view.h>
#include <gui/win/grid_layout.h>
#include <gui/win/lineup_layout.h>
#include <gui/win/attach_layout.h>
#include <base/string_util.h>
#include <base/blocking_queue.h>
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
  pixmap img(rect.size());
  graphics g(img);
  text_box box(str, rect, text_origin::center);
  g.clear(color::black);
  g.text(box, font::menu(), color);
  return img;
}
// --------------------------------------------------------------------------
// --------------------------------------------------------------------------
class RedImage : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;
  RedImage ();

  void onCreated (window*, const core::rectangle&);
  void open_image ();
  void load_image (const sys_fs::path&);
  void quit ();

  void show_raw ();
  void show_inverted ();

  void show_image (int i);
  void calc_image (int i);
  void reset_color_range (int i);
  void calc_image_and_show (int i, bool calc_rest_img = true);

  cv::Vec3b get_hsv_at (const core::size& win_size, const core::point& pt);
  void set_hsv_for (const cv::Vec3b& hsv, int i);

  data::hsv_range get_hsv_range_at (const core::size& win_size, const core::point& pt, const core::size& sz);
  void set_hsv_range_for (const data::hsv_range& hsv, int i);

  void calc_all_and_show ();
  void calc_rest ();

  void calc_color (int i);

  void show_full_image (int i);
  void show_filter_view ();
  void show_folder_view ();
  void show_mask_view ();

  void reset_current_color_range ();

  void load ();
  void save ();
  void saveAs ();

  void toggle_learning ();

  void calc_title ();
  void calc_status ();
  void show_hsv_value (const cv::Vec3b& hsv);

  void open_folder ();

  void show_next ();
  void show_prev ();

  void calc_folder ();

private:
  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu view_sub_menu;
  popup_menu help_sub_menu;

  status_bar status;

  pixmap hook_icon;
  pixmap cross_icon;

  over_view<6> filter_view;
  image_view full_image_view;
  thumb_view folder_view;
  over_view<8> mask_view;

  int curent_full_image_view;

  side_bar colors;

  bool learning_mode;

  std::thread background_thread;
  volatile bool is_active;

  cv::Mat raw_image;
  cv::Mat hsv_image;
  cv::Mat inverted_image;

  float portions[side_bar::COLOR_COUNT];

  cv::Mat image[5];

  data::redimage_settings settings;
  sys_fs::path settings_path;
  sys_fs::path image_path;
};

// --------------------------------------------------------------------------
RedImage::RedImage ()
  : super(20, 24, 260, 0)
//  , settings_path(sys_fs::absolute("redimage.xml"))
  , learning_mode(false)
  , curent_full_image_view(-1)
  , is_active(true)
{
  portions[0] = portions[1] = 0.0F;
  on_create(basepp::bind_method(this, &RedImage::onCreated));

  on_destroy([&]() {
    LogDebug << *this << " Destroyed!";
    win::quit_main_loop();
  });

  on_close(basepp::bind_method(this, &RedImage::quit));
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
    menu_entry("Open", 'o', basepp::bind_method(this, &RedImage::load), hot_key(keys::o, state::control), false),
    menu_entry("Save", 's', basepp::bind_method(this, &RedImage::save), hot_key(keys::s, state::control), false),
    menu_entry("SaveAs", 'a', basepp::bind_method(this, &RedImage::saveAs), hot_key(keys::s, state::control|state::shift), false),
    menu_entry("Open image", 'i', basepp::bind_method(this, &RedImage::open_image), hot_key(keys::l, state::control), true),
    menu_entry("Open directory", 'd', basepp::bind_method(this, &RedImage::open_folder), hot_key(keys::d, state::control), false),
    menu_entry("Exit", 'x', basepp::bind_method(this, &RedImage::quit), hot_key(keys::f4, state::alt), true)
  });

  const float icn_sz = core::global::scale<float>(20);
  core::rectangle icon_rect(0, 0, icn_sz, icn_sz);

  hook_icon = create_text_pixmap("X", icon_rect, color::dark_red);
  cross_icon = create_text_pixmap(" ", icon_rect, color::dark_green);

  edit_sub_menu.data.add_entries({
    menu_entry("Leaning mode", 'l', basepp::bind_method(this, &RedImage::toggle_learning), hot_key(keys::t, state::control), false, cross_icon),
    menu_entry("Calc all", 'a', basepp::bind_method(this, &RedImage::calc_all_and_show), hot_key(keys::a, state::control), false),
    menu_entry("Original", 'o', basepp::bind_method(this, &RedImage::show_raw), hot_key(keys::g, state::control), true),
    menu_entry("Invert", 'i', basepp::bind_method(this, &RedImage::show_inverted), hot_key(keys::i, state::control), false),
    menu_entry("Reset range", 'r', basepp::bind_method(this, &RedImage::reset_current_color_range), hot_key(keys::r, state::control), true),
  });

  view_sub_menu.data.add_entries({
    menu_entry("Filter View", 'r', basepp::bind_method(this, &RedImage::show_filter_view), hot_key(keys::v, state::control), false),
    menu_entry("Directory View", 'd', basepp::bind_method(this, &RedImage::show_folder_view), hot_key(keys::f, state::control), false),
    menu_entry("Masks View", 'm', basepp::bind_method(this, &RedImage::show_mask_view), hot_key(keys::m, state::control), false),
    menu_entry("Full View Image 1", '1', [&] () { RedImage::show_full_image(0); }, hot_key('1', state::control), true),
    menu_entry("Full View Image 2", '2', [&] () { RedImage::show_full_image(1); }, hot_key('2', state::control), false),
    menu_entry("Full View Image 3", '3', [&] () { RedImage::show_full_image(2); }, hot_key('3', state::control), false),
    menu_entry("Full View Image 4", '4', [&] () { RedImage::show_full_image(3); }, hot_key('4', state::control), false),
    menu_entry("Full View Image 5", '5', [&] () { RedImage::show_full_image(4); }, hot_key('5', state::control), false),
    menu_entry("Full View Image 6", '6', [&] () { RedImage::show_full_image(5); }, hot_key('6', state::control), false),
    menu_entry("Next Image", 'n', [&] () { RedImage::show_next(); }, hot_key(keys::right, state::none), true),
    menu_entry("Previous Image", 'P', [&] () { RedImage::show_prev(); }, hot_key(keys::left, state::none), false),
  });

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(*this, "About RedImage", "RedImage Version 0.0.1", "Ok");
    })
  );

  menu.create(*this);

  file_sub_menu.data.register_hot_keys(this);
  edit_sub_menu.data.register_hot_keys(this);
  view_sub_menu.data.register_hot_keys(this);

  colors.create(*this);
  status.create(*this);
  mask_view.create(*this);
  full_image_view.create(*this);
  folder_view.create(*this);
  filter_view.create(*this);

  for(int i = 0; i < 6; ++i) {
    filter_view.image_views[i].on_left_btn_dblclk([&, i] (os::key_state, const core::point) {
      show_full_image(i);
    });
    filter_view.image_views[i].on_mouse_move([&, i] (os::key_state, const core::point& pt) {
      show_hsv_value(get_hsv_at(filter_view.image_views[i].size(), pt));
    });
  }

  full_image_view.on_left_btn_dblclk([&] (os::key_state, const core::point) {
    show_filter_view();
  });
  full_image_view.on_left_btn_down([&] (os::key_state, const core::point& pt) {
    if (learning_mode) {
      set_hsv_range_for(get_hsv_range_at(full_image_view.size(), pt, core::size(5, 5)), curent_full_image_view - 1);
    }
  });
  full_image_view.on_mouse_move([&] (os::key_state, const core::point& pt) {
    show_hsv_value(get_hsv_at(full_image_view.size(), pt));
  });

  folder_view.on_selection_commit([&] () {
    auto idx = folder_view.get_selection();
    if (idx < folder_view.list.size()) {
      const auto& info = folder_view.list[idx];
      show_filter_view();
      load_image(info.filename);
    }
  });

  get_layout().set_center_top_bottom_left_right(&filter_view, &menu, &status, &colors, nullptr);

  for (int i = 0; i < side_bar::COLOR_COUNT; ++i) {
    filter_view.image_views[i + 1].on_left_btn_down([&, i] (os::key_state, const core::point& pt) {
      if (learning_mode) {
        set_hsv_range_for(get_hsv_range_at(filter_view.image_views[i + 1].size(), pt, core::size(5, 5)), i);
      }
    });
    colors.colors[i].hue.min_scroll.on_scroll([&, i](core::point::type){
      calc_color(i);
    });
    colors.colors[i].hue.max_scroll.on_scroll([&, i](core::point::type){
      calc_color(i);
    });
    colors.colors[i].saturation.min_scroll.on_scroll([&, i](core::point::type){
      calc_color(i);
    });
    colors.colors[i].saturation.max_scroll.on_scroll([&, i](core::point::type){
      calc_color(i);
    });
    colors.colors[i].value.min_scroll.on_scroll([&, i](core::point::type){
      calc_color(i);
    });
    colors.colors[i].value.max_scroll.on_scroll([&, i](core::point::type){
      calc_color(i);
    });
    colors.colors[i].hue.min_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image_and_show(i);
    });
    colors.colors[i].hue.max_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image_and_show(i);
    });
    colors.colors[i].saturation.min_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image_and_show(i);
    });
    colors.colors[i].saturation.max_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image_and_show(i);
    });
    colors.colors[i].value.min_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image_and_show(i);
    });
    colors.colors[i].value.max_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image_and_show(i);
    });
  }

  set_children_visible();
  full_image_view.to_back();
  folder_view.to_back();
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
void RedImage::set_hsv_range_for (const data::hsv_range& hsv, int i) {
  colors.colors[i].set(hsv);
  calc_image_and_show(i);
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
void RedImage::set_hsv_for (const cv::Vec3b& hsv, int i) {
  colors.colors[i].hue.set_range(hsv[0] - 1, hsv[0] + 1);
  colors.colors[i].saturation.set_range(70, 255);
  colors.colors[i].value.set_range(70, 255);
  calc_image_and_show(i);
}
//-----------------------------------------------------------------------------
void RedImage::show_full_image (int i) {
  cv::Mat src = (i == 0 ? raw_image : image[i - 1]);
  if (!src.empty()) {
    filter_view.to_back();
    folder_view.to_back();
    mask_view.to_back();
    full_image_view.to_front();
    get_layout().set_center(&full_image_view);
    super::layout();

    curent_full_image_view = i;
    full_image_view.set_image_and_scale(src);
  }
}
//-----------------------------------------------------------------------------
void RedImage::show_filter_view () {
  full_image_view.to_back();
  folder_view.to_back();
  mask_view.to_back();
  filter_view.to_front();
  get_layout().set_center(&filter_view);
  super::layout();
//  filter_view.layout();
  curent_full_image_view = -1;
}
//-----------------------------------------------------------------------------
void RedImage::show_folder_view () {
  full_image_view.to_back();
  filter_view.to_back();
  mask_view.to_back();
  folder_view.to_front();
  get_layout().set_center(&folder_view);
  super::layout();
}
//-----------------------------------------------------------------------------
void RedImage::show_mask_view () {
  full_image_view.to_back();
  folder_view.to_back();
  filter_view.to_back();
  mask_view.to_front();
  get_layout().set_center(&mask_view);
  super::layout();
//  mask_view.layout();
}
//-----------------------------------------------------------------------------
void RedImage::toggle_learning () {
  learning_mode = !learning_mode;
  edit_sub_menu.data[0].set_icon(learning_mode ? hook_icon : cross_icon);
  calc_status();
}
//-----------------------------------------------------------------------------
void RedImage::show_raw () {
  filter_view.image_views[0].set_image_and_scale(raw_image);
}
//-----------------------------------------------------------------------------
void RedImage::show_inverted () {
  filter_view.image_views[0].set_image_and_scale(inverted_image);
}
//-----------------------------------------------------------------------------
void isolateChannel (const cv::Mat& in, cv::Mat& out, int channel) {
  const auto blank = cv::Scalar(0);
  cv::Mat channels[3];
  cv::split(in, channels);
  switch (channel) {
    case 0:
      channels[1] = blank;
      channels[2] = blank;
      break;
    case 1:
      channels[0] = blank;
      channels[2] = blank;
      break;
    case 2:
      channels[0] = blank;
      channels[1] = blank;
      break;
    default:
      break;
  }
  cv::merge(channels, 3, out);
}
//-----------------------------------------------------------------------------
void RedImage::show_image (int i) {
  filter_view.image_views[i + 1].set_image_and_scale(image[i]);
}
//-----------------------------------------------------------------------------
void RedImage::calc_color (int i) {
  auto h = colors.colors[i].hue.get();
  auto s = colors.colors[i].saturation.get();
  auto v = colors.colors[i].value.get();
  colors.colors[i].colors.min_color.set_hsv_color(h.min(), std::max((byte)128, s.min()), std::max((byte)128, v.min()));
  colors.colors[i].colors.max_color.set_hsv_color(h.max(), s.max(), v.max());
}
//-----------------------------------------------------------------------------
void RedImage::calc_image (int i) {
  if (hsv_image.empty()) {
    return;
  }

  color_filter proc(colors.colors[i].get());

  cv::Mat mask[8];
  portions[i] = proc.calc(hsv_image, mask, true);

  image[i].setTo(cv::Scalar(0, 0, 0));
  raw_image.copyTo(image[i], mask[0]);
  mask[0].copyTo(image[i + 3]);
  if (i == 0) {
    for (int m = 0; m < 8; ++m) {
      mask_view.image_views[m].set_image_and_scale(mask[m]);
    }
  }
}
//-----------------------------------------------------------------------------
void RedImage::calc_image_and_show (int i, bool calc_rest_img) {
  if (hsv_image.empty()) {
    return;
  }
  calc_image(i);

  show_image(i);
  show_image(i + 3);
  if (calc_rest_img) {
    calc_rest();
  }
  calc_status();
  if (curent_full_image_view > -1) {
    show_full_image(curent_full_image_view);
  }
}
//-----------------------------------------------------------------------------
void RedImage::calc_rest () {
  cv::Mat mask = image[3] + image[4];
  image[2].setTo(cv::Scalar(255, 255, 255));
  raw_image.copyTo(image[2], ~mask);

  show_image(2);
}
//-----------------------------------------------------------------------------
void RedImage::calc_all_and_show () {
  if (get_layout().get_center() == &folder_view) {
    calc_folder();
  } else {
    for (int i = 0; i < 2; ++i) {
      calc_image_and_show(i, false);
    }
    calc_rest();
  }
}
//-----------------------------------------------------------------------------
void RedImage::reset_current_color_range () {
  if ((curent_full_image_view > 0) && (curent_full_image_view < 3)) {
    reset_color_range(curent_full_image_view - 1);
  }
}
//-----------------------------------------------------------------------------
void RedImage::reset_color_range (int i) {
  colors.colors[i].reset();
  calc_image(i);
}
//-----------------------------------------------------------------------------
void RedImage::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
//      if (!settings_path.empty()) {
//        save();
//      }
      is_active = false;
      if (background_thread.joinable()) {
        background_thread.join();
      }
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}
//-----------------------------------------------------------------------------
bool is_jpeg (const sys_fs::directory_entry& i) {
  auto ext = i.path().extension();
  return !(ext == ".jpg") && !(ext == ".jpeg") && !(ext == ".JPG") && !(ext == ".JPEG");
}
//-----------------------------------------------------------------------------
bool is_xml (const sys_fs::directory_entry& i) {
  auto ext = i.path().extension();
  return !(ext == ".xml") && !(ext == ".XML");
}
//-----------------------------------------------------------------------------
void RedImage::open_image () {
  auto old_path = sys_fs::current_path();
  if (!settings.last_path().empty() && sys_fs::exists(settings.last_path())) {
    sys_fs::current_path(settings.last_path());
  }
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    load_image(file);
  }, is_jpeg);
  sys_fs::current_path(old_path);
}
// --------------------------------------------------------------------------
void load_image (const sys_fs::path& file, cv::Mat& raw, cv::Mat& hsv) {
  cv::Mat srcImage = cv::imread(file.string(), cv::IMREAD_COLOR);
  cv::resize(srcImage, raw, cv::Size(1500, 1000), 0, 0, cv::INTER_LINEAR);
  cv::cvtColor(raw, hsv, cv::COLOR_BGR2HSV);
}
// --------------------------------------------------------------------------
void RedImage::load_image (const sys_fs::path& file) {
  if (sys_fs::exists(file)) {
    image_path = file;
    portions[0] = portions[1] = 0.0F;
    calc_title();
    settings.last_path(file.parent_path().string());

    ::load_image(file, raw_image, hsv_image);

    raw_image.copyTo(image[0]);
    raw_image.copyTo(image[1]);
    raw_image.copyTo(image[2]);

    inverted_image = ~raw_image;
    show_raw();
    calc_all_and_show();
  }
}
// --------------------------------------------------------------------------
void RedImage::load () {
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    settings_path = file;
    portions[0] = portions[1] = 0.0F;
    calc_title();
    calc_status();

    using boost::property_tree::ptree;

    ptree xml_main;
    try {
      xml::read_xml(settings_path.string(), xml_main, xml::no_comments);
      auto opt = xml_main.get_child_optional("redimage");
      if (opt) {
        settings.read(opt.get());
        colors.set(settings.colors());
      }
    } catch (std::exception& ex) {
      LogWarng << "Exception while reading redimage.xml:" << ex.what();
    }
  }, is_xml);
}
// --------------------------------------------------------------------------
void RedImage::save () {
  if (settings_path.empty()) {
    saveAs();
  } else {
    using boost::property_tree::ptree;

    settings.colors(colors.get());
    ptree main;
    settings.write(main);
    ptree xml_main;
    xml_main.put_child("redimage", main);

    boost::property_tree::xml_writer_settings<ptree::key_type> xml_settings('\t', 1);
    xml::write_xml(settings_path.string(), xml_main, std::locale(), xml_settings);
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
  if (portions[0] != 0.0F) {
    buffer << "Bad: " << (portions[0] * 100.0F) << "%";
  }
  if (portions[1] != 0.0F) {
    if (portions[0] != 0.0F) {
      buffer << " - ";
    }
    buffer << "Off: " << (portions[1] * 100.0F) << "%";
  }
  if ((portions[0] != 0.0F) && (portions[1] != 0.0F)) {
    float good = 1.0F - (portions[0] + portions[1]);
    buffer << " - Good: " << (good * 100.0F) << "% - Quality: " << (portions[0] /*/ good*/) * 100.0F << "%";
  }
  status.labels[1].set_text(buffer.str());
}
//-----------------------------------------------------------------------------
void RedImage::show_hsv_value (const cv::Vec3b& hsv) {
  std::ostringstream buffer;
  buffer << "H: " << (int)hsv[0] << " S: " << (int)hsv[1] << " V: " << (int)hsv[2];
  status.labels[2].set_text(buffer.str());
  status.color.set_hsv_color(hsv[0], hsv[1], hsv[2]);
}
// --------------------------------------------------------------------------
void RedImage::calc_folder () {
  typedef background_worker<image_info*> worker;
  worker calculation_threads;

  for (auto& info : folder_view.list) {
    calculation_threads.add(&info);
  }

  calculation_threads.start([&] (worker::queue& queue) {
    color_filter bad_filter(colors.colors[0].get());
    color_filter off_filter(colors.colors[1].get());

    image_info* info = nullptr;
    while (queue.try_dequeue(info)) {

      if (!is_active) {
        return;
      }

      cv::Mat raw, hsv;
      ::load_image(info->filename, raw, hsv);

      const float bad_portion = bad_filter.calc(hsv);
      const float off_portion = off_filter.calc(hsv);

      info->bad = bad_portion * 100.0F;
      info->off = off_portion * 100.0F;
      info->good = (1.0F - (bad_portion + off_portion)) * 100.0F;
      info->quality = info->bad;// / info.good * 100.0F;

      // thumbnail
      auto native_view_size = core::global::scale(folder_view.get_item_size() - core::size(4, 4));
      cv::Size thumb_size(native_view_size.width() / 2, native_view_size.height());
      cv::Mat thumb(thumb_size, raw.type());
      cv::resize(raw, thumb, thumb_size, 0, 0, cv::INTER_LINEAR);
      info->image = cvMat2pixmap(thumb);

      win::run_on_main([&] () {
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
      show_folder_view();
      folder_view.list.clear();
      for (const auto& i : 
#ifdef WIN32
           sys_fs::directory_iterator(folder)) {

#endif // WIN32
#ifdef X11
           sys_fs::directory_iterator(folder, sys_fs::directory_options::skip_permission_denied)) {
#endif // X11
        if (!is_jpeg(i)) {
          folder_view.list.emplace_back(image_info(i.path()));
        }
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

  main.create({50, 50, 800, 600});
  main.calc_title();
  main.set_visible();
  main.maximize();

  return run_main_loop();
}
