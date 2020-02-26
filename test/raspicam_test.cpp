
#include "raspi/encoder.h"
#include "util/ostreamfmt.h"
#include "util/command_line.h"
#include "logging/logger.h"


#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

template<typename T>
T parse_arg (const std::string& str, const std::string& name) {
  T t = T();
  std::istringstream(str) >> t;
  clog::debug() << "Found " << name << ":'" << t << "'";
  return t;
}

template<class T, size_t N>
constexpr size_t size(T (&)[N]) { return N; }

template<typename T, size_t N>
std::ostream& operator<< (std::ostream& out, const std::array<T, N>& a) {
  for (const T& t: a) {
    if (t != a[0]) {
      out << ", ";
    }
    out << t;
  }
  return out;
}

std::string generate_filename (const std::string& base, const std::string& ext, int count, int num) {
  if (count > 1) {
    return ostreamfmt(base << "." << num << "." << ext);
  } else {
    return ostreamfmt(base << "." << ext);
  }
}

// --------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
  clog::debug() << "raspi camera test";

  using namespace raspi::camera;
  using namespace raspi::encoder;
  still camera;

  raspi::core::four_cc encoding = 0;
  bool use_raw = false;
  bool needs_commit = false;
  std::string outname = "raspicam_test";
  int count = 1;

  util::command_line::parser("raspicam_test V 0.1.0",
  {
    {"-e", "--encoding", "<FOURCC>", "Use <FOURCC> encoding (BMP, PNG, PPM, JPEG(default), GIF, TGA)",
      [&](const std::string& arg) {
        encoding = parse_arg<raspi::core::four_cc>(arg, "encoding");
      }},
    {"-r", "--raw", {}, "Use raw capture (-f: BD10, bRA8, bGA8, BGGR, RGAA, I420, S420, I422, S422, RGBA, rgba, RGB3, rgb3)",
      [&](const std::string&) {
        use_raw = true;
        clog::info() << "Enable raw";
      }},
    {"-ss", "--shutter", "<us>", "Set shutter speed in us",
      [&](const std::string& arg) {
        camera.set_shutter_speed(parse_arg<int>(arg, "Shutter speed"));
      }},
    {"-ag", "--awb", "<R,B>", "Set AWB gains red (R) and blue (B)",
      [&](const std::string& arg) {
        camera.set_awb_mode(still::AWBMode::Off);
        camera.set_awb_gains(parse_arg<still::awb_gains>(arg, "AWB gains"));
      }},
    {"-am", "--awb-mode", "<Mode>", ostreamfmt("Set AWB-Mode (" << raspi::camera::AWBModes << ")"),
      [&](const std::string& arg) {
        camera.set_awb_mode(parse_arg<still::AWBMode>(arg, "AWB mode"));
      }},
    {"-cr", "--crop", "<X,Y,W,H>", "Crop image by <X,Y,W,H> in normalised coordinates [0.0-1.0]",
      [&](const std::string& arg) {
        camera.set_input_crop(parse_arg<raspi::core::crop>(arg, "Crop"));
        needs_commit = true;
      }},
    {"-sz", "--size", "<W,H>", "Use image size <W,H>",
      [&](const std::string& arg) {
        camera.set_resolution(parse_arg<still::size>(arg, "Size"));
        needs_commit = true;
      }},
//    {"-rz", "--resize", "<W,H>", "Use image resize <W,H>",
//      [&](const std::string& arg) {
//        auto sz = parse_arg<still::size>(arg, "Resize");
//        camera.set_resize({{}, MMAL_RESIZE_CROP, sz.width, sz.height, 0, false, false});
//      }},
    {"-i", "--iso", "<ISO>", "Set sensor ISO",
      [&](const std::string& arg) {
        camera.set_iso(parse_arg<uint32_t>(arg, "ISO"));
      }},
    {"-sh", "--sharpness", "[-1..1]", "Set image sharpness",
      [&](const std::string& arg) {
        camera.set_sharpness(parse_arg<float>(arg, "Sharpness"));
      }},
    {"-co", "--contrast", "[-1..1]", "Set image contrast",
      [&](const std::string& arg) {
        camera.set_contrast(parse_arg<float>(arg, "Contrast"));
      }},
    {"-br", "--brightness", "[0..1]", "Set image brightness",
      [&](const std::string& arg) {
        camera.set_brightness(parse_arg<float>(arg, "Brightness"));
      }},
    {"-sa", "--saturation", "[-1..1]", "Set image saturation",
      [&](const std::string& arg) {
        camera.set_saturation(parse_arg<float>(arg, "Saturation"));
      }},
    {"-sm", "--mode", "[1..7]", "Set sensor mode (1: Full HD 16:9, 2: Full size 4:3 low, 3: Full size 4:3 high, 4: Half size 4:3, 5: Half size 16:9, 6: 640x480 or 1280x720, 7: 640x480 4:3",
      [&](const std::string& arg) {
        camera.set_sensor_mode(parse_arg<still::SensorMode>(arg, "SensorMode"));
        needs_commit = true;
      }},
    {"-cn", "--count", "[0..x]", "Take <count> pictures",
      [&](const std::string& arg) {
       count = parse_arg<int>(arg, "Count");
      }},
    {"-o", "--out", "<filename>", "Set output file name",
      [&](const std::string& arg) {
        outname = util::string::trimed(arg);
        clog::info() << "Found output file name:'" << outname << "'";
      }}
  }).process(argc, argv);

  camera.enable();

  clog::debug() << "Camera info: " << camera;

  if (use_raw) {
    if (!encoding) {
      encoding = MMAL_ENCODING_BGR24;
    }
    raw encoder(camera.get_still_output_port(), raw::OutEncoding(encoding.type.uint32));
    clog::debug() << "raw capture " << encoding;
    encoder.enable();

    for (int i = 0; i < count; ++i) {
      encoder.capture(5000);

      const base::image_data& data = encoder.get_data();

      clog::debug() << "get_data size:" << data.size();

      std::ofstream file(generate_filename(outname, ostreamfmt(encoding), count, i));
      auto sz = camera.get_size();
      file << "P6\n"
           << "# pnm created by raspicam_test\n"
           << sz.width << " " << sz.height << " 255\n";

      auto ppl = data.size() / sz.height;
      for (int y = 0; y < sz.height; ++y) {
        auto ptr = data.data() + y * ppl;
        file.write((const char*)ptr, sz.width * 3);
      }
    }

  } else {
    if (!encoding) {
      encoding = MMAL_ENCODING_JPEG;
    }
    encoded encoder(camera.get_still_output_port(), encoded::OutEncoding(encoding.type.uint32));
    clog::debug() << "encoded capture " << encoding;

    encoder.enable();

    for (int i = 0; i < count; ++i) {
      encoder.capture(5000);

      auto data = encoder.get_data();

      clog::debug() << "get_data size:" << data.size();

      std::ofstream file(generate_filename(outname, ostreamfmt(encoding), count, i));
      file.write((const char*)data.data(), data.size());
    }
  }

  logging::core::instance().finish();

  return 0;
}
