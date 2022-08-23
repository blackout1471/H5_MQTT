#pragma once
namespace MQTT {
	namespace Protocol {
	
		enum ConnectActReturnCodeType : unsigned char
		{
			Accepted = 0, // Connection accepted
			Refused_Unacceptable_Protocol_Version, // The Server does not support the level of the MQTT protocol requested by the Client
			Refused_Identifier_Rejected, // The Client identifier is correct UTF-8 but not allowed by the Server
			Refused_Server_Unavailable, //	The Network Connection has been made but the MQTT service is unavailable
			Refused_Bad_Username_Or_Password, //The data in the user name or password is malformed
			Refused_Not_Authorized // The Client is not authorized to connect
		};

	}
}