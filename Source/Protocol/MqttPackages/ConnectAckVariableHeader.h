#pragma once
#include "ConnectAckReturnCodeType.h"

namespace MQTT {
	namespace Protocol {
		struct ConnectAckVariableHeader {
			unsigned char SessionPresentFlag;
			ConnectAckReturnCodeType ConnectAckReturnCode;
		};
	}
}