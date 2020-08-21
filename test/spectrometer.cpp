

#include <gui/layout/attach_layout.h>
#include <gui/layout/dynamic_border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/layout_container.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/drop_down.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/ctrl/scroll_view.h>
#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/io/wavelength_to_rgb.h>
#include <gui/io/pnm.h>
#include <logging/core.h>
#include <logging/file_logger.h>
#include <persistent/ptree_persistent.h>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/optional.hpp>

#ifdef BUILD_FOR_ARM
#include <bcm_host.h>
#include "raspi/encoder.h"
#endif // BUILD_FOR_ARM

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;
#ifdef BUILD_FOR_ARM
using namespace raspi::camera;
using namespace raspi::encoder;
#endif // BUILD_FOR_ARM


template<typename T>
uint32_t cu32 (T v) {
  return static_cast<uint32_t>(v);
}

template<typename T>
int32_t ci32 (T v) {
  return static_cast<int32_t>(v);
}

template<typename T>
uint8_t cu8 (T v) {
  return static_cast<uint8_t>(v);
}

template<typename T>
int8_t ci8 (T v) {
  return static_cast<int8_t>(v);
}


const os::color nero = color::rgb<64,66,68>::value;
const os::color silver = color::rgb<0xC3,0xC6,0xC7>::value;

template<os::color background = nero>
class image_view : public client_control<background> {
public:
  using super = client_control<background>;

  image_view ()
    : scan_pos(-1)
    , scan_width(100)
  {
    this->on_paint(draw::paint([&] (const graphics& graph) {
      graph.clear(background);
      if (img.is_valid()) {
        if (scan_pos > -1) {
          const core::rectangle r = get_image_area();
          graph.copy_from(img, r);
          graph.copy_from(img, core::rectangle(0, scan_pos - r.y(), r.width(), 1), core::point(0, scan_pos - r.y()), copy_mode::bit_dest_invert);
        } else {
          graph.copy_from(img, core::point::zero);
        }
      }
    }));
  }

  void set_image (const pixmap& pix) {
    img = pix;
    super::resize(get_image_area().size());
  }

  pixmap& get_image () {
    return img;
  }

  int32_t get_scan_pos () const {
    return scan_pos;
  }

  void set_scan_pos (int32_t value) {
    scan_pos = value;
    super::resize(get_image_area().size());
  }

  int32_t get_scan_width () const {
    return scan_width;
  }

  void set_scan_width (int32_t value) {
    scan_width = value;
  }

private:
  core::rectangle get_image_area () const;

  int32_t scan_pos;
  int32_t scan_width;
  pixmap img;
};

template<os::color B>
core::rectangle image_view<B>::get_image_area () const {
  if (img.is_valid()) {
    core::size sz = img.scaled_size();
    if (scan_pos > -1) {
      const float sw = static_cast<float>(scan_width);
      int32_t top = std::min(sz.height() - sw, std::max(0.0F, scan_pos - sw / 2.0F));
      return core::rectangle(0, top, sz.width(), sw);
    } else {
      return core::rectangle(sz);
    }
  }
  return {};
}

using button_group = group_window<layout::vertical_adaption<>>;

template<typename T>
class value_block : public button_group {
public:
  using super = button_group;
  using fnkt = std::function<void(T)>;

  value_block ()
    : low_step(1)
    , high_step(20)
  {
    on_create(util::bind_method(this, &value_block::onCreated));
  }

  value_block (const std::string& l, text_source&& src)
    : label(l)
    , value(std::move(src))
    , low_step(1)
    , high_step(20)
  {
    on_create(util::bind_method(this, &value_block::onCreated));
  }

  void set_label (const std::string& l) {
    label.set_text(l);
  }

  void set_value (text_source&& src) {
    value.set_text(std::move(src));
  }

  void set_steps (T low, T high) {
    low_step = low;
    high_step = high;
  }

  void set_handler (fnkt f) {
    up_button.on_clicked([&, f] () { f(low_step); });
    fast_up_button.on_clicked([&, f] () { f(this->high_step); });
    down_button.on_clicked([&, f] () { f(-low_step); });
    fast_down_button.on_clicked([&, f] () { f(-high_step); });
  }

  void set_handler (T low, T high, fnkt f) {
    set_steps(low, high);
    set_handler(f);
  }

  void refresh () {
    value.redraw();
  }

