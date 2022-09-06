#pragma once
#include <string>

namespace MQTT {
	namespace MqttPackages {
		struct PublishVariableHeader
		{
		public:
			std::string TopicName;
			unsigned short PacketIdentifier;
		};
	}
}