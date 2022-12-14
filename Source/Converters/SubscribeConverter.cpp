#include "mqttpch.h"
#include "SubscribeConverter.h"
#include "ConverterUtility.h"
namespace MQTT {
	namespace Converters
	{
		const MqttPackages::SubscribePackage SubscribeConverter::ToPackage(const std::vector<unsigned char>& buffer)
		{

			MqttPackages::ControlHeader ch;
			ch.PackageType = ConverterUtility::GetPackageType(buffer[0]);
			int packageSize = buffer[1];

			MqttPackages::SubscribeVariableHeader vh;
			vh.PacketIdentifier = ConverterUtility::ByteToInt(buffer[2], buffer[3]);

			// offset already read bytes
			int offset = 4;

			MqttPackages::SubscribePayload sp = ConvertPayload(buffer, packageSize, offset);

			MqttPackages::SubscribePackage subPackage;
			subPackage.Header = ch;
			subPackage.Payload = sp;
			subPackage.VariableHeader = vh;

			return subPackage;
		}

		const std::vector<unsigned char> SubscribeConverter::ToBuffer(const MqttPackages::SubscribePackage& to)
		{
			return std::vector<unsigned char>();
		}

		const MqttPackages::SubscribePayload SubscribeConverter::ConvertPayload(const std::vector<unsigned char>& buffer, int packageSize, int offset)
		{
			MqttPackages::SubscribePayload sp;
			do
			{
				// Get size of the topic
				int topicLength = ConverterUtility::ByteToInt(buffer[offset], buffer[offset + 1]);

				// offset the length of the topic bytes
				offset += 2;

				MqttPackages::SubscribeTopic st;
				std::vector<unsigned char> currentPath;

				for (int i = 0; i < topicLength; i++)
				{
					if (buffer[i + offset] == '#' || buffer[i + offset] == '+' || buffer[i + offset] == '$')
					{
						st.Wildcard = (MqttPackages::SubscribeTopicWildcardType)buffer[i + offset];
					}
					else if (buffer[i + offset] == '/')
					{
						st.HaveChild = true;
						st.Paths.push_back(currentPath);
						currentPath.clear();
					}
					else
					{
						currentPath.push_back(buffer[i + offset]);
					}
				}

				if (currentPath.size() != 0)
					st.Paths.push_back(currentPath);

				// The last byte of the topic 
				st.QoS = buffer[topicLength + offset];
				sp.Topics.push_back(st);

				offset += topicLength + 1;

				// check if all the topics have been read
			} while ((packageSize - offset) > 0);

			return sp;
		}

	}
}