  void onCreated (window*, const core::rectangle&) {
    label.create(*this);

    up_view.create(*this);
    up_button.create(up_view, "+");
    fast_up_button.create(up_view, "++");
    up_view.get_layout().add({lay(up_button), lay(fast_up_button)});

    value.create(*this);

    down_view.create(*this);
    down_button.create(down_view, "-");
    fast_down_button.create(down_view, "--");
    down_view.get_layout().add({lay(down_button), lay(fast_down_button)});

    get_layout().add({lay(label), lay(up_view), lay(value), lay(down_view)});
//    set_children_visible();
  }

private:
  group_window<layout::horizontal_adaption<0>> up_view;
  group_window<layout::horizontal_adaption<0>> down_view;

  ctrl::text_button up_button;
  ctrl::text_button fast_up_button;

  ctrl::text_button down_button;
  ctrl::text_button fast_down_button;

  ctrl::label_center label;
  ctrl::label_center value;

  T low_step;
  T high_step;
};

// --------------------------------------------------------------------------
class spectrometer : public layout_main_window<gui::layout::dynamic_border_layout<>, float, float, float, float> {
public:
  using super = layout_main_window<gui::layout::dynamic_border_layout<>, float, float, float, float>;
  spectrometer ();

  void onCreated (window*, const core::rectangle&);
  void quit ();

  void capture ();
  void prepare_data ();
  void display ();
  void extract_scanline ();
  void search_scanline ();
  void calc_spectrum ();

  void save_image ();
  void load_image ();

  void save_settings ();
  void load_settings ();

  void set_crop (const core::native_rect& c);
  const core::native_rect& get_crop () const;
  const core::native_size& get_capture_size () const;

  void change_crop_x (int32_t v);
  void change_crop_w (int32_t v);

  typedef void(crop_fnkt)(core::native_rect&);
  void change_crop (std::function<crop_fnkt>);

private:
  button_group left_button_view;
  ctrl::text_button fullview_button;
  ctrl::text_button halfview_button;
  ctrl::text_button quarterview_button;

  button_group mid_button_view;
  ctrl::check_box<> continuous;
  ctrl::text_button norm_button;
  ctrl::check_box<> compare_check;

  button_group right_button_view;
  ctrl::text_button capture_button;
  ctrl::text_button search_button;
  ctrl::text_button save_button;
  ctrl::text_button load_button;

#ifdef BUILD_FOR_ARM
  ctrl::drop_down_list<raspi::core::four_cc> encoder_out_encoding_down;
#else
  ctrl::drop_down_list<std::string> encoder_out_encoding_down;
#endif // BUILD_FOR_ARM

  value_block<int32_t> x_pos;
  value_block<int32_t> w_pos;
  value_block<int32_t> h_pos;

  value_block<float> d_gain;
  value_block<float> a_gain;

  value_block<uint32_t> ss;
  value_block<uint32_t> iso;

  value_block<int32_t> y_scanline;
  value_block<int32_t> nm_low;
  value_block<int32_t> nm_high;

  group_window<layout::horizontal_adaption<0, 2>> values_view;

  ctrl::scroll_view<nero> capture_scroll;
  image_view<> capture_view;
  image_view<> spectrum_view;

#ifdef BUILD_FOR_ARM
  still camera;

  typedef encoded encoder_type;
//  typedef resizer encoder_type;
//  typedef isp encoder_type;
//  typedef raw encoder_type;
  std::unique_ptr<encoder_type> encoder;

  encoder_type::image_data data;
#else
  std::vector<unsigned char*> data;
#endif // BUILD_FOR_ARM

  core::native_size capture_size;
  rgbmap rgb_image;
  draw::graymap scan_line;
  draw::graymap norm_line;

  core::native_rect current_crop;

  typedef core::range<int32_t> range_t;
  range_t nm_range;
};

void spectrometer::change_crop (std::function<crop_fnkt> fnkt) {
  fnkt(current_crop);

  x_pos.refresh();
  w_pos.refresh();
  h_pos.refresh();
}

const core::native_rect& spectrometer::get_crop () const {
  return current_crop;
}

const core::native_size& spectrometer::get_capture_size () const {
  return capture_size;
}

void spectrometer::set_crop (const core::native_rect& crop) {
  change_crop([=] (core::native_rect& c) { c = crop; });
}

