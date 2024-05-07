#pragma once
#include<stdint.h>
#include<vector>

namespace Stream {
	constexpr uint8_t SEEK_ORIGIN_BEGIN = 0;
	constexpr uint8_t SEEK_ORIGIN_CURRENT = 1;
	constexpr uint8_t SEEK_ORIGIN_END = 2;
}

class StreamInterface {
public:
	virtual uint64_t Write(std::vector<uint8_t> buffer) = 0;
	virtual uint64_t Write(uint8_t *buff, size_t size) = 0;
	virtual std::vector<uint8_t> Read(uint64_t size) = 0;
};