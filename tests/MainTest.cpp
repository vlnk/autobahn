#define BOOST_TEST_MODULE MainTest
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE( test1 ) {
  // reports 'error in "test1": test 2 == 1 failed'
  BOOST_CHECK( 1 == 1 );
}

// each test file may contain any number of test cases; each test case has to have unique name
BOOST_AUTO_TEST_CASE( test2 ) {
  int i = 0;

  // reports 'error in "test2": check i == 2 failed [0 != 2]'
  //BOOST_CHECK_EQUAL( i, 2 );
  BOOST_CHECK_EQUAL( i, 0 );
}
