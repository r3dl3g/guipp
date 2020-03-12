// SettingsFile Test: Test for reading and writing the camera settings file
//

#include "testlib.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <iomanip>

#include <logging/core.h>
#include <logging/dbgstream.h>

using namespace std;

#define NOTHING

DEFINE_LOGGING_CORE(NOTHING)

namespace testing {

  template<>
  bool equal_test (const double& testValue,
                   const double& expectedValue) {
    // doubles may differ below 1.0E-7
    const double diff = fabs(testValue - expectedValue);
    return (diff < 1.0E-7);
  }

  std::string string_from_file (const char* filename) {
    std::ifstream file(filename);
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
  }

}

namespace {
  int guipp_failed_test_count = 0;
  int guipp_test_count = 0;
}

void run_test_ (const std::string& name, testing::test_function& fn) {
  clog::warn() << name << " started";
  ++guipp_test_count;
  try {
    fn();\
    clog::warn() << name << " passed";\

  } catch (std::exception& ex) {
    ++guipp_failed_test_count;
    clog::fatal().raw() << name << " Test failed with " << ex.what();
  }
}

#ifdef X11
int main (int, char*[]) {
  logging::odebugstream dbgStrm(logging::level::info, logging::core::get_console_formatter());
#endif // X11
#ifdef WIN32
  int APIENTRY WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int) {
#endif // WIN32

  clog::warn() << "Running tests";

  test_main();

  if (guipp_failed_test_count) {
    clog::error() << guipp_failed_test_count << " of " << guipp_test_count << " tests failed";
  } else {
    clog::error() << "all " << guipp_test_count << " tests passed";
  }
  return guipp_failed_test_count;
}
