#pragma once

namespace MQTT {
	namespace Protocol {
		enum ControlPackageType : unsigned char
		{
			Connect = 1,
			ConnectAck,
			Publish,
			PublAck,
			PubRec,
			PubRel,
			PubComp,
			Subscribe,
			SubAck = 9,
			Unsubscribe,
			UnsubAck,
			PingReq,
			PingResp,
			Disconnect = 14
		};
	}
}