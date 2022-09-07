#include "mqttpch.h"
#include "ConnectAckConverter.h"

namespace MQTT {
	namespace Converters
	{
		const MqttPackages::ConnectAckPackage ConnectAckConverter::ToPackage(const std::vector<unsigned char>& buffer)
		{
			MqttPackages::ConnectAckVariableHeader connnectAckVariableHeader;
			MqttPackages::ControlHeader controlHeader;

			controlHeader.PackageType = (MqttPackages::ControlPackageType)buffer[0];
			int packageSize = buffer[1];

			if (packageSize > 0)
			{
				connnectAckVariableHeader.SessionPresentFlag = buffer[2];
				connnectAckVariableHeader.ConnectAckReturnCode = (MqttPackages::ConnectAckReturnCodeType)buffer[3];
			}

			MqttPackages::ConnectAckPackage connectAckPackage(controlHeader, connnectAckVariableHeader);

			return connectAckPackage;
		}

		const std::vector<unsigned char> ConnectAckConverter::ToBuffer(const MqttPackages::ConnectAckPackage& package)
		{
			std::vector<unsigned char> message;

			message.push_back(package.GetConnectControlHeader().PackageType * 16);
			message.push_back(0x02); // size, consider how to get size of the package
			message.push_back(package.GetConnectAckVariableHeader().SessionPresentFlag);
			message.push_back(package.GetConnectAckVariableHeader().ConnectAckReturnCode);

			return message;
		}
	}
}