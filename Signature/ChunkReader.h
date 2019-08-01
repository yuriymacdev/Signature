#pragma once

#include "Common.h"
#include <fstream>
#include <string>
#include <iostream>

class ChunkReader {
	std::ifstream inputFile;
	size_t blockSize;
	std::streampos inputFileSize;
public:
	ChunkReader(std::string inputFilePath, size_t chunkSize) {
		//string inputFilePath = "C:\\Users\\John\\Desktop\\Win7x64\\Win7x64.vdi";
		inputFile = std::ifstream(inputFilePath, std::ios::binary | std::ios::ate);
		inputFileSize = inputFile.tellg();

		//std::cout << "Input file size: " << inputFileSize / (double)(1024 * 1024) << " MB" << std::endl;
		inputFile.seekg(0, std::ios::beg);

		blockSize = chunkSize;
	}

	DataChunk readNextChunk() {
		DataChunk inputBuffer(blockSize);
		inputFile.read(inputBuffer.data(), blockSize);
		std::streamsize actuallyRead = inputFile.gcount();
		//std::cout << "Read: " << actuallyRead / (double)(1024 * 1024) << " MB" << std::endl;

		// Appending 0's
		if (actuallyRead > 0 && actuallyRead < blockSize) {
			memset(inputBuffer.data() + actuallyRead, 0, blockSize - actuallyRead);
		}

		return inputBuffer;
	}

	bool hasMoreChunks() {
		return !inputFile.eof();
	}

	unsigned long getNumberOfChunks() {
		if (blockSize == 0)
			blockSize = 1; // Just in case 
		return inputFileSize / blockSize + (inputFileSize % inputFileSize != 0); // Ceiling of a division
	}

	~ChunkReader() {
		inputFile.close();
	}
};