#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class ConnectReservedFlagNotSetRule : public IRule
			{
			public:
				ConnectReservedFlagNotSetRule(Protocol::ConnectFlagType flag) : m_Flag(flag) {};
				~ConnectReservedFlagNotSetRule() {};

				/*
				* Check whether the reserved flag is set.
				* returns true if not set, false if set.
				*/
				inline virtual bool Validate() override{

					return !(m_Flag & Protocol::ConnectFlagType::Reserved);
				}

			private:
				Protocol::ConnectFlagType m_Flag;
			};
		}
	}
}