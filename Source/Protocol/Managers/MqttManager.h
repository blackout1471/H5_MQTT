#pragma once
#include "mqttpch.h"
#include "Protocol/MqttPackages/ConnectAckPackage.h"
#include "Protocol/Converters/ConnectAckConverter.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		class MqttManager {

		public:
			bool ValidMessage(std::vector<unsigned char> _message);
			std::vector<unsigned char> GenerateConnectAckMessage(Protocol::ConnectAckReturnCodeType _returnCode);
		};
	}
}