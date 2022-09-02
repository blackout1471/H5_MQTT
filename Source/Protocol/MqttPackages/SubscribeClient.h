#pragma once
#include <string>

namespace MQTT {
	namespace Protocol {
		struct SubscribeClient {
		public:
			SubscribeClient(std::string clientID, unsigned char qos) {
				ClientID = clientID;
				QoS = qos;
			}
			const std::string& GetClientID() { return ClientID; }
			const unsigned char& GetQoS() { return QoS; }
			void SetQoS(const int qos) { QoS = qos; }
		private:
			std::string ClientID;
			unsigned char QoS;
		};
	}
}