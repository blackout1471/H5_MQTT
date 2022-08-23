#include "mqttpch.h"
#include "Converter.h"
#include "Protocol/ConnectPackage.h"

namespace MQTT {
	namespace Protocol {
		namespace Converter {
			Converter::Converter() {}

			Converter::~Converter() {}

			ControlPackageType Converter::GetPackageType(const unsigned char data)
			{
				return (ControlPackageType)((data >> 4) & 0xff);
			}

			int16_t Converter::ByteToInt(unsigned char byte1, unsigned char byte2)
			{
				return (byte1 << 8) | byte2;
			}

			int Converter::IntermiadateConvert(const unsigned char* dataPtr, const ConnectVariableHeader& headerPackage, ConnectPayload& package)
			{
				const unsigned char* data = dataPtr;

				package.ClientIdLength = ByteToInt(*data, data[1]);
				data += 2;

				package.ClientId.assign(data, data + package.ClientIdLength);
				data += package.ClientIdLength;


				if (headerPackage.VariableLevel & ConnectFlagType::Will_Flag)
				{
					int willLength = ByteToInt(*data, data[1]);
					data += 2;

					auto willTopic = std::string();
					willTopic.assign(data, data + willLength);
					data += willLength;

					auto willMessageLength = ByteToInt(*data, data[1]);
					data += 2;

					auto willMessage = std::string();
					willMessage.assign(data, data + willMessageLength);
					data += willMessageLength;
				}
				
				if (headerPackage.VariableLevel & ConnectFlagType::Username)
				{
					package.UsernameLength = ByteToInt(*data, data[1]);
					data += 2;

					package.Username.assign(data, data + package.UsernameLength);
					data += package.UsernameLength;
				}


				if (headerPackage.VariableLevel & ConnectFlagType::Password)
				{
					package.PasswordLength = ByteToInt(*data, data[1]);
					data += 2;

					package.Password.assign(data, data + package.PasswordLength);
					data += package.PasswordLength;
				}

				return data - dataPtr;
			}

			int Converter::IntermiadateConvert(const unsigned char* dataPtr, ConnectVariableHeader& package)
			{
				const unsigned char* data = dataPtr;

				package.NameLength = ByteToInt(*data, data[1]);
				data += 2;

				package.ProtocolName.assign(data, data + package.NameLength);
				data += package.NameLength;

				package.Level = *data;
				data++;

				package.VariableLevel = ConnectFlagType(*data);
				data++;

				package.KeepAlive = ByteToInt(*data, data[1]);
				data += 2;

				return data - dataPtr;
			}

			template<>
			bool Converter::Convert<std::vector<unsigned char>, ConnectPackage>(const std::vector<unsigned char>& from, ConnectPackage& to)
			{
				if (from.size() < 3)
					return false;

				const unsigned char* data = from.data();

				to.ControlHeader.PackageType = GetPackageType(*data);
				data++;

				unsigned char remainLength = *data;
				data++;

				data += IntermiadateConvert(data, to.ConnectVariableHeader);

				bool hasPayload = (from.data() + remainLength + 2) != data;
				if (!hasPayload)
					return true;

				int payloadSize = IntermiadateConvert(data, to.ConnectVariableHeader, to.ConnectPayload);

				return true;
			}

			template<>
			bool Converter::Convert<std::vector<unsigned char>, ConnectPackage>(const std::vector<unsigned char>&, ConnectPackage&);
		}
	}
};