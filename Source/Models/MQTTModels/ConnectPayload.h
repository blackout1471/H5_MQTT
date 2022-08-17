#pragma once
#include <cstdint>
#include <vector>

namespace MQTTModels
{
	struct ConnectPayload
	{		
		int16_t ClientIDLenght;
		std::vector<unsigned char> ClientID;

		int16_t UsernameLenght;
		std::vector<unsigned char> Username;
		
		int16_t PasswordLength;
		std::vector<unsigned char> Password;		
	};
}