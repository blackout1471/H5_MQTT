#pragma once

#include "MqttPackages/packages.h"

namespace MQTT {
	namespace Protocol {

		struct MqttClient
		{
			/*
			* The unique identifier of the client on the server side.
			*/
			std::string ConnectionIdentifier;

			/*
			* Whether the client is connected to the server or not.
			*/
			bool IsConnected;

			/*
			* The client id coming from the mqtt client
			*/
			std::string ClientId;

			/*
			* The clients will message.
			*/
			std::string WillMessage;

			/*
			* The connection flags which the client connected with.
			*/
			MqttPackages::ConnectFlagType ConnectionFlags;
		};

		/*
		* Retrieves the clients state from a vector client states.
		* Returns: pointer to object or nullptr if not found.
		*/
		static MqttClient* FindClient(const std::string& clientId, const std::vector<MqttClient*>& clients)
		{
			for (auto* client : clients)
				if (client->ClientId == clientId)
					return client;

			return nullptr;
		}

		/*
		* Deletes the matching client state from the given vector of client states.
		*/
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