
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/menu.h>
#include <gui/ctrl/virtual_view.h>
#include <gui/win/grid_layout.h>
#include <gui/win/lineup_layout.h>
#include <base/string_util.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include <opencv2/imgproc/imgproc.hpp>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;

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
class image_view : public control {
public:
  typedef control super;
  typedef no_erase_window_class<image_view> clazz;

  image_view ();

  void create (const win::container& parent,
               const core::rectangle& place = core::rectangle::def);

  void paint (const graphics& graph);
  void set_image (const pixmap& source);

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
image_view::image_view () {
  on_paint(draw::paint(this, &image_view::paint));
}

// --------------------------------------------------------------------------
class min_max_group : public win::group_window<layout::horizontal_lineup<65>> {
public:
  typedef win::group_window<layout::horizontal_lineup<65>> super;

  min_max_group ();

  label min_label;
  label max_label;
  edit_right min_edit;
  edit_right max_edit;
};

// --------------------------------------------------------------------------
min_max_group::min_max_group () {
  on_create([&] (window*, const core::rectangle&) {
    min_label.create(*this, "Min:");
    min_edit.create(*this, "0");
    max_label.create(*this, "Max:");
    max_edit.create(*this, "255");
  });
}

// --------------------------------------------------------------------------
class color_key : public win::group_window<layout::vertical_lineup<20>> {
public:
  typedef win::group_window<layout::vertical_lineup<20>> super;

  color_key ();

  void onCreated (window*, const core::rectangle&);

  void set (const std::string& name, byte min_value, byte max_value);
  void set_min (byte value);
  void set_max (byte value);

  byte get_min () const;
  byte get_max () const;

private:
  label name_label;
  min_max_group min_max;
  horizontal_scroll_bar min_scroll;
  horizontal_scroll_bar max_scroll;
};

// --------------------------------------------------------------------------
color_key::color_key () {
  on_create(basepp::bind_method(this, &color_key::onCreated));
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
void color_key::onCreated (window*, const core::rectangle&) {
  name_label.create(*this);
  min_max.create(*this);
  min_scroll.create(*this);
  min_scroll.set_min_max(0, 255);
  max_scroll.create(*this);
  max_scroll.set_min_max(0, 255);
}

// --------------------------------------------------------------------------
void color_key::set (const std::string& name, byte min_value, byte max_value) {
  name_label.set_text(name);
  set_min(min_value);
  set_max(max_value);
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
byte color_key::get_min () const {
  return (byte)min_scroll.get_value();
}
// --------------------------------------------------------------------------
byte color_key::get_max () const {
  return (byte)max_scroll.get_value();
}

// --------------------------------------------------------------------------
class color_key_group : public win::group_window<layout::vertical_lineup<80, 0, 2>> {
public:
  typedef win::group_window<layout::vertical_lineup<80, 0, 2>> super;

  color_key_group ();

  void onCreated (window*, const core::rectangle&);

  color_key red;
  color_key green;
  color_key blue;
};
// --------------------------------------------------------------------------
color_key_group::color_key_group () {
  on_create(basepp::bind_method(this, &color_key_group::onCreated));
}
// --------------------------------------------------------------------------
void color_key_group::onCreated (window*, const core::rectangle&) {
  red.create(*this);
  green.create(*this);
  blue.create(*this);

  red.set("Rot", 0, 255);
  green.set("Grün", 0, 255);
  blue.set("Blau", 0, 255);
}

// --------------------------------------------------------------------------
class side_bar : public win::group_window<layout::vertical_lineup<244, 0, 15>> {
public:
  typedef win::group_window<layout::vertical_lineup<244, 0, 15>> super;

  side_bar ();

  void onCreated (window*, const core::rectangle&);

  color_key_group first;
  color_key_group second;
  color_key_group third;
};
// --------------------------------------------------------------------------
side_bar::side_bar () {
  on_create(basepp::bind_method(this, &side_bar::onCreated));
}
// --------------------------------------------------------------------------
void side_bar::onCreated (window*, const core::rectangle&) {
  first.create(*this);
  second.create(*this);
  third.create(*this);
}
// --------------------------------------------------------------------------
class RedImage : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  typedef layout_main_window<gui::layout::border_layout<>, float, float, float, float> super;
  RedImage ();

  void onCreated (window*, const core::rectangle&);
  void open ();
  void quit ();
  void show_all ();
  void show_red ();
  void show_green ();
  void show_blue ();
  void show_inverted ();

private:
  main_menu menu;
  popup_menu file_sub_menu;
  popup_menu edit_sub_menu;
  popup_menu help_sub_menu;

  win::group_window<layout::grid_adaption<2, 2, 0, 1>> main_view;
  image_view image_views[4];

  side_bar colors;

  cv::Mat raw_image;
  cv::Mat inverted_image;

  sys_fs::path last_dir;
};

// --------------------------------------------------------------------------
RedImage::RedImage ()
  : super(20, 0, 260, 0)
{
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
    main_menu_entry("Window", 'W', [&]() {
    }, menu_state::disabled),
    main_menu_entry("Help", 'H', [&]() {
      help_sub_menu.popup(menu);
    })
  });

