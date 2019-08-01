#pragma once

#include "Common.h"
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

class ChunkWriter {
	std::ofstream outputFile;
	//std::mutex m;
public:
	ChunkWriter(std::string outputFilePath) {
		outputFile = std::ofstream(outputFilePath, std::ios::binary);
	}

	void writeChunk(DataChunk chunk) {
		//std::lock_guard<std::mutex> l(m); // Making the method thread-safe

		outputFile.write(reinterpret_cast <char*>(chunk.data()), chunk.size());
	}

	~ChunkWriter() {
		outputFile.close();
	}
};