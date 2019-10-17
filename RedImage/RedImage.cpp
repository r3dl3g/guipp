
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/tile_view.h>
#include <gui/win/grid_layout.h>
#include <gui/win/lineup_layout.h>
#include <gui/win/attach_layout.h>
#include <base/string_util.h>
#include <persistent/ptree_persistent.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;

namespace xml = boost::property_tree::xml_parser;


pixmap cvMat2pixmap (const cv::Mat& source) {
  auto sz = source.size();
  core::native_size nsize = {(uint32_t)sz.width, (uint32_t)sz.height};

  if (CV_8UC3 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), source.cols * source.elemSize(), PixelFormat::RGB);

    typedef const_image_data<PixelFormat::RGB> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<PixelFormat::RGB>(img_data(data, info));
    return image;
  } else if (CV_8UC4 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), source.cols * source.elemSize(), PixelFormat::RGBA);

    typedef const_image_data<PixelFormat::RGBA> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<PixelFormat::RGBA>(img_data(data, info));
    return image;
  } else if (CV_8UC1 == source.type()) {
    bitmap_info info(nsize.width(), nsize.height(), source.cols * source.elemSize(), PixelFormat::GRAY);

    typedef const_image_data<PixelFormat::GRAY> img_data;
    img_data::raw_type data(source.data, info.mem_size());

    pixmap image = datamap<PixelFormat::GRAY>(img_data(data, info));
    return image;
  }

  return pixmap();
}
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
namespace data {

  // --------------------------------------------------------------------------
  struct range : public persistent::ptree_struct<persistent::byte, persistent::byte> {
    typedef persistent::ptree_struct<persistent::byte, persistent::byte> super;

    static const char s_min[];
    static const char s_max[];

    range (byte min = 0, byte max = 255);
    range (const range&);

    persistent::byte min;
    persistent::byte max;
  };

  const char range::s_min[] = "min";
  const char range::s_max[] = "max";

  range::range (byte min_, byte max_)
    : super(min, max)
    , min(s_min, min_)
    , max(s_max, max_)
  {}

  range::range (const range& rhs)
    : range()
  {
    super::operator=(rhs);
  }

  // --------------------------------------------------------------------------
  struct rgb_range : public persistent::ptree_struct<persistent::type<range>,
                                                     persistent::type<range>,
                                                     persistent::type<range>> {
    typedef persistent::ptree_struct<persistent::type<range>,
                                     persistent::type<range>,
                                     persistent::type<range>> super;

    static const char s_red[];
    static const char s_green[];
    static const char s_blue[];

    rgb_range (const range& r = range(), const range& g = range(), const range& b = range());
    rgb_range (const rgb_range&);

    persistent::type<range> red;
    persistent::type<range> green;
    persistent::type<range> blue;
  };

  const char rgb_range::s_red[] = "red";
  const char rgb_range::s_green[] = "green";
  const char rgb_range::s_blue[] = "blue";

  rgb_range::rgb_range (const range& r, const range& g, const range& b)
    : super(red, green, blue)
    , red(s_red, r)
    , green(s_green, g)
    , blue(s_blue, b)
  {}

  rgb_range::rgb_range (const rgb_range& rhs)
    : rgb_range()
  {
    super::operator=(rhs);
  }

  // --------------------------------------------------------------------------
  struct hsv_range : public persistent::ptree_struct<persistent::type<range>,
                                                     persistent::type<range>,
                                                     persistent::type<range>> {
    typedef persistent::ptree_struct<persistent::type<range>,
                                     persistent::type<range>,
                                     persistent::type<range>> super;

    static const char s_hue[];
    static const char s_saturation[];
    static const char s_value[];

    hsv_range (const range& h = range(0, 180), const range& s = range(), const range& v = range());
    hsv_range (const hsv_range&);

    persistent::type<range> hue;
    persistent::type<range> saturation;
    persistent::type<range> value;
  };

  const char hsv_range::s_hue[] = "hue";
  const char hsv_range::s_saturation[] = "saturation";
  const char hsv_range::s_value[] = "value";

