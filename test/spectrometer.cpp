

#include <gui/layout/attach_layout.h>
#include <gui/layout/dynamic_border_layout.h>
#include <gui/layout/adaption_layout.h>
#include <gui/layout/layout_container.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/drop_down.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>
#include <gui/io/wavelength_to_rgb.h>
#include <gui/io/pnm.h>

#include <bcm_host.h>

#ifndef  BUILD_FOR_ARM
# define BUILD_FOR_ARM
#endif //  BUILD_FOR_ARM

#ifdef BUILD_FOR_ARM
#include "raspi/encoder.h"
#else
#define MMAL_ENCODING_RGB24_SLICE 0
#define VCOS_ALIGN_DOWN(p,n) (((ptrdiff_t)(p)) & ~((n)-1))
#define VCOS_ALIGN_UP(p,n) VCOS_ALIGN_DOWN((ptrdiff_t)(p)+(n)-1,(n))
namespace gui {
 namespace raspi {

  namespace core {
    // --------------------------------------------------------------------------
    struct four_cc {
      four_cc (const std::string& type)
        : m_type(type)
      {}

      four_cc () = default;

      bool operator== (const four_cc& rhs) const {
        return m_type == rhs.m_type;
      }

      std::string m_type;
    };

    std::ostream& operator<< (std::ostream& out, const four_cc& f) {
      return out << f.m_type;
    }

    std::istream& operator>> (std::istream& in, four_cc& f) {
      return in >> f.m_type;
    }


    struct port {
      std::vector<four_cc> get_supported_encodings () {
        return { {"RGB"}, {"RGBA"}, {"ARGB"}, {"BGR"}, {"BGRA"}, {"ABGR"} };
      }

      four_cc get_encoding () const { return {"RGB"}; }

      void set_encoding (four_cc) {}
    };

  } // namespace core

  namespace camera {
  struct raspi_camera {
    raspi_camera ()
      : cr{0, 0, 1, 1}
      , sz{3280, 2456}
      , g{1, 1}
      , ss(10000)
      , iso(100)
    {}

    struct crop {
      float x, y, width, height;
    };

    struct awb_gains {
      float r_gain, b_gain;
    };

    struct size {
      int width, height;
    };

    crop get_inout_crop () const { return cr; }
    void set_inout_crop (const crop& c) { cr = c; }
    crop get_abs_crop () const { return cr; }
    void set_abs_crop (const crop& c) { cr = c; }

    size get_size () const { return sz; }
    void set_size (const size& c) { sz = c; }

    int get_shutter_speed () const { return ss; }
    void set_shutter_speed (int s) { ss = s; }
    int get_iso () const { return iso; }
    void set_iso (int i) { iso = i; }

    core::port get_still_output_port () const { return {}; }
    int get_pixel_per_line () const { return 0; }

  private:
    crop cr;
    size sz;
    awb_gains g;
    int ss;
    int iso;
  };

  struct raspi_raw_encoder {
    enum OutEncoding {
      PPM
    };
    raspi_raw_encoder(core::port, int) {}

    void capture (int) {}

    using image_data = std::basic_string<uint8_t>;

    core::port get_output_port () const { return {}; }
    core::port get_input_port () const { return {}; }

    image_data get_data () { return {}; }
    void clear_data () {}
  };

  typedef raspi_raw_encoder raspi_image_encoder;
  }
 }
}
#endif // BUILD_FOR_ARM

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

using namespace gui;
using namespace gui::draw;
using namespace gui::layout;
using namespace gui::win;
using namespace gui::ctrl;
using namespace gui::raspi::camera;

const os::color nero = color::rgb<64,66,68>::value;
const os::color silver = color::rgb<0xC3,0xC6,0xC7>::value;

template<os::color background = nero>
class image_view : public client_control<background> {
public:
  image_view () {
    this->on_paint(draw::paint([&] (const graphics& graph) {
      if (image.is_valid()) {
        graph.copy_from(image, core::point::zero);
      } else {
        graph.fill(draw::rectangle(this->client_area()), background);
      }
    }));
  }

