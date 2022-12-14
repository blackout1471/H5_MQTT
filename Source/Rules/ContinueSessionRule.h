#pragma once
#include "IRule.h"
#include "Protocol/MqttClient.h"
#include "MqttPackages/ConnectFlagType.h"

#include <vector>
#include <string>

namespace MQTT {
	namespace Rules {
		class ContinueSessionRule : public IRule {
		public:
			ContinueSessionRule(const MqttPackages::ConnectFlagType ConnectFlag, const std::string& clientId, const std::vector<Protocol::MqttClient*>& clients)
				: m_Flag(ConnectFlag), m_Clients(clients), m_ClientId(clientId) {};
			virtual ~ContinueSessionRule() {};

			/*
			* Checks whether clean session is set to 0 if a session is stored.
			* Returns: true if session should be continued false if not.
			*/
			virtual bool Validate() {
				if (!(m_Flag & MqttPackages::ConnectFlagType::Clean_Session))
					for (auto* client : m_Clients)
						if (client->ClientId == m_ClientId)
							return true;

				return false;
			}

		private:
			const std::string& m_ClientId;
			const MqttPackages::ConnectFlagType m_Flag;
			const std::vector<Protocol::MqttClient*>& m_Clients;
		};
	}
}