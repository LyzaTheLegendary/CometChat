#include "MemoryStream.hpp"

uint64_t MemoryStream::Length()
{
	return m_buffer.size();
}

uint64_t MemoryStream::GetPosition()
{
	return m_position;
}

void MemoryStream::Seek(uint32_t offset, uint8_t seekOrigin)
{
	switch (seekOrigin) {
	case Stream::SEEK_ORIGIN_BEGIN:		m_position =  offset; break;
	case Stream::SEEK_ORIGIN_CURRENT:	m_position += offset; break;
	case Stream::SEEK_ORIGIN_END:		m_position -= offset; break;
	}
}

uint64_t MemoryStream::Write(std::vector<uint8_t> buffer)
{
	uint64_t currentSize = m_buffer.size();
	m_buffer.resize(currentSize + buffer.size());

	memcpy(m_buffer.data() + currentSize, buffer.data(), buffer.size());
	return buffer.size();
}

uint64_t MemoryStream::Write(uint8_t* buff, uint64_t size)
{
	size_t currentSize = m_buffer.size();
	m_buffer.resize(currentSize + size);

	memcpy(m_buffer.data() + currentSize, buff, size);
	return size;
}

std::vector<uint8_t> MemoryStream::Read(uint64_t size)
{
	assert(m_position + size > m_buffer.size());
	std::vector<uint8_t> buffer(size);

	memcpy(buffer.data(), m_buffer.data() + m_position, size);
	m_position += size;

	return buffer;
}
