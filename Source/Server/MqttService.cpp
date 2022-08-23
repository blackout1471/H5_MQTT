#pragma once

#include "mqttpch.h"
#include "MqttService.h"

namespace MQTT
{
	namespace Server {

		void MqttService::PackageReceived(const MQTT::Server::Client& _client, const std::vector<unsigned char>& _message)
		{
			if (!m_MqttManager.ValidMessage(_message))
			{
				// package is not valid
				m_Server->Disconnect(_client);
			}

			// TODO:
			// Right now it always response with accept

			// create ack package
			auto ackMessage = m_MqttManager.GenerateConnectAckMessage(Protocol::Accepted);

			m_Server->Send(_client, ackMessage);
		}
	}
}