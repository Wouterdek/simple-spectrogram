# simple-spectrogram
A simple single-header spectrogram generator in C++.
The generated spectrograms use a linear scale and a hanning window function. The window size is customizable, but 256 by default.
The code is fairly simple, but not very optimal. A more optimal implementation would replace the plain O(n^2) discrete fourier transform with an FFT algorithm like Cooley-Tukey that runs in O(n log n).
This repo also contains a CMake project with a sample application that uses the header.

This project was written for educational purposes and is licensed under the MIT License.

## Example
A 300hz sine wave, on a spectrogram between 0 and 8000hz:
![300hz](images/300hz.png "300hz sine wave")
