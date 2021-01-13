

#include <gui/io/pnm_to_src.h>
#include <util/command_line.h>
#include <util/sys_fs.h>
# include <experimental/filesystem>


using namespace gui;
using namespace gui::io;
using namespace gui::draw;

enum class Target {
  Source,
  Ascii,
  Binary
};

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  Target t = Target::Source;

  util::command_line::parser cmd("pnm2src", {
    {"-a", "--asscii", {}, "write pnm 2 ascii pnm",
      [&](const std::string&) {
        t = Target::Ascii;
      }
    },
    {"-b", "--binary", {}, "write pnm 2 binary pnm",
      [&](const std::string&) {
        t = Target::Binary;
      }
    },
    {"-s", "--source", {}, "write pnm 2 source header",
      [&](const std::string&) {
        t = Target::Source;
      }
    },
  });

  cmd.process(args);

  bool first = true;
  for (const auto& f: cmd.remaining_args()) {
    if (first) {
      first = false;
      continue;
    }
    basic_datamap bmp;
    load_pnm(f, bmp);

    namespace sys_fs = std::experimental::filesystem;

    sys_fs::path p(f);
    auto stem = p.stem().string();

    using F = pixel_format_t;
    switch (t) {
      case Target::Source: {
        p.replace_extension("h");
        auto src = p.string();
        clog::info() << "Convert " << f << " to " << src;
        src::save_pnm_src(src, bmp, stem);
        break;
      }
      case Target::Ascii: {
        p.replace_extension(".ascii" + p.extension().string());
        auto out = p.string();
        clog::info() << "Convert " << f << " to " << out;
        switch (bmp.pixel_format()) {
          case F::BW: io::save_pnm(out, bmp.const_reinterpret<F::BW>(), false); break;
          case F::GRAY: io::save_pnm(out, bmp.const_reinterpret<F::GRAY>(), false); break;
          case F::RGB: io::save_pnm(out, bmp.const_reinterpret<F::RGB>(), false); break;
          default:
            break;
        }
        break;
      }
      case Target::Binary: {
        p.replace_extension(".bin" + p.extension().string());
        auto out = p.string();
        clog::info() << "Convert " << f << " to " << out;
        switch (bmp.pixel_format()) {
          case F::BW: io::save_pnm(out, bmp.const_reinterpret<F::BW>(), true); break;
          case F::GRAY: io::save_pnm(out, bmp.const_reinterpret<F::GRAY>(), true); break;
          case F::RGB: io::save_pnm(out, bmp.const_reinterpret<F::RGB>(), true); break;
          default:
            break;
        }
        break;
      }
    }
  }

  return 0;
}
