#pragma once
#include <string>
#include <cstdint>

namespace MQTT {
	namespace Protocol {
		struct PublishVariableHeader
		{
		public:
			std::string TopicName;
			int16_t PacketIdentifier;
		};
	}
}