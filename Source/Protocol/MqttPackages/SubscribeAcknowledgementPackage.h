#pragma once
#include "ControlHeader.h"
#include "SubscribeAcknowledgementVariableHeader.h"
#include "SubscribeAcknowledgementPayload.h";
namespace MQTT {
	namespace Protocol {
		class SubscribeAcknowledgementPackage {
		public:
			ControlHeader Header;
			SubscribeAcknowledgementVariableHeader VariableHeader;
			SubscribeAcknowledgementPayload Payload;
		};
	}
}