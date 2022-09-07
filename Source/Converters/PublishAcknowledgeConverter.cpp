#include "mqttpch.h"
#include "PublishAcknowledgeConverter.h"
namespace MQTT {
	namespace Converters {
		const MqttPackages::PublishAcknowledgePackage PublishAcknowledgeConverter::ToPackage(const std::vector<unsigned char>& buffer)
		{
			return MqttPackages::PublishAcknowledgePackage();
		}
		const std::vector<unsigned char> PublishAcknowledgeConverter::ToBuffer(const MqttPackages::PublishAcknowledgePackage& to)
		{
			std::vector<unsigned char> packageBuffer;
			packageBuffer.push_back(to.Header.PackageType << 4);
			packageBuffer.push_back(0x2);

			std::vector<unsigned char> variableHeader = ConverterUtility::IntToBytes(to.PacketIdentifier);

			packageBuffer.push_back(variableHeader[0]);
			packageBuffer.push_back(variableHeader[1]);

			return packageBuffer;
		}
	}
}