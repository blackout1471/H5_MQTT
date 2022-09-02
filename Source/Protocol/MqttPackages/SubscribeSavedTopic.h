#pragma once
#include <vector>

namespace MQTT {
	namespace Protocol {
		struct SubscribeSavedTopic {
			std::vector<unsigned char> Message;
			unsigned char QoS;
		};
	}
}