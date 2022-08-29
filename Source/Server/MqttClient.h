#pragma once

namespace MQTT {
	namespace Server {

		struct MqttClient
		{
			std::string ConnectionIdentifier;
			std::string ClientId;
			bool IsConnected;
		};
	}
}