#include "mqttpch.h"
#include "PublishAckConverter.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			const PublishAckPackage PublishAckConverter::ToPackage(const std::vector<unsigned char>& buffer)
			{
				return PublishAckPackage();
			}
			const std::vector<unsigned char> PublishAckConverter::ToBuffer(const PublishAckPackage& to)
			{
				std::vector<unsigned char> packageBuffer;
				std::vector<unsigned char> variableHeader = ConverterUtility::IntToBytes(to.PacketIdentifier);

				packageBuffer.push_back(to.Header.PackageType << 4);
				packageBuffer.push_back(0x2);
				packageBuffer.push_back(to.PacketIdentifier);

				return packageBuffer;
			}
		}
	}
}