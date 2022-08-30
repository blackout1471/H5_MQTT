#pragma once

#include "Protocol/MqttPackages/ConnectFlagType.h"

namespace MQTT {
	namespace Server {

		struct MqttClient
		{
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
	}
}