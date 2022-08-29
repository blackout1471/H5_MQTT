#pragma once
namespace MQTT {
	namespace Protocol {
		struct SubscribeVariableHeader {
		public:
			int PacketIdentifier;
		};
	}
}