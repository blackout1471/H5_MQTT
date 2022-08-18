#pragma once

namespace MQTT
{
	namespace Models
	{
		enum ConnectFlagType : int
		{
			UserName = 128,
			Password = 64,
			Wil_Remain = 32,
			Will_QoS_MSB = 16,
			Will_QoS_LSB = 8,
			Will_Flag = 4,
			Clean_Session = 2
		};
	}
}