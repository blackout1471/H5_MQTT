#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class ConnectReservedFlagSetRule : public IRule
			{
			public:
				ConnectReservedFlagSetRule(Protocol::ConnectFlagType flag) : m_Flag(flag) {};
				~ConnectReservedFlagSetRule() {};

				/*
				* Check whether the reserved flag is set.
				* returns true if set, false if not.
				*/
				inline virtual bool Validate() override{

					return m_Flag & Protocol::ConnectFlagType::Reserved;
				}

			private:
				Protocol::ConnectFlagType m_Flag;
			};
		}
	}
}