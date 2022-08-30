#pragma once
#include "mqttpch.h"
#include "Protocol/MqttPackages/ConnectAckPackage.h"
#include "Protocol/Converters/ConnectAckConverter.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		class MqttManager {

		public:
			std::vector<unsigned char> GenerateConnectAckMessage(Protocol::ConnectAckReturnCodeType _returnCode);
		};
	}
}