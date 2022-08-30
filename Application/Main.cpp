#include "mqttpch.h"
#include "Server/MqttService.h"
#include "Server/SocketServer.h"

#include <iostream>

using namespace MQTT::Server;


int main() {
	try {

		MqttService mqttService = MqttService(new SocketServer(1883));
		mqttService.Start();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}