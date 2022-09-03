#include "mqttpch.h"
#include "Server/MqttService.h"
#include "Server/SocketServer.h"

#include <iostream>

using namespace MQTT::Server;


int main() {
	try {
		auto socketServer = SocketServer(1883);
		MqttService mqttService = MqttService(&socketServer);
		mqttService.Start();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}