#pragma once

namespace MQTT
{
	namespace Protocol
	{
		enum ConnectFlagType : unsigned char
		{
			Username = 128,
			Password = 64,
			Will_Remain = 32,
			Will_QoS_MSB = 16,
			Will_QoS_LSB = 8,
			Will_Flag = 4,
			Clean_Session = 2,
			Reserved = 1
		};
	}
}
