#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishQosRule : public IRule
			{
			public:
				PublishQosRule(PublishPackage& package) : m_Package(package) {};
				~PublishQosRule() {};

				/*
				* Checks if QoSLsb and QosMsb is set, if they are, then dup must be set to 0
				* Returns: true if they are not set, false if any are.
				*/
				virtual bool Validate() override {
					if ((m_Package.HeaderFlag & PublishHeaderFlag::QoSLsb + m_Package.HeaderFlag & PublishHeaderFlag::QoSMsb) == 0)
					{
						m_Package.HeaderFlag & ~PublishHeaderFlag::Dup;
						return false;
					}

					return true;
				}
			private:
				PublishPackage& m_Package;
			};
		}
	}
}