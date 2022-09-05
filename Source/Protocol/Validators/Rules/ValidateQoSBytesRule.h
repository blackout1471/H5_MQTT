#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class ValidateQoSBytesRule : public IRule
			{
			public:
				ValidateQoSBytesRule(const PublishPackage& package) : m_Package(package) {};
				~ValidateQoSBytesRule() {};

				/*
				* Checks whether the both Qos flags are set for publish package.
				* Returns: true if not, false if it is.
				*/
				virtual bool Validate() override {
					if (m_Package.HeaderFlag & PublishHeaderFlag::QoSLsb && m_Package.HeaderFlag & PublishHeaderFlag::QoSMsb)
						return false;

					return true;
				};

			private:
				const PublishPackage& m_Package;
			};
		}
	}
}
