#pragma once
#include <vector>
#include "Header.hpp"
class Message {
public:
	Message(uint16_t id) {
		m_id = id;
	}
	Message(uint16_t id, std::vector<uint8_t> buff) {
		m_id = id;
		m_buffer = buff;
	}
	std::vector<uint8_t> GetBytes();
	uint16_t GetSize();

	template<typename T>
	Message& operator << (T& data ) {
		static_assert(std::is_standard_layout<T>::value, "Structure is too complex to parse.");

		uint16_t size = m_buffer.size();
		m_buffer.resize(m_buffer.size() + sizeof(T));

		memcpy(m_buffer.data() + size, &data, sizeof(T));

		return *this;
	}

	template<typename T>
	T operator >> (T& data) {
		static_assert(std::is_standard_layout<T>::value, "Structure is too complex to parse.");

		uint16_t size = m_buffer.size() - sizeof(T);
		memcpy(data, m_buffer.data() + size, sizeof(T));

		m_buffer.resize(size);

		return this;
	}

private:
	uint16_t m_id;
	std::vector<uint8_t> m_buffer;

	Header GetHeader();
};