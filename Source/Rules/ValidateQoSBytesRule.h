#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Rules {
		class ValidateQoSBytesRule : public IRule
		{
		public:
			ValidateQoSBytesRule(const MqttPackages::PublishPackage& package) : m_Package(package) {};
			~ValidateQoSBytesRule() {};

			/*
			* Checks whether the both Qos flags are set for publish package.
			* Returns: true if not, false if it is.
			*/
			virtual bool Validate() override {
				if (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSLsb && m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSMsb)
					return false;

				return true;
			};

		private:
			const MqttPackages::PublishPackage& m_Package;
		};
	}
}
