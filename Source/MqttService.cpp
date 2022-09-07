#pragma once

#include "mqttpch.h"
#include "MqttService.h"
#include "Converters/ConverterUtility.h"

namespace MQTT {
	MqttService::MqttService(Server::IServer* server, Protocol::IProtocolHandler* protocolHandler) 
		: m_Server(server), m_ProtocolHandler(protocolHandler)
	{
		InitialiseServer();
	}

	MqttService::~MqttService() {}

	void MqttService::Start()
	{
		m_Server->Start();
	}

	void MqttService::Stop()
	{
		m_Server->Stop();
	}

	void MqttService::OnReceivedData(const Server::Client& client, const std::vector<unsigned char>& buffer)
	{
		auto type = Converters::ConverterUtility::GetPackageType(buffer[0]);

		switch (type)
		{
		case MqttPackages::Connect:
			m_ProtocolHandler->OnClientConnected(m_Server, client, buffer);
			break;
		case MqttPackages::Publish:
			m_ProtocolHandler->OnClientPublished(m_Server, client, buffer);
			break;
		case MqttPackages::Subscribe:
			m_ProtocolHandler->OnClientSubscribed(m_Server, client, buffer);
			break;
		case MqttPackages::Disconnect:
			m_ProtocolHandler->OnClientDisconnected(m_Server, client, buffer);
			break;
		default:
			for (int i = 0; i < buffer.size(); i++)
			{
				printf("%02X ", buffer[i]);
			}
		}
	}

	void MqttService::InitialiseServer()
	{
		m_Server->OnReceivedData = std::bind(&MqttService::OnReceivedData, this, std::placeholders::_1, std::placeholders::_2);
	}
}
