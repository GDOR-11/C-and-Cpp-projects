#include "canvas.hpp"

Canvas::Canvas(int width, int height) {
	pixels = new unsigned char[0];
	initializePallete();
	resize(width, height);
}
Canvas::~Canvas() {
	delete[] pixels;
}


int Canvas::idx(int x, int y) {
	return y * _width + x;
}
unsigned char Canvas::getCharOfPixel(int index) {
	return pallete[(pallete.length() - 1) * pixels[index] / 255];
}
unsigned char Canvas::getCharOfPixel(int x, int y) {
	return getCharOfPixel(idx(x, y));
}
void Canvas::initializePallete() { // is this really necessary? idk
	pallete = " `.\',_-:~\"^!;/\\<>+)(|vL?[]tzr}{xcilYf*IjnuJ1oakhZXCwUmqpdb@#O%%&8W0B$QM";
}


void Canvas::resize(int width, int height) {
	if(width < 0 || height < 0) {
		std::cout << "ERROR: Cannot resize canvas because one or more dimensions are negative";
		throw;
	}
	if((long) width * (long) height > INT_MAX) {
		std::cout << "ERROR: Cannot resize canvas because the requested amount of pixels is greater than " << INT_MAX << " (INT_MAX).\n";
		throw;
	}
	delete[] pixels;
	pixels = new unsigned char[width * height];
	_width = width;
	_height = height;
}
unsigned char Canvas::getPixel(int x, int y) {
	return pixels[idx(x, y)];
}
void Canvas::setPixel(int x, int y, unsigned char color) {
	pixels[idx(x, y)] = color;
}
void Canvas::render() {
    std::string finalString((_width + 1) * _height, '\0'); // initializing the string with ((_width + 1) * _height) null characters, to avoid resizing efficiency lack
    int characterIndex = 0; // where we are writing to finalString
    for(int y = 0;y < _height;y++) {
        for(int x = 0;x < _width;x++) {
            finalString[characterIndex++] = getCharOfPixel(x, y);
        }
        finalString[characterIndex++] = '\n';
    }
    std::cout << finalString;
}
void Canvas::applyToAllPixels(unsigned char (*func)(int, int, std::va_list), ...) {
	unsigned char* pixels2 = new unsigned char[_width * _height];
	std::va_list args;
	va_start(args, func);
	for(int i = 0;i < _width * _height;i++) {
		pixels2[i] = func(i % _width, floor(i / _width), args);
	}
	va_end(args);
	for(int i = 0;i < _width * _height;i++) {
		pixels[i] = pixels2[i];
	}
}
unsigned char* Canvas::getPixels() {
	unsigned char* pixels2 = new unsigned char[_width * _height];
	for(int i = 0;i < _width * _height;i++) {
		pixels2[i] = pixels[i];
	}
	return pixels2;
}
void Canvas::setPixels(unsigned char* pixels2) {
	for(int i = 0;i < _width * _height;i++) {
		pixels[i] = pixels2[i];
	}
}


int Canvas::width() {
	return _width;
}
int Canvas::height() {
	return _height;
}