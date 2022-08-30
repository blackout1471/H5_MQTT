#pragma once
#include "ClientUtility.h"
#include <uuid/uuid.h>
namespace MQTT {
	namespace Server {
		std::string ClientUtility::GenerateUniqueId()
		{
			uuid_t uuid;
			uuid_generate_random(uuid);
			char s[37];
			uuid_unparse(uuid, s);
			return s;
		}
	}
}