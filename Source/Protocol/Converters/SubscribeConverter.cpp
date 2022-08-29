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

				int topicsCount = 1;

				/*	for (int i = 0; i < (packageSize - sp.TopicLength) - 1; i++)
					{
						if (buffer[i + offset] == '/')
							topicsCount++;
					}*/

				SubscribeTopic st;
				for (int i = 0; i < sp.TopicLength - 2; i++)
				{
					st.Topic.push_back(buffer[i + offset]);
				}
				st.Wildcard = (SubscribeTopicWildcardType)buffer[sp.TopicLength + offset - 1];
				
				st.QoS = buffer[sp.TopicLength + offset];

				sp.Topics.push_back(st);

				return SubscribePackage();
			}

			const std::vector<unsigned char> SubscribeConverter::ToBuffer(const SubscribePackage& to)
			{
				return std::vector<unsigned char>();
			}

		}
	}
}