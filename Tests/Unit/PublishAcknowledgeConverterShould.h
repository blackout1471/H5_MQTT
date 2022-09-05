#pragma once
#include <gtest/gtest.h>
#include <Protocol/Converters/PublishAcknowledgeConverter.h>


MQTT::Protocol::PublishAcknowledgePackage PAckGenerate() {
	MQTT::Protocol::PublishAcknowledgePackage package;
	package.Header.PackageType = MQTT::Protocol::PublAck;
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
	MQTT::Protocol::PublishAcknowledgePackage package = PAckGenerate();

	auto converter = MQTT::Protocol::Converters::PublishAcknowledgeConverter();

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
	MQTT::Protocol::PublishAcknowledgePackage package = PAckGenerate();

	auto converter = MQTT::Protocol::Converters::PublishAcknowledgeConverter();

	auto expected = PAckBufferGenerate();

	// Act
	std::vector<unsigned char> actual = converter.ToBuffer(package);;

	// Assert
	EXPECT_EQ(expected[1], actual[1]);
}