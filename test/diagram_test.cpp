
#include <gui/draw/diagram.h>
#include <gui/draw/graphics.h>
#include <testlib/testlib.h>

using namespace gui;
using namespace gui::draw;
using namespace gui::draw::diagram;

// --------------------------------------------------------------------------
void test_limits () {
  typedef limits<double, scaling::linear> lim;
  typedef core::range<double> ran;
  EXPECT_EQUAL((lim::calc(0, 1))     , ran(0, 1)      );
  EXPECT_EQUAL((lim::calc(0, 10))    , ran(0, 10)     );
  EXPECT_EQUAL((lim::calc(0, 100))   , ran(0, 100)    );
  EXPECT_EQUAL((lim::calc(0, 0.1))   , ran(0, 0.1)    );
  EXPECT_EQUAL((lim::calc(0, 0.01))  , ran(0, 0.01)   );

  EXPECT_EQUAL((lim::calc(0, 1.1))   , ran(0, 2)      );
  EXPECT_EQUAL((lim::calc(0, 4.5))   , ran(0, 5)      );
  EXPECT_EQUAL((lim::calc(0, 0.45))  , ran(0, 0.5)    );
  EXPECT_EQUAL((lim::calc(0, 45))    , ran(0, 50)     );
  EXPECT_EQUAL((lim::calc(0, 8.99))  , ran(0, 9)      );
  EXPECT_EQUAL((lim::calc(0, 9.99))  , ran(0, 10)     );
  EXPECT_EQUAL((lim::calc(0, 10.99)) , ran(0, 20)     );

  EXPECT_EQUAL((lim::calc(0.1, 0.2)) , ran(0.1, 0.2)  );
  EXPECT_EQUAL((lim::calc(0.1, 1))   , ran(0.1, 1)    );
  EXPECT_EQUAL((lim::calc(0.1, 10))  , ran(0, 10)     );
  EXPECT_EQUAL((lim::calc(0.1, 100)) , ran(0, 100)    );
  EXPECT_EQUAL((lim::calc(0.1, 1000)), ran(0, 1000)   );

  EXPECT_EQUAL((lim::calc(1.1, 2.1)) , ran(1, 3)      );
  EXPECT_EQUAL((lim::calc(1.1, 10))  , ran(1, 10)     );
  EXPECT_EQUAL((lim::calc(1.1, 100)) , ran(0, 100)    );
  EXPECT_EQUAL((lim::calc(1.1, 1000)), ran(0, 1000)   );

  EXPECT_EQUAL((lim::calc(11, 100))  , ran(10, 100)   );
  EXPECT_EQUAL((lim::calc(11, 1000)) , ran(0, 1000)   );

  EXPECT_EQUAL((lim::calc(111, 1000)), ran(100, 1000) );

  EXPECT_EQUAL((lim::calc(-1.1, 2.1)) , ran(-2, 3)    );
  EXPECT_EQUAL((lim::calc(-1.1, 10))  , ran(-2, 10)   );
  EXPECT_EQUAL((lim::calc(-1.1, 100)) , ran(-10, 100) );
  EXPECT_EQUAL((lim::calc(-1.1, 1000)), ran(-100, 1000));

  EXPECT_EQUAL((lim::calc(-2.1, 1.1)) , ran(-3, 2)      );
  EXPECT_EQUAL((lim::calc(-10,  1.1)) , ran(-10, 2)     );
  EXPECT_EQUAL((lim::calc(-100, 1.1)) , ran(-100, 10)   );
  EXPECT_EQUAL((lim::calc(-1000, 1.1)), ran(-1000, 100) );

  EXPECT_EQUAL((lim::calc(-1.1, 0))   , ran(-2, 0)    );
  EXPECT_EQUAL((lim::calc(-4.5, 0))   , ran(-5, 0)    );
  EXPECT_EQUAL((lim::calc(-0.45, 0))  , ran(-0.5, 0)  );
  EXPECT_EQUAL((lim::calc(-45, 0))    , ran(-50, 0)   );
  EXPECT_EQUAL((lim::calc(-8.99, 0))  , ran(-9, 0)    );
  EXPECT_EQUAL((lim::calc(-9.99, 0))  , ran(-10, 0)   );
  EXPECT_EQUAL((lim::calc(-10.99, 0)) , ran(-20, 0)   );

}

