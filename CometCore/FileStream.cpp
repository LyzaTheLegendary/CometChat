#include "FileStream.hpp"

int64_t FileStream::GetSize()
{
    return m_fileSize;
}

int64_t FileStream::GetPos()
{
    return m_fstream.tellp();
}

void FileStream::Seek(int64_t offset, uint8_t seekOrigin)
{
    switch (seekOrigin) {
    case Stream::SEEK_ORIGIN_BEGIN:
        m_fstream.seekp(0, std::ios::beg);
        break;
    case Stream::SEEK_ORIGIN_CURRENT:
        m_fstream.seekp(0, std::ios::cur);
        break;
    case Stream::SEEK_ORIGIN_END:
        m_fstream.seekp(0, std::ios::end);
        break;
    }
}

void FileStream::Close()
{
    m_fstream.flush();
    m_fstream.close();
}

uint64_t FileStream::Write(std::vector<uint8_t> buffer)
{
    m_fstream.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
    return buffer.size();
}

void FileStream::WriteString(std::string str)
{
    uint16_t strLen = str.length();

    Write<uint16_t>(strLen);
    Write(reinterpret_cast<uint8_t*>(str.data()), strLen);
}

uint64_t FileStream::Write(uint8_t* buff, uint64_t size)
{
    m_fstream.write(reinterpret_cast<char*>(buff), size);
    return size;
}

std::vector<uint8_t> FileStream::Read(uint64_t size)
{
    std::vector<uint8_t> buff(size);
    m_fstream.read(reinterpret_cast<char*>(buff.data()), size);
    
    return buff;
}

std::string FileStream::ReadString()
{
    auto vecBytes = Read(Read<uint16_t>());
    return std::string(vecBytes.begin(), vecBytes.end());
    //std::string str;
    //str.reserve(strLen);

    //std::vector<uint8_t> strBuff = Read(strLen);
    //memcpy(str.data(), strBuff.data(), strLen);
    //return str;
}

void FileStream::Flush()
{
    m_fstream.flush();
}
