#include <string>
#include <iostream>
#include <cmath>
#include <cstdarg>

class Canvas {

private:
	unsigned char* pixels;
	int _width;
	int _height;

	int idx(int x, int y);
	unsigned char getCharOfPixel(int x, int y);
	unsigned char getCharOfPixel(int index);
	void initializePallete();

public:
	std::string pallete; // " `.\',_-:~\"^!;/\\<>+)(|vL?[]tzr}{xcilYf*IjnuJ1oakhZXCwUmqpdb@#O%%&8W0B$QM"
	Canvas(int width, int height);
	~Canvas();

	void resize(int width, int height);
	unsigned char getPixel(int x, int y);
	void setPixel(int x, int y, unsigned char color);
	void render();
	void applyToAllPixels(unsigned char (*func)(int, int, std::va_list), ...);


	unsigned char* getPixels();
	void setPixels(unsigned char* pixels2);
	int width();
	int height();
};