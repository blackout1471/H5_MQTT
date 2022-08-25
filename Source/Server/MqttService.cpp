#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Protocol/Converters/ConverterUtility.h"

#include <algorithm>

namespace MQTT {
	namespace Server {
		MqttService::MqttService(IServer* server) : m_Server(server), m_ClientStates(), m_Manager()
		{
			InitialiseServer();
		}

		MqttService::~MqttService()
		{
			for (auto clientStates : m_ClientStates)
				delete clientStates;

			delete m_Server;
		}

		void MqttService::Start()
		{
			m_Server->Start();
		}

		void MqttService::OnReceivedData(const Client& client, const std::vector<unsigned char>& buffer)
		{
			auto type = Protocol::Converters::ConverterUtility::GetPackageType(buffer[0]);

			// TODO :: do validation
			switch (type)
			{
			case MQTT::Protocol::Connect:
				OnClientConnect(client, Protocol::Converters::ConnectConverter().ToPackage(buffer));
				break;
			case MQTT::Protocol::ConnectAck:
				break;
			case MQTT::Protocol::Publish:
				break;
			case MQTT::Protocol::PublAck:
				break;
			case MQTT::Protocol::PubRec:
				break;
			case MQTT::Protocol::PubRel:
				break;
			case MQTT::Protocol::PubComp:
				break;
			case MQTT::Protocol::Subscribe:
				break;
			case MQTT::Protocol::SubAck:
				break;
			case MQTT::Protocol::Unsubscribe:
				break;
			case MQTT::Protocol::UnsubAck:
				break;
			case MQTT::Protocol::PingReq:
				break;
			case MQTT::Protocol::PingResp:
				break;
			case MQTT::Protocol::Disconnect:
				break;
			default:
				for (int i = 0; i < buffer.size(); i++)
				{
					printf("%02X ", buffer[i]);
				}
			}
		}

		void MqttService::OnClientConnect(const Client& client, const Protocol::ConnectPackage& package)
		{
			auto packageClientId = std::string(package.ConnectPayload.ClientId.begin(), package.ConnectPayload.ClientId.end());

			auto position = std::find_if(m_ClientStates.begin(), m_ClientStates.end(), [&](const MqttClient* c)
{
					return c->ClientId == packageClientId;
			});

			if (position != m_ClientStates.end())
		{
				if ((*position)->IsConnected)
			{
					m_Server->Disconnect(client);
					(*position)->IsConnected = false;
					return;
				}	
			}

			auto clientState = new MqttClient();
			clientState->IsConnected = true;
			clientState->ClientId = packageClientId;

			m_ClientStates.push_back(clientState);

			auto ackMessage = m_Manager.GenerateConnectAckMessage(Protocol::Accepted);

			m_Server->Send(client, ackMessage);
		}

		void MqttService::InitialiseServer()
		{
			m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
		}
	}
}