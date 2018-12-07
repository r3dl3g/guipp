

#include <gui/win/attach_layout.h>
#include <gui/win/border_layout.h>
#include <gui/win/adaption_layout.h>
#include <gui/win/layout_container.h>
#include <gui/ctrl/button.h>
#include <gui/ctrl/std_dialogs.h>
#include <gui/draw/graphics.h>
#include <gui/draw/bitmap.h>

#ifdef BUILD_FOR_ARM
#include "raspi/encoder.h"
#else
#define MMAL_ENCODING_RGB24_SLICE 0
namespace gui {
  namespace raspi {
    namespace camera {

      struct raspi_camera {
        raspi_camera ()
          : cr{0, 0, 1, 1}
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

        crop get_crop () const { return cr; }
        void set_crop (const crop& c) { cr = c; }

        int get_shutter_speed () const { return ss; }
        void set_shutter_speed (int s) { ss = s; }
        int get_iso () const { return iso; }
        void set_iso (int i) { iso = i; }

      private:
        crop cr;
        awb_gains g;
        int ss;
        int iso;
      };

      struct raspi_raw_encoder {
        raspi_raw_encoder(raspi_camera&, int) {}

        void capture (int) {}

        std::string get_data () { return {}; }
      };
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
      if (image) {
        graph.copy_from(image, core::point::zero);
      } else {
        graph.fill(draw::rectangle(this->client_area()), background);
      }
    }));
  }

  rgbmap image;
};

class value_block : public group_window<layout::vertical_adaption<>> {
public:
  using super = layout_container<layout::vertical_adaption<>>;
  using fnkt = std::function<void()>;

  value_block () {
    on_create(basepp::bind_method(this, &value_block::onCreated));
  }

  value_block (const std::string& l, text_source&& src)
    : label(l)
    , value(std::move(src))
  {
    on_create(basepp::bind_method(this, &value_block::onCreated));
  }

  void set_label (const std::string& l) {
    label.set_text(l);
  }

  void set_value (text_source&& src) {
    value.set_text(std::move(src));
  }

  void onCreated (window*, const core::rectangle&) {
    label.create(*this);
    label.set_visible();
    up_button.create(*this, "+");
    up_button.set_visible();
    value.create(*this);
    value.set_visible();
    down_button.create(*this, "-");
    down_button.set_visible();
    set_visible();
  }

  void set_handler (fnkt&& up, fnkt&& down) {
    up_button.on_clicked(std::move(up));
    down_button.on_clicked(std::move(down));
  }

  void refresh () {
    value.redraw();
  }

private:
  ctrl::text_button up_button;
  ctrl::text_button down_button;
  ctrl::label_center label;
  ctrl::label_center value;
};

// --------------------------------------------------------------------------
class spectrometer : public layout_main_window<gui::layout::border_layout<>, float, float, float, float> {
public:
  using super = layout_main_window<gui::layout::border_layout<>, float, float, float, float>;
  spectrometer ();

  void onCreated (window*, const core::rectangle&);
  void quit ();

  void capture ();
  void calc_spectrum ();
  void save_image ();

  void save_settings ();
  void load_settings ();

private:
  group_window<layout::vertical_adaption<>> button_view;
  ctrl::text_button capture_button;
  ctrl::text_button save_button;

  value_block x_pos;
  value_block y_pos;
  value_block w_pos;
  value_block h_pos;

  value_block ss;
  value_block iso;

  group_window<layout::horizontal_adaption<0, 5>> values_view;

  image_view<color::black> capture_view;
  image_view<> spectrum_view;

  raspi_camera camera;
  raspi_raw_encoder encoder;

};

