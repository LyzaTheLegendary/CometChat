#pragma once
#include "StreamInterface.hpp"
#include <string>
#include <memory>
#include <exception>
#include <system_error>
#include<fstream>
// rewrite to use the std library

#define ReadBinary std::ios::in | std::ios::binary
#define WriteBinary std::ios::binary | std::ios::out

class FileStream : public StreamInterface
{
public:
	FileStream(const std::string& path, int flags = ReadBinary) {
		m_fstream = std::fstream(path, flags);
		if (!m_fstream.is_open()) {
			throw std::exception("File not found");
		}

		m_fstream.seekp(0, std::ios::end);
		m_fileSize = m_fstream.tellp();
		m_fstream.seekp(0, std::ios::beg);
	}

	~FileStream() {
		Close();
	}

	int64_t GetSize();
	int64_t GetPos();

	void Seek(int64_t offset, uint8_t seekOrigin);
	void Close();

	uint64_t Write(std::vector<uint8_t> buffer) override;
	void WriteString(std::string str);

	template <typename T>
	uint64_t Write(T data) {
		if (!std::is_standard_layout<T>())
			throw std::invalid_argument("Structure can not be cast into bytes, as it is too advanced.");
		
		std::vector<uint8_t> buff(sizeof(T));

		memcpy(buff.data(), reinterpret_cast<uint8_t*>(&data), sizeof(T));
		return Write(buff);
	}

	uint64_t Write(uint8_t* buff, uint64_t size) override;

	//	Resets the file pointer to the start to read everything from the start
	std::vector<uint8_t> ReadAll();
	std::vector<uint8_t> Read(uint64_t size) override;
	std::string ReadString();
	template <typename T>
	T Read() {
		if (!std::is_standard_layout<T>())
			throw std::invalid_argument("Structure can not be cast into bytes, as it is too advanced.");

		std::vector<uint8_t> buff = Read(sizeof(T));

		return *reinterpret_cast<T*>(buff.data());
	}

	void Flush();
private:
	std::fstream m_fstream;
	int64_t m_fileSize = 0;
};

