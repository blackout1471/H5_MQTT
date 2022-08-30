#pragma once

#include "gtest/gtest.h"
#include <Protocol/MqttPackages/ConnectPackage.h>
#include <Protocol/Converters/ConnectConverter.h>

using namespace MQTT::Protocol;

#pragma region Utility
static ConnectPackage Generate(ConnectFlagType flags)
{
	auto package = ConnectPackage();
	package.Header.PackageType = ControlPackageType::Connect;
	package.VariableHeader = { "mqtt", (unsigned char)4, flags, int16_t(60) };

	package.Payload.ClientId = "c";

	if (flags & ConnectFlagType::Username)
		package.Payload.Username = "u";

	if (flags & ConnectFlagType::Password)
		package.Payload.Password = "p";

	if (flags & ConnectFlagType::Will_Flag)
	{
		package.Payload.WillTopic = "wt";
		package.Payload.WillMessage = "wm";
	}

	return package;
}

std::vector<unsigned char> ToBuffer(const ConnectPackage& package)
{
	auto v = std::vector<unsigned char>();
	int remainLength = 0;

	// Control package
	v.push_back(package.Header.PackageType << 4);

	// protocol name
	v.push_back(0); v.push_back(package.VariableHeader.ProtocolName.size());
	for (int i = 0; i < package.VariableHeader.ProtocolName.size(); i++)
		v.push_back(package.VariableHeader.ProtocolName[i]);
	remainLength += 2;
	remainLength += package.VariableHeader.ProtocolName.size();

	// Level
	v.push_back(package.VariableHeader.Level);
	remainLength += 1;

	// flags
	v.push_back(package.VariableHeader.VariableLevel);
	remainLength += 1;

	// keep alive
	v.push_back(0); v.push_back(package.VariableHeader.KeepAlive);
	remainLength += 2;

	// client identifier
	v.push_back(0); v.push_back(package.Payload.ClientId.size());
	for (int i = 0; i < package.Payload.ClientId.size(); i++)
		v.push_back(package.Payload.ClientId[i]);
	remainLength += 2;
	remainLength += package.Payload.ClientId.size();

	// will topic
	if (package.Payload.WillTopic.size() > 0)
	{
		v.push_back(0); v.push_back(package.Payload.WillTopic.size());
		for (int i = 0; i < package.Payload.WillTopic.size(); i++)
			v.push_back(package.Payload.WillTopic[i]);
		remainLength += 2;
		remainLength += package.Payload.WillTopic.size();
	}

	// will message
	if (package.Payload.WillMessage.size() > 0)
	{
		v.push_back(0); v.push_back(package.Payload.WillMessage.size());
		for (int i = 0; i < package.Payload.WillMessage.size(); i++)
			v.push_back(package.Payload.WillMessage[i]);
		remainLength += 2;
		remainLength += package.Payload.WillMessage.size();
	}

	// will username
	if (package.Payload.Username.size() > 0)
	{
		v.push_back(0); v.push_back(package.Payload.Username.size());
		for (int i = 0; i < package.Payload.Username.size(); i++)
			v.push_back(package.Payload.Username[i]);
		remainLength += 2;
		remainLength += package.Payload.Username.size();
	}

	// will password
	if (package.Payload.Password.size() > 0)
	{
		v.push_back(0); v.push_back(package.Payload.Password.size());
		for (int i = 0; i < package.Payload.Password.size(); i++)
			v.push_back(package.Payload.Password[i]);
		remainLength += 2;
		remainLength += package.Payload.Password.size();
	}

	v.insert(v.begin() + 1, remainLength);

	return v;
}

static bool operator==(const ConnectPackage& p1, const ConnectPackage& p2)
{
	return (
		p1.Header.PackageType == p2.Header.PackageType &&

		p1.VariableHeader.ProtocolName == p2.VariableHeader.ProtocolName &&
		p1.VariableHeader.Level == p2.VariableHeader.Level &&
		p1.VariableHeader.VariableLevel == p2.VariableHeader.VariableLevel &&
		p1.VariableHeader.KeepAlive == p2.VariableHeader.KeepAlive &&

		p1.Payload.ClientId == p2.Payload.ClientId &&
		p1.Payload.WillTopic == p2.Payload.WillTopic &&
		p1.Payload.WillMessage == p2.Payload.WillMessage &&
		p1.Payload.Username == p2.Payload.Username &&
		p1.Payload.Password == p2.Payload.Password
	);
}
#pragma endregion


TEST(ConnectionPackageConverterShould, ReturnTrue_WhenUsernameAndPasswordIsDefined)
{
	// Arrange
	auto expected = Generate(ConnectFlagType(ConnectFlagType::Username | ConnectFlagType::Password));
	ConnectPackage actual;
	auto converter = Converters::ConnectConverter();

	// Act
	auto byffer = ToBuffer(expected);
	actual = converter.ToPackage(ToBuffer(expected));

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionPackageConverterShould, ReturnTrue_WhenClientIdIsNotGiven)
{
	// Arrange
	auto expected = Generate(ConnectFlagType(ConnectFlagType::Username | ConnectFlagType::Password));
	expected.Payload.ClientId = "";
	ConnectPackage actual;
	auto converter = Converters::ConnectConverter();

	// Act
	auto byffer = ToBuffer(expected);
	actual = converter.ToPackage(ToBuffer(expected));

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionPackageConverterShould, ReturnTrue_WhenWillFlagIsSet)
{
	// Arrange
	auto expected = Generate(ConnectFlagType(ConnectFlagType::Username | ConnectFlagType::Password | ConnectFlagType::Will_Flag));
	ConnectPackage actual;
	auto converter = Converters::ConnectConverter();

	// Act
	auto byffer = ToBuffer(expected);
	actual = converter.ToPackage(ToBuffer(expected));

	// Assert
	EXPECT_TRUE(expected == actual);
}