

#include <gui/io/pnm_to_src.h>
#include <util/sys_fs.h>

using namespace gui;
using namespace gui::io;
using namespace gui::draw;

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& args) {
  bool first = true;
  for (const auto& f: args) {
    if (first) {
      first = false;
      continue;
    }
    sys_fs::path p(f);
    auto stem = p.stem().string();
    p.replace_extension("h");
    auto src = p.string();
    clog::info() << "Convert " << f << " to " << src;
    basic_datamap bmp;
    load_pnm(f, bmp);
    src::save_pnm_src(src, bmp, stem);
  }

  return 0;
}
