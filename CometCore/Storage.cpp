#include "Storage.hpp"

std::vector<uint8_t> Storage::FetchFile(std::string key)
{
	DataEntry dataEntry = m_fileMap[key];

	std::vector<FilePosition> positions = dataEntry.GetPositions();
	std::string filePath = m_directory + "/DATA";

	FileStream fs(filePath.c_str());

	std::vector<uint8_t> buffer;
	for (auto& position : positions) {
		int64_t oldSize = buffer.size();
		int64_t posSize = position.CalcSize();

		buffer.resize(oldSize + posSize);
		fs.Seek(position.start, Stream::SEEK_ORIGIN_BEGIN);

		std::vector<uint8_t> tempBuff = fs.Read(posSize);
		memcpy(buffer.data() + oldSize, tempBuff.data(), posSize);
	}

	return buffer;
}

void Storage::AddFile(std::string key, std::vector<uint8_t> buffer)
{
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

	m_fileMap[key] = DataEntry(0, pos); // TODO fix memory violation? ( skill issue ) 
	SaveMap();
}

void Storage::RemoveFile(std::string key)
{
	std::vector<FilePosition> positions = m_fileMap[key].GetPositions();
	m_fileMap.erase(key);
	

	for (auto position : positions)
		m_fragments.push(position);

	SaveMap();
}

void Storage::SaveMap()
{
	int32_t dataEntries = m_fileMap.size();
	FileStream fs((m_directory + "/MAP").c_str(), WriteBinary);

	fs.Write<int32_t>(dataEntries);

	for( auto& element : m_fileMap) {
		std::string key = element.first;
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

