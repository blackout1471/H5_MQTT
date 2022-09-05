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
				return std::vector<unsigned char>();
			}
		}
	}
}
