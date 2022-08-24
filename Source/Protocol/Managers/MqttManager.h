#pragma once
#include "mqttpch.h"
#include <vector>
#include "Protocol/ConnectAckPackage.h"
#include "Protocol/Converters/ConnectAckConverter.h"

namespace MQTT {
	namespace Protocol {
		class MqttManager {

		public:
			bool ValidMessage(std::vector<unsigned char> _message);
			std::vector<unsigned char> GenerateConnectAckMessage(Protocol::ConnectActReturnCodeType _returnCode);
		};
	}
}