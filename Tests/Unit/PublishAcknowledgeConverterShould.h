#pragma once
#include <gtest/gtest.h>
#include <Converters/PublishAcknowledgeConverter.h>


using namespace MQTT::MqttPackages;
using namespace MQTT::Converters;

PublishAcknowledgePackage PAckGenerate() {
	PublishAcknowledgePackage package;
	package.Header.PackageType = PublAck;
	package.PacketIdentifier = 123;

	return package;
}

std::vector<unsigned char> PAckBufferGenerate() {
	
	auto expected = std::vector<unsigned char>{
		0x4U << 4, 0x02, 0x0, 0x7B
	};

	return expected;
}

TEST(PublishAckConverterShould, BeEqual_PacketIdentifier)
{
	// Arrange
	PublishAcknowledgePackage package = PAckGenerate();

	auto converter = PublishAcknowledgeConverter();

	auto expected = PAckBufferGenerate();

	// Act
	std::vector<unsigned char> actual = converter.ToBuffer(package);;

	// Assert

	EXPECT_EQ(expected[2], actual[2]);
	EXPECT_EQ(expected[3], actual[3]);
}


TEST(PublishAckConverterShould, BeEqual_PackageSize)
{
	// Arrange
	PublishAcknowledgePackage package = PAckGenerate();

	auto converter = PublishAcknowledgeConverter();

	auto expected = PAckBufferGenerate();

	// Act
	std::vector<unsigned char> actual = converter.ToBuffer(package);;

	// Assert
	EXPECT_EQ(expected[1], actual[1]);
}