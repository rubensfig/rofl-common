#include "rofl/common/caddress.h"
#include "rofl/common/cmemory.h"
#include "rofl/common/openflow/cofmeterbandstats.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class cofmeterbandstats_test : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(cofmeterbandstats_test);
  CPPUNIT_TEST(testDefaultConstructor);
  CPPUNIT_TEST(testCopyConstructor);
  CPPUNIT_TEST(testPackUnpack);
  CPPUNIT_TEST_SUITE_END();

private:
public:
  void setUp();
  void tearDown();

  void testDefaultConstructor();
  void testCopyConstructor();
  void testPackUnpack();
};