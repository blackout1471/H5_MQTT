#pragma once
#include "IRule.h"
#include "Server/MqttClient.h"

#include <string>
#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class ClientNotConnectedRule : public IRule
			{
			public:
				ClientNotConnectedRule(const std::string& clientId, const std::vector<Server::MqttClient*>& clientStates)
					: m_ClientId(clientId), m_ClientStates(clientStates) {};

				/*
				* Checks whether the clientId given exists and if they do if they are already connected.
				* Returns: false if exists or is connected, true if not
				*/
				inline virtual bool Validate() override {
					for (auto& clientState : m_ClientStates)
						if (clientState->ClientId == m_ClientId)
							return clientState->IsConnected;

					return true;
				}

			private:
				const std::string m_ClientId;
				const std::vector<Server::MqttClient*>& m_ClientStates;
			};
		}
	}
}