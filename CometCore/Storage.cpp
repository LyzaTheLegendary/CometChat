#include "Storage.hpp"
#include <regex>

Storage::Storage(const char* directory)
{
	if (!Filesystem::DirectoryExists(directory)) {
		Filesystem::xCreateDirectory(directory);
		return;
	}

	m_fileMap = std::unordered_map<std::string, DataEntry>();
	m_directory = std::string(directory);
	std::string path(directory);
	path += "/MAP";
	// ik denk dat de data undertussen niet meer opgeslagen was
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

		m_fileMap[key] = DataEntry(flags, positions);			//
	}
	int32_t fragmentEntries = fs.Read<int32_t>();

	for (int32_t i = 0; i < fragmentEntries; i++) {

		FilePosition position = fs.Read<FilePosition>();
		m_fragments.push(position);
	}
	fs.Close();
}

std::vector<uint8_t> Storage::FetchFile(std::string& key)
{
	DataEntry& dataEntry = m_fileMap.at(key); // Here we use key data

	std::vector<FilePosition>& positions = dataEntry.GetPositions();
	std::string filePath = m_directory + "/DATA";

	FileStream fs(filePath);
	std::vector<uint8_t> buffer;

	for (const auto& position : positions) {
		int64_t oldSize = buffer.size();
		int64_t posSize = position.CalcSize();

		buffer.resize(oldSize + posSize); // TODO: maybe memory leak if it does not start at 0 
		fs.Seek(position.start, Stream::SEEK_ORIGIN_BEGIN);

		std::vector<uint8_t> tempBuff = fs.Read(posSize);
		memcpy(buffer.data() + oldSize, tempBuff.data(), posSize);
	}
	return buffer;
}

void Storage::AddFile(std::string key, std::vector<uint8_t> buffer)
{
	if (key == "") {
		throw std::invalid_argument("Key cannot be an empty string.");
	}

	FileStream fs((m_directory + "/DATA").c_str(), WriteBinary);
	//implementation for if there are fragmented files? // what if fragment is bigger?
	//TODO: IMPLEMENT ME!



	//implementation for singular file
	fs.Seek(0, Stream::SEEK_ORIGIN_END);
	FilePosition pos {
		fs.GetPos(),
		buffer.size()
	};

	fs.Write(buffer);

	m_fileMap[key] = DataEntry(0, pos);
	SaveMap();
}

void Storage::RemoveFile(std::string key)
{

	DataEntry& dataEntry = m_fileMap.at(key);
	m_fileMap.erase(key);
	

	for (FilePosition position : dataEntry.GetPositions())
		m_fragments.push(position);

	SaveMap();
}

void Storage::SaveMap()
{

	int32_t dataEntries = std::count_if(m_fileMap.begin(), m_fileMap.end(), [](const auto& pair) {return pair.first != ""; });//m_fileMap.size();
	FileStream fs((m_directory + "/MAP").c_str(), WriteBinary);

	fs.Write<int32_t>(dataEntries);

	for( auto& element : m_fileMap) {
		std::string key = element.first;
		
		if (key.empty()) continue;
		
		printf("Key: %s with positionCount: %d", key.c_str(), element.second.GetPositions().size());

		DataEntry entry = element.second;
		std::vector<FilePosition> positions = entry.GetPositions();

		fs.WriteString(key);
		fs.Write<int16_t>(entry.Flags());
		fs.Write<uint16_t>(positions.size());

		for (const FilePosition& position : positions) {
			fs.Write<FilePosition>(position);
		}
	}

	int32_t fragmentEntries = m_fragments.size();
	fs.Write<int32_t>(fragmentEntries);

	for (const FilePosition& position : m_fragments._Get_container())
		fs.Write<FilePosition>(position);

}

