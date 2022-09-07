#include "mqttpch.h"
#include "MqttService.h"
#include "Server/SocketServer.h"
#include "Protocol/Mqtt311Handler.h"

#include <cstring>
#include <iostream>

using namespace MQTT::Server;
using namespace MQTT;

const int SOCKET_PORT = 1883;

int getPort(int argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
		if (strcmp(argv[i], "-p") == 0)
			return atoi(argv[i + 1]);

	return SOCKET_PORT;
}

void OnSocketErrorPrint(std::string errorMessage) {
	std::cout << errorMessage << std::endl;
}

void OnRequestPrint(MqttPackages::ControlPackageType type, const Server::Client& client, const void* package) {
	
	std::string message;
	
	switch (type)
	{
	case MQTT::MqttPackages::Connect:
		message = "Client connected: ClientId->" + ((MqttPackages::ConnectPackage*)package)->Payload.ClientId + " ClientIdentifier-> " + client.GetIdentifier();
		break;
	case MQTT::MqttPackages::Publish:
		message = "Client publish: ClientId-> " + client.GetIdentifier() + " Payload-> " + ((MqttPackages::PublishPackage*)package)->Payload;
		break;
	case MQTT::MqttPackages::Subscribe:
		message = "Client subscribed: ClientIdentifier->" + client.GetIdentifier();
		break;
	case MQTT::MqttPackages::Disconnect:
		message = "Client disconnected: ClientIdentifier->" + client.GetIdentifier();
		break;
	default:
		break;
	}

	std::cout << message << std::endl;
}

int main(int argc, char** argv) {
	try
	{
		std::cout << "Started on port: " << getPort(argc, argv) << std::endl;
		auto socketServer = SocketServer(getPort(argc, argv));
		socketServer.ErrorEvent = OnSocketErrorPrint;

		auto protocolHandler = Protocol::Mqtt311Handler();
		protocolHandler.OnRequestEvent = OnRequestPrint;

		MqttService mqttService = MqttService(&socketServer, &protocolHandler);
		mqttService.Start();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}