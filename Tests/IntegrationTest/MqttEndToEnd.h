#pragma once
#include "SocketUtility.h"

#include <gtest/gtest.h>
#include <mqtt.h>
#include <Server/MqttService.h>
#include <Server/SocketServer.h>
#include <thread>

using namespace MQTT::Server;


TEST(MqttEndToEnd, ClientCanConnect)
{
	// Arrange
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	int socket;
	struct mqtt_client client;
	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */
	const char* client_id = "hej";
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	std::thread serverThread;
	MQTTErrors actual = MQTTErrors::MQTT_ERROR_NULLPTR;
	MQTTErrors expected = MQTTErrors::MQTT_OK;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/

	socket = create_socket(1883);
	actual = mqtt_init(&client, socket, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), [](void** unused, struct mqtt_response_publish* published) {});
	actual = mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
	close_socket(socket);
	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(MqttEndToEnd, ClientCanDisconnect)
{
	// Arrange
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	int socket;
	struct mqtt_client client;
	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */
	const char* client_id = "hej";
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	std::thread serverThread;
	MQTTErrors actual = MQTTErrors::MQTT_ERROR_NULLPTR;
	MQTTErrors expected = MQTTErrors::MQTT_OK;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/
	socket = create_socket(1883);
	actual = mqtt_init(&client, socket, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), [](void** unused, struct mqtt_response_publish* published) {});
	actual = mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
	actual = mqtt_disconnect(&client);
	close_socket(socket);
	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(MqttEndToEnd, ClientCanSubscribe)
{
	// Arrange
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	int socket;
	struct mqtt_client client;
	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */
	const char* client_id = "hej";
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	std::thread serverThread;
	MQTTErrors actual = MQTTErrors::MQTT_ERROR_NULLPTR;
	MQTTErrors expected = MQTTErrors::MQTT_OK;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/
	socket = create_socket(1883);
	actual = mqtt_init(&client, socket, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), [](void** unused, struct mqtt_response_publish* published) {});
	actual = mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
	actual = mqtt_subscribe(&client, "jacket", 0);
	close_socket(socket);
	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(MqttEndToEnd, ClientCanPublish)
{
	// Arrange
	auto socketServer = new SocketServer(1883);
	auto mqttService = new MqttService(socketServer);
	int socket;
	struct mqtt_client client;
	uint8_t sendbuf[2048]; /* sendbuf should be large enough to hold multiple whole mqtt messages */
	uint8_t recvbuf[1024]; /* recvbuf should be large enough any whole mqtt message expected to be received */
	const char* client_id = "hej";
	uint8_t connect_flags = MQTT_CONNECT_CLEAN_SESSION;
	std::thread serverThread;
	MQTTErrors actual = MQTTErrors::MQTT_ERROR_NULLPTR;
	MQTTErrors expected = MQTTErrors::MQTT_OK;

	// Act
	serverThread = std::thread([&]() { mqttService->Start(); });
	while (!socketServer->IsRunning()) {}; // Wait for thread :/
	socket = create_socket(1883);
	actual = mqtt_init(&client, socket, sendbuf, sizeof(sendbuf), recvbuf, sizeof(recvbuf), [](void** unused, struct mqtt_response_publish* published) {});
	actual = mqtt_connect(&client, client_id, NULL, NULL, 0, NULL, NULL, connect_flags, 400);
	actual = mqtt_publish(&client, "jacket", "hej", 3, 0);
	close_socket(socket);
	mqttService->Stop();
	serverThread.join();

	// Assert
	EXPECT_TRUE(expected == actual);
}