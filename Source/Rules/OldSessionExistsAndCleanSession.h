#pragma once

#include "IRule.h"
#include "MqttPackages/ConnectFlagType.h"
#include "Protocol/MqttClient.h"

#include <string>
#include <vector>

namespace MQTT {
	namespace Rules {
		class OldSessionExistsAndCleanSession : public IRule {
		public:
			OldSessionExistsAndCleanSession(MqttPackages::ConnectFlagType flags, const std::string& clientId, const std::vector<Protocol::MqttClient*>& clients)
				: m_Flags(flags), m_ClientId(clientId), m_ClientStates(clients) {};
			~OldSessionExistsAndCleanSession() {};

			/*
			* Checks whether the clientId given exists and if clean session is given.
			* Returns: true if exists and clean session == 1, false otherwise.
			*/
			inline virtual bool Validate() override {
				if (m_Flags & MqttPackages::ConnectFlagType::Clean_Session)
					for (auto& clientState : m_ClientStates)
						if (clientState->ClientId == m_ClientId)
							return true;

				return false;
			}
		private:
			MqttPackages::ConnectFlagType m_Flags;
			const std::string& m_ClientId;
			const std::vector<Protocol::MqttClient*>& m_ClientStates;
		};
	}
}