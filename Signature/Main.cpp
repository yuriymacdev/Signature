#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <memory>
#include "Common.h"
#include "ChunkReader.h"
#include "ChunkWriter.h"
#include "Hasher.h"
#include "MD5Hasher.h"
#include <thread>

using namespace std;

int main(int argc, char *argv[]) {

	// Let's not import any external command-line argument parsing libs and do the parsing manually for now, as the usage logic is pretty simple.
	unsigned blockSize = 1024*1024*1; // The block size is in bytes
	string inputFilePath;
	inputFilePath = "c:\\Users\\John\\inputFile.txt";

	string outputFilePath = "c:\\Users\\John\\outputFile.txt";
	if (argc == 4) {
		// Block size is specified as a first argument.
		blockSize = atof(argv[1])*1024*1024;
		inputFilePath = argv[2];
		outputFilePath = argv[3];
	} else if (argc == 3) {
		// Only input and output filenames are specified.
		inputFilePath = argv[1];
		outputFilePath = argv[2];
	}
	else {
		// Print usage.
		cerr << "Signature - generating an MD5 based signature of an input file. The utility splits an input file into blocks, calculates an MD5 hash of each block and writes binary hashes sequentially into the output file." << endl;
		cerr << "Usage: " << argv[0] << " [block size in MB] inputFilePath outputFilePath" << endl;
		return 1;
	}

	try {
		
		ChunkReader reader(inputFilePath, blockSize);
		ChunkWriter writer(outputFilePath);

		cout << "Number of blocks: " << reader.getNumberOfChunks() << endl;

		size_t chunkNum = 0; // For logging
		
		const bool useMultithreading = false;

		if (useMultithreading) {
			// ===========A multi threaded solution===========
			std::thread processingThread([]() {});
			while (reader.hasMoreChunks()) {
				cout << "Reading a chunk #" << chunkNum << "\n";
				DataChunk inChunk = reader.readNextChunk();

				processingThread.join(); // Let's wait for previous processing thread to complete before starting a new one

				processingThread = std::thread([&writer, inChunk, chunkNum]() {
					cout << "Processing a chunk #" << chunkNum << endl;
					// Process the chunk and write the result here
					unique_ptr<Hasher> hasher(new MD5Hasher());
					DataChunk outChunk = hasher->calculateHash(inChunk);

					writer.writeChunk(outChunk);
					cout << "Processed and saved the result for chunk #" << chunkNum << "\n";
				});

				chunkNum++;
			}

			processingThread.join(); // Let's not quit before the last thread finished
			// ============End of multi threaded solution=============
		}
		else {

			// ==========A single threaded solution for comparison===========
			while (reader.hasMoreChunks()) {
				cout << "Reading a chunk #" << chunkNum << "\n";
				DataChunk inChunk = reader.readNextChunk();
				cout << "Processing a chunk #" << chunkNum << endl;

				unique_ptr<Hasher> hasher(new MD5Hasher());
				DataChunk outChunk = hasher->calculateHash(inChunk);

				writer.writeChunk(outChunk);
				cout << "Processed and saved the result for chunk #" << chunkNum << "\n";

				chunkNum++;
			}
			// ===========End of single threaded solution============
			// End of single threaded solution
		}
	}
	catch (bad_alloc &e) {
		cerr << "Not enough memory to fit your chunks! (" << e.what() <<")" << endl;
	}
	catch (exception &e) {
		cout << "Unknown error: " << e.what() << endl;
	}
	//system("pause");
	return 0;
}
