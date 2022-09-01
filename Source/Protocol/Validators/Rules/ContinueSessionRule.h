#pragma once
#include "IRule.h"
#include "Server/MqttClient.h"
#include "Protocol/MqttPackages/ConnectFlagType.h"

#include <vector>
#include <string>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class ContinueSessionRule : public IRule {
			public:
				ContinueSessionRule(const ConnectFlagType ConnectFlag, const std::string& clientId, const std::vector<Server::MqttClient*>& clients)
				 : m_Flag(ConnectFlag), m_Clients(clients), m_ClientId(clientId) {};
				virtual ~ContinueSessionRule() {};

				/*
				* Checks whether clean session is set to 0 if a session is stored.
				* Returns: true if session should be continued false if not.
				*/
				virtual bool Validate() {
					if (!(m_Flag & ConnectFlagType::Clean_Session))
						for (auto* client : m_Clients)
							if (client->ClientId == m_ClientId)
								return true;
					
					return false;
				}

			private:
				const std::string& m_ClientId;
				const ConnectFlagType m_Flag;
				const std::vector<Server::MqttClient*>& m_Clients;
			};
		}
	}
}