#include "MqttManager.h"


namespace MQTT {
	namespace Protocol {
		bool MqttManager::ValidMessage(std::vector<unsigned char> _message) {
			
			// Message is whack
			if (_message.size() == 0)
				return false;

			if (_message[0] == MQTT::Protocol::Connect) {
				// convert here


				// protocol validator here

				return true;
			}
		}

		std::vector<unsigned char> MqttManager::GenerateConnectAckMessage(Protocol::ConnectActReturnCodeType _returnCode)
		{
			Protocol::ControlHeader controlHeader;
			controlHeader.m_PackageType = ConnectAck;

			Protocol::ConnectAckVariableHeader ackVariableHeader;
			ackVariableHeader.ConnectActReturnCode = _returnCode;
			// TODO:
			ackVariableHeader.SessionPresentFlag = 0x01;

			Protocol::ConnectAckPackage ackPackage(controlHeader, ackVariableHeader);

			Protocol::Converters::ConnectAckConverter ackConverter;			
			return ackConverter.ConvertFromPackage(ackPackage);
		}

	}
}