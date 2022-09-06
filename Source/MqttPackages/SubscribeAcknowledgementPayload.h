#pragma once
#include <vector>
#include "SubscribeAcknowledgementPayloadQoS.h"
namespace MQTT {
	namespace MqttPackages {
		struct SubscribeAcknowledgementPayload {
			std::vector<SubscribeAcknowledgementQoS> Payload;
		};
	}
}
