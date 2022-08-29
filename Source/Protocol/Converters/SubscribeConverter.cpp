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

				for (int i = 0; i < sp.TopicLength; i++)
				{
					SubscribeTopic st;
					//st.Topic
				}

				return SubscribePackage();
			}

			const std::vector<unsigned char> SubscribeConverter::ToBuffer(const SubscribePackage& to)
			{
				return std::vector<unsigned char>();
			}

		}
	}
}