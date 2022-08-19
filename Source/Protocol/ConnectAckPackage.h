#pragma once
#include "ControlHeader.h"
#include "ConnectAckVariableHeader.h"

namespace MQTT {
	namespace Protocol {
	
		class ConnectAckPackage {
		public:
			ConnectAckPackage(const ControlHeader& controlHeader, const ConnectAckVariableHeader& variableHeader)
				: m_ControlHeader(controlHeader), m_ConnectAckVariableHeader(variableHeader) {};

			virtual ~ConnectAckPackage() = 0;

		public:

			inline const ControlHeader& GetConnectControlHeader() const { return m_ControlHeader; }
			inline void SetConnectControlHeader(const ControlHeader& _controlHeader) { m_ControlHeader = _controlHeader; }

			inline const ConnectAckVariableHeader& GetConnectAckVariableHeader() const { return m_ConnectAckVariableHeader; }
			inline void SetConnectVariableHeader(const ConnectAckVariableHeader& _variableHeader) { m_ConnectAckVariableHeader = _variableHeader; }

		private:
			ControlHeader m_ControlHeader;
			ConnectAckVariableHeader m_ConnectAckVariableHeader;
		};
	}
}