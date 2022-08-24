#include "mqttpch.h"
#include "Server/MqttService.h"
#include <iostream>
int main() {
	try {

		MQTT::Server::MqttService mqttService = MQTT::Server::MqttService();
	}
	catch (const std::exception& e) {
		std::cout << "Error: " << e.what() << std::endl;
	}
	return 0;
}