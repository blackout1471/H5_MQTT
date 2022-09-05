#pragma once

#include "Protocol/MqttPackages/ConnectFlagType.h"

namespace MQTT {
	namespace Server {

		struct MqttClient
		{
			std::string ConnectionIdentifier;
			bool IsConnected;
			std::string ClientId;
			std::string WillMessage;
			Protocol::ConnectFlagType ConnectionFlags;
		};

		static MqttClient* FindClient(const std::string& clientId, const std::vector<MqttClient*>& clients)
		{
			for (auto* client : clients)
				if (client->ClientId == clientId)
					return client;

			return nullptr;
		}

		static void DeleteClient(const std::string& clientId, std::vector<MqttClient*>& clients)
		{
			for (int i = 0; i < clients.size(); i++)
				if (clients[i]->ClientId == clientId)
				{
					clients.erase(clients.begin() + i);
					return;
				}
		}
	}
}