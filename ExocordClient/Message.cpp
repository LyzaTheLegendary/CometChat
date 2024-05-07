#include "Message.hpp"

std::vector<uint8_t> Message::GetBytes()
{
	std::vector<uint8_t> buff(sizeof(Header) + m_buffer.size());
	Header header = GetHeader();
	memcpy(buff.data(), reinterpret_cast<void*>(&header), sizeof(Header));
	memcpy(buff.data() + sizeof(Header), m_buffer.data(), m_buffer.size());

	return buff;
}

uint16_t Message::GetSize()
{
	return m_buffer.size();
}

Header Message::GetHeader()
{
	return Header(m_buffer.size(), m_id);
}
