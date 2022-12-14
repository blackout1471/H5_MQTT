#pragma once
#include "IRule.h"
#include "MqttPackages/Packages.h"
#include "Protocol/Managers/SubscribeManager.h"

namespace MQTT {
	namespace Rules {
		class PublishStoreMessageRule : public IRule
		{
		public:
			PublishStoreMessageRule(const MqttPackages::PublishPackage& package, Protocol::SubscribeManager& manager) : m_Package(package), m_Manager(manager) {};
			~PublishStoreMessageRule() {};

			/*
			* Checks whether the retain flag is set, if it is. Store the message.
			* Returns: True if they are stored, false if not.
			*/
			virtual bool Validate() override {
				if (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::Retain)
				{
					std::vector<unsigned char> topic(m_Package.VariableHeader.TopicName.begin(), m_Package.VariableHeader.TopicName.end());

					auto matchinTree = m_Manager.GetMatchingBTree(topic);


					if (matchinTree != nullptr)
					{
						auto qos = (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSLsb) + (m_Package.HeaderFlag & MqttPackages::PublishHeaderFlag::QoSMsb);
						matchinTree->AddSavedMessage(MqttPackages::SubscribeSavedMessage(m_Package.Payload, qos));
					}

					return true;
				}

				return false;
			};

		private:
			const MqttPackages::PublishPackage& m_Package;
			Protocol::SubscribeManager& m_Manager;
		};
	}
};