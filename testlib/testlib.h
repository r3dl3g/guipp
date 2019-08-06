// testlib.h
//

#include <sstream>

using namespace std;

#define TEST_MAIN()\
namespace {\
  int guipp_test_result = 0;\
}\
\
int gui_main(const std::vector<std::string>& /*args*/) {

#define TEST_MAIN_END()\
  if (!guipp_test_result) LogWarng << "All pixmap tests passed";\
  return guipp_test_result;\
}

#define DECLARE_TEST(a)\
  void a ()

#define RUN_TEST(a)\
  try {\
    a();\
  } catch (std::exception& ex) {\
    LogFatal << #a << " failed with " << ex.what();\
    guipp_test_result = 1;\
  }

#define DEFINE_TEST(a)\
void a () {\
  LogWarng << #a << " started";\

#define END_TEST(a)\
  LogWarng << #a << " passed";\
}

#define EXPECT_EQUAL(test, expect)\
  if (!(testing::equal_test(test, expect))) testing::throw_error(test, expect, #test, #expect, "equal", __FILE__, __LINE__);

#define EXPECT_NOT_EQUAL(test, expect)\
  if (testing::equal_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "not equal", __FILE__, __LINE__);

#define EXPECT_LOWER(test, expect)\
  if (!testing::lower_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "lower than", __FILE__, __LINE__);

#define EXPECT_LOWER_OR_EQUAL(test, expect)\
  if (!testing::lower_equal_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "lower or equal than", __FILE__, __LINE__);

#define EXPECT_HIGHER(test, expect)\
  if (!testing::higher_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "higher than", __FILE__, __LINE__);

#define EXPECT_HIGHER_OR_EQUAL(test, expect)\
  if (!testing::higher_equal_test(test, expect)) testing::throw_error(test, expect, #test, #expect, "higher or equal than", __FILE__, __LINE__);

#define EXPECT_TRUE(test)\
  if (!testing::equal_test<bool, bool>(test, true)) testing::throw_error<bool, bool>(test, true, #test, "true", "equal", __FILE__, __LINE__);

#define EXPECT_FALSE(test)\
  if (!testing::equal_test<bool, bool>(test, false)) testing::throw_error<bool, bool>(test, false, #test, "false", "equal", __FILE__, __LINE__);

namespace testing {

    template<typename T1, typename T2>
  void throw_error (const T1& testValue,
                    const T2& expectedValue,
                    const char* testName,
                    const char* expectedName,
                    const char* equality,
                    const char* fileName,
                    const int   lineNumber) {
    std::ostringstream os;
    os << fileName << ":" << lineNumber << ": "
       << "Expected " << testName << " "
       << "to be " << equality << " " << expectedValue << " (" << expectedName << ") "
       << "but it was " << testValue;
    throw std::runtime_error(os.str());
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

}
