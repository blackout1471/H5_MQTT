#pragma once
#include "ConnectActReturnCodeType.h"

namespace MQTT {
	namespace Protocol {
		struct ConnectAckVariableHeader {
			unsigned char SessionPressentFlag;
			ConnectActReturnCodeType ConnectActReturnCode;
		};
	}
}