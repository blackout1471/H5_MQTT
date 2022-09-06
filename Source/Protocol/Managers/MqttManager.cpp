#include "mqttpch.h"
#include "MqttManager.h"


namespace MQTT {
	namespace Protocol {

		std::vector<unsigned char> MqttManager::GenerateConnectAckMessage(MqttPackages::ConnectAckReturnCodeType _returnCode)
		{
			MqttPackages::ControlHeader controlHeader;
			controlHeader.PackageType = MqttPackages::ConnectAck;

			MqttPackages::ConnectAckVariableHeader ackVariableHeader;
			ackVariableHeader.ConnectAckReturnCode = _returnCode;

			// TODO: change this according to connect package or saved client state otherwise set to 0x00
			ackVariableHeader.SessionPresentFlag = 0x01;

			MqttPackages::ConnectAckPackage ackPackage(controlHeader, ackVariableHeader);

			Converters::ConnectAckConverter ackConverter;

			return ackConverter.ToBuffer(ackPackage);
		}

	}
}