#pragma once
#include <string>

namespace MQTT {
	namespace MqttPackages {
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