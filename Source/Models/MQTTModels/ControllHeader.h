#pragma once

namespace MQTTModels
{
	struct ControllHeader
	{
	public:
		ControllFieldType PacketType;
	};

	enum ControllFieldType : int {
		Connect = 1,
		ConnectAck,
		Publish,
		PublishAck,
		Pubrec,
		Pubrel,
		Pubcomp,
		Subscribe,
		Suback,
		Unsubscribe,
		Unsuback,
		Pingreq,
		Pingresp,
		Disconnect = 0
	};
}