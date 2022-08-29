#pragma once
#include <vector>
namespace MQTT {
	namespace Protocol {
		struct SubscribeTopic {
			std::vector<unsigned char> Topic;
			unsigned char TopicWildCard; // Consider enum
			unsigned char QoS;
		};
	}
}