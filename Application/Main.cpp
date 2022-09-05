#include "mqttpch.h"
#include "Server/MqttService.h"
#include "Server/SocketServer.h"
#include <cstring>
#include <iostream>

using namespace MQTT::Server;

const int SOCKET_PORT = 1883;

int getPort(int argc, char** argv)
{
	for (int i = 0; i < argc; ++i)
		if (strcmp(argv[i], "-p") == 0)
			return atoi(argv[i + 1]);

	return SOCKET_PORT;
}

int main(int argc, char** argv) {
	try
	{
		auto socketServer = SocketServer(getPort(argc, argv));
		MqttService mqttService = MqttService(&socketServer);
		mqttService.Start();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}