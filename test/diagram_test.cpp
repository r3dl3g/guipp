
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

// --------------------------------------------------------------------------
void test_main (const testing::start_params& params) {
  std::cout << "Running diagram_test" << std::endl;
  run_test(test_limits);
}

// --------------------------------------------------------------------------

