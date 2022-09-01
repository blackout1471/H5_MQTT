#pragma once
#include <gtest/gtest.h>
#include <Protocol/Converters/SubscribeAcknowledgementConverter.h>
using namespace MQTT::Protocol;

static SubscribeAcknowledgementPackage Generate() {

	SubscribeAcknowledgementPackage package;

	package.Header.PackageType = ControlPackageType::SubAck;
	package.VariableHeader.PacketIdentifier = 400;
	package.Payload.payload.push_back(SubscribeAcknowledgementQoS::MaximumQoS0);
	package.Payload.payload.push_back(SubscribeAcknowledgementQoS::MaximumQoS1);
	package.Payload.payload.push_back(SubscribeAcknowledgementQoS::MaximumQoS2);
	package.Payload.payload.push_back(SubscribeAcknowledgementQoS::Failure);
	return package;
}

static bool operator==(const std::vector<unsigned char>& buffer1, const std::vector<unsigned char>& buffer2)
{
	if (buffer1.size() != buffer2.size())
		return false;

	for (int i = 0; i < buffer1.size(); i++)
		if (buffer1[i] != buffer2[i])
			return false;

	return true;
}

TEST(SubscribeAcknowledgementPackageShould, ReturnExpected_WhenEveryFieldIsSet)
{
	// Arrange
	auto expected = std::vector<unsigned char>{
		0x9 << 4,
		6,
		1,
		144,
		0,
		1,
		2,
		128
	};
	std::vector<unsigned char> actual;
	auto converter = Converters::SubscribeAcknowledgementConverter();
	auto data = Generate();

	// Act
	actual = converter.ToBuffer(data);

	// Assert
	EXPECT_TRUE(expected == actual);
}
