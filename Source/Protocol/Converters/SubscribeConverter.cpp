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
				sp.TopicLength = ConverterUtility::ByteToInt(buffer[4], buffer[5]);

				int offset = 6;

				bool readAllTopics = false;
				while (!readAllTopics) {

					SubscribeTopic st;
					if (offset != 6) {
						sp.TopicLength = ConverterUtility::ByteToInt(buffer[offset], buffer[offset + 1]);
						offset += 2;
					}

					for (int i = 0; i < sp.TopicLength; i++)
					{
						st.Topic.push_back(buffer[i + offset]);

						if (buffer[i + offset] == '#') // check for other wild cards
						{
							// There is a wild card
							st.Wildcard = (SubscribeTopicWildcardType)buffer[i + offset];
							st.HaveChild = true;
						}
						else if (buffer[i + offset] == '/')
						{
							st.HaveChild = true;
						}
					}
					st.QoS = buffer[sp.TopicLength + offset];
					sp.Topics.push_back(st);

					if ((packageSize - (sp.TopicLength + offset)) <= 2) {
						// there no more packages
						readAllTopics = true;
					}
					else {
						offset += sp.TopicLength + 1;
					}
				}

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