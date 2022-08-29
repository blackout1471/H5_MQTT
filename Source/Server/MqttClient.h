#pragma once

#include "Protocol/MqttPackages/ConnectFlagType.h"

namespace MQTT {
	namespace Server {

		struct MqttClient
		{
			bool IsConnected;
			std::string ClientId;
			std::string WillMessage;
			Protocol::ConnectFlagType ConnectionFlags;
		};
	}
}