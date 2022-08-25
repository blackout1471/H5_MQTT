#include "mqttpch.h"
#include "ConnectAckConverter.h"

namespace MQTT {

	namespace Protocol {
		namespace Converters
		{
			ConnectAckPackage ConnectAckConverter::ConvertToPackage(std::vector<unsigned char> _message)
			{
				ConnectAckVariableHeader connnectAckVariableHeader;
				ControlHeader controlHeader;

				controlHeader.PackageType = (ControlPackageType)_message[0];
				int packageSize = _message[1];

				if (packageSize > 0)
				{
					connnectAckVariableHeader.SessionPresentFlag = _message[2];
					connnectAckVariableHeader.ConnectAckReturnCode = (ConnectActReturnCodeType)_message[3];
				}

				ConnectAckPackage connectAckPackage(controlHeader, connnectAckVariableHeader);

				return connectAckPackage;
			}

			std::vector<unsigned char> ConnectAckConverter::ConvertFromPackage(ConnectAckPackage _package)
			{
				std::vector<unsigned char> message;

				message.push_back(_package.GetConnectControlHeader().PackageType * 16);
				message.push_back(0x02); // size, consider how to get size of the package
				message.push_back(_package.GetConnectAckVariableHeader().SessionPresentFlag);
				message.push_back(_package.GetConnectAckVariableHeader().ConnectAckReturnCode);

				return message;
			}
		}
	}
}