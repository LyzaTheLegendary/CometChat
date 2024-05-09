#pragma once
#include<vector>
#include<stdint.h>
#include"FilePosition.hpp"

struct DataEntry {

public:
	DataEntry(int16_t flags, std::vector<FilePosition> filePositions) {
		m_flags = flags;
		m_filePositions = filePositions;

	}

	DataEntry(int16_t flags, FilePosition position) {
		m_flags = flags;
		m_filePositions.push_back(position);
		
	}

	DataEntry() = default;

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