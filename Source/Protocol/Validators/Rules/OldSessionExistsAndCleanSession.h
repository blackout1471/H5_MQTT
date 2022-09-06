#pragma once

#include "IRule.h"
#include "Protocol/MqttPackages/ConnectFlagType.h"
#include "Server/MqttClient.h"

#include <string>
#include <vector>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class OldSessionExistsAndCleanSession : public IRule {
			public:
				OldSessionExistsAndCleanSession(ConnectFlagType flags, const std::string& clientId, const std::vector<Server::MqttClient*>& clients) 
					: m_Flags(flags), m_ClientId(clientId), m_ClientStates(clients) {};
				~OldSessionExistsAndCleanSession() {};

				/*
				* Checks whether the clientId given exists and if clean session is given.
				* Returns: true if exists and clean session == 1, false otherwise.
				*/
				inline virtual bool Validate() override {
					if (m_Flags & ConnectFlagType::Clean_Session)
						for (auto& clientState : m_ClientStates)
							if (clientState->ClientId == m_ClientId)
								return true;

					return false;
				}
			private:
				ConnectFlagType m_Flags;
				const std::string& m_ClientId;
				const std::vector<Server::MqttClient*>& m_ClientStates;
			};
		}
	}
}