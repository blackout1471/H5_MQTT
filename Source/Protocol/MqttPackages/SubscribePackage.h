#pragma once
#include "SubscribePayload.h"
#include "SubscribeVariableHeader.h"
#include "ControlHeader.h"
namespace MQTT {
	namespace Protocol {
		struct SubscribePackage {
		public:
			ControlHeader Header;
			SubscribeVariableHeader VariableHeader;
			SubscribePayload Payload;
		};
	}
}