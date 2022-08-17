#pragma once
#include "ConnectPayload.h"
#include "ConnectVariableHeader.h"
#include "ControllHeader.h"

namespace MQTTModels
{
	struct ConnectPacket 
	{

	private:
		ControllHeader m_controllHeader;
		ConnectVariableHeader m_ConnectVariableHeader;
		ConnectPayload m_ConnectPayload;
	public:

		inline ControllHeader GetConnectControllHeader() { return m_controllHeader; }
		inline void SetConnectControllHeader(ControllHeader _controllHeader) { m_controllHeader = _controllHeader;  }

		inline ConnectVariableHeader GetConnectVariableHeader() { return m_ConnectVariableHeader; }
		inline void SetConnectVariableHeader(ConnectVariableHeader _variableHeader) { m_ConnectVariableHeader = _variableHeader; }

		inline ConnectPayload GetConnectPayload() { return m_ConnectPayload; }
		inline void SetConnectVariableHeader(ConnectPayload _connectPayload) { m_ConnectPayload = _connectPayload; }
	};
}