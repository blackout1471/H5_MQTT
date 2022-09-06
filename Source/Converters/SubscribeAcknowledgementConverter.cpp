#include "mqttpch.h"
#include "SubscribeAcknowledgementConverter.h"
#include "ConverterUtility.h"
namespace MQTT {
	namespace Converters {
		const MqttPackages::SubscribeAcknowledgementPackage SubscribeAcknowledgementConverter::ToPackage(const std::vector<unsigned char>& buffer)
		{
			return MqttPackages::SubscribeAcknowledgementPackage();
		}
		const std::vector<unsigned char> SubscribeAcknowledgementConverter::ToBuffer(const MqttPackages::SubscribeAcknowledgementPackage& to)
		{
			std::vector<unsigned char> packageBuffer;
			std::vector<unsigned char> variableHeader = ConverterUtility::IntToBytes(to.VariableHeader.PacketIdentifier);

			packageBuffer.push_back(to.Header.PackageType << 4);
			packageBuffer.push_back(variableHeader[0]);
			packageBuffer.push_back(variableHeader[1]);

			for (auto& returnType : to.Payload.Payload)
			{
				packageBuffer.push_back(returnType);
			}

			unsigned char remainLength = packageBuffer.size() - 1;

			packageBuffer.insert(packageBuffer.begin() + 1, remainLength);

			return packageBuffer;
		}
	}
}
