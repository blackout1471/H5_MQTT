#pragma once

#include "Protocol/MqttPackages/Packages.h"
#include "Server/MqttClient.h"
#include "../Managers/SubscribeManager.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishValidator
			{
			public:
				enum Action {
					RejectPublish,
					AcknowledgePublish,
					DisconnectClient
				};
			public:
				PublishValidator();
				~PublishValidator();

				/*
				* Validates the given package for the specific rules in 3.1.1.
				* Returns: The action which should be done.
				*/
				Action ValidatePackage(PublishPackage& package, const Server::MqttClient& client, SubscribeManager& subscribeManager);
			};
		}
	}
}