void spectrometer::change_crop_x (int32_t v) {
  change_crop([&,v] (core::native_rect& crop) {
    crop.x(std::min<int32_t>(std::max<int32_t>(crop.x() + v, 0), ci32(get_capture_size().width())));
    crop.width(std::min<uint32_t>(crop.width(), cu32(get_capture_size().width() - crop.x())));
  });
}

void spectrometer::change_crop_w (int32_t v) {
  change_crop([&,v] (core::native_rect& crop) {
    crop.width(std::min<uint32_t>(std::max<uint32_t>(cu32(crop.width() + v), 0), get_capture_size().width()));
    crop.x(std::min<int32_t>(crop.x(), ci32(get_capture_size().width()) - ci32(crop.width())));
  });
}

void spectrometer::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      save_settings();
      set_children_visible(false);
      win::quit_main_loop(this);
    } else {
      take_focus();
    }
  });
}

void spectrometer::onCreated (window*, const core::rectangle&) {
  capture_scroll.create(*this);
  capture_view.create(capture_scroll);
  spectrum_view.create(*this);
  values_view.create(*this);

  left_button_view.create(values_view);
  fullview_button.create(left_button_view);
  halfview_button.create(left_button_view);
  quarterview_button.create(left_button_view);

  left_button_view.get_layout().add({layout::lay(fullview_button), layout::lay(halfview_button), layout::lay(quarterview_button)});

  x_pos.create(values_view);
  w_pos.create(values_view);
  h_pos.create(values_view);
  d_gain.create(values_view);
  a_gain.create(values_view);
  ss.create(values_view);
  iso.create(values_view);
  y_scanline.create(values_view);
  nm_low.create(values_view);
  nm_high.create(values_view);

  mid_button_view.create(values_view);
  continuous.create(mid_button_view);
  norm_button.create(mid_button_view);
  compare_check.create(mid_button_view);

  mid_button_view.get_layout().add({layout::lay(continuous), layout::lay(norm_button),
                                    layout::lay(compare_check), layout::lay(encoder_out_encoding_down)});

  right_button_view.create(values_view);
  capture_button.create(right_button_view);
  search_button.create(right_button_view);
  save_button.create(right_button_view);
  load_button.create(right_button_view);

  right_button_view.get_layout().add({layout::lay(capture_button), layout::lay(search_button),
                                      layout::lay(save_button), layout::lay(load_button)});

  values_view.get_layout().add({layout::lay(left_button_view), layout::lay(x_pos), layout::lay(w_pos), layout::lay(h_pos),
                                layout::lay(d_gain), layout::lay(a_gain), layout::lay(ss), layout::lay(iso),
                                layout::lay(y_scanline), layout::lay(nm_low), layout::lay(nm_high),
                                layout::lay(mid_button_view), layout::lay(right_button_view)});

#ifdef BUILD_FOR_ARM
  auto out_encodings = encoder->get_output_port().get_supported_encodings();
  encoder_out_encoding_down.set_data([=](std::size_t i) { return out_encodings[i]; }, out_encodings.size());
#else
  encoder_out_encoding_down.set_data([=](std::size_t i) { return "PPM"; }, 1);
#endif // BUILD_FOR_ARM
  encoder_out_encoding_down.create(mid_button_view, {0,0, 20, 20});;
  encoder_out_encoding_down.set_visible_items(8);
#ifdef BUILD_FOR_ARM
  encoder_out_encoding_down.set_selected_item(encoder->get_output_port().get_encoding());
  encoder_out_encoding_down.items().on_selection_changed([&] (event_source) {
    auto port = encoder->get_output_port();
    port.set_encoding(encoder_out_encoding_down.get_selected_item());
    port.commit_format_change();
  });
#endif // BUILD_FOR_ARM

  get_layout().set_center_top_bottom_left_right(layout::lay(&capture_scroll), layout::lay(&spectrum_view), layout::lay(&values_view), nullptr, nullptr);
  load_settings();
//  set_children_visible();
}

