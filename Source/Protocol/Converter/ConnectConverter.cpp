#include "mqttpch.h"
#include "ConnectConverter.h"
#include "ConverterUtility.h"

#include <stdexcept>

namespace MQTT {
    namespace Protocol {
        namespace Converter {

            ConnectConverter::ConnectConverter() {}

            ConnectConverter::~ConnectConverter() {}

            const ConnectPackage ConnectConverter::ToPackage(const std::vector<unsigned char>& buffer)
            {
                auto package = ConnectPackage();

                if (buffer.size() < 3)
                    return package;

                const unsigned char* data = buffer.data();

                package.ControlHeader.PackageType = ConverterUtility::GetPackageType(*data);
                data++;

                unsigned char remainLength = *data;
                data++;

                data += ConvertToVariableHeader(data, package.ConnectVariableHeader);

                bool hasPayload = (buffer.data() + remainLength + 2) != data;
                if (!hasPayload)
                    return package;

                int payloadSize = ConvertToPayload(data, package.ConnectVariableHeader, package.ConnectPayload);

                return package;
            }

            const std::vector<unsigned char> ConnectConverter::ToBuffer(const ConnectPackage& to)
            {
                throw std::logic_error("Not implemented yet");
            }

            int ConnectConverter::ConvertToVariableHeader(const unsigned char* dataPtr, ConnectVariableHeader& package)
            {
                const unsigned char* data = dataPtr;

                package.NameLength = ConverterUtility::ByteToInt(*data, data[1]);
                data += 2;

                package.ProtocolName.assign(data, data + package.NameLength);
                data += package.NameLength;

                package.Level = *data;
                data++;

                package.VariableLevel = ConnectFlagType(*data);
                data++;

                package.KeepAlive = ConverterUtility::ByteToInt(*data, data[1]);
                data += 2;

                return data - dataPtr;
            }

            int ConnectConverter::ConvertToPayload(const unsigned char* dataPtr, const ConnectVariableHeader& headerPackage, ConnectPayload& package)
            {
                const unsigned char* data = dataPtr;

                package.ClientIdLength = ConverterUtility::ByteToInt(*data, data[1]);
                data += 2;

                package.ClientId.assign(data, data + package.ClientIdLength);
                data += package.ClientIdLength;


                if (headerPackage.VariableLevel & ConnectFlagType::Will_Flag)
                {
                    int willLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    auto willTopic = std::string();
                    willTopic.assign(data, data + willLength);
                    data += willLength;

                    auto willMessageLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    auto willMessage = std::string();
                    willMessage.assign(data, data + willMessageLength);
                    data += willMessageLength;
                }

                if (headerPackage.VariableLevel & ConnectFlagType::Username)
                {
                    package.UsernameLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    package.Username.assign(data, data + package.UsernameLength);
                    data += package.UsernameLength;
                }


                if (headerPackage.VariableLevel & ConnectFlagType::Password)
                {
                    package.PasswordLength = ConverterUtility::ByteToInt(*data, data[1]);
                    data += 2;

                    package.Password.assign(data, data + package.PasswordLength);
                    data += package.PasswordLength;
                }

                return data - dataPtr;
            }
        }
    }
}
