#pragma once
#include <cstdint>
#include <vector>

namespace MQTTModels
{
	struct ConnectVariableHeader
	{
	public:
		int16_t NameLength;
		std::vector<unsigned char> ProtocolName;
		unsigned char Level;
		ConnectFlag VariableLevel;
		int16_t KeepAlive;
	};

	// move to its own header?
	enum ConnectFlag {
		UserName = 128,
		Password = 64,
		Wil_Remain = 32,
		Will_QoS_MSB = 16,
		Will_QoS_LSB = 8,
		Will_Flag = 4,
		Clean_Session = 2
	};

}