  pixmap image;
};

template<typename T>
class value_block : public group_window<layout::vertical_adaption<>> {
public:
  using super = layout_container<layout::vertical_adaption<>>;
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
  void display ();
  void calc_spectrum ();
  void save_image ();

  void save_settings ();
  void load_settings ();

  void set_crop (const MMAL_RECT_T& c);
  MMAL_RECT_T get_crop ();

  void change_crop_x (int32_t v);
  void change_crop_y (int32_t v);
  void change_crop_w (int32_t v);
  void change_crop_h (int32_t v);

  void update_encodings ();

  typedef void(crop_fnkt)(MMAL_RECT_T&);
  void change_crop (std::function<crop_fnkt>);

private:
  group_window<layout::vertical_adaption<>> right_button_view;
  ctrl::text_button capture_button;
  ctrl::text_button save_button;
  ctrl::text_button clear_button;

  group_window<layout::vertical_adaption<>> left_button_view;
  ctrl::text_button fullview_button;
  ctrl::text_button halfview_button;
  ctrl::text_button quarterview_button;

  ctrl::drop_down_list<gui::raspi::core::four_cc> camera_out_encoding_down;
  ctrl::drop_down_list<raspi::core::four_cc> encoder_in_encoding_down;
  ctrl::drop_down_list<raspi::core::four_cc> encoder_out_encoding_down;

  value_block<int32_t> x_pos;
  value_block<int32_t> y_pos;
  value_block<int32_t> w_pos;
  value_block<int32_t> h_pos;

  value_block<int32_t> res_w_pos;
  value_block<int32_t> res_h_pos;

  value_block<int> ss;
  value_block<int> iso;

  value_block<int> extra;
  int extra_bytes;

  group_window<layout::horizontal_adaption<1, 0>> values_view;

  image_view<color::black> capture_view;
  image_view<> spectrum_view;

  raspi_camera camera;

//  typedef raspi_image_encoder encoder_type;
//  typedef raspi_resizer encoder_type;
//  typedef raspi_isp encoder_type;
  typedef raspi_raw_encoder encoder_type;
  std::unique_ptr<encoder_type> encoder;

  encoder_type::image_data data;
};

void spectrometer::change_crop (std::function<crop_fnkt> fnkt) {
  auto crop = camera.get_abs_crop();
  fnkt(crop);
  //camera.set_abs_crop(crop);

//  MMAL_STATUS_T status =
//  LogDebug << "camera.set_abs_crop(" << crop << ") returned :"
//           << mmal_status_to_string(status);

#ifdef TRY_CAMERA_CONFIG
  auto cfg = camera.get_camera_config();
  LogDebug << "Current camera config:" << cfg;

  MMAL_RECT_T crop{0, 0, (int32_t)cfg.max_stills_w, (int32_t)cfg.max_stills_h};
  fnkt(crop);
  cfg.max_stills_w = crop.width;
  cfg.max_stills_h = crop.height;

  camera.set_camera_config(cfg);
#endif // TRY_CAMERA_CONFIG
#ifdef TRY_ENCODER_CROP
  auto port = encoder.get_output_port();
  auto resize = port.get_resize();
  auto crop = port.get_crop();

  fnkt(crop);

  resize.mode = MMAL_RESIZE_CROP;
  resize.max_width = crop.width;
  resize.max_height = crop.height;
  resize.preserve_aspect_ratio = MMAL_TRUE;
  resize.allow_upscaling = MMAL_FALSE;

  MMAL_STATUS_T status = MMAL_SUCCESS;
//  status = port.set_resize(resize);
//  LogDebug << "encoder.set_resize(" << resize << ") returned :"
//           << mmal_status_to_string(status);

  status = port.set_crop(crop);
  LogDebug << "port.set_crop(" << crop << ") returned :"
           << mmal_status_to_string(status);

  status = port.commit_format_change();
  LogDebug << "= port.commit_format_change() returned :"
           << mmal_status_to_string(status);

//  camera.set_abs_crop(crop);
//  LogDebug << "camera.set_abs_crop(" << crop << ") returned :"
//           << mmal_status_to_string(status);
#endif // TRY_ENCODER_CROP

  extra.refresh();
  x_pos.refresh();
  y_pos.refresh();
  w_pos.refresh();
  h_pos.refresh();
  res_w_pos.refresh();
  res_h_pos.refresh();
}

