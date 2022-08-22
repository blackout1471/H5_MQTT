#include "mqttpch.h"

#include <iostream>
#include "Server/SocketServer.h"
using namespace MQTT::Server;

int main() {

	SocketServer a(1883);
	a.Start();

}