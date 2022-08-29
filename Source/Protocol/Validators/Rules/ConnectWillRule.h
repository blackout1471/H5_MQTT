#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"
#include "Server/MqttClient.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class ConnectWillRule : public IRule {
			public:
				ConnectWillRule(Server::MqttClient* client, const ConnectFlagType& flag) : m_Client(client), m_Flag(flag) {};
				virtual ~ConnectWillRule() {};

				/*
				* Checks whether will flag is set.
				* if set -> store messages and check qos level
				* else set qos for client
				* Return: true if pass, false if not
				*/
				virtual bool Validate() override{
					if (m_Flag & ConnectFlagType::Will_Flag)
					{
						// Todo :: store will messages in client.

						if (m_Flag & ConnectFlagType::Will_QoS_LSB && m_Flag & ConnectFlagType::Will_QoS_MSB)
							return false;
					}

					// Todo :: Set client will qos & will retain to 0

					return true;
				}

			private:
				Server::MqttClient* m_Client;
				const ConnectFlagType& m_Flag;
			};

		}
	}
}