spectrometer::spectrometer ()
  : super(255, 160, 0, 0)
  , current_crop{80, 500, 1300, 100}
  , nm_range{330, 830}
{
#ifdef BUILD_FOR_ARM
  camera.set_sensor_mode(raspi::camera::still::SensorModeV2::SM_1640x922_16_9_video_2x2);
    camera.set_iso(50);
    camera.set_shutter_speed(50000);
    camera.set_resolution({1640, 922});
    camera.set_awb_mode(still::AWBMode::Off);
    clog::info() << "Exposure mode:" << camera.get_exposure_mode();
//    camera.set_exposure_mode(MMAL_PARAM_EXPOSUREMODE_OFF);

    camera.enable();

    encoder = std::unique_ptr<encoder_type>(new encoder_type(camera.get_still_output_port(), encoder_type::OutEncoding::PPM));
    encoder->enable();

    encoder->register_handler([&] (const encoder_type::image_data& d) {
        data = d;
        capture_size = {camera.get_sensor_size().width, camera.get_sensor_size().height};
        gui::win::run_on_main([this] () {
            prepare_data();
            search_scanline();
            display();
            if (continuous.is_checked()) {
              encoder->capture(0);
            }
        });
    });
#else
  capture_size = {1640, 922};
#endif // BUILD_FOR_ARM

  on_create(util::bind_method(this, &spectrometer::onCreated));
  on_destroy(&gui::win::quit_main_loop);
  on_close(util::bind_method(this, &spectrometer::quit));

  capture_button.set_text("Capture");
  continuous.set_text("Continuous");
  norm_button.set_text("Norm");
  compare_check.set_text("Compare");
  save_button.set_text("Save");
  load_button.set_text("Load");
  search_button.set_text("Search");

  fullview_button.set_text("1/1");
  halfview_button.set_text("1/2");

  quarterview_button.set_text("1/4");

  capture_button.on_clicked(util::bind_method(this, &spectrometer::capture));
  save_button.on_clicked(util::bind_method(this, &spectrometer::save_image));
  load_button.on_clicked(util::bind_method(this, &spectrometer::load_image));
  search_button.on_clicked(util::bind_method(this, &spectrometer::search_scanline));

  norm_button.on_clicked([&] () {
    norm_line = scan_line;
  });

  fullview_button.on_clicked([&] () {
#ifdef BUILD_FOR_ARM
    auto sz = camera.get_sensor_size();
    set_crop({0, 0, sz.width, sz.height});
    camera.set_resolution(sz);
#else
    set_crop({{}, capture_size});
#endif // BUILD_FOR_ARM
  });

  halfview_button.on_clicked([&] () {
#ifdef BUILD_FOR_ARM
    still::size sz = camera.get_sensor_size();
    sz = still::size{static_cast<uint32_t>(sz.width / 2), static_cast<uint32_t>(sz.height / 2)};
    set_crop({static_cast<int32_t>(sz.width / 4), static_cast<int32_t>(sz.height / 4), sz.width, sz.height});
    camera.set_resolution(sz);
#else
    set_crop({static_cast<int32_t>(capture_size.width() / 4), static_cast<int32_t>(capture_size.height() / 4),
              capture_size.width() / 2, capture_size.height() / 2});
#endif // BUILD_FOR_ARM
  });
  quarterview_button.on_clicked([&] () {
#ifdef BUILD_FOR_ARM
    still::size sz = camera.get_sensor_size();
    sz = still::size{static_cast<uint32_t>(sz.width / 4), static_cast<uint32_t>(sz.height / 4)};
    set_crop({static_cast<int32_t>(sz.width / 8 * 3), static_cast<int32_t>(sz.height / 8 * 3), sz.width, sz.height});
    camera.set_resolution(sz);
#else
    set_crop({static_cast<int32_t>(capture_size.width() / 8 * 3), static_cast<int32_t>(capture_size.height() / 8 * 3),
              capture_size.width() / 4, capture_size.height() / 4});
#endif // BUILD_FOR_ARM
  });

  x_pos.set_handler(8, 80, util::bind_method(this, &spectrometer::change_crop_x));
  x_pos.set_label("X");
  x_pos.set_value([&] () { return ostreamfmt(get_crop().x()); });

  w_pos.set_handler(8, 80, util::bind_method(this, &spectrometer::change_crop_w));
  w_pos.set_label("W");
  w_pos.set_value([&] () { return ostreamfmt(get_crop().width()); });

  h_pos.set_handler(8, 40, [&] (int32_t value) {
    capture_view.set_scan_width(capture_view.get_scan_width() + value);
  });
  h_pos.set_label("H");
  h_pos.set_value([&] () {
    return ostreamfmt(capture_view.get_scan_width());
  });

  d_gain.set_label("D-Gain");
  a_gain.set_label("A-Gain");

  ss.set_steps(15, 20);
  ss.set_label("Shutter");
  iso.set_steps(15, 20);
  iso.set_label("ISO");

#ifdef BUILD_FOR_ARM
  d_gain.set_handler(0.025F, 0.1F, [&] (float v) {
    auto g = camera.get_digital_gain();
    g += v;
    camera.set_digital_gain(g);
  });
  a_gain.set_handler(0.025F, 0.1F, [&] (float v) {
    auto g = camera.get_analog_gain();
    g += v;
    camera.set_analog_gain(g);
  });
  d_gain.set_value([&] () { return ostreamfmt(camera.get_digital_gain()); });
  a_gain.set_value([&] () { return ostreamfmt(camera.get_analog_gain()); });

  ss.set_handler([&] (int32_t step) {
    auto current = camera.get_shutter_speed();
    if (step > 0) {
      camera.set_shutter_speed(current * cu32(step) / 10);
    } else {
      camera.set_shutter_speed(current * 10 / cu32(-step));
    }
    ss.refresh();
//    capture();
  });
  ss.set_value([&] () { return ostreamfmt(camera.get_shutter_speed()); });

  iso.set_handler([&] (int32_t step) {
    auto current = camera.get_iso();
    if (step > 0) {
      camera.set_iso(current * cu32(step) / 10);
    } else {
      camera.set_iso(std::max<uint32_t>(current * 10 / cu32(-step), 25));
    }
    iso.refresh();
//    capture();
  });
  iso.set_value([&] () { return ostreamfmt(camera.get_iso()); });
#endif // BUILD_FOR_ARM

  y_scanline.set_handler([&] (int32_t step) {
    capture_view.set_scan_pos(std::max<int32_t>(capture_view.get_scan_pos() + step, 0));
    y_scanline.refresh();
    extract_scanline();
    calc_spectrum();
    capture_view.invalidate();
  });
  y_scanline.set_steps(1, 10);
  y_scanline.set_label("Scanline");
  y_scanline.set_value([&] () { return ostreamfmt(capture_view.get_scan_pos()); });

  nm_low.set_handler(0, 1500, [&] (int32_t l) {
    nm_range = {nm_range.begin() + l, nm_range.end()};
    nm_low.refresh();
    calc_spectrum();
  });
  nm_low.set_steps(1, 20);
  nm_low.set_label("Low nm");
  nm_low.set_value([&] () { return ostreamfmt(nm_range.begin()); });

  nm_high.set_handler(0, 1500, [&] (int32_t h) {
    nm_range = {nm_range.begin(), nm_range.end() + h};
    nm_high.refresh();
    calc_spectrum();
  });
  nm_high.set_steps(1, 20);
  nm_high.set_label("High nm");
  nm_high.set_value([&] () { return ostreamfmt(nm_range.end()); });
}