  hsv_range::hsv_range (const range& h, const range& s, const range& v)
    : super(hue, saturation, value)
    , hue(s_hue, h)
    , saturation(s_saturation, s)
    , value(s_value, v)
  {}

  hsv_range::hsv_range (const hsv_range& rhs)
    : hsv_range()
  {
    super::operator=(rhs);
  }

  // --------------------------------------------------------------------------
  struct color_sets : public persistent::ptree_struct<persistent::vector<hsv_range>> {
    typedef persistent::ptree_struct<persistent::vector<hsv_range>> super;

    static const char s_ranges[];

    color_sets ();
    color_sets (const color_sets&);

    persistent::vector<hsv_range> ranges;
  };

  const char color_sets::s_ranges[] = "ranges";

  color_sets::color_sets ()
    : super(ranges)
    , ranges(s_ranges)
  {
    ranges().resize(5);
  }

  color_sets::color_sets (const color_sets& rhs)
    : color_sets()
  {
    super::operator=(rhs);
  }

  // --------------------------------------------------------------------------
  struct redimage_settings : public persistent::ptree_struct<persistent::type<color_sets>,
                                                             persistent::string> {
    typedef persistent::ptree_struct<persistent::type<color_sets>,
                                     persistent::string> super;

    static const char s_color_sets[];
    static const char s_last_path[];

    redimage_settings ();
    redimage_settings (const redimage_settings&);

    persistent::type<color_sets> colors;
    persistent::string last_path;
  };

  const char redimage_settings::s_color_sets[] = "color_sets";
  const char redimage_settings::s_last_path[] = "last_path";

  redimage_settings::redimage_settings ()
    : super(colors, last_path)
    , colors(s_color_sets)
    , last_path(s_last_path)
  {}

  redimage_settings::redimage_settings (const redimage_settings& rhs)
    : redimage_settings()
  {
    super::operator=(rhs);
  }

} // namespace data

// --------------------------------------------------------------------------
class image_view : public control {
public:
  typedef control super;
  typedef no_erase_window_class<image_view> clazz;

  image_view ();

  void create (const win::container& parent,
               const core::rectangle& place = core::rectangle::def);

  void paint (const graphics& graph);
  void set_image (const pixmap& source);
  void set_image_and_scale (const cv::Mat& source);

private:
  pixmap image;
};

// --------------------------------------------------------------------------
void image_view::create (const win::container& parent,
                         const core::rectangle& place) {
  super::create(clazz::get(), parent, place);
}

// --------------------------------------------------------------------------
void image_view::paint (const graphics& graph) {
  graph.clear(color::dark_gray);
  if (image.is_valid()) {
    graph.copy_from(image);
  }
}

// --------------------------------------------------------------------------
void image_view::set_image (const pixmap& source) {
  image = source;
  invalidate();
}

// --------------------------------------------------------------------------
void image_view::set_image_and_scale (const cv::Mat& src) {
  auto native_size = core::global::scale(size());
  cv::Size sz(native_size.width(), native_size.height());
  cv::Mat target = cv::Mat(sz, src.type());
  cv::resize(src, target, sz, 0, 0, cv::INTER_NEAREST);
  set_image(cvMat2pixmap(target));
}

// --------------------------------------------------------------------------
image_view::image_view () {
  on_paint(draw::paint(this, &image_view::paint));
}

// --------------------------------------------------------------------------
class color_view : public control {
public:
  typedef control super;
  typedef no_erase_window_class<color_view> clazz;

  color_view ();

  void create (const win::container& parent,
               const core::rectangle& place = core::rectangle::def);

  void paint (const graphics& graph);
  void set_rgb_color (byte r, byte g, byte b);
  void set_hsv_color (byte h, byte s, byte v);

  os::color value;
};
// --------------------------------------------------------------------------
color_view::color_view ()
  : value(color::black) {
  on_paint(draw::paint(this, &color_view::paint));
}
// --------------------------------------------------------------------------
void color_view::create (const win::container& parent,
                         const core::rectangle& place) {
  super::create(clazz::get(), parent, place);
}

