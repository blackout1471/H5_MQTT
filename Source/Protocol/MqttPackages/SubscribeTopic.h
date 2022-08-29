#pragma once
#include <vector>
#include "SubscribeTopicWildcard.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribeTopic {
			std::vector<unsigned char> Topic;
			SubscribeTopicWildcardType Wildcard = SubscribeTopicWildcardType::NoWildcard;
			unsigned char QoS;
			bool HaveChild = false; // TODO: change later to binary tree
		};
	}
}