#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"
#include "Protocol/Managers/SubscribeManager.h"

namespace MQTT {
	namespace Rules {
		class PublishStoreNonQoSMessagesRule : public IRule
		{
		public:
			PublishStoreNonQoSMessagesRule(const MqttPackages::PublishPackage& package, Protocol::SubscribeManager& manager) : m_Package(package), m_Manager(manager) {};
			~PublishStoreNonQoSMessagesRule() {};

			/*
			* Checks whether both retain and qos = 0, and discards the previous retain message if they are.
			* After discarding the previous messages, the retain message will be the one in the package.
			* Returns: true if retain and qos = 0, false else.
			*/
			virtual bool Validate() override {

				if (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::Retain &&
					(m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSLsb) + (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSMsb) == 0)
				{
					std::vector<unsigned char> topic(m_Package.VariableHeader.TopicName.begin(), m_Package.VariableHeader.TopicName.end());

					auto matchinTree = m_Manager.GetMatchingBTree(topic);


					if (matchinTree != nullptr)
					{
						matchinTree->GetSavedMessages().clear();

						auto qos = (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSLsb) + (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSMsb);
						matchinTree->AddSavedMessage(MqttPackages::SubscribeSavedMessage(m_Package.Payload, qos));
					}

					return true;
				}

				return false;
			}

		private:
			const MqttPackages::PublishPackage& m_Package;
			Protocol::SubscribeManager& m_Manager;
		};
	}
}
