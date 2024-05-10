#pragma once
#include <stdint.h>
struct FilePosition {
public:
	int64_t start;
	int64_t end;

	int64_t CalcSize() {
		return end - start;
	}
};