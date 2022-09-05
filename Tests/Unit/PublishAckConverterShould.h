#pragma once
#include <gtest/gtest.h>
#include <Protocol/Converters/PublishAcknowledgeConverter.h>


MQTT::Protocol::PublishAcknowledgePackage PAckGenerate() {
	MQTT::Protocol::PublishAcknowledgePackage package;
	package.Header.PackageType = MQTT::Protocol::PublAck;
	package.PacketIdentifier = 123;

	return package;
}

std::vector<unsigned char> PAckBufferGenerate(unsigned char packetIdentifier) {
	
	auto expected = std::vector<unsigned char>{
		0x4U << 4, 0x02, packetIdentifier
	};

	return expected;
}

TEST(PublishAckConverterShould, BeEqual_PacketIdentifier)
{
	// Arrange
	MQTT::Protocol::PublishAcknowledgePackage package = PAckGenerate();

	auto converter = MQTT::Protocol::Converters::PublishAcknowledgeConverter();

	auto expected = PAckBufferGenerate(package.PacketIdentifier);

	// Act
	std::vector<unsigned char> actual = converter.ToBuffer(package);;

	// Assert
	EXPECT_EQ(expected[2], actual[2]);
}


TEST(PublishAckConverterShould, BeEqual_PackageSize)
{
	// Arrange
	MQTT::Protocol::PublishAcknowledgePackage package = PAckGenerate();

	auto converter = MQTT::Protocol::Converters::PublishAcknowledgeConverter();

	auto expected = PAckBufferGenerate(package.PacketIdentifier);

	// Act
	std::vector<unsigned char> actual = converter.ToBuffer(package);;

	// Assert
	EXPECT_EQ(expected[1], actual[1]);
}