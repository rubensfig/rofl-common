#include "rofl/common/openflow/cofhelloelems.h"
#include "rofl/common/cmemory.h"
#include "rofl/common/caddress.h"
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

using namespace rofl::openflow;

class cofhelloelemsTest : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE( cofhelloelemsTest );
	CPPUNIT_TEST( testPack );
	CPPUNIT_TEST( testUnpack );
	CPPUNIT_TEST_SUITE_END();

private:

	cofhelloelems	*elems;

public:
	void setUp();
	void tearDown();

	void testPack();
	void testUnpack();
};

