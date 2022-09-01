#pragma once
namespace MQTT {
	namespace Protocol {
		enum SubscribeAcknowledgementQoS : unsigned char {
			MaximumQoS0 = 0,
			MaximumQoS1 = 1,
			MaximumQoS2 = 2,
			Failure = 128
		};
	}
}