#pragma once
#include "mqttpch.h"
#include <vector>
#include "Protocol/ConnectAckPackage.h"
#include "Protocol/Converters/ConnectAckConverter.h"

namespace MQTT {
	namespace Protocol {
		class MqttManager {

		private:



		public:
			// consider out with the converter package
			bool ValidMessage(std::vector<unsigned char> _message);
			std::vector<unsigned char> GenerateConnectAckMessage(Protocol::ConnectActReturnCodeType _returnCode);
		};
	}
}