// --------------------------------------------------------------------------
void color_view::paint (const graphics& graph) {
  graph.clear(value);
}
// --------------------------------------------------------------------------
void color_view::set_rgb_color (byte r, byte g, byte b) {
  value = color::calc_rgb(r, g, b);
  invalidate();
}
// --------------------------------------------------------------------------
void color_view::set_hsv_color (byte h, byte s, byte v) {
  cv::Mat hsv(1, 1, CV_8UC3);
  hsv.setTo(cv::Scalar(h, s, v));
  cv::Mat rgb(1, 1, CV_8UC3);
  cv::cvtColor(hsv, rgb, cv::COLOR_HSV2RGB);
  cv::Vec3b v3 = rgb.at<cv::Vec3b>(0, 0);
  set_rgb_color(v3[0], v3[1], v3[2]);
}
// --------------------------------------------------------------------------
class min_max_group : public win::group_window<layout::horizontal_lineup<52>> {
public:
  typedef win::group_window<layout::horizontal_lineup<52>> super;

  min_max_group (byte min = 0, byte max = 255);

  label main_label;
  label_right min_label;
  edit_left min_edit;
  label_right max_label;
  edit_left max_edit;
};
// --------------------------------------------------------------------------
min_max_group::min_max_group (byte min, byte max) {
  on_create([&, min, max] (window*, const core::rectangle&) {
    main_label.create(*this);
    min_label.create(*this, "Min:");
    min_edit.create(*this, ostreamfmt((int)min));
    max_label.create(*this, "Max:");
    max_edit.create(*this, ostreamfmt((int)max));
  });
}
// --------------------------------------------------------------------------
class color_key : public win::group_window<layout::vertical_lineup<20>> {
public:
  typedef win::group_window<layout::vertical_lineup<20>> super;

  color_key (byte min = 0, byte max = 255);

  void set (const std::string& name, const data::range& value);
  void set_min (byte value);
  void set_max (byte value);
  void set_range (byte min, byte max);

  data::range get () const;
  byte get_min () const;
  byte get_max () const;

  min_max_group min_max;
  horizontal_scroll_bar min_scroll;
  horizontal_scroll_bar max_scroll;
};

// --------------------------------------------------------------------------
color_key::color_key (byte min, byte max)
  : min_max(min, max)
{
  on_create([&, min, max] (window*, const core::rectangle&) {
    min_max.create(*this);
    min_scroll.create(*this);
    min_scroll.set_min_max(min, max);
    max_scroll.create(*this);
    max_scroll.set_min_max(min, max);
  });
  min_scroll.on_scroll([&] (const core::point::type) {
    min_max.min_edit.set_text(ostreamfmt((int)min_scroll.get_value()));
  });
  max_scroll.on_scroll([&] (const core::point::type) {
    min_max.max_edit.set_text(ostreamfmt((int)max_scroll.get_value()));
  });
  min_max.min_edit.on_content_changed([&] () {
    int value = 0;
    std::istringstream(min_max.min_edit.get_text()) >> value;
    min_scroll.set_value(value, false);
  });
  min_max.max_edit.on_content_changed([&] () {
    int value = 0;
    std::istringstream(min_max.max_edit.get_text()) >> value;
    max_scroll.set_value(value, false);
  });
}
// --------------------------------------------------------------------------
void color_key::set (const std::string& name, const data::range& value) {
  min_max.main_label.set_text(name);
  set_min(value.min());
  set_max(value.max());
}
// --------------------------------------------------------------------------
data::range color_key::get () const {
  return data::range(get_min(), get_max());
}
// --------------------------------------------------------------------------
void color_key::set_min (byte value) {
  min_max.min_edit.set_text(ostreamfmt((int)value));
  min_scroll.set_value(value, false);
}
// --------------------------------------------------------------------------
void color_key::set_max (byte value) {
  min_max.max_edit.set_text(ostreamfmt((int)value));
  max_scroll.set_value(value, false);
}
// --------------------------------------------------------------------------
void color_key::set_range (byte min, byte max) {
  set_min(min);
  set_max(max);
}
// --------------------------------------------------------------------------
byte color_key::get_min () const {
  return (byte)min_scroll.get_value();
}
// --------------------------------------------------------------------------
byte color_key::get_max () const {
  return (byte)max_scroll.get_value();
}
// --------------------------------------------------------------------------
class color_group : public win::group_window<layout::horizontal_lineup<128, 1, 1>> {
public:
  typedef win::group_window<layout::horizontal_lineup<128, 1, 1>> super;