typedef scaler<double, scaling::linear> linear_scale;
typedef scaler<double, scaling::log> log_scale;
typedef scaler<double, scaling::symlog> symlog_scale;

void test_linear_scaler_0 () {
  linear_scale sc({0, 100}, {0, 100});
  for (double i = 0; i < 100; i += M_PI / 10.0) {
    EXPECT_EQUAL(sc(i), i);
  }
}

void test_linear_scaler_0a () {
  linear_scale sc({0, 100}, {0, 100});
  for (double i = -110; i < 110; i += M_PI / 10.0) {
    const double expected = std::max(std::min(i, 100.0), 0.0);
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_1 () {
  linear_scale sc({-100, 100}, {-100, 100});
  for (double i = -100; i < 100; i += M_PI / 10.0) {
    EXPECT_EQUAL(sc(i), i);
  }
}

void test_linear_scaler_1a () {
  linear_scale sc({-100, 100}, {-100, 100});
  for (double i = -110; i < 110; i += M_PI / 10.0) {
    const double expected = std::max(std::min(i, 100.0), -100.0);
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_2 () {
  linear_scale sc({-100, 100}, {100, 200});
  for (double i = -100; i < 100; i += M_PI / 10.0) {
    const double expected = i / 2.0 + 150.0;
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_2a () {
  linear_scale sc({-100, 100}, {100, 200});
  for (double i = -110; i < 110; i += M_PI / 10.0) {
    const double expected = std::max(std::min(i / 2.0 + 150.0, 200.0), 100.0);
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_3 () {
  linear_scale sc({-100, 100}, {-100, -200});
  for (double i = -100; i < 100; i += M_PI / 10.0) {
    const double expected = (i + 100) / -2.0 - 100.0;
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_3a () {
  linear_scale sc({-100, 100}, {-100, -200});
  for (double i = -110; i < 110; i += M_PI / 10.0) {
    const double expected = std::max(std::min((i + 100) / -2.0 - 100.0, -100.0), -200.0);
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_4 () {
  linear_scale sc({0.1, 0.2}, {-100, -200});
  for (double i = 0.1; i < 0.2; i += M_PI / 300.0) {
    const double expected = i * -1000.0;
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_4a () {
  linear_scale sc({0.1, 0.2}, {-100, -200});
  for (double i = 0; i < 0.3; i += M_PI / 300.0) {
    const double expected = std::max(std::min(i * -1000.0, -100.0), -200.0);
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_5 () {
  linear_scale sc({100, 200}, {0.1, 0.2});
  for (double i = 100; i < 200; i += M_PI / 10.0) {
    const double expected = i / 1000.0;
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_5a () {
  linear_scale sc({100, 200}, {0.1, 0.2});
  EXPECT_EQUAL(sc(100), 0.1);
  EXPECT_EQUAL(sc(150), 0.15);
  EXPECT_EQUAL(sc(200), 0.2);
  for (double i = 90; i < 210; i += M_PI / 10.0) {
    const double expected = std::min(std::max(i / 1000.0, 0.1), 0.2);
    EXPECT_EQUAL(sc(i), expected);
  }
}

void test_linear_scaler_6 () {
  linear_scale sc({-100, 100}, {0.2, 0.1});
  EXPECT_EQUAL(sc(-100), 0.2);
  EXPECT_EQUAL(sc(0), 0.15);
  EXPECT_EQUAL(sc(100), 0.1);
}

void test_log_scaler_0 () {
  log_scale sc({0.01, 1000}, {0, 100});
  EXPECT_EQUAL(sc(0.01), 0);
  EXPECT_EQUAL(sc(0.1), 20);
  EXPECT_EQUAL(sc(1), 40);
  EXPECT_EQUAL(sc(10), 60);
  EXPECT_EQUAL(sc(100), 80);
  EXPECT_EQUAL(sc(1000), 100);
}

void test_log_scaler_0a () {
  log_scale sc({0.01, 1000}, {0, 100});
  EXPECT_EQUAL(sc(0.005), 0);
  EXPECT_EQUAL(sc(1500), 100);
  EXPECT_EQUAL(sc(10000), 100);
}

void test_log_scaler_1 () {
  log_scale sc({0.01, 1000}, {100, -100});
  EXPECT_EQUAL(sc(0.01), 100);
  EXPECT_EQUAL(sc(0.1), 60);
  EXPECT_EQUAL(sc(1), 20);
  EXPECT_EQUAL(sc(10), -20);
  EXPECT_EQUAL(sc(100), -60);
  EXPECT_EQUAL(sc(1000), -100);
}

void test_symlog_scale_fn_1 () {
  typedef detail::scale_fn<double, scaling::symlog> fn;
  EXPECT_EQUAL(fn::range({0.1, 100}), 8);
  EXPECT_EQUAL(fn::precalc({0.1, 100}), -4);

  EXPECT_EQUAL(fn::calc(100, 0.1), 4);
  EXPECT_EQUAL(fn::calc(10, 0.1), 3);
  EXPECT_EQUAL(fn::calc(1, 0.1), 2);
  EXPECT_EQUAL(fn::calc(0.1, 0.1), 1);
  EXPECT_EQUAL(fn::calc(0.09, 0.1), 0.9);
  EXPECT_EQUAL(fn::calc(0.01, 0.1), 0.1);
  EXPECT_EQUAL(fn::calc(0.005, 0.1), 0.05);
  EXPECT_EQUAL(fn::calc(0.0, 0.1), 0);
  EXPECT_EQUAL(fn::calc(-0.005, 0.1), -0.05);
  EXPECT_EQUAL(fn::calc(-0.01, 0.1), -0.1);
  EXPECT_EQUAL(fn::calc(-0.09, 0.1), -0.9);
  EXPECT_EQUAL(fn::calc(-0.1, 0.1), -1);
  EXPECT_EQUAL(fn::calc(-1, 0.1), -2);
  EXPECT_EQUAL(fn::calc(-10, 0.1), -3);
  EXPECT_EQUAL(fn::calc(-100, 0.1), -4);
}

void test_symlog_scale_fn_2 () {
  typedef detail::scale_fn<double, scaling::symlog> fn;
  EXPECT_EQUAL(fn::range({0.01, 10}), 8);
  EXPECT_EQUAL(fn::precalc({0.01, 10}), -4);

  EXPECT_EQUAL(fn::calc(10, 0.01), 4);
  EXPECT_EQUAL(fn::calc(1, 0.01), 3);
  EXPECT_EQUAL(fn::calc(0.1, 0.01), 2);
  EXPECT_EQUAL(fn::calc(0.01, 0.01), 1);
  EXPECT_EQUAL(fn::calc(0.009, 0.01), 0.9);
  EXPECT_EQUAL(fn::calc(0.001, 0.01), 0.1);
  EXPECT_EQUAL(fn::calc(0.0005, 0.01), 0.05);
  EXPECT_EQUAL(fn::calc(0.0, 0.01), 0);
  EXPECT_EQUAL(fn::calc(-0.0005, 0.01), -0.05);
  EXPECT_EQUAL(fn::calc(-0.001, 0.01), -0.1);
  EXPECT_EQUAL(fn::calc(-0.009, 0.01), -0.9);
  EXPECT_EQUAL(fn::calc(-0.01, 0.01), -1);
  EXPECT_EQUAL(fn::calc(-0.1, 0.01), -2);
  EXPECT_EQUAL(fn::calc(-1, 0.01), -3);
  EXPECT_EQUAL(fn::calc(-10, 0.01), -4);
}

void test_symlog_scale_fn_3 () {
  typedef detail::scale_fn<double, scaling::symlog> fn;
  EXPECT_EQUAL(fn::range({10, 1000}), 6);
  EXPECT_EQUAL(fn::precalc({10, 1000}), -3);

  EXPECT_EQUAL(fn::calc(1000, 10), 3);
  EXPECT_EQUAL(fn::calc(100, 10), 2);
  EXPECT_EQUAL(fn::calc(10, 10), 1);
  EXPECT_EQUAL(fn::calc(9, 10), 0.9);
  EXPECT_EQUAL(fn::calc(1, 10), 0.1);
  EXPECT_EQUAL(fn::calc(0.5, 10), 0.05);
  EXPECT_EQUAL(fn::calc(0.0, 10), 0);
  EXPECT_EQUAL(fn::calc(-0.5, 10), -0.05);
  EXPECT_EQUAL(fn::calc(-1, 10), -0.1);
  EXPECT_EQUAL(fn::calc(-9, 10), -0.9);
  EXPECT_EQUAL(fn::calc(-10, 10), -1);
  EXPECT_EQUAL(fn::calc(-100, 10), -2);
  EXPECT_EQUAL(fn::calc(-1000, 10), -3);
}

void test_symlog_scaler_1 () {

  symlog_scale sc({0.01, 100}, {-100, 100});
  EXPECT_EQUAL(sc(0), 0);
  EXPECT_EQUAL(sc(100), 100);
  EXPECT_EQUAL(sc(-100), -100);
  EXPECT_EQUAL(sc(10), 80);
  EXPECT_EQUAL(sc(-10), -80);
  EXPECT_EQUAL(sc(1), 60);
  EXPECT_EQUAL(sc(-1), -60);
  EXPECT_EQUAL(sc(0.1), 40);
  EXPECT_EQUAL(sc(-0.1), -40);
  EXPECT_EQUAL(sc(0.01), 20);
  EXPECT_EQUAL(sc(-0.01), -20);
  EXPECT_EQUAL(sc(0.005), 10);
  EXPECT_EQUAL(sc(-0.005), -10);
}

void test_symlog_scaler_2 () {

  symlog_scale sc({0.001, 10}, {-100, 100});
  EXPECT_EQUAL(sc(0), 0);
  EXPECT_EQUAL(sc(10), 100);
  EXPECT_EQUAL(sc(-10), -100);
  EXPECT_EQUAL(sc(1), 80);
  EXPECT_EQUAL(sc(-1), -80);
  EXPECT_EQUAL(sc(0.1), 60);
  EXPECT_EQUAL(sc(-0.1), -60);
  EXPECT_EQUAL(sc(0.01), 40);
  EXPECT_EQUAL(sc(-0.01), -40);
  EXPECT_EQUAL(sc(0.001), 20);
  EXPECT_EQUAL(sc(-0.001), -20);
  EXPECT_EQUAL(sc(0.0005), 10);
  EXPECT_EQUAL(sc(-0.0005), -10);
}

void test_symlog_scaler_3 () {

  symlog_scale sc({10, 100000}, {-100, 100});
  EXPECT_EQUAL(sc(0), 0);
  EXPECT_EQUAL(sc(100000), 100);
  EXPECT_EQUAL(sc(-100000), -100);
  EXPECT_EQUAL(sc(10000), 80);
  EXPECT_EQUAL(sc(-10000), -80);
  EXPECT_EQUAL(sc(1000), 60);
  EXPECT_EQUAL(sc(-1000), -60);
  EXPECT_EQUAL(sc(100), 40);
  EXPECT_EQUAL(sc(-100), -40);
  EXPECT_EQUAL(sc(10), 20);
  EXPECT_EQUAL(sc(-10), -20);
  EXPECT_EQUAL(sc(9), 18);
  EXPECT_EQUAL(sc(-9), -18);
  EXPECT_EQUAL(sc(5), 10);
  EXPECT_EQUAL(sc(-5), -10);
  EXPECT_EQUAL(sc(1), 2);
  EXPECT_EQUAL(sc(-1), -2);
}

// --------------------------------------------------------------------------
void test_main (const testing::start_params& params) {
  std::cout << "Running diagram_test" << std::endl;
  run_test(test_limits);
  run_test(test_linear_scaler_0);
  run_test(test_linear_scaler_0a);
  run_test(test_linear_scaler_1);
  run_test(test_linear_scaler_1a);
  run_test(test_linear_scaler_2);
  run_test(test_linear_scaler_2a);
  run_test(test_linear_scaler_3);
  run_test(test_linear_scaler_3a);
  run_test(test_linear_scaler_4);
  run_test(test_linear_scaler_4a);
  run_test(test_linear_scaler_5);
  run_test(test_linear_scaler_5a);
  run_test(test_linear_scaler_6);
  run_test(test_log_scaler_0);
  run_test(test_log_scaler_0a);
  run_test(test_log_scaler_1);
  run_test(test_symlog_scale_fn_1);
  run_test(test_symlog_scale_fn_2);
  run_test(test_symlog_scale_fn_3);
  run_test(test_symlog_scaler_1);
  run_test(test_symlog_scaler_2);
  run_test(test_symlog_scaler_3);
}

// --------------------------------------------------------------------------

