#pragma once

#include "Server/Client.h"
#include "Server/IServer.h"
#include "MqttPackages/Packages.h"

#include <vector>
#include <functional>

namespace MQTT {
	namespace Protocol {
		class IProtocolHandler {
		public:
			/*
			* Called when the server has received a connect package.
			*/
			virtual void OnClientConnected(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) = 0;

			/*
			* Called when the server has recieved a disconnect package.
			*/
			virtual void OnClientDisconnected(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) = 0;

			/*
			* Called when the server has recieved a publish package.
			*/
			virtual void OnClientPublished(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) = 0;

			/*
			* Called when the server has recieved a subscribe package.
			*/
			virtual void OnClientSubscribed(Server::IServer* server, const Server::Client& client, const std::vector<unsigned char> buffer) = 0;

			/*
			* Event for when a incoming request is handled.
			*/
			std::function<void(MqttPackages::ControlPackageType type, const Server::Client& client, const void* package)> OnRequestEvent;

		};
	}
}