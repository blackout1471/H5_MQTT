#pragma once
#include <gtest/gtest.h>
#include <Converters/ConnectAckConverter.h>
#include <MqttPackages/packages.h>

#pragma region Utility
static bool Equal(const std::vector<unsigned char>& b1, const std::vector<unsigned char>& b2)
{
	if (b1.size() != b2.size())
		return false;

	for (int i = 0; i < b1.size(); i++)
	{
		if (b1[i] != b2[i])
			return false;
	}

	return true;
}
#pragma endregion


TEST(ConnectionAcknowledgementConverterShould, BeEqual_WhenPackageIsAcknowledged)
{
	// Arrange
	auto package = ConnectAckPackage({ ControlPackageType::ConnectAck }, {0x01, ConnectAckReturnCodeType::Accepted});
	auto converter = ConnectAckConverter();
	std::vector<unsigned char> actual;
	auto expected = std::vector<unsigned char> {
		0x02 << 4, 0x02, 0x01, 0x0
	};

	// Act
	actual = converter.ToBuffer(package);

	// Assert
	EXPECT_TRUE(Equal(expected, actual));
}

TEST(ConnectionAcknowledgementConverterShould, BeEqual_WhenPackageIsRefusedBadUsernameOrPassword)
{
	// Arrange
	auto package = ConnectAckPackage({ ControlPackageType::ConnectAck }, { 0x01, ConnectAckReturnCodeType::Refused_Bad_Username_Or_Password });
	auto converter = ConnectAckConverter();
	std::vector<unsigned char> actual;
	auto expected = std::vector<unsigned char>{
		0x02 << 4, 0x02, 0x01, 0x4
	};

	// Act
	actual = converter.ToBuffer(package);

	// Assert
	EXPECT_TRUE(Equal(expected, actual));
}

TEST(ConnectionAcknowledgementConverterShould, BeEqual_WhenPackageIsRefusedIdentifierRejected)
{
	// Arrange
	auto package = ConnectAckPackage({ ControlPackageType::ConnectAck }, { 0x01, ConnectAckReturnCodeType::Refused_Identifier_Rejected });
	auto converter = ConnectAckConverter();
	std::vector<unsigned char> actual;
	auto expected = std::vector<unsigned char>{
		0x02 << 4, 0x02, 0x01, 0x2
	};

	// Act
	actual = converter.ToBuffer(package);

	// Assert
	EXPECT_TRUE(Equal(expected, actual));
}

TEST(ConnectionAcknowledgementConverterShould, BeEqual_WhenPackageIsRefusedNotAuthorized)
{
	// Arrange
	auto package = ConnectAckPackage({ ControlPackageType::ConnectAck }, { 0x01, ConnectAckReturnCodeType::Refused_Not_Authorized });
	auto converter = ConnectAckConverter();
	std::vector<unsigned char> actual;
	auto expected = std::vector<unsigned char>{
		0x02 << 4, 0x02, 0x01, 0x5
	};

	// Act
	actual = converter.ToBuffer(package);

	// Assert
	EXPECT_TRUE(Equal(expected, actual));
}

TEST(ConnectionAcknowledgementConverterShould, BeEqual_WhenPackageIsRefusedServerUnavailable)
{
	// Arrange
	auto package = ConnectAckPackage({ ControlPackageType::ConnectAck }, { 0x01, ConnectAckReturnCodeType::Refused_Server_Unavailable });
	auto converter = ConnectAckConverter();
	std::vector<unsigned char> actual;
	auto expected = std::vector<unsigned char>{
		0x02 << 4, 0x02, 0x01, 0x3
	};

	// Act
	actual = converter.ToBuffer(package);

	// Assert
	EXPECT_TRUE(Equal(expected, actual));
}

TEST(ConnectionAcknowledgementConverterShould, BeEqual_WhenPackageIsRefusedUnacceptableProtocolVersion)
{
	// Arrange
	auto package = ConnectAckPackage({ ControlPackageType::ConnectAck }, { 0x01, ConnectAckReturnCodeType::Refused_Unacceptable_Protocol_Version });
	auto converter = ConnectAckConverter();
	std::vector<unsigned char> actual;
	auto expected = std::vector<unsigned char>{
		0x02 << 4, 0x02, 0x01, 0x1
	};

	// Act
	actual = converter.ToBuffer(package);

	// Assert
	EXPECT_TRUE(Equal(expected, actual));
}