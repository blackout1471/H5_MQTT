#pragma once
#include "ConnectPayload.h"
#include "ConnectVariableHeader.h"
#include "ControlHeader.h"

namespace MQTT {
	namespace Protocol {
		struct ConnectPackage
		{
		public:
			ConnectPackage(const ControlHeader& controlHeader, const ConnectVariableHeader& variableHeader, const ConnectPayload& payload)
				: m_ControlHeader(controlHeader), m_ConnectVariableHeader(variableHeader), m_ConnectPayload(payload) {};
			
			virtual ~ConnectPackage() = 0;

		public:

			inline const ControlHeader& GetConnectControlHeader() const { return m_ControlHeader; }
			inline void SetConnectControlHeader(const ControlHeader& _controlHeader) { m_ControlHeader = _controlHeader; }

			inline const ConnectVariableHeader& GetConnectVariableHeader() const { return m_ConnectVariableHeader; }
			inline void SetConnectVariableHeader(const ConnectVariableHeader& _variableHeader) { m_ConnectVariableHeader = _variableHeader; }

			inline const ConnectPayload& GetConnectPayload() const { return m_ConnectPayload; }
			inline void SetConnectVariableHeader(const ConnectPayload& _connectPayload) { m_ConnectPayload = _connectPayload; }

		private:
			ControlHeader m_ControlHeader;
			ConnectVariableHeader m_ConnectVariableHeader;
			ConnectPayload m_ConnectPayload;
		};

	}

};