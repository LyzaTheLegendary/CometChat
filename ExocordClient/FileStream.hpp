#pragma once
#include "StreamInterface.hpp"
#include <string>
#include <memory>
#include <exception>
#include <system_error>
#define ReadModeBinary "rb"
#define WriteModeBinary "wb"
#define AppendModeBinary "ab"
// rewrite to use the std library
class FileStream : public StreamInterface
{
public:
	FileStream(const char* path, const char *mode = ReadModeBinary) {

		errno_t err = fopen_s(&m_fp, path, mode);
		
		if (err != 0) {
			if (m_fp != nullptr) {
				fclose(m_fp);
			}

			char errorMessage[256];
			strerror_s(errorMessage, sizeof(errorMessage), err);
			throw std::exception((char*)&errorMessage);
		}

		fseek(m_fp, 0, SEEK_END);
		m_fileSize = ftell(m_fp);
		fseek(m_fp, 0, SEEK_SET);
	}

	~FileStream() {
		if (m_fp == nullptr)
			return;

		fflush(m_fp);
		fclose(m_fp);
	}

	int64_t GetSize();
	int64_t GetPos();

	void Seek(int64_t offset, uint8_t seekOrigin);

	uint64_t Write(std::vector<uint8_t> buffer) noexcept override;

	template <typename T>
	uint64_t Write(T data) {
		if (!std::is_standard_layout<T>())
			throw std::invalid_argument("Structure can not be cast into bytes, as it is too advanced.");
		
		std::vector<uint8_t> buff(sizeof(T));

		memcpy(buff.data(), reinterpret_cast<uint8_t*>(&data), sizeof(T));
		return Write(buff);
	}

	uint64_t Write(uint8_t* buff, uint64_t size) noexcept override;

	std::vector<uint8_t> Read(uint64_t size) override;
	std::string ReadString();
	template <typename T>
	T Read() {
		if (!std::is_standard_layout<T>())
			throw std::invalid_argument("Structure can not be cast into bytes, as it is too advanced.");

		std::vector<uint8_t> buff = Read(sizeof(T));

		return *reinterpret_cast<T*>(buff.data());
	}

private:
	FILE* m_fp = nullptr;
	int64_t m_fileSize = 0;
};

