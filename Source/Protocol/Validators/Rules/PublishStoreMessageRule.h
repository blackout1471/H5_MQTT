#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishStoreMessageRule : public IRule
			{
			public:
				PublishStoreMessageRule(const PublishPackage& package) : m_Package(package) {};
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
			};
		}
	}
}
