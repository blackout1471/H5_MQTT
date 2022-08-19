#pragma once
#include "Client.h"
#include <vector>
#include <functional>
namespace MQTT {
	namespace Server {
		class IServer {
		public:
			
			virtual void Start() = 0;
			virtual void Stop() = 0;
			virtual void Disconnect(const Client& client) = 0;
			virtual void Send(const Client& client, const std::vector<unsigned char>& data) = 0;
			std::function<void(const Client&, const std::vector<unsigned char>&)> OnReceivedData;
		};

	}
}