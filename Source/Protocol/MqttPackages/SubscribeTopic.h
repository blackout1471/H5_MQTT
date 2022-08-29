#pragma once
#include <vector>
#include "SubscribeTopicWildcard.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribeTopic {
			std::vector<unsigned char> Topic;
			SubscribeTopicWildcardType Wildcard;
			unsigned char QoS;
		};
	}
}