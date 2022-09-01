#pragma once
namespace MQTT {
	namespace Protocol {
		struct SubscribeAcknowledgementVariableHeader {
		public:
			int PacketIdentifier;
		};
	}
}
