#pragma once
#include <stdint.h>
struct FilePosition {
public:
	uint64_t start;
	uint64_t end;

	int64_t CalcSize() {
		return end - start;
	}
};