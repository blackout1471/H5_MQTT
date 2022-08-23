#include "mqttpch.h"

#include "Server/SocketServer.h"
#include "Protocol/Converter/Converter.h"
#include "Protocol/ConnectPackage.h"

static MQTT::Server::SocketServer* server;
static auto* conv = new MQTT::Protocol::Converter::Converter();

static const std::vector<unsigned char> acceptBuffer = {
	0x20, 0x02, 0x01, 0x0
};

static void t(const MQTT::Server::Client& c, const std::vector<unsigned char>& d)
{
	auto p = MQTT::Protocol::ConnectPackage();

	switch (d[0])
	{
	case 0x10:
		printf("Connect received");
		conv->Convert(d, p);
		server->Send(c, acceptBuffer);
		break;

	default:
		for (int i = 0; i < d.size(); i++)
		{
			printf("%02X ", d[i]);
		}
		break;
	}

	printf("\n");
}

#include <iostream>

int main() {

	server = new MQTT::Server::SocketServer(1883);
	server->OnReceivedData = t;

	server->Start();
	server->Stop();

	return 0;
}