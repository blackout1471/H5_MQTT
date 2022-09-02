#pragma once
#include <vector>
#include "SubscribeAcknowledgementPayloadQoS.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribeAcknowledgementPayload {
			std::vector<SubscribeAcknowledgementQoS> Payload;
		};
	}
}
