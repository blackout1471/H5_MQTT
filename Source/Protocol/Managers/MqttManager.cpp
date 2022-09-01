#include "mqttpch.h"
#include "MqttManager.h"


namespace MQTT {
	namespace Protocol {

		std::vector<unsigned char> MqttManager::GenerateConnectAckMessage(Protocol::ConnectAckReturnCodeType _returnCode)
		{
			Protocol::ControlHeader controlHeader;
			controlHeader.PackageType = ConnectAck;

			Protocol::ConnectAckVariableHeader ackVariableHeader;
			ackVariableHeader.ConnectAckReturnCode = _returnCode;

			// TODO: change this according to connect package or saved client state otherwise set to 0x00
			ackVariableHeader.SessionPresentFlag = 0x01;

			Protocol::ConnectAckPackage ackPackage(controlHeader, ackVariableHeader);

			Protocol::Converters::ConnectAckConverter ackConverter;

			return ackConverter.ToBuffer(ackPackage);
		}

	}
}