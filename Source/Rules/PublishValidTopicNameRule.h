#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"

namespace MQTT {
	namespace Rules {
		class PublishValidTopicNameRule : public IRule
		{
		public:
			PublishValidTopicNameRule(const MqttPackages::PublishPackage& package) : m_Package(package) {};
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
			};

		private:
			const MqttPackages::PublishPackage& m_Package;
		};
	}
}
