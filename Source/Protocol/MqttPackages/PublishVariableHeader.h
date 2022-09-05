#pragma once
#include <string>

namespace MQTT {
	namespace Protocol {
		struct PublishVariableHeader
		{
		public:
			std::string TopicName;
			unsigned short PacketIdentifier;
		};
	}
}