#pragma once
#include <string>
namespace MQTT {
	namespace Server {
		class ClientUtility
		{
		public:
			// Generates a unique id
			static std::string GenerateUniqueId();
		private:

			ClientUtility() {};
			~ClientUtility() {};
		};
	}
}