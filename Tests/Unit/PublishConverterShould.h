#pragma once
#include <gtest/gtest.h>
#include <Protocol/Converters/PublishConverter.h>

using namespace MQTT::Protocol::Converters;
using namespace MQTT::Protocol;


#pragma region Utility
static PublishPackage Generate() {
	auto p = PublishPackage();

	p.Header.PackageType = ControlPackageType::Publish;
	p.HeaderFlag = PublishHeaderFlag::Retain;

	p.VariableHeader.TopicName = "a";

	p.Payload = "a";

	return p;
}

static bool operator==(const PublishPackage& p1, const PublishPackage& p2) {
	return (
		p1.Header.PackageType == p2.Header.PackageType &&

		p1.HeaderFlag == p2.HeaderFlag &&
		p1.VariableHeader.TopicName == p2.VariableHeader.TopicName &&
		p1.VariableHeader.PacketIdentifier == p2.VariableHeader.PacketIdentifier &&

		p1.Payload == p2.Payload
	);
}
#pragma endregion

TEST(PublishConverterShould, ReturnPackageWithNoIdentifier_WhenQoSIsZero)
{
	// Arrange
	auto expected = Generate();
	PublishPackage actual;
	auto converter = PublishConverter();
	auto data = std::vector<unsigned char>{
		(3 << 4) + 1,
		0, 1, 97,
		97
	};
	data.insert(data.begin() + 1, data.size() - 1);

	// Act
	actual = converter.ToPackage(data);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishConverterShould, ReturnPackageWithIdentifier_WhenQoSIsMsb)
{
	// Arrange
	auto expected = Generate();
	expected.HeaderFlag = PublishHeaderFlag::QoSMsb;
	expected.VariableHeader.PacketIdentifier = 400;
	PublishPackage actual;
	auto converter = PublishConverter();
	auto data = std::vector<unsigned char>{
		(3 << 4) + 2,
		0, 1, 97,
		1,  16 + 128,
		97
	};
	data.insert(data.begin() + 1, data.size() - 1);

	// Act
	actual = converter.ToPackage(data);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishConverterShould, ReturnPackageWithIdentifier_WhenQoSIsLsb)
{
	// Arrange
	auto expected = Generate();
	expected.HeaderFlag = PublishHeaderFlag::QoSLsb;
	expected.VariableHeader.PacketIdentifier = 400;
	PublishPackage actual;
	auto converter = PublishConverter();
	auto data = std::vector<unsigned char>{
		(3 << 4) + 4,
		0, 1, 97,
		1,  16 + 128,
		97
	};
	data.insert(data.begin() + 1, data.size() - 1);

	// Act
	actual = converter.ToPackage(data);

	// Assert
	EXPECT_TRUE(expected == actual);
}