spectrometer::spectrometer ()
  : super(200, 160, 0, 0)
  , encoder(camera, MMAL_ENCODING_RGB24_SLICE)
{
  on_create(basepp::bind_method(this, &spectrometer::onCreated));
  on_destroy(&gui::win::quit_main_loop);
//  on_close(basepp::bind_method(this, &spectrometer::quit));

  capture_button.on_clicked(basepp::bind_method(this, &spectrometer::capture));
  save_button.on_clicked(basepp::bind_method(this, &spectrometer::save_image));

  x_pos.set_handler([&] () {
    auto crop = camera.get_crop();
    crop.x = std::min(crop.x + 0.05F, crop.width);
    camera.set_crop(crop);
    x_pos.refresh();
  }, [&] () {
    auto crop = camera.get_crop();
    crop.x = std::max(crop.x - 0.05F, 0.0F);
    camera.set_crop(crop);
    x_pos.refresh();
  });
  x_pos.set_label("X");
  x_pos.set_value([&] () { return ostreamfmt(camera.get_crop().x); });

  y_pos.set_handler([&] () {
    auto crop = camera.get_crop();
    crop.y = std::min(crop.y + 0.05F, crop.height);
    camera.set_crop(crop);
    y_pos.refresh();
  }, [&] () {
    auto crop = camera.get_crop();
    crop.y = std::max(crop.y - 0.05F, 0.0F);
    camera.set_crop(crop);
    y_pos.refresh();
  });
  y_pos.set_label("Y");
  y_pos.set_value([&] () { return ostreamfmt(camera.get_crop().y); });

  w_pos.set_handler([&] () {
    auto crop = camera.get_crop();
    crop.width = std::min(crop.width + 0.05F, 1.0F - crop.x);
    camera.set_crop(crop);
    w_pos.refresh();
  }, [&] () {
    auto crop = camera.get_crop();
    crop.width = std::max(crop.width - 0.05F, 0.0F);
    camera.set_crop(crop);
    w_pos.refresh();
  });
  w_pos.set_label("W");
  w_pos.set_value([&] () { return ostreamfmt(camera.get_crop().width); });

  h_pos.set_handler([&] () {
    auto crop = camera.get_crop();
    crop.height = std::min(crop.height + 0.05F, 1.0F - crop.y);
    camera.set_crop(crop);
    h_pos.refresh();
  }, [&] () {
    auto crop = camera.get_crop();
    crop.height = std::max(crop.height - 0.05F, 0.0F);
    camera.set_crop(crop);
    h_pos.refresh();
  });
  h_pos.set_label("H");
  h_pos.set_value([&] () { return ostreamfmt(camera.get_crop().height); });

  ss.set_handler([&] () {
    camera.set_shutter_speed(camera.get_shutter_speed() * 1.5);
    ss.refresh();
  }, [&] () {
    camera.set_shutter_speed(camera.get_shutter_speed() / 1.5);
    ss.refresh();
  });
  ss.set_label("Shutter");
  ss.set_value([&] () { return ostreamfmt(camera.get_shutter_speed()); });

  iso.set_handler([&] () {
    camera.set_iso(camera.get_iso() * 2);
    iso.refresh();
  }, [&] () {
    camera.set_iso(camera.get_iso() / 2);
    iso.refresh();
  });
  iso.set_label("ISO");
  iso.set_value([&] () { return ostreamfmt(camera.get_iso()); });

  capture_button.set_text("Capture");
  save_button.set_text("Save");
}

void spectrometer::quit () {
  yes_no_dialog::ask(*this, "Question!", "Do you realy want to exit?", "Yes", "No", [&] (bool yes) {
    if (yes) {
      save_settings();
      win::quit_main_loop();
    } else {
      take_focus();
    }
  });
}

void spectrometer::onCreated (window*, const core::rectangle&) {
  capture_view.create(*this);
  capture_view.set_visible();
  spectrum_view.create(*this);
  spectrum_view.set_visible();
  values_view.create(*this);
  values_view.set_visible();

  x_pos.create(values_view);
  x_pos.set_visible();
  y_pos.create(values_view);
  y_pos.set_visible();
  w_pos.create(values_view);
  w_pos.set_visible();
  h_pos.create(values_view);
  h_pos.set_visible();
  ss.create(values_view);
  ss.set_visible();
  iso.create(values_view);
  iso.set_visible();
  button_view.create(values_view);
  button_view.set_visible();
  capture_button.create(button_view);
  capture_button.set_visible();
  save_button.create(button_view);
  save_button.set_visible();

  get_layout().set_center_top_bottom_left_right(&spectrum_view, &capture_view, &values_view, nullptr, nullptr);
  load_settings();
  set_children_visible();
}

void spectrometer::capture () {
  encoder.capture(5000);
  auto data = encoder.get_data();
}

void spectrometer::calc_spectrum () {

}

void spectrometer::save_image () {

}

void spectrometer::save_settings () {

}

void spectrometer::load_settings () {

}

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  spectrometer main;

  main.create({0, 0, 800, 600});
  main.set_title("Raspi Spectrometer");
  main.set_visible();

  return gui::win::run_main_loop();
}
