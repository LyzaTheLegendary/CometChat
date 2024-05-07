#pragma once
#include <stdint.h>

#pragma pack(1)
struct Header {
public:
	uint16_t GetSize()	{ return m_size; }
	uint16_t GetId()	{ return m_id; }

	Header(const uint16_t size, const uint16_t id) {
		m_size = size;
		m_id = id;
	}

private:
	uint16_t m_size;
	uint16_t m_id;
};