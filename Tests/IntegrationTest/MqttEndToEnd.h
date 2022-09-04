#pragma once
#include "SocketClient.h"

#include <gtest/gtest.h>
#include <mqtt.h>
#include <Server/MqttService.h>
#include <Server/SocketServer.h>
#include <thread>


using namespace MQTT::Server;

static bool operator==(const std::vector<unsigned char>& v1, const std::vector<unsigned char>& v2)
{
	if (v1.size() != v2.size())
		return false;

	for (size_t i = 0; i < v1.size(); i++)
		if (v1[i] != v2[i])
			return false;
	
	return true;
}


TEST(MqttEndToEnd, ClientCanConnect)
{
	// Arrange
	auto expected = std::vector<unsigned char>{32, 2, 1, 0};
	std::vector<unsigned char> actual;
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	std::vector<unsigned char> connectPackage = { 16,26,0,4,77,81,84,84,4,194,0,60,0,4,109,113,116,116,0,3,104,101,106,0,3,104,101,106 };
	std::thread serverThread;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/

	SocketClient client = SocketClient("127.0.0.1", "1883");
	client.Connect();
	client.Send(connectPackage);
	actual = client.Listen();

	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(MqttEndToEnd, ClientCanDisconnect)
{
	// Arrange
	auto expected = true;
	bool actual = true;
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	std::vector<unsigned char> connectPackage = { 16,26,0,4,77,81,84,84,4,194,0,60,0,4,109,113,116,116,0,3,104,101,106,0,3,104,101,106 };
	std::vector<unsigned char> disconnectPackage = { 224, 0 };
	std::thread serverThread;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/

	SocketClient client = SocketClient("127.0.0.1", "1883");
	client.Connect();
	client.Send(connectPackage);

	client.SetErrorEvent([&]() { actual = false; });
	client.Send(disconnectPackage);

	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(MqttEndToEnd, ClientCanSubscribe)
{
	// Arrange
	auto expected = std::vector<unsigned char> {144, 3, 251, 126, 0};
	std::vector<unsigned char> actual;
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	std::vector<unsigned char> connectPackage = { 16,26,0,4,77,81,84,84,4,194,0,60,0,4,109,113,116,116,0,3,104,101,106,0,3,104,101,106 };
	std::vector<unsigned char> subscribePackage = { 130, 16, 251, 126, 0, 11, 116, 101, 115, 116, 116, 111, 112, 105, 99, 47, 35, 0};
	std::thread serverThread;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/

	SocketClient client = SocketClient("127.0.0.1", "1883");
	client.Connect();
	client.Send(connectPackage);
	client.Listen();
	client.Send(subscribePackage);
	actual = client.Listen();

	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}

// TODO :: This test can not be done yet...

//TEST(MqttEndToEnd, ClientCanPublish)
//{
//	// Arrange
//	auto expected = std::vector<unsigned char>{ 5 << 4, 2, 0, 1 };
//	std::vector<unsigned char> actual;
//	auto socketServer = new SocketServer(1883);
//	auto mqttService = new MqttService(socketServer);
//	std::vector<unsigned char> connectPackage = { 16,26,0,4,77,81,84,84,4,194,0,60,0,4,109,113,116,116,0,3,104,101,106,0,3,104,101,106 };
//	std::vector<unsigned char> publishPackage = { 48,27,0,5,104,101,108,108,111,123,10,32,32,34,109,115,103,34,58,32,34,104,101,108,108,111,34,10,125, };
//	std::thread serverThread;
//
//	// Act
//	serverThread = std::thread([&]() { mqttService->Start(); });
//	while (!socketServer->IsRunning()) {}; // Wait for thread :/
//
//	SocketClient client = SocketClient("127.0.0.1", "1883");
//	client.Connect();
//	client.Send(connectPackage);
//	client.Listen();
//	client.Send(publishPackage);
//	actual = client.Listen();
//
//	mqttService->Stop();
//	serverThread.join();
//
//	// Assert
//	EXPECT_TRUE(expected == actual);
//}