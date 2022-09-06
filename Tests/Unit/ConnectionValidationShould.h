#pragma once
#include <gtest/gtest.h>
#include <Protocol/Validators/ConnectValidator.h>

using namespace MQTT::Protocol::Validators;
using namespace MQTT::Protocol;
using namespace MQTT::Server;

#pragma region Utility
static ConnectPackage GeneratePackage()
{
	auto p = ConnectPackage();
	p.Header.PackageType = ControlPackageType::Connect;

	p.VariableHeader.KeepAlive = 60;
	p.VariableHeader.Level = 4;
	p.VariableHeader.ProtocolName = "MQTT";
	p.VariableHeader.VariableLevel = ConnectFlagType(ConnectFlagType::Username | ConnectFlagType::Password);

	p.Payload.ClientId = "test";
	p.Payload.Password = "test";
	p.Payload.Username = "test";

	return p;
}
#pragma endregion



TEST(ConnectionValidationShould, ReturnDisconnect_WhenClientIsAlreadyConnected)
{
	// Arrange
	auto expected = ConnectValidator::Disconnect;
	auto package = GeneratePackage();
	auto clientState = new MqttClient{ "", true, package.Payload.ClientId, "", ConnectFlagType::Username};
	auto clientStates = std::vector<MqttClient*>{
		clientState
	};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ReturnDisconnect_WhenProtocolNameIsIncorrect)
{
	// Arrange
	auto expected = ConnectValidator::Disconnect;
	auto package = GeneratePackage();
	package.VariableHeader.ProtocolName = "mttq";
	auto clientState = new MqttClient{ "", true, package.Payload.ClientId, "", ConnectFlagType::Username};
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ReturnReject_WhenProtocolLevelIsIncorrect)
{
	// Arrange
	auto expected = ConnectValidator::RejectProtocolLevel;
	auto package = GeneratePackage();
	package.VariableHeader.Level = 2;
	auto clientState = new MqttClient{ "", true, package.Payload.ClientId, "", ConnectFlagType::Username };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ReturnDisconnect_WhenReservedBitIsSetInControlPackage)
{
	// Arrange
	auto expected = ConnectValidator::Disconnect;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel |= ConnectFlagType::Reserved;
	auto clientState = new MqttClient{ "", true, package.Payload.ClientId, "", ConnectFlagType::Username };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ReturnDisconnect_WhenUsernameIsNotSetAndPasswordIs)
{
	// Arrange
	auto expected = ConnectValidator::Disconnect;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel = ConnectFlagType::Password;
	auto clientState = new MqttClient{ "", true, package.Payload.ClientId, "", ConnectFlagType::Username };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ContainClientWillMessage_WhenWillFlagIsSet)
{
	// Arrange
	auto expected = "message";
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel = ConnectFlagType::Will_Flag;
	package.Payload.WillMessage = expected;
	auto clientState = new MqttClient{ "", true, package.Payload.ClientId, "", ConnectFlagType::Username };
	auto clientStates = std::vector<MqttClient*>();
	auto validator = ConnectValidator();

	// Act
	validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == clientState->WillMessage);
}

TEST(ConnectionValidationShould, ReturnDisconnect_WhenWillFlagIsSetAndQoSLevelIs3)
{
	// Arrange
	auto expected = ConnectValidator::Disconnect;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel = ConnectFlagType::Will_Flag | ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_QoS_MSB;
	auto clientState = new MqttClient{ "", false, package.Payload.ClientId, "", ConnectFlagType::Username };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ContainRetainZeroAndWillRetainZero_WhenWillFlagIsZero)
{
	// Arrange
	auto expected = 0;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel = ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_Remain;
	auto clientState = new MqttClient{ "", false, package.Payload.ClientId, "", ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_Remain };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == clientState->ConnectionFlags);
}

TEST(ConnectionValidationShould, ReturnReject_WhenCleanSessionIsZeroAndClientIdIsNotSet)
{
	// Arrange
	auto expected = ConnectValidator::RejectUserIdentifier;
	auto package = GeneratePackage();
	package.Payload.ClientId = "";
	package.VariableHeader.VariableLevel = ConnectFlagType::Username | ConnectFlagType::Password;
	auto clientState = new MqttClient{ "", false, "test", "", ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_Remain };
	auto clientStates = std::vector<MqttClient*>{clientState};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, ReturnContinueSession_WhenClientHasSetCleanSessionZeroAndExists)
{
	// Arrange
	auto expected = ConnectValidator::ContinueState;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel = ConnectFlagType::Username | ConnectFlagType::Password;
	auto clientState = new MqttClient{ "", false, package.Payload.ClientId, "", ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_Remain };
	auto clientStates = std::vector<MqttClient*>{ clientState };
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, GenerateId_WhenClientIdIsNotSet)
{
	// Arrange
	auto package = GeneratePackage();
	package.Payload.ClientId = "";
	package.VariableHeader.VariableLevel |= ConnectFlagType::Clean_Session;
	auto clientState = new MqttClient{ "", false, package.Payload.ClientId, "", ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_Remain };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto state = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(clientState->ClientId != "");
}

TEST(ConnectionValidationShould, ReturnNewSession_WhenPackageIsFulfilling)
{
	// Arrange
	auto expected = ConnectValidator::CreateNewState;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel |= ConnectFlagType::Clean_Session;
	auto clientState = new MqttClient{ "", false, package.Payload.ClientId, "", ConnectFlagType::Will_QoS_LSB | ConnectFlagType::Will_Remain };
	auto clientStates = std::vector<MqttClient*>{};
	auto validator = ConnectValidator();

	// Act
	auto actual = validator.ValidateClient(package, clientStates, clientState);

	// Assert
	EXPECT_TRUE(expected == actual);
}

TEST(ConnectionValidationShould, DeleteOldClientState_WhenCleanSessionIsOneAndOldExists)
{
	// Arrange
	auto expected = 0;
	int actual = -1;
	auto package = GeneratePackage();
	package.VariableHeader.VariableLevel |= ConnectFlagType::Clean_Session;
	auto clientState = new MqttClient{ "", false, package.Payload.ClientId, "" };
	auto clientStates = std::vector<MqttClient*>{
		clientState
	};
	auto validator = ConnectValidator();

	// Act
	validator.ValidateClient(package, clientStates, clientState);
	actual = clientStates.size();

	// Assert
	EXPECT_EQ(expected, actual);
}