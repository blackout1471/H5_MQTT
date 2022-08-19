#pragma once
#include "ConnectActReturnCodeType.h"

namespace MQTT {
	namespace Protocol {
		struct ConnectAckVariableHeader {
			unsigned char SessionPresentFlag;
			ConnectActReturnCodeType ConnectActReturnCode;
		};
	}
}