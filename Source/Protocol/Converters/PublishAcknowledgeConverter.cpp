#include "mqttpch.h"
#include "PublishAcknowledgeConverter.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters {
			const PublishAcknowledgePackage PublishAcknowledgeConverter::ToPackage(const std::vector<unsigned char>& buffer)
			{
				return PublishAcknowledgePackage();
			}
			const std::vector<unsigned char> PublishAcknowledgeConverter::ToBuffer(const PublishAcknowledgePackage& to)
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