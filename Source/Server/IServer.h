#pragma once
#include "Client.h"
#include <vector>
#include <functional>
namespace MQTT {
	namespace Server {
		class IServer {
		public:
			/* Starts the server to clients can begin to connect. */
			virtual void Start() = 0;
			/* Stops the server and disconnects clients */
			virtual void Stop() = 0;
			/* Forcibly disconnects a client from the server */
			virtual void Disconnect(const Client& client) = 0;
			/* Sends the byte data to the client */
			virtual void Send(const Client& client, const std::vector<unsigned char>& data) = 0;
			/* Called when client recieves data */
			std::function<void(const Client&, const std::vector<unsigned char>&)> OnReceivedData;
		};

	}
}