  file_sub_menu.data.add_entries({
    menu_entry("Open", 'o', basepp::bind_method(this, &RedImage::open), hot_key(keys::o, state::control), false),
    menu_entry("Exit", 'x', basepp::bind_method(this, &RedImage::quit), hot_key(keys::f4, state::alt), true)
  });

  edit_sub_menu.data.add_entries({
    menu_entry("All", 'a', basepp::bind_method(this, &RedImage::show_all), hot_key(keys::a, state::control), false),
    menu_entry("Red", 'r', basepp::bind_method(this, &RedImage::show_red), hot_key(keys::r, state::control), false),
    menu_entry("Green", 'g', basepp::bind_method(this, &RedImage::show_green), hot_key(keys::g, state::control), false),
    menu_entry("Blue", 'b', basepp::bind_method(this, &RedImage::show_blue), hot_key(keys::b, state::control), false),
    menu_entry("Invert", 'i', basepp::bind_method(this, &RedImage::show_inverted), hot_key(keys::i, state::control), true),
  });

  help_sub_menu.data.add_entry(
    menu_entry("About", 'A', [&]() {
      message_dialog::show(*this, "About RedImage", "RedImage Version 0.0.1", "Ok");
    })
  );

  menu.create(*this);

  file_sub_menu.data.register_hot_keys(this);
  edit_sub_menu.data.register_hot_keys(this);

  main_view.create(*this);
  for(int i = 0; i < 4; ++i) {
    image_views[i].create(main_view);
  }

  colors.create(*this);

  get_layout().set_top(&menu);
  get_layout().set_center(&main_view);
  get_layout().set_left(&colors);

  set_children_visible();
}
//-----------------------------------------------------------------------------
void RedImage::show_all () {
  image_views[0].set_image(cvMat2pixmap(raw_image));
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
void RedImage::show_red () {
  cv::Mat mixed;
  isolateChannel(raw_image, mixed, 2);
  image_views[1].set_image(cvMat2pixmap(mixed));
}
//-----------------------------------------------------------------------------
void RedImage::show_green () {
  cv::Mat mixed;
  isolateChannel(raw_image, mixed, 1);
  image_views[2].set_image(cvMat2pixmap(mixed));
}
//-----------------------------------------------------------------------------
void RedImage::show_blue () {
  cv::Mat mixed;
  isolateChannel(raw_image, mixed, 0);
  image_views[3].set_image(cvMat2pixmap(mixed));
}
//-----------------------------------------------------------------------------
void RedImage::show_inverted () {
  image_views[0].set_image(cvMat2pixmap(inverted_image));
}
//-----------------------------------------------------------------------------
void RedImage::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}
//-----------------------------------------------------------------------------
void RedImage::open () {
  if (!last_dir.empty()) {
    sys_fs::current_path(last_dir);
  }
  file_open_dialog::show(*this, "Open File", "Open", "Cancel", [&] (const sys_fs::path& file) {
    if (sys_fs::exists(file)) {
      last_dir = file.parent_path();
      cv::Mat srcImage = cv::imread(file.string(), cv::IMREAD_COLOR);
      auto size = core::global::scale(image_views[0].size());
      cv::Size sz(size.width(), size.height());
      raw_image = cv::Mat(sz, srcImage.type());
      cv::resize(srcImage, raw_image, sz, 0, 0, cv::INTER_LINEAR);

      inverted_image = ~raw_image;
      show_all();
      show_red();
      show_green();
      show_blue();
    }
  });
}
// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  RedImage main;

  main.create({50, 50, 800, 600});
  main.set_title("RedImage");
  main.set_visible();
  main.maximize();

  return run_main_loop();
}
