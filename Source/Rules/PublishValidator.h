#pragma once

#include "MqttPackages/Packages.h"
#include "Server/MqttClient.h"
#include "Protocol/Managers/SubscribeManager.h"

namespace MQTT {
	namespace Rules {
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
			Action ValidatePackage(MqttPackages::PublishPackage& package, const Server::MqttClient& client, Protocol::SubscribeManager& subscribeManager);
		};
	}
}

