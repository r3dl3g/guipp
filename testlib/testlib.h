// testlib.h
//

#include <sstream>
#include <logging/logger.h>
#include <logging/dbgstream.h>


using namespace std;

namespace testing {

  namespace detail {

    template<typename T>
    inline void print_value (std::ostream& out, const T& v) {
      out << v;
    }

    template<>
    inline void print_value <unsigned long> (std::ostream& out, const unsigned long& v) {
      out << "0x" << std::hex << v << std::dec;
    }

    template<>
    inline void print_value <unsigned int> (std::ostream& out, const unsigned int& v) {
      out << "0x" << std::hex << v << std::dec;
    }

    template<>
    inline void print_value <unsigned short> (std::ostream& out, const unsigned short& v) {
      out << "0x" << std::hex << v << std::dec;
    }

    template<>
    inline void print_value <unsigned char> (std::ostream& out, const unsigned char& v) {
      out << "0x" << std::hex << (unsigned short)v << std::dec;
    }

    inline void print_to_stream (std::ostream&) {}

    template<typename Head, typename...Tail>
    void print_to_stream (std::ostream& stream, const Head& h, const Tail&... t) {
      stream << h;
      print_to_stream(stream, t...);
    }

  } // namespace detail

  template<typename T1, typename T2, typename ... Arguments>
  void create_error_message (std::ostream& os,
                             const T1& testValue,
                             const T2& expectedValue,
                             const char* testName,
                             const char* expectedName,
                             const char* equality,
                             const char* fileName,
                             const int   lineNumber,
                             const Arguments... args) {
    os << fileName << ":" << lineNumber << ": "
       << "Expected " << testName << " "
       << "to be " << equality << " '";
    detail::print_value(os, expectedValue);
    os << "' (" << expectedName << ") but it was '";
    detail::print_value(os, testValue);
    os << "'";
    detail::print_to_stream(os, args...);
  }

  template<typename T1, typename T2, typename ... Arguments>
  void throw_error (const T1& testValue,
                    const T2& expectedValue,
                    const char* testName,
                    const char* expectedName,
                    const char* equality,
                    const char* fileName,
                    const int   lineNumber,
                    const Arguments... args) {
    std::ostringstream os;
    create_error_message(os, testValue, expectedValue, testName, expectedName,
                         equality, fileName, lineNumber, args...);
    throw std::runtime_error(os.str());
  }

  template<typename T1, typename T2, typename ... Arguments>
  void log_error (const T1& testValue,
                  const T2& expectedValue,
                  const char* testName,
                  const char* expectedName,
                  const char* equality,
                  const char* fileName,
                  const int   lineNumber,
                  const Arguments... args) {
    create_error_message(LogDebug, testValue, expectedValue,
                         testName, expectedName, equality,
                         fileName, lineNumber, args...);
  }

  template<typename T1, typename T2>
  bool equal_test (const T1& testValue,
                   const T2& expectedValue) {
    return testValue == expectedValue;
  }

  template<typename T1, typename T2>
  bool lower_test (const T1& testValue,
                   const T2& expectedValue) {
    return testValue < expectedValue;
  }

  template<typename T1, typename T2>
  bool lower_equal_test (const T1& testValue,
                         const T2& expectedValue) {
    return testValue <= expectedValue;
  }

  template<typename T1, typename T2>
  bool higher_test (const T1& testValue,
                    const T2& expectedValue) {
    return testValue > expectedValue;
  }

  template<typename T1, typename T2>
  bool higher_equal_test (const T1& testValue,
                          const T2& expectedValue) {
    return testValue >= expectedValue;
  }

  template<>
  bool equal_test (const double& testValue,
                   const double& expectedValue);

  std::string string_from_file (const char* filename);

} // namespace testing


#define TEST_MAIN(a)\
namespace {\
  int guipp_failed_test_count = 0;\
  int guipp_test_count = 0;\
}\
\
int gui_main(const std::vector<std::string>& /*args*/) {\
  logging::odebugstream dbgStrm;\
  logging::core::instance().add_sink(&dbgStrm, logging::level::info, logging::core::get_console_formatter());\
  LogWarng << "Running " #a " tests";

#define TEST_MAIN_END(a)\
  if (guipp_failed_test_count) {\
    LogError << #a << ": " << guipp_failed_test_count << " of " << guipp_test_count << " tests failed";\
  } else {\
    LogError << #a << ": all " << guipp_test_count << " tests passed";\
  }\
  logging::core::instance().remove_sink(&dbgStrm);\
  return guipp_failed_test_count;\
}

#define DECLARE_TEST(a)\
  void a ()

#define RUN_TEST(a)\
  ++guipp_test_count;\
  try {\
    a();\
  } catch (std::exception& ex) {\
    ++guipp_failed_test_count;\
    LogFatal.raw() << #a << " failed with " << ex.what();\
  }

#define DEFINE_TEST(a)\
void a () {\
  const std::string __test_name = #a;\
  LogWarng << #a << " started";

#define END_TEST(...)\
  LogWarng << __test_name << " passed";\
}

#define EXPECT_EQUAL(test, expect, ...)\
  if (!(testing::equal_test(test, expect))) testing::throw_error(test, expect, #test, #expect, "equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_NOT_EQUAL(test, expect, ...)\
  if (testing::equal_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "not equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_LOWER(test, expect, ...)\
  if (!testing::lower_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "lower than", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_LOWER_OR_EQUAL(test, expect, ...)\
  if (!testing::lower_equal_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "lower or equal than", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_HIGHER(test, expect, ...)\
  if (!testing::higher_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "higher than", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_HIGHER_OR_EQUAL(test, expect, ...)\
  if (!testing::higher_equal_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "higher or equal than", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_TRUE(test, ...)\
  if (!testing::equal_test<bool, bool>(test, true)) testing::throw_error<bool, bool>(test, true, #test, "true", "equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define EXPECT_FALSE(test, ...)\
  if (!testing::equal_test<bool, bool>(test, false)) testing::throw_error<bool, bool>(test, false, #test, "false", "equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_EQUAL(test, expect, ...)\
  if (!(testing::equal_test(test, expect))) testing::log_error(test, expect, #test, #expect, "equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_NOT_EQUAL(test, expect, ...)\
  if (testing::equal_test(test, expect)) testing::log_error(test, expect, #test, #expect, "not equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_LOWER(test, expect, ...)\
  if (!testing::lower_test(test, expect)) testing::log_error(test, expect, #test, #expect, "lower than", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_LOWER_OR_EQUAL(test, expect, ...)\
  if (!testing::lower_equal_test(test, expect)) testing::log_error(test, expect, #test, #expect, "lower or equal than", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_HIGHER(test, expect, ...)\
  if (!testing::higher_test(test, expect)) testing::log_error(test, expect, #test, #expect, "higher than", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_HIGHER_OR_EQUAL(test, expect, ...)\
  if (!testing::higher_equal_test(test, expect)) testing::log_error(test, expect, #test, #expect, "higher or equal than", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_TRUE(test, ...)\
  if (!testing::equal_test<bool, bool>(test, true)) testing::log_error<bool, bool>(test, true, #test, "true", "equal", __FILE__, __LINE__, ##__VA_ARGS__);

#define TEST_FALSE(test, ...)\
  if (!testing::equal_test<bool, bool>(test, false)) testing::log_error<bool, bool>(test, false, #test, "false", "equal", __FILE__, __LINE__, ##__VA_ARGS__);
