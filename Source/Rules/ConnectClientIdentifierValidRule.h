#pragma once
#include "IRule.h"
#include "Protocol/MqttClient.h"

#include <string>
#include <vector>

namespace MQTT {
	namespace Rules {
		class ConnectClientIdentifierValidRule : public IRule
		{
		public:
			ConnectClientIdentifierValidRule(const std::string& clientId, MqttPackages::ConnectFlagType flag)
				: m_ClientId(clientId), m_Flags(flag) {};

			/*
			* Checks whether client id is set and if clean session is 0
			* Returns false if thet are as described, true else.
			*/
			inline virtual bool Validate() override {
				if (m_ClientId.size() == 0 && !(m_Flags & MqttPackages::ConnectFlagType::Clean_Session))
					return false;

				return true;
			}

		private:
			const std::string m_ClientId;
			const MqttPackages::ConnectFlagType m_Flags;
		};
	}
};