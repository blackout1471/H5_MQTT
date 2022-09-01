#pragma once
#include <vector>
#include "SubscribeTopicWildCard.h"
#include "SubscribeTopic.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribePayload {
		public:
			std::vector<SubscribeTopic> Topics;
		};
	}
}