void spectrometer::capture () {
#ifdef BUILD_FOR_ARM
  auto state = camera.get_capture_status();
  if (state == MMAL_PARAM_CAPTURE_STATUS_CAPTURE_STARTED) {
    clog::info() << "Already capturing:" << state;
    return;
  }
  clog::debug() << "Capture image";

  try {
    encoder->clear_data();
    encoder->capture(0);

  } catch (std::exception& ex) {
    clog::fatal() << ex;
  }
#endif // BUILD_FOR_ARM
}

void spectrometer::search_scanline () {
  if (rgb_image.is_valid()) {
    auto raw = rgb_image.get_data();
    uint32_t current_row = 0;
    uint32_t current_sum = 0;
    for (uint32_t y = 0; y < raw.height(); ++y) {
      auto row = raw.row(y);
      uint32_t sum = 0;
      for (uint32_t x = 0; x < raw.width(); ++x) {
        sum += pixel::get_gray(row[x]);
      }
      if (sum > current_sum) {
        current_sum = sum;
        current_row = y;
      }
    }
    capture_view.set_scan_pos(ci32(current_row));
    y_scanline.refresh();
    extract_scanline();
    calc_spectrum();
  }
}

void spectrometer::display () {
  capture_view.redraw();
}

void spectrometer::extract_scanline () {
  if (rgb_image.is_valid() && (capture_view.get_scan_pos() > -1)) {
      const auto sub = rgb_image.sub(0, cu32(capture_view.get_scan_pos()), get_capture_size().width(), 1);
      scan_line = sub.convert<pixel_format_t::GRAY>();
  }
}

