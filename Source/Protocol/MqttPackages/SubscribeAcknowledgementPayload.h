#pragma once
#include <vector>
#include "SubscribeAcknowledgementPayloadReturnTypes.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribeAcknowledgementPayload {
			std::vector<SubscribeAcknowledgementQoS> payload;
		};
	}
}
