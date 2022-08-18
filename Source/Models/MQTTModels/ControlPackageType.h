#pragma once

namespace MQTT
{
	namespace Models
	{
		enum ControlPackageType : int
		{
			Connect = 1,
			ConnectAck,
			Publish,
			PublAck,
			PubRec,
			PubRel,
			PubComp,
			Subscribe,
			SubAck,
			Unsubscribe,
			UnsubAck,
			PingReq,
			PingResp,
			Disconnect = 0
		};
	}
}