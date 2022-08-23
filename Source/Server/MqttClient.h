#pragma once

namespace MQTT {
	namespace Server {

		struct MqttClient
		{
			std::string ClientId;
			bool IsConnected;
		};
	}
}