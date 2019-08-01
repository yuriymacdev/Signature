#pragma once

#include "Common.h"

class Hasher {
public:
	virtual DataChunk calculateHash(const DataChunk &chunk) = 0;
	virtual ~Hasher() {};
};
