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

}

