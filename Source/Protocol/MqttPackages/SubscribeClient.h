#pragma once
#include <string>

namespace MQTT {
	namespace Protocol {
		struct SubscribeClient {
			std::string ClientID;
			unsigned char QoS;
		public:
			SubscribeClient(std::string clientID, unsigned char qos) {
				ClientID = clientID;
				QoS = qos;
			}
		};
	}
}