  color_group () {
    on_create([&] (window*, const core::rectangle&) {
      min_color.create(*this);
      max_color.create(*this);
    });
  }

  color_view min_color;
  color_view max_color;
};
// --------------------------------------------------------------------------
class color_key_group : public win::group_window<layout::vertical_lineup<60, 0, 1>> {
public:
  typedef win::group_window<layout::vertical_lineup<80, 0, 2>> super;

  color_key_group ();

  void set (const data::hsv_range&);
  data::hsv_range get () const;

  color_key hue;
  color_key saturation;
  color_key value;
  color_group colors;
};
// --------------------------------------------------------------------------
color_key_group::color_key_group ()
  : hue(0, 180)
  , saturation()
  , value()
{
  on_create([&] (window*, const core::rectangle&) {
    hue.create(*this);
    saturation.create(*this);
    value.create(*this);
    colors.create(*this);
  });
}
// --------------------------------------------------------------------------
void color_key_group::set (const data::hsv_range& hsv) {
  hue.set("H", hsv.hue());
  saturation.set("S", hsv.saturation());
  value.set("V", hsv.value());
  colors.min_color.set_hsv_color(hsv.hue().min(), hsv.saturation().min(), hsv.value().min());
  colors.max_color.set_hsv_color(hsv.hue().max(), hsv.saturation().max(), hsv.value().max());
}
// --------------------------------------------------------------------------
data::hsv_range color_key_group::get () const {
  return data::hsv_range(hue.get(), saturation.get(), value.get());
}
// --------------------------------------------------------------------------
class side_bar : public win::group_window<layout::vertical_lineup<244, 0, 2>> {
public:
  typedef win::group_window<layout::vertical_lineup<244, 0, 10>> super;

  enum {
    COLOR_COUNT = 2
  };

  side_bar ();

  void set (const data::color_sets&);
  data::color_sets get () const;

  std::vector<color_key_group> colors;

};
// --------------------------------------------------------------------------
side_bar::side_bar () {
  colors.resize(COLOR_COUNT);
  on_create([&] (window*, const core::rectangle&) {
    for (auto i = colors.begin(), e = colors.end(); i != e; ++i) {
      i->create(*this);
    }
  });
}
// --------------------------------------------------------------------------
void side_bar::set (const data::color_sets& s) {
  for (auto i = colors.begin(), e = colors.end(); i != e; ++i) {
    i->set(s.ranges()[std::distance(colors.begin(), i)]);
  }
}
// --------------------------------------------------------------------------
data::color_sets side_bar::get () const {
  data::color_sets sets;
  for (auto i = colors.begin(), e = colors.end(); i != e; ++i) {
    sets.ranges()[std::distance(colors.begin(), i)] = i->get();
  }
  return sets;
}
// --------------------------------------------------------------------------
struct image_info {
  image_info ();

  void set_info (float good, float bad, float off, float quality);

  pixmap image;
  float good;
  float bad;
  float off;
  float quality;

