#include "spectrogram.h"
#include "lib/lodepng.h"
#include "lib/AudioFile.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <cstdint>
#include <vector>
#include <chrono>

using Clock = std::chrono::high_resolution_clock;

std::unique_ptr<std::vector<char>> readFile(const char* filename){
	// Open file.
	std::ifstream in(filename, std::ofstream::binary);
	if(!in.is_open()){
		return nullptr;
	}

	// Get file length.
	in.seekg(0, in.end);
	uint64_t length = in.tellg();
	in.seekg(0, in.beg);

	// Read data into buffer.
	auto buffer = std::make_unique<std::vector<char>>(length);
	in.read(buffer->data(), length);

	// If reading failed, return null. Otherwise, return buffer.
	if(in.fail()){
		return nullptr;
	}
	return std::move(buffer);
}

void writePNG(const char* filename, int width, int height, const std::vector<float>& data){
	std::vector<unsigned char> rgba(data.size() * 4);
	for (size_t i = 0; i < data.size(); i++)
	{
		unsigned char value = data[i] * 255;
		rgba[(i*4)] = value;
		rgba[(i*4)+1] = value;
		rgba[(i*4)+2] = value;
		rgba[(i*4)+3] = 255;
	}
	lodepng::encode(filename, rgba, width, height);
}

int main(const int argc, const char** argv){
	if(argc != 3){
		std::cerr << "Incorrect number of arguments" << std::endl;
		std::cerr << "Usage: simple_spectrogram input.wav output.png" << std::endl;
		return 1;
	}

	AudioFile<float> file;
	if(!file.load(std::string(argv[1]))){
		std::cerr << "Failed to read input file" << std::endl;
		return 1;
	}

	uint32_t width = 1100;
	uint32_t height = 128;
	std::vector<float> imageBuffer(width * height);

	auto start = Clock::now();
	SpectrogramRenderer<float>::render(file.samples[0], file.getSampleRate(), imageBuffer, width, height);
	auto end = Clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Rendering took " << duration << " ms." << std::endl;

	writePNG(argv[2], width, height, imageBuffer);

	return 0;
}