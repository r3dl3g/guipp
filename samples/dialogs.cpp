

#include <gui/ctrl/std_dialogs.h>
#include <logging/core.h>

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

// --------------------------------------------------------------------------
int gui_main(const std::vector<std::string>& /*args*/) {
  using namespace gui::ctrl;
  using namespace gui::win;

  main_window main;
 
  yes_no_dialog::ask(main, "Question", "This is a Yes/No dialog", "Yes", "no", [&] (bool y) {
    message_dialog::show(main, "Message", ostreamfmt("This is a message dialog\nYou choose: " << (y ? "Yes" : "No")), "ok");
  });

  return 0;//run_main_loop();
}
