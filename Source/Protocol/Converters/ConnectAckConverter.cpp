#include "mqttpch.h"
#include "ConnectAckConverter.h"

namespace MQTT {

	namespace Protocol {
		namespace Converters
		{
			const ConnectAckPackage ConnectAckConverter::ToPackage(const std::vector<unsigned char>& buffer)
			{
				ConnectAckVariableHeader connnectAckVariableHeader;
				ControlHeader controlHeader;

				controlHeader.PackageType = (ControlPackageType)buffer[0];
				int packageSize = buffer[1];

				if (packageSize > 0)
				{
					connnectAckVariableHeader.SessionPresentFlag = buffer[2];
					connnectAckVariableHeader.ConnectAckReturnCode = (ConnectAckReturnCodeType)buffer[3];
				}

				ConnectAckPackage connectAckPackage(controlHeader, connnectAckVariableHeader);

				return connectAckPackage;
			}

			const std::vector<unsigned char> ConnectAckConverter::ToBuffer(const ConnectAckPackage& package)
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
}