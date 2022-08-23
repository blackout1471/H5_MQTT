#include "mqttpch.h"
#include "Protocol/ConnectAckPackage.h"
#include "Protocol/ControlHeader.h"
#include "Server/SocketServer.h"

static MQTT::Server::SocketServer* server;
using namespace MQTT::Protocol;

static const std::vector<unsigned char> acceptBuffer = {
	0x20, 0x02, 0x01, 0x0
};

static void t(const MQTT::Server::Client& c, const std::vector<unsigned char>& d)
{
	switch (d[0])
	{
	case 0x10:
	{
		printf("Connect received");
	
		ControlHeader ch;
		ch.m_PackageType = ControlPackageType::ConnectAck;
		ConnectAckVariableHeader cavh;
		cavh.ConnectActReturnCode = ConnectActReturnCodeType::Refused_Not_Authorized;
		cavh.SessionPresentFlag = 0x00; // or 0x01

		ConnectAckPackage cap(ch, cavh);
		std::vector<unsigned char> ack;

		ack.push_back(cap.GetConnectControlHeader().m_PackageType * 16);
		ack.push_back(0x02); // size
		ack.push_back(cap.GetConnectAckVariableHeader().SessionPresentFlag);
		ack.push_back(cap.GetConnectAckVariableHeader().ConnectActReturnCode);

		server->Send(c, ack);
		break;

	}
	default:
		for (int i = 0; i < d.size(); i++)
		{
			printf("%02X ", d[i]);
		}
		break;
	}

	printf("\n");
}

int main() {

	server = new MQTT::Server::SocketServer(1883);
	server->OnReceivedData = t;

	server->Start();
	server->Stop();

	return 0;
}