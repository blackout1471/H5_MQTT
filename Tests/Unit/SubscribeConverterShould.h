#pragma once
#include <gtest/gtest.h>
#include <Protocol/Converters/SubscribeConverter.h>
#include <vector>

using namespace MQTT::Protocol;

static std::vector<unsigned char> GenerateBuffer(const SubscribePackage package) {

	std::vector<unsigned char> buffer;
	int remainLength = 0;

	// TODO: 

	// Control package
	buffer.push_back(package.Header.PackageType << 4);
}

static bool operator==(const std::vector<unsigned char>& buffer1, const std::vector<unsigned char>& buffer2)
{
	if (buffer1.size() != buffer2.size())
		return false;

	for (int i = 0; i < buffer1.size(); i++)
		if (buffer1[i] != buffer2[i])
			return false;

	return true;
}