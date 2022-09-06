#include "mqttpch.h"
#include "PublishConverter.h"

#include "ConverterUtility.h"

namespace MQTT {
	namespace Protocol {
		namespace Converters {
			PublishConverter::PublishConverter() {}
			PublishConverter::~PublishConverter() {}

			const PublishPackage PublishConverter::ToPackage(const std::vector<unsigned char>& buffer)
			{
				auto package = PublishPackage();

				if (buffer.size() < 3)
					return package;

				const unsigned char* data = buffer.data();

				package.Header.PackageType = ConverterUtility::GetPackageType(*data);
				package.HeaderFlag = PublishHeaderFlag(*data & 0xf);
				data++;

				unsigned char remainLength = *data;
				data++;

				auto topicLength = ConverterUtility::ByteToInt(*data, data[1]);
				data += 2;

				package.VariableHeader.TopicName = std::string(data, data + topicLength);
				data += topicLength;

				if (package.HeaderFlag & PublishHeaderFlag::QoSLsb || package.HeaderFlag & PublishHeaderFlag::QoSMsb)
				{
					package.VariableHeader.PacketIdentifier = ConverterUtility::ByteToShort(*data, data[1]);
					data += 2;
				}

				auto totalLength = (buffer.data() + remainLength + 2);
				auto payloadLength = (buffer.data() + remainLength + 2) - data;

				package.Payload = std::string(data, data + payloadLength);
				data += payloadLength;

				return package;
			}

			const std::vector<unsigned char> PublishConverter::ToBuffer(const PublishPackage& to)
			{
				std::vector<unsigned char> publishBuffer;
				std::vector<unsigned char> variableHeader = ConverterUtility::IntToBytes(to.VariableHeader.TopicName.length());

				publishBuffer.push_back((to.Header.PackageType << 4) | to.HeaderFlag);
				publishBuffer.push_back(variableHeader[0]);
				publishBuffer.push_back(variableHeader[1]);

				for (auto& i : to.VariableHeader.TopicName) publishBuffer.push_back(i);
				if (to.VariableHeader.PacketIdentifier != 0)
				{
					std::vector<unsigned char> packetIdentifier = ConverterUtility::IntToBytes(to.VariableHeader.PacketIdentifier);
					publishBuffer.push_back(packetIdentifier[0]);
					publishBuffer.push_back(packetIdentifier[1]);
				}

				for (auto& payload : to.Payload)
				{
					publishBuffer.push_back(payload);
				}

				unsigned char remainLength = publishBuffer.size() - 1;

				publishBuffer.insert(publishBuffer.begin() + 1, remainLength);

				return publishBuffer;
			}
		}
	}
}
