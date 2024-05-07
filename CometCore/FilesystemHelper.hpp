#pragma once
#include<exception>
#include<filesystem>
namespace fs = std::filesystem;
namespace Filesystem {
	bool DirectoryExists(const char* path) noexcept;

	void CreateDirectory(const char* path);
}
