#include "mqttpch.h"
#include "PublishValidator.h"
#include "Rules/Rules.h"
#include <Protocol/Validators/RuleEngine.h>

namespace MQTT {
	namespace Protocol {
		namespace Validators {
			PublishValidator::PublishValidator() {};
			PublishValidator::~PublishValidator() {};

			PublishValidator::Action PublishValidator::ValidatePackage(MqttPackages::PublishPackage& package, const Server::MqttClient& client, SubscribeManager& subscribeManager)
			{
				// First check whether the current client has broken any rules which should make the server disconnect the client.
				auto shouldDisconnect = !RuleEngine({
					{new ValidateQoSBytesRule(package), true},
					{new IsStringEmptyRule(package.VariableHeader.TopicName), false}
					}).Run();
				if (shouldDisconnect)
					return DisconnectClient;

				auto ruleHandler = std::vector<IRule*>{
					new PublishQosRule(package),
					new PublishStoreMessageRule(package, subscribeManager),
					new PublishStoreNonQoSMessagesRule(package, subscribeManager)
				};
				for (auto* rule : ruleHandler)
				{
					rule->Validate();
					delete rule;
				}

				auto shouldRejectPublish = RuleEngine({
					{new PublishValidTopicNameRule(package), true}
				}).Run();
				if (!shouldRejectPublish)
					return RejectPublish;

				return AcknowledgePublish;
			}
		}
	}
}