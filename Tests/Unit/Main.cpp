#include <gtest/gtest.h>
#include "BaseMathTest.h"
#include "ConnectionConverterShould.h"
#include "SubscribeAcknowledgementConverterShould.h"



int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

