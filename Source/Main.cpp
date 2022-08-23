#include "mqttpch.h"

#include "Server/MqttService.h"

using namespace MQTT::Server;

static MqttService* service;

int main() {

	auto server = new SocketServer(1883);
	service = new MqttService(server);
	service->Start();

	return 0;
}