#pragma once
#include <string>

namespace MQTT {
	namespace Protocol {
		struct SubscribeSavedMessage {
		public:
			SubscribeSavedMessage(std::string message, unsigned char qos) {
				Message = message;
				QoS = qos;
			}
			std::string Message;
			unsigned char QoS;
		};
	}
}