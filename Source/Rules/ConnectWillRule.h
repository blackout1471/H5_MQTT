#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"
#include "Protocol/MqttClient.h"

namespace MQTT {
	namespace Rules {
		class ConnectWillRule : public IRule {
		public:
			ConnectWillRule(Protocol::MqttClient* client, const MqttPackages::ConnectFlagType& flag, const std::string& willMessage)
				: m_Client(client), m_Flag(flag), m_WillMessage(willMessage) {};
			virtual ~ConnectWillRule() {};

			/*
			* Checks whether will flag is set.
			* if set -> store messages and check qos level
			* else set qos for client
			* Return: true if pass, false if not
			*/
			virtual bool Validate() override {
				if (m_Flag & MqttPackages::ConnectFlagType::Will_Flag)
				{
					if (m_Flag & MqttPackages::ConnectFlagType::Will_QoS_LSB && m_Flag & MqttPackages::ConnectFlagType::Will_QoS_MSB)
						return false;

					m_Client->WillMessage = m_WillMessage;
					return true;
				}

				m_Client->ConnectionFlags = (MqttPackages::ConnectFlagType)(m_Client->ConnectionFlags & (~MqttPackages::ConnectFlagType::Will_Remain));
				m_Client->ConnectionFlags = (MqttPackages::ConnectFlagType)(m_Client->ConnectionFlags & (~MqttPackages::ConnectFlagType::Will_QoS_LSB));
				m_Client->ConnectionFlags = (MqttPackages::ConnectFlagType)(m_Client->ConnectionFlags & (~MqttPackages::ConnectFlagType::Will_QoS_MSB));

				return true;
			}

		private:
			Protocol::MqttClient* m_Client;
			const MqttPackages::ConnectFlagType& m_Flag;
			const std::string& m_WillMessage;
		};
	}
}