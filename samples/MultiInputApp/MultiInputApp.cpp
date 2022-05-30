

#include <iostream>
#include <gui/ctrl/multi_input_dialog.h>
#include <util/currency.h>

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::ctrl;
  using namespace gui::win;
  using namespace util::currency_literals;
  using namespace std::chrono_literals;

  typedef util::time::time_point time_point;
  typedef util::time::duration duration;
  typedef util::Euro currency;

  auto now = std::chrono::system_clock::now();
  auto oneh2m3s = 1h + 2min + 3s;

  main_window main;

  multi_input_dialog<std::string, bool, int, float, time_point, duration, currency>
      ::ask(main, "Input", {"Text:", "Boolean", "Number:", "Float:", "Time point:", "Duration:", "Currency:"},
            std::make_tuple(std::string("Text"), false, 17, 47.11, now, oneh2m3s, 12.34_EUR),
            "Yes", "no",
            [&] (const std::tuple<std::string, bool, int, float, time_point, duration, currency>& t) {
    message_dialog::show(main, "Your input",
                         str_fmt() << "Your text: '" << std::get<0>(t) << "'"
                                   << "\nbool: " << std::get<1>(t)
                                   << "\nnumber: " << std::get<2>(t)
                                   << ", float: " << std::get<3>(t)
                                   << ", currency: " << std::get<6>(t)
                                   << "\ntime point: " << std::get<4>(t)
                                   << "\nduration: " << std::get<5>(t),
                         "ok");
  });

  return 0;//run_main_loop();
}
