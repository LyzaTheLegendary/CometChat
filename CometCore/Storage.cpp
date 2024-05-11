#include "Storage.hpp"

#define MAP_FILE "MAP"
#define DATA_FILE "DATA"

Storage::Storage(std::string directory) : m_directory(directory)
{
	if (!Filesystem::DirectoryExists(directory.c_str()))
		Filesystem::xCreateDirectory(directory.c_str());

	LoadMap();
}

void Storage::LoadMap()
{
	std::string path = std::format("{}/{}", m_directory, MAP_FILE);
	if (!Filesystem::FileExists(path.c_str()))
		return;

	FileStream fs(path);

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

	for (int32_t i = 0; i < fragmentEntries; i++) {
		FilePosition position = fs.Read<FilePosition>();
		m_fragments.push(position);
	}
}

std::vector<uint8_t> Storage::FetchFile(std::string& key)
{
	DataEntry& dataEntry = m_fileMap.at(key);

	FileStream fs(std::format("{}/{}", m_directory, DATA_FILE));
	std::vector<uint8_t> buffer;

	for (const FilePosition& position : dataEntry.GetPositions()) {
		int64_t oldSize = buffer.size();
		int64_t posSize = position.CalcSize();

		buffer.resize(oldSize + posSize); // TODO: maybe memory leak if it does not start at 0 
		fs.Seek(position.start, Stream::SEEK_ORIGIN_BEGIN);

		std::vector<uint8_t> tempBuff = fs.Read(posSize);
		memcpy(buffer.data() + oldSize, tempBuff.data(), posSize);
	}

	return buffer;
}

void Storage::AddFile(std::string& key, std::vector<uint8_t>& buffer)
{
	if (key.empty())
		throw std::invalid_argument("Key cannot be an empty string.");
	
	if (m_fragments.size() == 0) {
		InsertFile(key, buffer);
		return;
	}

	InsertFileSegmented(key, buffer);
}

void Storage::RemoveFile(std::string& key)
{
	DataEntry& dataEntry = m_fileMap.at(key);
	
	//Cannot be a reference as it has to be copied
	for (FilePosition position : dataEntry.GetPositions())
		m_fragments.push(position);

	m_fileMap.erase(key);
	SaveMap();
}

void Storage::SaveMap()
{

	int32_t dataEntries = std::count_if(m_fileMap.begin(), m_fileMap.end(), [](const auto& pair) {return pair.first != ""; });//m_fileMap.size();
	FileStream fs(std::format("{}/{}", m_directory, MAP_FILE), WriteBinary);
	//FileStream fs((m_directory + "/MAP").c_str(), WriteBinary);

	fs.Write<int32_t>(dataEntries);

	for(auto& element : m_fileMap) {
		std::string key = element.first;
		
		if (key.empty()) continue;

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

void Storage::InsertFile(std::string& key, std::vector<uint8_t>& buffer)
{
	FileStream fs(std::format("{}/{}", m_directory, DATA_FILE), WriteBinary);
	fs.Seek(0, Stream::SEEK_ORIGIN_END);
	FilePosition pos{
		fs.GetPos(),
		buffer.size()
	};

	fs.Write(buffer);

	m_fileMap[key] = DataEntry(0, pos);
	SaveMap();
}

void Storage::InsertFileSegmented(std::string& key, std::vector<uint8_t>& buffer)
{
	int32_t fileDataRemaining = buffer.size();
	int32_t fileWritten = 0;

	std::vector<FilePosition> positions;
	FileStream fs(std::format("{}/{}", m_directory, DATA_FILE), WriteBinary);

	while (fileDataRemaining > 0) {
		//Write remaining data to the end
		if (m_fragments.size() == 0) {
			int32_t toWrite = fileDataRemaining - fileWritten;
			std::vector<uint8_t> buff(toWrite);

			memcpy(buff.data(), buffer.data() + fileWritten, fileDataRemaining);

			fs.Seek(0, Stream::SEEK_ORIGIN_END);
			positions.push_back(FilePosition{ fs.GetPos(), fs.GetPos() + toWrite });
			break;
		}

		FilePosition position = m_fragments.front();
		m_fragments.pop();

		int64_t posSize = position.CalcSize();

		// we slice off the amount of data we don't need and return it to the fragmentation queue
		if (posSize > fileDataRemaining) {
			FilePosition newPosition{ (position.end -= fileDataRemaining), position.end };
			m_fragments.push(newPosition);

			position.end -= fileDataRemaining;
			//	Recalculate the size
			posSize = position.CalcSize();
		}

		fs.Seek(position.start, Stream::SEEK_ORIGIN_BEGIN);
		fs.Write(buffer.data() + fileDataRemaining, posSize);

		fileDataRemaining -= posSize;
		fileWritten += posSize;

		positions.push_back(position);

	}

	SaveMap();
}