  sys_fs::path filename;

};
// --------------------------------------------------------------------------
image_info::image_info ()
  : good(0)
  , bad(0)
  , off(0)
  , quality(0)
{}
// --------------------------------------------------------------------------
void image_info::set_info (float good, float bad, float off, float quality) {
  this->good = good;
  this->bad = bad;
  this->off = off;
  this->quality = quality;
}
// --------------------------------------------------------------------------
template<draw::frame::drawer F = draw::frame::sunken_relief>
void image_info_drawer (const image_info& info,
                        const draw::graphics& g,
                        const core::rectangle& place,
                        const draw::brush& background,
                        bool selected,
                        bool hilited) {
  using namespace draw;
  g.fill(draw::rectangle(place), background);
  g.copy_from(info.image, core::point(2, 2));
  g.text(draw::text_box(info.filename.filename().string(), core::rectangle(160, 0, 100, 20)), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Good: " << info.good), core::rectangle(160, 20, 100, 20)), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Bad: " << info.bad), core::rectangle(160, 40, 100, 20)), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Off: " << info.off), core::rectangle(160, 60, 100, 20)), font::system(), color::black);
  g.text(draw::text_box(ostreamfmt("Quality: " << info.quality), core::rectangle(160, 80, 100, 20)), font::system(), color::black);
  frame::raised_relief(g, place);
}

// --------------------------------------------------------------------------
class thumb_view : public ctrl::vertical_tile_view {
public:
  typedef ctrl::vertical_tile_view super;
  typedef simple_list_data<image_info, image_info_drawer> image_info_list;

  thumb_view ();

  image_info_list list;
};
thumb_view::thumb_view () {
  set_drawer(list);
}
// --------------------------------------------------------------------------
class over_view : public win::group_window<layout::grid_adaption<3, 2, 0, 1>> {
public:
  typedef win::group_window<layout::grid_adaption<3, 2, 0, 1>> super;

  image_view image_views[6];

};
// --------------------------------------------------------------------------
class RedImage : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;
  RedImage ();

  void onCreated (window*, const core::rectangle&);
  void loadImage ();
  void quit ();

  void show_raw ();
  void show_inverted ();

  void show_image (int i);
  void calc_image (int i, bool calc_rest_img = true);

  cv::Vec3b get_hsv_at (const core::size& win_size, const core::point& pt);
  void set_hsv_for (const cv::Vec3b& hsv, int i);

  void calc_all ();
  void calc_rest ();

  void calc_color (int i);

  void show_full_image (int i);
  void show_filter_view ();

  void load ();
  void save ();
  void saveAs ();

  void toggle_learning ();
  void calc_title ();

private:
  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu view_sub_menu;
  popup_menu help_sub_menu;

  pixmap hook_icon;
  pixmap cross_icon;

  over_view filter_view;
  image_view full_image_view;

  int curent_full_image_view;

  side_bar colors;

  bool learning_mode;

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
  : super(20, 0, 260, 0)
