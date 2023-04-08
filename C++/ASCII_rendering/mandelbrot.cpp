// compile with g++ -o mandelbrot mandelbrot.cpp canvas.cpp

// this project compiles in all c++ versions that g++ supports :)

#include "canvas.hpp"
#include <string>
#include <complex>
#include <unistd.h>

template <typename T> double abs_sq(std::complex<T> z) {
	T re = real(z);
	T im = imag(z);
	return re * re + im * im;
}

const int maxIterations = 1000;
const double k = 0.01;


Canvas cursorCanvas(0, 0);
Canvas mandelbrotCanvas(0, 0);
int width;
int height;

int cursorI = 1;
int cursorJ = 1;
bool showCursor = true;
int zoomMagnitude = 3;
std::complex<double> screenStart(-3, -2);
std::complex<double> screenEnd(1, 2);

unsigned char mandelbrotPixel(int i, int j, std::va_list args) {
	double x = (real(screenEnd) - real(screenStart)) * i / width + real(screenStart);
	double y = (imag(screenEnd) - imag(screenStart)) * j / height + imag(screenStart);

	std::complex<double> c(x, y);
	std::complex<double> z(c);
	std::complex<double> dz(1, 0);

	double m2;
	int iter = 0;
	for(;iter < maxIterations;iter++) {
		dz = 2.0 * z * dz;
		z = z * z + c;
		m2 = abs_sq(z);
		if(m2 > 1000) {
			break;
		}
	}

	int color;
	if(iter == maxIterations) {
		color = 0;
	} else {
		double distance = sqrt(m2 / abs_sq(dz)) * 0.5 * log(m2);
		distance /= real(screenEnd) - real(screenStart);
		color = 255 * (2 / (1 + exp(-k / distance)) - 1);
		color = color < 255 ? color : 255;
	}

	return (unsigned char) color;
}
unsigned char cursorPixel(int i, int j, std::va_list args) {
	unsigned char previousPixel = mandelbrotCanvas.getPixel(i, j);

	double x = (double) i / width;
	double y = (double) j / height;

	if(showCursor && x > (double) cursorI / zoomMagnitude && x < (double) (cursorI + 1) / zoomMagnitude && y > (double) cursorJ / zoomMagnitude && y < (double) (cursorJ + 1) / zoomMagnitude) {
		return (unsigned char) 255 - previousPixel;
	} else {
		return previousPixel;
	}
}

void moveCursor(int x, int y) {
	cursorI = (cursorI + x + zoomMagnitude) % zoomMagnitude;
	cursorJ = (cursorJ + y + zoomMagnitude) % zoomMagnitude;
}
void zoom() {
	double newScreenSize = (real(screenEnd) - real(screenStart)) / zoomMagnitude;
	screenStart += newScreenSize * std::complex<double>(cursorI, cursorJ);
	screenEnd = screenStart + std::complex<double>(newScreenSize, newScreenSize);
}

int main(int argc, const char *argv[]) {
	if(argc == 1) {
		width = 444;
		height = 222;
	} else if(argc == 2) {
		int size = atoi(argv[1]);
		int width = size * 2;
		int height = size;
	}
	cursorCanvas.resize(width, height);
	mandelbrotCanvas.resize(width, height);

	mandelbrotCanvas.applyToAllPixels(mandelbrotPixel);
	bool loop = true;
	while(loop) {
		cursorCanvas.applyToAllPixels(cursorPixel);
		cursorCanvas.render();
		fflush(stdin);
		char action = getchar();

		switch(action) {
			case 'w':
			case 'W':
				moveCursor(0, -1);
				break;
			case 'a':
			case 'A':
				moveCursor(-1, 0);
				break;
			case 's':
			case 'S':
				moveCursor(0, 1);
				break;
			case 'd':
			case 'D':
				moveCursor(1, 0);
				break;
			case 'h':
			case 'H':
				showCursor = !showCursor;
				break;
			case 'e':
			case 'E':
				loop = false;
				break;
			case ' ':
				zoom();
				mandelbrotCanvas.applyToAllPixels(mandelbrotPixel);
				break;
		}
	}
}