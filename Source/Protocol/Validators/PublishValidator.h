#pragma once

#include "Protocol/MqttPackages/Packages.h"
#include "Server/MqttClient.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishValidator
			{
			public:
				enum Action {
					Reject,
					Acknowledge
				};
			public:
				PublishValidator();
				~PublishValidator();

				Action ValidatePackage(const PublishPackage& package, const Server::MqttClient& client);
			};
		}
	}
}

