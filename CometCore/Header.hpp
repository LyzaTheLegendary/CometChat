#pragma once
#include <stdint.h>

//#pragma pack(push, 1)
struct Header {
public:
	const uint16_t GetSize()	{ return m_size; }
	const uint16_t GetId()	{ return m_id; }

	Header(const uint16_t size, const uint16_t id) {
		m_size = size;
		m_id = id;
	}

private:
	uint16_t m_size;
	uint16_t m_id;
};
//#pragma pack(pop)