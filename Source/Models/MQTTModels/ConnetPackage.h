#pragma once
#include "ConnectPayload.h"
#include "ConnectVariableHeader.h"
#include "ControlHeader.h"

namespace MQTT
{
	namespace Models
	{
		struct ConnectPackage
		{
		private:
			ControlHeader& m_ControlHeader;
			ConnectVariableHeader& m_ConnectVariableHeader;
			ConnectPayload& m_ConnectPayload;
		public:

			inline const ControlHeader& GetConnectControllHeader() { return m_ControlHeader; }
			inline void SetConnectControllHeader(const ControlHeader& _controllHeader) { m_ControlHeader = _controllHeader; }

			inline const ConnectVariableHeader& GetConnectVariableHeader() { return m_ConnectVariableHeader; }
			inline void SetConnectVariableHeader(const ConnectVariableHeader& _variableHeader) { m_ConnectVariableHeader = _variableHeader; }

			inline const ConnectPayload& GetConnectPayload() { return m_ConnectPayload; }
			inline void SetConnectVariableHeader(const ConnectPayload& _connectPayload) { m_ConnectPayload = _connectPayload; }
		};

	}

}