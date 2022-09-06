#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Rules {
		class ConnectReservedFlagSetRule : public IRule
		{
		public:
			ConnectReservedFlagSetRule(MqttPackages::ConnectFlagType flag) : m_Flag(flag) {};
			~ConnectReservedFlagSetRule() {};

			/*
			* Check whether the reserved flag is set.
			* returns true if set, false if not.
			*/
			inline virtual bool Validate() override {

				return m_Flag & MqttPackages::ConnectFlagType::Reserved;
			}

		private:
			MqttPackages::ConnectFlagType m_Flag;
		};
	}
}