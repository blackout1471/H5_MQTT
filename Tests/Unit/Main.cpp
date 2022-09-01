#include <gtest/gtest.h>
#include "ConnectionConverterShould.h"
#include "ConnectionAcknowledgementConverterShould.h"
#include "MqttServiceShould.h"
#include "ConnectionValidationShould.h"
#include "PublishConverterShould.h"



int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