void spectrometer::prepare_data () {
  auto sz = get_capture_size();
#ifdef BUILD_FOR_ARM
  auto encoding = static_cast<MMAL_FOURCC_T>(encoder->get_encoding());
#endif // BUILD_FOR_ARM
  clog::debug() << "Display " << data.size() << " Bytes with dimensions:" << sz.width() << "x" << sz.height()
#ifdef BUILD_FOR_ARM
                << " ppl:" << camera.get_bytes_per_line() << " enc:" << raspi::core::four_cc(encoding)
#endif // BUILD_FOR_ARM
                 ;
  if (data.size()) {
#ifdef BUILD_FOR_ARM
    switch (encoding) {
      case MMAL_ENCODING_PPM: {
#endif // BUILD_FOR_ARM
        std::istringstream strm(std::string(reinterpret_cast<char*>(data.data()), data.size()));
        gui::io::load_pnm<pixel_format_t::RGB>(strm, rgb_image);
#ifndef BUILD_FOR_ARM
        capture_size = rgb_image.native_size();
#endif // BUILD_FOR_ARM
        clog::debug() << "Display image with dimensions:" << get_capture_size() << ", ppl:" << rgb_image.pixel_format();
        if ((current_crop.width() != sz.width())/* || (current_crop.height() != sz.height())*/) {
          auto sub = rgb_image.sub(current_crop.x(), 0, current_crop.width(), sz.height());
          rgb_image = sub;
        }
        capture_view.set_image(rgb_image);
#ifdef BUILD_FOR_ARM
        break;
      }
      case MMAL_ENCODING_RGBA:
      case MMAL_ENCODING_RGBA_SLICE: {
        bitmap_info bmi(sz.width(), sz.height(), camera.get_bytes_per_line(), pixel_format_t::RGBA);
        const_image_data<pixel_format_t::RGBA> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        clog::debug() << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        rgb_image = image_data;
        capture_view.set_image(rgb_image);
//        scan_line.create(sz.width, 1);
//        if (capture_view.get_scan_pos() > -1) {
//            convert::format::line<pixel_format_t::RGBA, pixel_format_t::GRAY>::convert(image_data.row(capture_view.get_scan_pos()), scan_line.get_data().row(0), sz.width);
//        }
        break;
      }
      case MMAL_ENCODING_RGB24:
      case MMAL_ENCODING_RGB24_SLICE: {
        bitmap_info bmi(sz.width(), sz.height(), camera.get_bytes_per_line(), pixel_format_t::RGB);
        const_image_data<pixel_format_t::RGB> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        clog::debug() << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        rgb_image = image_data;
        capture_view.set_image(rgb_image);
//        scan_line.create(sz.width, 1);
//        if (capture_view.get_scan_pos() > -1) {
//            convert::format::line<pixel_format_t::RGB, pixel_format_t::GRAY>::convert(image_data.row(capture_view.get_scan_pos()), scan_line.get_data().row(0), sz.width);
//        }
        break;
      }
      case MMAL_ENCODING_BGRA:
      case MMAL_ENCODING_BGRA_SLICE: {
        bitmap_info bmi(sz.width(), sz.height(), camera.get_bytes_per_line(), pixel_format_t::BGRA);
        const_image_data<pixel_format_t::BGRA> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        clog::debug() << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        rgb_image = image_data;
        capture_view.set_image(rgb_image);
//        scan_line.create(sz.width, 1);
//        if (capture_view.get_scan_pos() > -1) {
//            convert::format::line<pixel_format_t::BGRA, pixel_format_t::GRAY>::convert(image_data.row(capture_view.get_scan_pos()), scan_line.get_data().row(0), sz.width);
//        }
        break;
      }
      case MMAL_ENCODING_BGR24:
      case MMAL_ENCODING_BGR24_SLICE: {
        bitmap_info bmi(sz.width(), sz.height(), camera.get_bytes_per_line(), pixel_format_t::BGR);
        const_image_data<pixel_format_t::BGR> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        clog::debug() << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        rgb_image = image_data;
        capture_view.set_image(rgb_image);
//        scan_line.create(sz.width, 1);
//        if (capture_view.get_scan_pos() > -1) {
//            convert::format::line<pixel_format_t::BGR, pixel_format_t::GRAY>::convert(image_data.row(capture_view.get_scan_pos()), scan_line.get_data().row(0), sz.width);
//        }
        break;
      }

      default:
        clog::debug() << "Can not display image with encoding: " << encoding;
        break;
    }
#endif // BUILD_FOR_ARM
  }
}

