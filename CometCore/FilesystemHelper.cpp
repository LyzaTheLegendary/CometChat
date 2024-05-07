#include "FilesystemHelper.hpp"

namespace Filesystem {
	// in this case we can use a normal bool, The microsoft bool is not used here as it only returns true or false
	bool DirectoryExists(const char* location) noexcept
	{
		return fs::is_directory(fs::path(location));
	}

	void CreateDirectory(const char* location)
	{
		if (fs::create_directories(fs::path(location)) == false)
			throw std::exception("Path not found.");
	}
}
