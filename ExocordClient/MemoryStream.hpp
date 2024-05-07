#pragma once
#include"StreamInterface.hpp"
#include <assert.h>



class MemoryStream : public StreamInterface {
public:
	MemoryStream(std::vector<uint8_t> buff) {
		m_buffer = buff;
		m_position = 0;
	}

	MemoryStream(uint8_t* buff, size_t size) {
		m_buffer = std::vector<uint8_t>(size);
		m_position = 0;
		memcpy(m_buffer.data(), buff, size);
	}

	uint64_t Length();
	uint64_t GetPosition();

	// NOTE: When the seek Origin is END, It will go backwards instead.
	void Seek(uint32_t offset, uint8_t seekOrigin);

	uint64_t Write(std::vector<uint8_t> buffer);
	uint64_t Write(uint8_t *buff, uint64_t size);

	std::vector<uint8_t> Read(uint64_t size);

private:
	std::vector<uint8_t> m_buffer;
	uint64_t m_position;
};