void spectrometer::calc_spectrum () {
  core::size sz = { static_cast<float>(scan_line.scaled_size().width() > 0 ? scan_line.scaled_size().width() : get_capture_size().width()), spectrum_view.size().height() };
  if (spectrum_view.get_image().scaled_size() != sz) {
    spectrum_view.get_image().create(sz);
  }

  graphics g(spectrum_view.get_image());
  double scale = (nm_range.end() - nm_range.begin()) / sz.width();
  for (int x = 0; x < sz.width(); ++x) {
    os::color rgb = io::optics::wave_length_to_rgb(nm_range.begin() + static_cast<double>(x) * scale);
    g.draw_lines({core::point(x, 0), core::point(x, sz.height())}, draw::pen(rgb));
  }

  if (capture_view.get_scan_pos() > -1) {

    auto display = core::global::get_instance();
#ifdef X11
    XGCValues values;
    values.function = GXinvert;
    XChangeGC(display, g.os(), GCFunction, &values);
#endif // X11
#ifdef WIN32
    const auto oldROP = GetROP2(g);
    SetROP2(g, R2_NOT);
#endif // WIN32


    std::vector<core::point> lines;
    if (compare_check.is_checked() && norm_line.is_valid()) {
      const auto row = const_cast<const draw::graymap&>(scan_line).get_data();
      const auto comp = const_cast<const draw::graymap&>(norm_line).get_data();

      for (uint32_t x = 0; x < row.width(); ++x) {
        auto pixel = row.pixel(x, 0);
        auto norm = comp.pixel(x, 0);

        core::point next = {static_cast<float>(x), 255.0F - (static_cast<float>(norm.value) - static_cast<float>(pixel.value))};
        lines.push_back(next);
      }

    } else {
      const auto row = const_cast<const draw::graymap&>(scan_line).get_data();
      for (uint32_t x = 0; x < row.width(); ++x) {
        auto pixel = row.pixel(x, 0);
        core::point next = {static_cast<float>(x), 255.0F - static_cast<float>(pixel.value)};
        lines.push_back(next);
      }
    }
    g.draw_lines(lines, color::white);

#ifdef X11
    values.function = GXcopy;
    XChangeGC(display, g.os(), GCFunction, &values);
#endif // X11
#ifdef WIN32
    SetROP2(g, oldROP);
#endif // WIN32
  }

  spectrum_view.invalidate();
}

void spectrometer::save_image () {
  file_save_dialog::show(*this, "Save captured data", "spectrogram.dat", "Name:", "OK", "Cancel",
                         [&] (const sys_fs::path& path) {
    std::ofstream(path).write(reinterpret_cast<char*>(data.data()), data.size());
  });
}

void spectrometer::load_image () {
  file_open_dialog::show(*this, "Load captured data", "OK", "Cancel",
                         [&] (const sys_fs::path& path) {
    std::ifstream f(path, std::ios::binary|std::ios::ate);
    auto len = f.tellg();
    data.resize(len);
    f.seekg(0, std::ios::beg);
    f.read(reinterpret_cast<char*>(data.data()), data.size());
    prepare_data();
    search_scanline();
    display();
  });
}

