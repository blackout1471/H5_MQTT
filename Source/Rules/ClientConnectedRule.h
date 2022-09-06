#pragma once
#include "IRule.h"
#include "Server/MqttClient.h"

#include <string>
#include <vector>

namespace MQTT {
	namespace Rules {
		class ClientConnectedRule : public IRule
		{
		public:
			ClientConnectedRule(const std::string& clientId, const std::vector<Server::MqttClient*>& clientStates)
				: m_ClientId(clientId), m_ClientStates(clientStates) {};

			/*
			* Checks whether the clientId given exists and if they do if they are already connected.
			* Returns: true if exists or is connected, false if not
			*/
			inline virtual bool Validate() override {
				for (auto& clientState : m_ClientStates)
					if (clientState->ClientId == m_ClientId)
						return clientState->IsConnected;

				return false;
			}

		private:
			const std::string m_ClientId;
			const std::vector<Server::MqttClient*>& m_ClientStates;
		};
	}
}