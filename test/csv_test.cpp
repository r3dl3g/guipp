
#include <util/csv_reader.h>
#include <testlib/testlib.h>

using namespace util::csv;

// --------------------------------------------------------------------------
void test_parse_text () {
  std::istringstream buffer("test\"");
  int ch = '"';
  std::string s = parse_text(buffer, ch);

  EXPECT_EQUAL(s, std::string("test"));
}

// --------------------------------------------------------------------------
void test_parse_text2 () {
  std::istringstream buffer("test'");
  int ch = '\'';
  std::string s = parse_text(buffer, ch);

  EXPECT_EQUAL(s, std::string("test"));
}

// --------------------------------------------------------------------------
void test_parse_text3 () {
  std::istringstream buffer("te;, st'");
  int ch = '\'';
  const std::string s = parse_text(buffer, ch);

  EXPECT_EQUAL(s, std::string("te;, st"));
}

// --------------------------------------------------------------------------
void test_parse_none_text () {
  std::istringstream buffer("123.456;");
  int ch = '0';
  const std::string s = parse_none_text(buffer, ch, ';');

  EXPECT_EQUAL(s, std::string("0123.456"));
}

// --------------------------------------------------------------------------
void test_parse_text_entry () {
  std::istringstream buffer("test\"");
  int ch = '"';
  const std::string s = parse_entry(buffer, ch, ';');

  EXPECT_EQUAL(s, std::string("test"));
}

// --------------------------------------------------------------------------
void test_parse_none_text_entry () {
  std::istringstream buffer("123.456;");
  int ch = '0';
  const std::string s = parse_entry(buffer, ch, ';');

  EXPECT_EQUAL(s, std::string("0123.456"));
}

// --------------------------------------------------------------------------
namespace std {
  template<typename T>
  std::ostream& operator<< (std::ostream& out, const std::vector<T>& l) {
    out << '[';
    bool first = true;
    for (const auto& i : l) {
      out << (first ? "" : ", ") << i;
      first = false;
    }
    out << ']';
    return out;
  }
}

// --------------------------------------------------------------------------
void test_parse_csv_line () {
  using namespace util::csv;

  std::istringstream buffer("0123.456;'test'");
  reader::string_list list = reader().parse_csv_line(buffer);

  reader::string_list expected = {"0123.456", "test"};
  EXPECT_EQUAL(list, expected);
}

// --------------------------------------------------------------------------
void test_parse_csv_data () {
  using namespace util::csv;

  std::istringstream buffer("0123.456;'test'\n1234.567;'foo'");
  typedef std::vector<std::vector<std::string>> matrix;
  matrix m;
  reader().read_csv_data(buffer, [&] (const reader::string_list& l) {
    m.push_back(l);
  });

  matrix expected = {{"0123.456", "test"}, {"1234.567", "foo"}};
  EXPECT_EQUAL(m, expected);
}

// --------------------------------------------------------------------------
void test_parse_csv_data_ignore_header () {
  using namespace util::csv;

  std::istringstream buffer("0123.456;'test'\n1234.567;'foo'");
  typedef std::vector<std::vector<std::string>> matrix;
  matrix m;
  reader(';', true).read_csv_data(buffer, [&] (const reader::string_list& l) {
    m.push_back(l);
  });

  matrix expected = {{"1234.567", "foo"}};
  EXPECT_EQUAL(m, expected);
}

// --------------------------------------------------------------------------
void test_main (const testing::start_params&) {
  std::cout << "Running csv_test" << std::endl;
  run_test(test_parse_text);
  run_test(test_parse_text2);
  run_test(test_parse_text3);
  run_test(test_parse_none_text);
  run_test(test_parse_text_entry);
  run_test(test_parse_none_text_entry);
  run_test(test_parse_csv_line);
  run_test(test_parse_csv_data);
  run_test(test_parse_csv_data_ignore_header);
}

// --------------------------------------------------------------------------

