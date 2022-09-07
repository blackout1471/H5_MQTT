#pragma once
#include "ConnectAckReturnCodeType.h"

namespace MQTT {
	namespace MqttPackages {
		struct ConnectAckVariableHeader {
			unsigned char SessionPresentFlag;
			ConnectAckReturnCodeType ConnectAckReturnCode;
		};
	}
}