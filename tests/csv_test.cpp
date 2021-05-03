
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
  std::vector<std::string> list = parse_csv_line(buffer);

  std::vector<std::string> expected = {"0123.456", "test"};
  EXPECT_EQUAL(list, expected);
}

// --------------------------------------------------------------------------
void test_parse_csv_data () {
  using namespace util::csv;

  std::istringstream buffer("0123.456;'test'\n1234.567;'foo'");
  typedef std::vector<std::vector<std::string>> matrix;
  matrix m;
  read_csv_data(buffer, ';', false, [&] (const std::vector<std::string>& l) {
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
  read_csv_data(buffer, ';', true, [&] (const std::vector<std::string>& l) {
    m.push_back(l);
  });

  matrix expected = {{"1234.567", "foo"}};
  EXPECT_EQUAL(m, expected);
}

// --------------------------------------------------------------------------
void test_parse_csv_tuple () {
  using namespace util::csv;
  typedef tuple_reader<double, double> test_reader;

  std::istringstream buffer("Eins;Zwei\n1.1;2.2\n3.3;4.4\n5.5;6.6");
  int count = 0;
  test_reader::read_csv(buffer, ';', true, [&](const test_reader::tuple& t) {
    switch (count) {
      case 0:
        EXPECT_EQUAL(std::get<0>(t), 1.1);
        EXPECT_EQUAL(std::get<1>(t), 2.2);
      break;
      case 1:
        EXPECT_EQUAL(std::get<0>(t), 3.3);
        EXPECT_EQUAL(std::get<1>(t), 4.4);
      break;
      case 2:
        EXPECT_EQUAL(std::get<0>(t), 5.5);
        EXPECT_EQUAL(std::get<1>(t), 6.6);
      break;
    }
    ++count;
  });

}

// --------------------------------------------------------------------------
void test_parse_csv_tuple_empty_end () {
  using namespace util::csv;
  typedef tuple_reader<double, double> test_reader;

  std::istringstream buffer("Eins;Zwei\n1.1;\n3.3;\n5.5;");
  int count = 0;
  test_reader::read_csv(buffer, ';', true, [&](const test_reader::tuple& t) {
    switch (count) {
      case 0:
        EXPECT_EQUAL(std::get<0>(t), 1.1);
        EXPECT_EQUAL(std::get<1>(t), 0.0);
      break;
      case 1:
        EXPECT_EQUAL(std::get<0>(t), 3.3);
        EXPECT_EQUAL(std::get<1>(t), 0.0);
      break;
      case 2:
        EXPECT_EQUAL(std::get<0>(t), 5.5);
        EXPECT_EQUAL(std::get<1>(t), 0.0);
      break;
    }
    ++count;
  });

}
// --------------------------------------------------------------------------
void test_parse_csv_tuple_cut () {
  using namespace util::csv;
  typedef tuple_reader<double, double> test_reader;

  std::istringstream buffer("Eins;Zwei\n1.1;2.2\n3.3");
  int count = 0;
  test_reader::read_csv(buffer, ';', true, [&](const test_reader::tuple& t) {
    switch (count) {
      case 0:
        EXPECT_EQUAL(std::get<0>(t), 1.1);
        EXPECT_EQUAL(std::get<1>(t), 2.2);
      break;
      case 1:
        EXPECT_EQUAL(std::get<0>(t), 3.3);
        EXPECT_EQUAL(std::get<1>(t), 0.0);
      break;
    }
    ++count;
  });
  EXPECT_EQUAL(count, 2);

}
// --------------------------------------------------------------------------
void test_parse_csv_tuple_skip_1 () {
  using namespace util::csv;
  typedef tuple_reader<skip, double> test_reader;

  std::istringstream buffer("Eins;Zwei\n1.1;2.2\n3.3;4.4\n5.5;6.6");
  int count = 0;
  test_reader::read_csv(buffer, ';', true, [&](const test_reader::tuple& t) {
    switch (count) {
      case 0:
        EXPECT_EQUAL(std::get<0>(t), skip());
        EXPECT_EQUAL(std::get<1>(t), 2.2);
      break;
      case 1:
        EXPECT_EQUAL(std::get<0>(t), skip());
        EXPECT_EQUAL(std::get<1>(t), 4.4);
      break;
    }
    ++count;
  });

}

// --------------------------------------------------------------------------
void test_parse_csv_tuple_skip_2 () {
  using namespace util::csv;
  typedef tuple_reader<double, skip> test_reader;

  std::istringstream buffer("Eins;Zwei\n1.1;2.2\n3.3;4.4\n5.5;6.6");
  int count = 0;
  test_reader::read_csv(buffer, ';', true, [&](const test_reader::tuple& t) {
    switch (count) {
      case 0:
        EXPECT_EQUAL(std::get<0>(t), 1.1);
        EXPECT_EQUAL(std::get<1>(t), skip());
      break;
      case 1:
        EXPECT_EQUAL(std::get<0>(t), 3.3);
        EXPECT_EQUAL(std::get<1>(t), skip());
      break;
    }
    ++count;
  });

}

// --------------------------------------------------------------------------
void test_main (const testing::start_params& params) {
  testing::init_gui(params);
  testing::log_info("Running csv_test");
  run_test(test_parse_text);
  run_test(test_parse_text2);
  run_test(test_parse_text3);
  run_test(test_parse_none_text);
  run_test(test_parse_text_entry);
  run_test(test_parse_none_text_entry);
  run_test(test_parse_csv_line);
  run_test(test_parse_csv_data);
  run_test(test_parse_csv_data_ignore_header);
  run_test(test_parse_csv_tuple);
  run_test(test_parse_csv_tuple_empty_end);
  run_test(test_parse_csv_tuple_cut);
  run_test(test_parse_csv_tuple_skip_1);
  run_test(test_parse_csv_tuple_skip_2);
}

// --------------------------------------------------------------------------

