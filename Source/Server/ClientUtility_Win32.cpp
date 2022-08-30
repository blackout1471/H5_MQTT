#pragma once
#include "ClientUtility.h"
#include <Rpc.h>
namespace MQTT {
	namespace Server {
		std::string ClientUtility::GenerateUniqueId() 
		{
			GUID identifier;
			CoCreateGuid(&identifier);

			char guid_cstr[39];
			snprintf(guid_cstr, sizeof(guid_cstr),
				"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
				identifier.Data1, identifier.Data2, identifier.Data3,
				identifier.Data4[0], identifier.Data4[1], identifier.Data4[2], identifier.Data4[3],
				identifier.Data4[4], identifier.Data4[5], identifier.Data4[6], identifier.Data4[7]);

			return std::string(guid_cstr);
		}
	}
}