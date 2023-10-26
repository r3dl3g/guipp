

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
  main.create();

  auto option = mk_selectable_option<std::string, std::array<std::string, 5>>("Default", {"One", "Two", "Default", "Three", "Four"});

  std::string value = "Default";
  std::array<std::string, 5> values{"One", "Two", "Default", "Three", "Four"};
  auto option2 = mk_selectable_option(value, values);

  std::vector<std::string> values2{"One", "Two", "Default", "Three", "Four"};
  auto option3 = mk_selectable_option(value, values2);

  multi_input_dialog<std::string, bool, int, float, time_point, duration, currency, decltype(option2)>
      ::ask(main, "Input", {"Text:", "Boolean", "Number:", "Float:", "Time point:", "Duration:", "Currency:", "Option"},
            std::make_tuple(std::string("Text"), false, 17, 47.11, now, oneh2m3s, 12.34_EUR, option2),
            "Yes", "no",
            [&] (const std::tuple<std::string, bool, int, float, time_point, duration, currency, std::string>& t) {
    message_dialog::show(main, "Your input",
                         str_fmt() << "Your text: '" << std::get<0>(t) << "'"
                                   << "\nbool: " << std::get<1>(t)
                                   << "\nnumber: " << std::get<2>(t)
                                   << ", float: " << std::get<3>(t)
                                   << ", currency: " << std::get<6>(t)
                                   << "\ntime point: " << std::get<4>(t)
                                   << "\nduration: " << std::get<5>(t)
                                   << "\noption: " << std::get<7>(t),
                         "ok");
  });

  return 0;//run_main_loop();
}
