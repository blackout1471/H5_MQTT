#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"
#include <vector>
namespace MQTT {
	namespace Rules {
		class SubscribeWildcardRule : public IRule {
		public:
			SubscribeWildcardRule(std::vector<MqttPackages::SubscribeTopicWildcardType> notSupportedWildcards, std::vector<MqttPackages::SubscribeTopic> topics) : m_NotSupportedWildcards(notSupportedWildcards), m_Topics(topics) {};

			/*
			* Checks whether the payload topics have a wildcard that is not supported
			* Return: true if payload doesnt have not supported wildcard
			* Return: false if payload have a not supported wildcard
			*/
			inline virtual bool Validate() override
			{
				for (auto topic : m_Topics)
				{
					for (auto noSupportWildcard : m_NotSupportedWildcards)
					{
						if (topic.Wildcard == noSupportWildcard)
							return false;
					}

				}
				return true;
			}

		private:
			std::vector<MqttPackages::SubscribeTopicWildcardType> m_NotSupportedWildcards;
			std::vector<MqttPackages::SubscribeTopic> m_Topics;

		};
	}
}