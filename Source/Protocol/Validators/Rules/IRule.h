#pragma once

namespace MQTT {
	namespace Protocol {
		namespace Validators {
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
}