#pragma once
#include "Server/IServer.h"
#include "Protocol/IProtocolHandler.h"

namespace MQTT {
	class MqttService {

	public:
		/*
		* Creates the service with the specified server.
		*/
		MqttService(Server::IServer* server, Protocol::IProtocolHandler* handler);
		~MqttService();

		// Starts the server and begin goes in a loop to listen for incoming clients.
		void Start();

		/*
		* Stops the server and disconnects all clients.
		*/
		void Stop();

	private:
		/*
		* Initialises the server and sets the required data and events.
		*/
		void InitialiseServer();

		/*
		* Called when the server recieves data.
		*/
		void OnReceivedData(const Server::Client& client, const std::vector<unsigned char>& buffer);

	private:
		Server::IServer* m_Server;
		Protocol::IProtocolHandler* m_ProtocolHandler;
	};
}