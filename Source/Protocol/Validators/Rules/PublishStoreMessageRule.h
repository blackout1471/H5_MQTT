#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"
#include <Protocol/Managers/SubscribeManager.h>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishStoreMessageRule : public IRule
			{
			public:
				PublishStoreMessageRule(const PublishPackage& package, SubscribeManager& manager) : m_Package(package), m_Manager(manager) {};
				~PublishStoreMessageRule() {};

				/*
				* Checks whether the retain flag is set, if it is. Store the message.
				* Returns: True if they are stored, false if not.
				*/
				virtual bool Validate() override {
					if (m_Package.HeaderFlag & PublishHeaderFlag::Retain)
					{
						// Todo :: Store messages
						// Todo :: Store Qos in messages
						return true;
					}

					return false;
				}

			private:
				const PublishPackage& m_Package;
				SubscribeManager& m_Manager;
			};
		}
	}
}