struct settings : public persistent::ptree_struct<int32_t, int32_t, uint32_t, uint32_t,
                                                  float, float,
                                                  uint32_t, uint32_t, uint32_t,
                                                  int32_t, int32_t> {
  typedef persistent::ptree_struct<int32_t, int32_t, uint32_t, uint32_t,
                                   float, float,
                                   uint32_t, uint32_t, uint32_t,
                                   int32_t, int32_t> super;

  static const char s_x[];
  static const char s_y[];
  static const char s_w[];
  static const char s_h[];
  static const char s_dgain[];
  static const char s_again[];
  static const char s_ss[];
  static const char s_iso[];
  static const char s_scanline[];
  static const char s_nm_low[];
  static const char s_nm_high[];

  settings (const core::native_rect& crop = {},
            float_t dg = 0, float_t ag = 0,
            uint32_t ss_ = 0, uint32_t iso_ = 0,
            uint32_t y_s = 0, int32_t low = 0, int32_t high = 0)
    : super(x, y, w, h, d_gain, a_gain, ss, iso, y_scanline, nm_low, nm_high)
    , x(s_x, crop.x())
    , y(s_y, crop.y())
    , w(s_w, crop.width())
    , h(s_h, crop.height())
    , d_gain(s_dgain, dg)
    , a_gain(s_again, ag)
    , ss(s_ss, ss_)
    , iso(s_iso, iso_)
    , y_scanline(s_scanline, y_s)
    , nm_low(s_nm_low, low)
    , nm_high(s_nm_high, high)
  {}

  settings (const settings& rhs)
    : settings()
  {
    operator=(rhs);
  }

  persistent::int32 x;
  persistent::int32 y;
  persistent::dword w;
  persistent::dword h;
  persistent::float32 d_gain;
  persistent::float32 a_gain;
  persistent::dword ss;
  persistent::dword iso;
  persistent::dword y_scanline;
  persistent::int32 nm_low;
  persistent::int32 nm_high;
};

const char settings::s_x[] = "x";
const char settings::s_y[] = "y";
const char settings::s_w[] = "w";
const char settings::s_h[] = "h";
const char settings::s_dgain[] = "dgain";
const char settings::s_again[] = "again";
const char settings::s_ss[] = "ss";
const char settings::s_iso[] = "iso";
const char settings::s_scanline[] = "scanline";
const char settings::s_nm_low[] = "nm_low";
const char settings::s_nm_high[] = "nm_high";

void spectrometer::save_settings () {
#ifdef BUILD_FOR_ARM
  float dg = camera.get_digital_gain();
  float ag = camera.get_analog_gain();
  uint32_t ss = camera.get_shutter_speed();
  uint32_t iso = camera.get_iso();
#else
  float dg = 1;
  float ag = 1;
  uint32_t ss = 50000;
  uint32_t iso = 50;
#endif // BUILD_FOR_ARM

  settings s(current_crop, dg, ag, ss, iso, capture_view.get_scan_pos(), nm_range.begin(), nm_range.end());

  using boost::property_tree::ptree;
  namespace xml = boost::property_tree::xml_parser;
  namespace json = boost::property_tree::json_parser;

  try {
    ptree main;
    s.write(main);
    ptree xml_main;
    xml_main.put_child("spectrometer", main);

    boost::property_tree::xml_writer_settings<ptree::key_type> xml_settings('\t', 1);
    xml::write_xml("spectrometer.xml", xml_main, std::locale(), xml_settings);
    json::write_json("spectrometer.json", xml_main, std::locale());
  } catch (std::exception& ex) {
    clog::warn() << "Exception while writing spectrometer.xml:" << ex.what();
  }
}

void spectrometer::load_settings () {

  using boost::property_tree::ptree;
  namespace xml = boost::property_tree::xml_parser;

  try {
    ptree xml_main;
    xml::read_xml("spectrometer.xml", xml_main, xml::no_comments);
    auto opt = xml_main.get_child_optional("spectrometer");
    if (opt) {
      settings s(current_crop, 1, 1, 50000, 50, capture_view.get_scan_pos(), nm_range.begin(), nm_range.end());
      s.read(opt.get());

      current_crop = {s.x(), s.y(), s.w(), s.h()};
      capture_view.set_scan_pos(s.y_scanline());
      nm_range = {s.nm_low(), s.nm_high()};

      y_scanline.refresh();
      x_pos.refresh();
      w_pos.refresh();
      h_pos.refresh();

      nm_low.refresh();
      nm_high.refresh();
    }
  } catch (std::exception& ex) {
    clog::warn() << "Exception while reading spectrometer.xml:" << ex.what();
  }
}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
//  bcm_host_init();
//  bcm_host_deinit();
//  return 0;

  std::string fname = "spektrometer.log";
  logging::core::rename_file_with_max_count(fname, 5);
  logging::file_logger flog(fname, logging::level::debug, logging::core::get_standard_formatter());

  spectrometer main;

  main.create({0, 0, 1420, 800});
  main.set_title("Raspi Spectrometer");
  main.set_visible();

  return gui::win::run_main_loop();
}