//  , settings_path(sys_fs::absolute("redimage.xml"))
  , learning_mode(false)
  , curent_full_image_view(-1)
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
    menu_entry("Open image", 'i', basepp::bind_method(this, &RedImage::loadImage), hot_key(keys::l, state::control), true),
    menu_entry("Exit", 'x', basepp::bind_method(this, &RedImage::quit), hot_key(keys::f4, state::alt), true)
  });

  const float icn_sz = core::global::scale<float>(20);
  core::rectangle icon_rect(0, 0, icn_sz, icn_sz);

  hook_icon = create_text_pixmap("X", icon_rect, color::dark_red);
  cross_icon = create_text_pixmap(" ", icon_rect, color::dark_green);

  edit_sub_menu.data.add_entries({
    menu_entry("Leaning mode", 'l', basepp::bind_method(this, &RedImage::toggle_learning), hot_key(keys::t, state::control), false, cross_icon),
    menu_entry("Calc all", 'a', basepp::bind_method(this, &RedImage::calc_all), hot_key(keys::a, state::control), false),
    menu_entry("Raw", 'r', basepp::bind_method(this, &RedImage::show_raw), hot_key(keys::r, state::control), true),
    menu_entry("Invert", 'i', basepp::bind_method(this, &RedImage::show_inverted), hot_key(keys::i, state::control), false),
  });

  view_sub_menu.data.add_entries({
    menu_entry("Filter View", 'r', basepp::bind_method(this, &RedImage::show_filter_view), hot_key(keys::f, state::control), false),
    menu_entry("Full View Image 1", '1', [&] () { RedImage::show_full_image(0); }, hot_key('1', state::control), false),
    menu_entry("Full View Image 2", '2', [&] () { RedImage::show_full_image(1); }, hot_key('2', state::control), false),
    menu_entry("Full View Image 3", '3', [&] () { RedImage::show_full_image(2); }, hot_key('3', state::control), false),
    menu_entry("Full View Image 4", '4', [&] () { RedImage::show_full_image(3); }, hot_key('4', state::control), false),
    menu_entry("Full View Image 5", '5', [&] () { RedImage::show_full_image(4); }, hot_key('5', state::control), false),
    menu_entry("Full View Image 6", '6', [&] () { RedImage::show_full_image(5); }, hot_key('6', state::control), false),
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

  filter_view.create(*this);
  for(int i = 0; i < 6; ++i) {
    filter_view.image_views[i].create(filter_view);
    filter_view.image_views[i].on_left_btn_dblclk([&, i] (os::key_state, const core::point) {
      show_full_image(i);
    });
  }
  colors.create(*this);
  full_image_view.create(*this);
  full_image_view.on_left_btn_dblclk([&] (os::key_state, const core::point) {
    show_filter_view();
  });
  full_image_view.on_left_btn_down([&] (os::key_state, const core::point& pt) {
    if (learning_mode) {
      set_hsv_for(get_hsv_at(full_image_view.size(), pt), curent_full_image_view - 1);
    }
  });

  get_layout().set_top(&menu);
  get_layout().set_center(&filter_view);
  get_layout().set_left(&colors);

  for (int i = 0; i < side_bar::COLOR_COUNT; ++i) {
    filter_view.image_views[i + 1].on_left_btn_down([&, i] (os::key_state, const core::point& pt) {
      if (learning_mode) {
        set_hsv_for(get_hsv_at(filter_view.image_views[i + 1].size(), pt), i);
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
      calc_image(i);
    });
    colors.colors[i].hue.max_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image(i);
    });
    colors.colors[i].saturation.min_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image(i);
    });
    colors.colors[i].saturation.max_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image(i);
    });
    colors.colors[i].value.min_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image(i);
    });
    colors.colors[i].value.max_scroll.on_left_btn_up([&, i](os::key_state, core::point){
      calc_image(i);
    });
  }

  set_children_visible();
  full_image_view.to_back();
}
//-----------------------------------------------------------------------------
cv::Vec3b RedImage::get_hsv_at (const core::size& win_size, const core::point& pt) {
  const auto img_size = hsv_image.size();
  const int x = (int)(pt.x() / win_size.width() * (float)img_size.width);
  const int y = (int)(pt.y() / win_size.height() * (float)img_size.height);
  return hsv_image.at<cv::Vec3b>(cv::Point(x, y));
}
//-----------------------------------------------------------------------------
void RedImage::set_hsv_for (const cv::Vec3b& hsv, int i) {
  colors.colors[i].hue.set_range(hsv[0] - 1, hsv[0] + 1);
  colors.colors[i].saturation.set_range(70, 255);
  colors.colors[i].value.set_range(70, 255);
  calc_image(i);
}
//-----------------------------------------------------------------------------
void RedImage::show_full_image (int i) {
  cv::Mat src = (i == 0 ? raw_image : image[i - 1]);
  if (!src.empty()) {
    filter_view.to_back();
    full_image_view.to_front();
    get_layout().set_center(&full_image_view);
    super::layout();

    full_image_view.set_image_and_scale(src);
    curent_full_image_view = i;
  }
}
//-----------------------------------------------------------------------------
void RedImage::show_filter_view () {
  full_image_view.to_back();
  filter_view.to_front();
  get_layout().set_center(&filter_view);
  super::layout();
  filter_view.layout();
  curent_full_image_view = -1;
}
//-----------------------------------------------------------------------------
void RedImage::toggle_learning () {
  learning_mode = !learning_mode;
  edit_sub_menu.data[0].set_icon(learning_mode ? hook_icon : cross_icon);
  calc_title();
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
void RedImage::calc_image (int i, bool calc_rest_img) {
  if (hsv_image.empty()) {
    return;
  }
  auto h = colors.colors[i].hue.get();
  auto s = colors.colors[i].saturation.get();
  auto v = colors.colors[i].value.get();

  image[i].setTo(cv::Scalar(0, 0, 0));

  cv::Mat mask;
  if (h.min() > h.max()) {
    cv::Mat mask2;
    cv::inRange(hsv_image,
                cv::Scalar(h.min(), s.min(), v.min()),
                cv::Scalar(180, s.max(), v.max()),
                mask2);
    cv::inRange(hsv_image,
                cv::Scalar(0, s.min(), v.min()),
                cv::Scalar(h.max(), s.max(), v.max()),
                mask);
    mask = mask + mask2;
  } else {
    cv::inRange(hsv_image,
                cv::Scalar(h.min(), s.min(), v.min()),
                cv::Scalar(h.max(), s.max(), v.max()),
                mask);
  }

  int mask_count = cv::countNonZero(mask);
  int full_count = mask.cols * mask.rows;
  portions[i] = (float)((double)mask_count / (double)full_count);

  raw_image.copyTo(image[i], mask);
  mask.copyTo(image[i + 3]);

  show_image(i);
  show_image(i + 3);
  if (calc_rest_img) {
    calc_rest();
  }
  calc_title();
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
void RedImage::calc_all () {
  for (int i = 0; i < 2; ++i) {
    calc_image(i, false);
  }
  calc_rest();
}
//-----------------------------------------------------------------------------
void RedImage::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      if (!settings_path.empty()) {
        save();
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
void RedImage::loadImage () {
  auto old_path = sys_fs::current_path();
  if (!settings.last_path().empty() && sys_fs::exists(settings.last_path())) {
    sys_fs::current_path(settings.last_path());
  }
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      image_path = file;
      portions[0] = portions[1] = 0.0F;
      calc_title();
      settings.last_path(file.parent_path().string());
      cv::Mat srcImage = cv::imread(file.string(), cv::IMREAD_COLOR);
      auto size = core::global::scale(filter_view.image_views[0].size());
      cv::Size sz(size.width(), size.height());
      raw_image = cv::Mat(sz, srcImage.type());
      cv::resize(srcImage, raw_image, sz, 0, 0, cv::INTER_LINEAR);

      cv::cvtColor(raw_image, hsv_image, cv::COLOR_BGR2HSV);

      raw_image.copyTo(image[0]);
      raw_image.copyTo(image[1]);
      raw_image.copyTo(image[2]);

      inverted_image = ~raw_image;
      show_raw();
      calc_all();
    }
  }, is_jpeg);
  sys_fs::current_path(old_path);
}
// --------------------------------------------------------------------------
void RedImage::load () {
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    settings_path = file;
    portions[0] = portions[1] = 0.0F;
    calc_title();

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
void RedImage::calc_title () {
  std::ostringstream buffer;
  buffer << "RedImage";
  if (!settings_path.empty()) {
    buffer << " - " << settings_path.filename();
  }
  if (!image_path.empty()) {
    buffer << " - " << image_path.filename();
  }
  if (learning_mode) {
    buffer << " - Learning Mode";
  }
  if (portions[0] != 0.0F) {
    buffer << " - Bad: " << (portions[0] * 100.0F) << "%";
  }
  if (portions[1] != 0.0F) {
    buffer << " - Off: " << (portions[1] * 100.0F) << "%";
  }
  if ((portions[0] != 0.0F) && (portions[1] != 0.0F)) {
    float good = 1.0F - (portions[0] + portions[1]);
    buffer << " - Good: " << (good * 100.0F) << "% - Quality: " << (portions[0] / good) * 100.0F << "%";
  }

  set_title(buffer.str());
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
