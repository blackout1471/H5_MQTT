#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishStoreNonQoSMessagesRule : public IRule
			{
			public:
				PublishStoreNonQoSMessagesRule(const PublishPackage& package) : m_Package(package) {};
				~PublishStoreNonQoSMessagesRule() {};

				/*
				* Checks whether both retain and qos = 0, and discards the previous retain message if they are.
				* After discarding the previous messages, the retain message will be the one in the package.
				* Returns: true if retain and qos = 0, false else.
				*/
				virtual bool Validate() override {
					
					if (m_Package.HeaderFlag & PublishHeaderFlag::Retain &&
						(m_Package.HeaderFlag & PublishHeaderFlag::QoSLsb) + (m_Package.HeaderFlag & PublishHeaderFlag::QoSMsb) == 0)
					{
						// Todo:: Discard last retain messages
						// Todo:: Store package message as new.

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
