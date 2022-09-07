#pragma once

namespace MQTT {
	namespace Rules {
		class IRule {
		public:
			/*
			* The validation of the rule system.
			* Is called to validate the given rule.
			*/
			virtual bool Validate() = 0;
		};
	}
}