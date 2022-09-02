#pragma once
#include <gtest/gtest.h>
#include <Protocol/Validators/PublishValidator.h>
using namespace MQTT::Protocol;
using namespace MQTT::Protocol::Validators;
using namespace MQTT::Server;

#pragma region Utility
static PublishPackage pGenerate() {
	auto p = PublishPackage();

	p.Header.PackageType = ControlPackageType::Publish;
	p.HeaderFlag = PublishHeaderFlag::Retain;

	p.VariableHeader.TopicName = "a";

	p.Payload = "a";

	return p;
}
#pragma endregion


TEST(PublishValidatorShould, SetDupToZero_WhenQoSEqualsZero)
{
	// Arrange
	PublishHeaderFlag actual;
	auto expected = PublishHeaderFlag::Retain;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto data = pGenerate();
	auto manager = SubscribeManager();
	data.HeaderFlag |= PublishHeaderFlag::Dup;

	// Act
	converter.ValidatePackage(data, client, manager);
	actual = data.HeaderFlag;

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishValidatorShould, ReturnDisconnect_WhenBothQoSBitsAreSet)
{
	// Arrange
	PublishValidator::Action actual;
	auto expected = PublishValidator::DisconnectClient;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto data = pGenerate();
	auto manager = SubscribeManager();
	data.HeaderFlag |= PublishHeaderFlag::QoSLsb | PublishHeaderFlag::QoSMsb;

	// Act
	actual = converter.ValidatePackage(data, client, manager);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishValidatorShould, StoreQoSMessage_WhenQoSMsbBitIsSet)
{
	// Arrange
	PublishHeaderFlag actual;
	auto expected = PublishHeaderFlag::QoSMsb;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto data = pGenerate();
	auto manager = SubscribeManager();
	manager.AddParentTree(BTree::NewBTree({'a'}, NoWildcard));
	data.HeaderFlag |= PublishHeaderFlag::QoSMsb;
	data.Payload = "message";

	// Act
	converter.ValidatePackage(data, client, manager);
	actual = (PublishHeaderFlag)manager.GetMatchingBTree({ 'a' })->GetSavedMessages()[0].QoS;

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishValidatorShould, RemoveQoSMessage_WhenQoSMsbBitIsSet)
{
	// Arrange
	int actual;
	auto expected = 1;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto data = pGenerate();
	auto manager = SubscribeManager();
	manager.AddParentTree(BTree::NewBTree({ 'a' }, NoWildcard));
	data.HeaderFlag |= PublishHeaderFlag::Retain;
	data.Payload = "message";

	// Act
	manager.GetMatchingBTree({ 'a' })->AddSavedMessage(SubscribeSavedMessage(data.Payload, data.HeaderFlag));
	converter.ValidatePackage(data, client, manager);
	actual = manager.GetMatchingBTree({ 'a' })->GetSavedMessages().size();

	// Assert
	EXPECT_TRUE(expected == actual);
}


TEST(PublishValidatorShould, ReturnRejectPublish_WhenTopicNameContainsWildcard)
{
	// Arrange
	PublishValidator::Action actual;
	auto expected = PublishValidator::RejectPublish;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto manager = SubscribeManager();
	auto data = pGenerate();
	data.VariableHeader.TopicName = "test#";

	// Act
	actual = converter.ValidatePackage(data, client, manager);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishValidatorShould, ReturnRejected_WhenTopicNameIsEmpty)
{
	// Arrange
	PublishValidator::Action actual;
	auto expected = PublishValidator::RejectPublish;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto manager = SubscribeManager();
	auto data = pGenerate();
	data.VariableHeader.TopicName = "";

	// Act
	actual = converter.ValidatePackage(data, client, manager);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(PublishValidatorShould, ReturnAcknowledged_WhenPackageIsDefinedAsSpecified)
{
	// Arrange
	PublishValidator::Action actual;
	auto expected = PublishValidator::AcknowledgePublish;

	auto converter = PublishValidator();
	auto client = MqttClient();
	auto manager = SubscribeManager();
	auto data = pGenerate();
	data.VariableHeader.TopicName = "test";

	// Act
	actual = converter.ValidatePackage(data, client, manager);

	// Assert
	EXPECT_TRUE(expected == actual);
}