MMAL_RECT_T spectrometer::get_crop () {
    try {
        return camera.get_abs_crop();
    } catch (...) {
        return MMAL_RECT_T();
    }
}

void spectrometer::set_crop (const MMAL_RECT_T& crop) {
  change_crop([=] (MMAL_RECT_T& c) { c = crop; });
}

void spectrometer::change_crop_x (int32_t v) {
  change_crop([&,v] (MMAL_RECT_T& crop) {
    crop.x = std::min<int32_t>(std::max<int32_t>(crop.x + v, 0), (int32_t)camera.get_sensor_size().width - crop.width);
  });
}

void spectrometer::change_crop_y (int32_t v) {
  change_crop([&,v] (MMAL_RECT_T& crop) {
    crop.y = std::min<int32_t>(std::max<int32_t>(crop.y + v, 0), (int32_t)camera.get_sensor_size().height - crop.height);
  });
}

void spectrometer::change_crop_w (int32_t v) {
  change_crop([&,v] (MMAL_RECT_T& crop) {
    crop.width = std::min<int32_t>(std::max<int32_t>(crop.width + v, 0), (int32_t)camera.get_sensor_size().width - crop.x);
  });
}

void spectrometer::change_crop_h (int32_t v) {
  change_crop([&,v] (MMAL_RECT_T& crop) {
    crop.height = std::min<int32_t>(std::max<int32_t>(crop.height + v, 0), (int32_t)camera.get_sensor_size().height - crop.y);
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
  capture_view.create(*this);
  spectrum_view.create(*this);
  values_view.create(*this);

  left_button_view.create(values_view);
  fullview_button.create(left_button_view);
  halfview_button.create(left_button_view);
  quarterview_button.create(left_button_view);

  left_button_view.get_layout().add({layout::lay(fullview_button), layout::lay(halfview_button), layout::lay(quarterview_button)});

  x_pos.create(values_view);
  y_pos.create(values_view);
  w_pos.create(values_view);
  h_pos.create(values_view);
  res_w_pos.create(values_view);
  res_h_pos.create(values_view);
  ss.create(values_view);
  iso.create(values_view);
  extra.create(values_view);

  right_button_view.create(values_view);
  capture_button.create(right_button_view);
  save_button.create(right_button_view);
  clear_button.create(right_button_view);

  values_view.get_layout().add({layout::lay(left_button_view), layout::lay(x_pos), layout::lay(y_pos), layout::lay(w_pos), layout::lay(h_pos),
                                layout::lay(res_w_pos), layout::lay(res_h_pos), layout::lay(ss), layout::lay(iso),
                                layout::lay(extra), layout::lay(right_button_view)});

  auto camera_encodings = camera.get_still_output_port().get_supported_encodings();
  camera_out_encoding_down.set_data([=](std::size_t i) { return camera_encodings[i]; }, camera_encodings.size());
  camera_out_encoding_down.create(right_button_view, {0,0, 20, 20});
  camera_out_encoding_down.set_visible_items(8);
  camera_out_encoding_down.items().on_selection_changed([&] (event_source) {
    auto port = camera.get_still_output_port();
    port.set_encoding(camera_out_encoding_down.get_selected_item());
    port.commit_format_change();
  });

  auto in_encodings = encoder->get_input_port().get_supported_encodings();
  encoder_in_encoding_down.set_data([=](std::size_t i) { return in_encodings[i]; }, in_encodings.size());;
  encoder_in_encoding_down.create(right_button_view, {0,0, 20, 20});
  encoder_in_encoding_down.set_visible_items(8);
  encoder_in_encoding_down.items().on_selection_changed([&] (event_source) {
    auto port = encoder->get_input_port();
    port.set_encoding(encoder_in_encoding_down.get_selected_item());
    port.commit_format_change();
  });

  auto out_encodings = encoder->get_output_port().get_supported_encodings();
  encoder_out_encoding_down.set_data([=](std::size_t i) { return out_encodings[i]; }, out_encodings.size());;
  encoder_out_encoding_down.create(right_button_view, {0,0, 20, 20});;
  encoder_out_encoding_down.set_visible_items(8);
  encoder_out_encoding_down.set_selected_item(encoder->get_output_port().get_encoding());
  encoder_out_encoding_down.items().on_selection_changed([&] (event_source) {
    auto port = encoder->get_output_port();
    port.set_encoding(encoder_out_encoding_down.get_selected_item());
    port.commit_format_change();
  });

  right_button_view.get_layout().add({layout::lay(capture_button), layout::lay(save_button), layout::lay(clear_button),
                                      layout::lay(camera_out_encoding_down), layout::lay(encoder_in_encoding_down), layout::lay(encoder_out_encoding_down)});

  get_layout().set_center_top_bottom_left_right(layout::lay(&capture_view), layout::lay(&spectrum_view), layout::lay(&values_view), nullptr, nullptr);
  load_settings();
  update_encodings();
//  set_children_visible();
}

void spectrometer::update_encodings () {
  camera_out_encoding_down.set_selected_item(camera.get_still_output_port().get_encoding());
  encoder_in_encoding_down.set_selected_item(encoder->get_input_port().get_encoding());
  encoder_out_encoding_down.set_selected_item(encoder->get_output_port().get_encoding());
  camera_out_encoding_down.invalidate();
  encoder_in_encoding_down.invalidate();
  encoder_out_encoding_down.invalidate();
}

spectrometer::spectrometer ()
  : super(160, 160, 0, 0)
  , extra_bytes(0)
{
    camera.set_iso(100);
    camera.set_shutter_speed(50000);
    camera.set_sensor_mode(raspi::camera::raspi_camera::SensorModeV2::SM_1640x1232_4_3_video_2x2);
    camera.set_resolution({1640, 1232});

    camera.enable();

    encoder = std::unique_ptr<encoder_type>(new encoder_type(camera.get_still_output_port(), encoder_type::OutEncoding::BGR24));
    encoder->enable();

    encoder->register_handler([&] (const encoder_type::image_data& d) {
        data = d;
        gui::win::run_on_main([&] () {
            update_encodings();
            display();
            calc_spectrum();
        });
    });

  on_create(util::bind_method(this, &spectrometer::onCreated));
  on_destroy(&gui::win::quit_main_loop);
  on_close(util::bind_method(this, &spectrometer::quit));

  //  MMAL_PARAMETER_CROP_T crop = {{MMAL_PARAMETER_CROP, sizeof(MMAL_PARAMETER_CROP_T)}};
  //  encoder.get_output_port().get(crop.hdr);
  //  crop.rect.height = sz.height / 2;
  //  crop.rect.width = sz.width;
  //  encoder.get_output_port().set(crop.hdr);

  capture_button.set_text("Capture");
  save_button.set_text("Save");
  clear_button.set_text("Clear");

  fullview_button.set_text("1/1");
  halfview_button.set_text("1/2");

  quarterview_button.set_text("1/4");

  capture_button.on_clicked(util::bind_method(this, &spectrometer::capture));
  save_button.on_clicked(util::bind_method(this, &spectrometer::save_image));
  clear_button.on_clicked([&] () {
    extra_bytes = 0;
    extra.refresh();
  });
  fullview_button.on_clicked([&] () {
    auto sz = camera.get_sensor_size();
    set_crop({0, 0, (int32_t)sz.width, (int32_t)sz.height});
    camera.set_resolution(sz);
  });
  halfview_button.on_clicked([&] () {
    raspi_camera::size sz = camera.get_sensor_size();
    sz = raspi_camera::size{(uint32_t)(sz.width / 2), (uint32_t)(sz.height / 2)};
    set_crop({(int32_t)(sz.width / 4), (int32_t)(sz.height / 4), (int32_t)sz.width, (int32_t)sz.height});
    camera.set_resolution(sz);
  });
  quarterview_button.on_clicked([&] () {
    raspi_camera::size sz = camera.get_sensor_size();
    sz = raspi_camera::size{(uint32_t)(sz.width / 4), (uint32_t)(sz.height / 4)};
    set_crop({(int32_t)((sz.width / 8) * 3), (int32_t)((sz.height / 8) * 3), (int32_t)sz.width, (int32_t)sz.height});
    camera.set_resolution(sz);
  });

  x_pos.set_handler(16, 160, util::bind_method(this, &spectrometer::change_crop_x));
  x_pos.set_label("X");
  x_pos.set_value([&] () { return ostreamfmt(get_crop().x); });

  y_pos.set_handler(16, 80, util::bind_method(this, &spectrometer::change_crop_y));
  y_pos.set_label("Y");
  y_pos.set_value([&] () { return ostreamfmt(get_crop().y); });

  w_pos.set_handler(32, 160, util::bind_method(this, &spectrometer::change_crop_w));
  w_pos.set_label("W");
  w_pos.set_value([&] () { return ostreamfmt(get_crop().width); });

  h_pos.set_handler(16, 80, util::bind_method(this, &spectrometer::change_crop_h));
  h_pos.set_label("H");
  h_pos.set_value([&] () { return ostreamfmt(get_crop().height); });

  res_w_pos.set_handler(16, 80, [&] (int32_t v) {
    auto sz = camera.get_resolution();
    sz.width += v;
    camera.set_resolution(sz);
  });
  res_w_pos.set_label("Res X");
  res_w_pos.set_value([&] () { return ostreamfmt(camera.get_resolution().width); });

  res_h_pos.set_handler(16, 80, [&] (int32_t v) {
    auto sz = camera.get_resolution();
    sz.height += v;
    camera.set_resolution(sz);
  });
  res_h_pos.set_label("Res Y");
  res_h_pos.set_value([&] () { return ostreamfmt(camera.get_resolution().height); });

  ss.set_handler([&] (int step) {
    auto current = camera.get_shutter_speed();
    if (step > 0.0F) {
      camera.set_shutter_speed(current * step);
    } else {
      camera.set_shutter_speed(current / -step);
    }
    ss.refresh();
//    capture();
  });
  ss.set_steps(1.1F, 2.0F);
  ss.set_label("Shutter");
  ss.set_value([&] () { return ostreamfmt(camera.get_shutter_speed()); });

  iso.set_handler([&] (int step) {
    auto current = camera.get_iso();
    if (step > 0.0F) {
      camera.set_iso(current * step);
    } else {
      camera.set_iso(std::max<uint32_t>(current / -step, 25));
    }
    iso.refresh();
//    capture();
  });
  iso.set_steps(2.0F, 4.0F);
  iso.set_label("ISO");
  iso.set_value([&] () { return ostreamfmt(camera.get_iso()); });

  extra.set_handler([&] (int step) {
    extra_bytes = std::max(extra_bytes + step, 0);
    extra.refresh();
    display();
  });
  extra.set_steps(1, 10);
  extra.set_label("Extra");
  extra.set_value([&] () { return ostreamfmt(extra_bytes); });

}

void spectrometer::capture () {
  LogDebug << "Capture image";

  try {
    encoder->clear_data();
    capture_view.image.clear();
    encoder->capture(0);

  } catch (std::exception& ex) {
    LogFatal << ex;
  }
}

void spectrometer::display () {
  auto sz = camera.get_size();
  auto encoding = (MMAL_FOURCC_T)encoder->get_encoding();
  LogDebug << "Display " << data.size() << " Bytes with dimensions:" << sz.width << "x" << sz.height << " ppl:" << camera.get_pixel_per_line() << " enc:" << raspi::core::four_cc(encoding);
  if (data.size()) {
    switch (encoding) {
      case MMAL_ENCODING_PPM: {
        rgbmap image_data;
        std::istringstream strm(std::string((const char*)data.data(), data.size()));
        gui::io::load_pnm<PixelFormat::RGB>(strm, image_data);
        LogDebug << "Display image with dimensions:" << image_data.native_size() << ", ppl:" << image_data.pixel_format();
        capture_view.image = image_data;
        break;
      }
      case MMAL_ENCODING_RGBA:
      case MMAL_ENCODING_RGBA_SLICE: {
        bitmap_info bmi(sz.width, sz.height, camera.get_pixel_per_line() * 4 + extra_bytes, PixelFormat::RGBA);
        const_image_data<PixelFormat::RGBA> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        LogDebug << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        capture_view.image = image_data;
        break;
      }
      case MMAL_ENCODING_RGB24:
      case MMAL_ENCODING_RGB24_SLICE: {
        bitmap_info bmi(sz.width, sz.height, camera.get_pixel_per_line() * 3 + extra_bytes, PixelFormat::RGB);
        const_image_data<PixelFormat::RGB> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        LogDebug << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        capture_view.image = image_data;
        break;
      }
      case MMAL_ENCODING_BGRA:
      case MMAL_ENCODING_BGRA_SLICE: {
        bitmap_info bmi(sz.width, sz.height, camera.get_pixel_per_line() * 4 + extra_bytes, PixelFormat::BGRA);
        const_image_data<PixelFormat::BGRA> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        LogDebug << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        capture_view.image = image_data;
        break;
      }
      case MMAL_ENCODING_BGR24:
      case MMAL_ENCODING_BGR24_SLICE: {
        bitmap_info bmi(sz.width, sz.height, camera.get_pixel_per_line() * 3 + extra_bytes, PixelFormat::BGR);
        const_image_data<PixelFormat::BGR> image_data(core::array_wrapper<const byte>(data.data(), data.size()), bmi);
        LogDebug << "Display image with dimensions:" << bmi.size() << ", fmt:" << bmi.pixel_format;
        capture_view.image = image_data;
        break;
      }

      default:
        LogDebug << "Can not display image with encoding: " << encoding;
        break;
    }

//  rgbmap image(capture_view.size());
//  image.stretch_from(rgbmap(image_data));
  }
  capture_view.redraw();
}

void spectrometer::calc_spectrum () {
  auto sz = spectrum_view.size();
  if (spectrum_view.image.scaled_size() != sz) {
    spectrum_view.image.create(sz);
  }
  graphics g(spectrum_view.image);
  double scale = 500.0 / sz.width();
  for (int x = 0; x < sz.width(); ++x) {
    os::color rgb = io::optics::wave_length_to_rgb(330.0 + static_cast<double>(x) * scale);
    g.draw_lines({core::point(x, 0), core::point(x, sz.height())}, draw::pen(rgb));
  }
  spectrum_view.redraw();
}

void spectrometer::save_image () {
  file_save_dialog::show(*this, "Save captured data", "spectrogram.ppm", "Name:", "OK", "Cancel",
                         [&] (const sys_fs::path& path) {
    std::ofstream file(path);
    if ((MMAL_FOURCC_T)encoder->get_encoding() != MMAL_ENCODING_PPM) {
      auto crop = camera.get_abs_crop();
      file << "P6" << std::endl << crop.width << " " << crop.height << std::endl << "255" << std::endl;
    }
    file.write((const char*)data.data(), data.size());
  });
}

void spectrometer::save_settings () {

}

void spectrometer::load_settings () {

}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
//  bcm_host_init();
//  bcm_host_deinit();
//  return 0;

  spectrometer main;

  main.create({0, 0, 1420, 800});
  main.set_title("Raspi Spectrometer");
  main.set_visible();

  return gui::win::run_main_loop();
}
