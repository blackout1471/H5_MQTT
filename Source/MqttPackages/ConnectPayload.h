#pragma once
#include <string>
#include <vector>

namespace MQTT {
	namespace MqttPackages {

		struct ConnectPayload
		{
			std::string ClientId;
			std::string WillTopic;
			std::string WillMessage;
			std::string Username;
			std::string Password;
		};
	}
}