#pragma once
#include "IRule.h"
#include "Protocol/MqttPackages/Packages.h"

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			class PublishValidTopicNameRule : public IRule
			{
			public:
				PublishValidTopicNameRule(const PublishPackage& package) : m_Package(package) {};
				~PublishValidTopicNameRule() {};

				/*
				* Checks whether the topic name has a wildcard or not
				* Returns: True if it doesn't have wildcard, false otherwise
				*/
				virtual bool Validate() override {

					if (m_Package.VariableHeader.TopicName.find("#") == std::string::npos)
						return true;
					else
						return false;
				}

			private:
				const PublishPackage& m_Package;
			};
		}
	}
}
