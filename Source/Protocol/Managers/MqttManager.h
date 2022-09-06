#pragma once
#include "mqttpch.h"
#include "MqttPackages/packages.h"
#include "Protocol/Converters/ConnectAckConverter.h"

#include <vector>

namespace MQTT {
	namespace Protocol {
		class MqttManager {

		public:
			/*
			* Generates the appropriate message based upon the given return code wanted.
			*/
			std::vector<unsigned char> GenerateConnectAckMessage(MqttPackages::ConnectAckReturnCodeType _returnCode);
		};
	}
}