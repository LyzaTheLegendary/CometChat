#pragma once
#include<vector>
#include<stdint.h>
#include"FilePosition.hpp"

struct DataEntry {

public:
	DataEntry(int16_t flags, std::vector<FilePosition> filePosition) {
		m_flags = flags;
		m_filePositions = filePosition;
	}
	DataEntry() {};

	uint16_t Fragments() {
		return m_filePositions.size();
	}

	int16_t Flags() {
		return m_flags;
	}

	std::vector<FilePosition> GetPositions() {
		return m_filePositions;
	}
private:
	int16_t m_flags;
	std::vector<FilePosition> m_filePositions;
};