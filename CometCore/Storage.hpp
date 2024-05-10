#pragma once
#include "FilesystemHelper.hpp"
#include "FileStream.hpp"
#include "DataEntry.hpp"
#include <unordered_map>
#include <queue>
#include <stdint.h>

class Storage {
public:
	Storage(const char* directory) {
		if (!Filesystem::DirectoryExists(directory)) {
			Filesystem::xCreateDirectory(directory);
			return;
		}
		m_directory = std::string(directory);
		std::string path(directory);
		path += "/MAP";
		
		if (!Filesystem::FileExists(path.c_str()))
			return;
	
		FileStream fs(path.c_str());

		int32_t dataEntries = fs.Read<int32_t>();
		m_fileMap.reserve(dataEntries);

		for (int32_t i = 0; i < dataEntries; i++) {
			std::string key = fs.ReadString();
			int16_t flags = fs.Read<int16_t>();

			uint16_t positionCount = fs.Read<uint16_t>();
			std::vector<FilePosition> positions;

			for (uint16_t i = 0; i < positionCount; i++)
				positions.push_back(fs.Read<FilePosition>());

			m_fileMap[key] = DataEntry(flags, positions);
		}

		int32_t fragmentEntries = fs.Read<int32_t>();

		FilePosition position;
		for (int32_t i = 0; i < fragmentEntries; i++) {
			position = fs.Read<FilePosition>();
			m_fragments.push(position);
		}
	}

	std::vector<uint8_t> FetchFile(std::string key);
	void AddFile(std::string key, std::vector<uint8_t> buffer);
	void RemoveFile(std::string key);
	void SaveMap();
private:
	std::unordered_map<std::string, DataEntry> m_fileMap;
	std::queue<FilePosition> m_fragments;
	std::string m_directory;
};