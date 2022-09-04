#pragma once
#include <string>
#include <vector>
#include <functional>

class SocketClient
{
public:
	SocketClient(const std::string& ip, const std::string& port);
	~SocketClient();

	void Connect();
	void Send(const std::vector<unsigned char>& data);
	std::vector<unsigned char> Listen();

	void SetErrorEvent(const std::function<void()>& ev);

private:
	const std::string m_Ip;
	const std::string m_Port;
	int m_Id;

	std::function<void()> m_ErrorEvent;
};