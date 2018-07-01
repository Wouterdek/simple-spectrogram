/*
 * Simple spectrogram renderer.
 * Uses a linear scale and hanning window function.
 * Written by Wouter De Keersmaecker, 2018.
 * Licensed under MIT license.
 */

#pragma once
#include <memory>
#include <vector>
#include <cmath>
#include <complex>

const double E = 2.7182818284590452353602874713527d; // Eulers number
const double PI = 3.1415926535897932384626433832795d;

template<typename TSample>
class SpectrogramRenderer {
private:
	// Hanning window function, returns the weight of the ith sample out of n samples in the window.
	static inline double hann(double i, double n){
		return std::pow(std::sin((PI*i)/(n-1)), 2.0);
	}

	// Returns the amplitude of the specified frequency in a range of samples (discrete fourier transform)
	static inline double fourier(double frequency, const std::vector<TSample>& sampleBuffer, std::size_t offset, std::size_t count, uint32_t sampleRate){
		std::complex<double> sum = 0;

		for (std::size_t i = 0; i < count; i++)
		{
			double time = (1.0/sampleRate) * i;
			std::complex<double> exponent(0, -2.0 * PI * frequency * time);
			std::complex<double> sampleVector = (double)sampleBuffer[offset+i] * std::pow(E, exponent);
			sum += sampleVector * hann(i, count);
		}

		return std::abs(sum);
	}

	SpectrogramRenderer(){}
	~SpectrogramRenderer(){}

public:
	// Create a spectrogram greyscale image from the samples in sampleBuffer.
	// The image will be written into imageBuffer with floats in range [0;1] as pixels.
	static void render(
			const std::vector<TSample>& sampleBuffer, // PCM sample values as floats
            uint32_t sampleRate, // for example: 44100hz
			std::vector<float>& imageBuffer, // must have room for width*height values
			uint32_t width, // width of the image
			uint32_t height, // height of the image
			double minFreq = 0, // lowest frequency to map, will be placed at the bottom of the image.
			double maxFreq = 8000, // highest frequency to map, will be placed at the top of the image.
            std::size_t sampleWindowSize = 256 // how many samples should be used for a single pixel?
	){
		double freqDelta = maxFreq - minFreq;
		double maxAmplitude = -1E99;

		for (uint32_t x = 0; x < width; x++)
		{
			uint32_t offset = ((double)x / (double)width) * sampleBuffer.size();

			for (uint32_t y = 0; y < height; y++)
			{
				double curFreq = (((double)y / (double)height) * freqDelta) + minFreq;

				size_t curWindowSize = std::min(sampleWindowSize, sampleBuffer.size() - offset);
				double amplitude = fourier(curFreq, sampleBuffer, offset, curWindowSize, sampleRate);

				maxAmplitude = std::max(maxAmplitude, amplitude);

				auto imageRow = (height - y - 1); // put low frequencies at the bottom
				imageBuffer[(imageRow*width) + x] = amplitude;
			}
		}

		// Remap onto [0;1] range
		for (std::vector<float>::size_type i = 0; i < imageBuffer.size(); i++) {
			imageBuffer[i] /= maxAmplitude;
		}
	}
};



