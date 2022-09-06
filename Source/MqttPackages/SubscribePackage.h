#pragma once
#include "SubscribePayload.h"
#include "SubscribeVariableHeader.h"
#include "ControlHeader.h"
namespace MQTT {
	namespace MqttPackages {
		struct SubscribePackage {
		public:
			ControlHeader Header;
			SubscribeVariableHeader VariableHeader;
			SubscribePayload Payload;
		};
	}
}