#include "BaseMathTest.h"


int BaseMathTest::times(int amount, int by) {
	return amount * by;
}


TEST(BaseMathTest, TimesTest)
{
	BaseMathTest mathTest;

	EXPECT_EQ(100, mathTest.times(10, 10));
}