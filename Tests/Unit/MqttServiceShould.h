#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <Server/IServer.h>
#include <Server/Client.h>
#include <Protocol/Mqtt311Handler.h>

#include <MqttService.h>

using namespace MQTT::Server;
using namespace MQTT;
using namespace MQTT::Protocol;
using ::testing::AtLeast;

class MockServer : public IServer
{
public:
	MockServer() {};
	~MockServer() {};

	MOCK_METHOD(void, Start, (), (override));
	MOCK_METHOD(void, Stop, (), (override));
	MOCK_METHOD(void, Disconnect, (const Client& client), (override));
	MOCK_METHOD(void, Send, (const Client& client, const std::vector<unsigned char>& data), (override));
	MOCK_METHOD(bool, IsRunning, (), (const, override));
	MOCK_METHOD(const std::vector<Client*>&, GetClients, (), (const override));

};

TEST(MqttServiceShould, CallServerStart_WhenServiceCallsStart)
{
	// Arrange
	Mqtt311Handler b = Mqtt311Handler();
	MockServer a;
	EXPECT_CALL(a, Start());
	auto service = MqttService(&a, &b);

	// Act & Assert
	service.Start();
}
