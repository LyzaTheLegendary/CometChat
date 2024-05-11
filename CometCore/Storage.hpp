#pragma once
#include "FilesystemHelper.hpp"
#include "StringHelper.hpp"
#include "FileStream.hpp"
#include "MemoryStream.hpp"
#include "DataEntry.hpp"
#include <unordered_map>
#include <queue>
#include <stdint.h>

class Storage {
public:
	Storage(std::string directory);
	std::vector<uint8_t> FetchFile(std::string& key);
	void AddFile(std::string key, std::vector<uint8_t> buffer);
	void RemoveFile(std::string key);
	void SaveMap();
private:
	void InsertFile(std::string& key, std::vector<uint8_t>& buffer);
	void InsertFileSegmented(std::string& key, std::vector<uint8_t>& buffer);
	void LoadMap();
	std::unordered_map<std::string, DataEntry> m_fileMap;
	std::queue<FilePosition> m_fragments;
	std::string m_directory;
};