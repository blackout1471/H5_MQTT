#include "mqttpch.h"
#include "SubscribeConverter.h"
#include "ConverterUtility.h"
namespace MQTT {
	namespace Protocol {
		namespace Converters
		{
			const SubscribePackage SubscribeConverter::ToPackage(const std::vector<unsigned char>& buffer)
			{

				ControlHeader ch;
				ch.PackageType = ConverterUtility::GetPackageType(buffer[0]);
				int packageSize = buffer[1];

				SubscribeVariableHeader vh;
				vh.PacketIdentifier = ConverterUtility::ByteToInt(buffer[2], buffer[3]);

				SubscribePayload sp;

				// offset already read bytes
				int offset = 4;

				do
				{
					// Get size of the topic
					int topicLength = ConverterUtility::ByteToInt(buffer[offset], buffer[offset + 1]);

					// offset the length of the topic bytes
					offset += 2;

					SubscribeTopic st;
					std::vector<unsigned char> currentTopic;

					for (int i = 0; i < topicLength; i++)
					{
						if (buffer[i + offset] == '#') // TODO: check for other wild cards
						{
							st.Wildcard = (SubscribeTopicWildcardType)buffer[i + offset];
							st.HaveChild = true;
						}
						else if (buffer[i + offset] == '/')
						{
							st.HaveChild = true;
							st.Topics.push_back(currentTopic);
							currentTopic.clear();
						}
						else
						{
							currentTopic.push_back(buffer[i + offset]);
						}
					}

					if (currentTopic.size() != 0)
						st.Topics.push_back(currentTopic);

					// The last byte of the topic 
					st.QoS = buffer[topicLength + offset];
					sp.Topics.push_back(st);

					offset += topicLength + 1;

					// check if all the topics have been read
				} while ((packageSize - offset) > 0);

				SubscribePackage subPackage;
				subPackage.ControlHeader = ch;
				subPackage.SubscribePayload = sp;
				subPackage.SubscribeVariableHeader = vh;

				return subPackage;
			}

			const std::vector<unsigned char> SubscribeConverter::ToBuffer(const SubscribePackage& to)
			{
				return std::vector<unsigned char>();
			}

		}
	}
}