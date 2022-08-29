#pragma once

#include "gtest/gtest.h"
#include <Protocol/MqttPackages/ConnectPackage.h>
#include <Protocol/Converters/ConnectConverter.h>

using namespace MQTT::Protocol;

#pragma region Utility
ConnectPackage Generate()
{
	auto package = ConnectPackage();
	package.ControlHeader.PackageType = ControlPackageType::Connect;
	package.ConnectVariableHeader = { "mqtt", (unsigned char)4, ConnectFlagType(ConnectFlagType::Username | ConnectFlagType::Password), int16_t(60) };
	package.ConnectPayload = { "c", "wt", "wm", "u", "p" };

	return package;
}

std::vector<unsigned char> ToBuffer(const ConnectPackage& package)
{
	auto v = std::vector<unsigned char>();
	int remainLength = 0;

	// Control package
	v.push_back(package.ControlHeader.PackageType);

	// protocol name
	v.push_back(0); v.push_back(package.ConnectVariableHeader.ProtocolName.size());
	for (int i = 0; i < package.ConnectVariableHeader.ProtocolName.size(); i++)
		v.push_back(package.ConnectVariableHeader.ProtocolName[i]);
	remainLength += 2;
	remainLength += package.ConnectVariableHeader.ProtocolName.size();

	// Level
	v.push_back(package.ConnectVariableHeader.Level);
	remainLength += 1;

	// flags
	v.push_back(package.ConnectVariableHeader.VariableLevel);
	remainLength += 1;

	// keep alive
	v.push_back(0); v.push_back(package.ConnectVariableHeader.KeepAlive);
	remainLength += 2;

	// client identifier
	v.push_back(0); v.push_back(package.ConnectPayload.ClientId.size());
	for (int i = 0; i < package.ConnectPayload.ClientId.size(); i++)
		v.push_back(package.ConnectPayload.ClientId[i]);
	remainLength += 2;
	remainLength += package.ConnectPayload.ClientId.size();

	// will topic
	v.push_back(0); v.push_back(package.ConnectPayload.WillTopic.size());
	for (int i = 0; i < package.ConnectPayload.WillTopic.size(); i++)
		v.push_back(package.ConnectPayload.WillTopic[i]);
	remainLength += 2;
	remainLength += package.ConnectPayload.WillTopic.size();

	// will message
	v.push_back(0); v.push_back(package.ConnectPayload.WillMessage.size());
	for (int i = 0; i < package.ConnectPayload.WillMessage.size(); i++)
		v.push_back(package.ConnectPayload.WillMessage[i]);
	remainLength += 2;
	remainLength += package.ConnectPayload.WillMessage.size();

	// will username
	v.push_back(0); v.push_back(package.ConnectPayload.Username.size());
	for (int i = 0; i < package.ConnectPayload.Username.size(); i++)
		v.push_back(package.ConnectPayload.Username[i]);
	remainLength += 2;
	remainLength += package.ConnectPayload.Username.size();

	// will password
	v.push_back(0); v.push_back(package.ConnectPayload.Password.size());
	for (int i = 0; i < package.ConnectPayload.Password.size(); i++)
		v.push_back(package.ConnectPayload.Password[i]);
	remainLength += 2;
	remainLength += package.ConnectPayload.Password.size();

	v.insert(v.begin() + 1, remainLength);

	return v;
}

bool operator==(const ConnectPackage& p1, const ConnectPackage& p2)
{
	return (
		p1.ControlHeader.PackageType == p2.ControlHeader.PackageType &&

		p1.ConnectVariableHeader.ProtocolName == p2.ConnectVariableHeader.ProtocolName &&
		p1.ConnectVariableHeader.Level == p2.ConnectVariableHeader.Level &&
		p1.ConnectVariableHeader.VariableLevel == p2.ConnectVariableHeader.VariableLevel &&
		p1.ConnectVariableHeader.KeepAlive == p2.ConnectVariableHeader.KeepAlive &&

		p1.ConnectPayload.ClientId == p2.ConnectPayload.ClientId &&
		p1.ConnectPayload.WillTopic == p2.ConnectPayload.WillTopic &&
		p1.ConnectPayload.WillMessage == p2.ConnectPayload.WillMessage &&
		p1.ConnectPayload.Username == p2.ConnectPayload.Username &&
		p1.ConnectPayload.Password == p2.ConnectPayload.Password
	);
}
#pragma endregion


TEST(ConnectionPackageConverterShould, GiveUsernameAndPassword)
{
	// Arrange
	auto expected = Generate();
	ConnectPackage actual;
	auto converter = Converters::ConnectConverter();

	// Act
	actual = converter.ToPackage(ToBuffer(expected));

	// Assert
	EXPECT_TRUE(expected == actual);
}