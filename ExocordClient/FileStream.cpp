#include "FileStream.hpp"

int64_t FileStream::GetSize()
{
    return m_fileSize;
}

int64_t FileStream::GetPos()
{
    return ftell(m_fp);
}

void FileStream::Seek(int64_t offset, uint8_t seekOrigin)
{
    int errorcode = 0;
    switch (seekOrigin) {
    case Stream::SEEK_ORIGIN_BEGIN:
        errorcode = fseek(m_fp, offset, SEEK_SET);
        break;
    case Stream::SEEK_ORIGIN_CURRENT:
        errorcode = fseek(m_fp, offset, SEEK_CUR);
        break;
    case Stream::SEEK_ORIGIN_END:
        errorcode = fseek(m_fp, offset, SEEK_END);
        break;
    }

    if (errorcode != 0) {
        throw std::exception(std::system_category().message(errorcode).c_str());
    }
}

uint64_t FileStream::Write(std::vector<uint8_t> buffer) noexcept
{
    return fwrite(reinterpret_cast<void*>(buffer.data()), sizeof uint8_t, buffer.size(), m_fp);;
}

uint64_t FileStream::Write(uint8_t* buff, uint64_t size) noexcept
{
    return fwrite(reinterpret_cast<void*>(buff), sizeof uint8_t, size, m_fp);
}

std::vector<uint8_t> FileStream::Read(uint64_t size)
{
    std::vector<uint8_t> buff(size);
    size_t result = fread(buff.data(), sizeof(uint8_t), size, m_fp);

    if (result != size) 
        throw std::runtime_error("Failed to read data from file");
    
    return buff;
}

std::string FileStream::ReadString()
{
    uint16_t strLen = Read<uint16_t>();

    std::string str;
    str.reserve(strLen);

    std::vector<uint8_t> strBuff = Read(strLen);
    memcpy(str.data(), strBuff.data(), strLen);
    return str;
}
