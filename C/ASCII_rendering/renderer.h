#include <stdlib.h>
#include <stdio.h>

const char* pallete = " `.\',_-:~\"^!;/\\<>+)(|vL?[]tzr}{xcilYf*IjnuJ1oakhZXCwUmqpdb@#O%%&8W0B$QM";
const int palleteLength = 70;

typedef struct {
	int* image;
	int width;
	int height;
} Canvas;



Canvas createCanvas(int width, int height) {
	Canvas canvas;
	canvas.width = width;
	canvas.height = height;
	canvas.image = malloc(sizeof(int) * width * height);
	return canvas;
}

void resizeCanvas(Canvas canvas, int width, int height) {
	canvas.width = width;
	canvas.height = height;
	canvas.image = realloc(canvas.image, sizeof(int) * width * height);
}

void clearScreen(Canvas canvas) {
	for(int i = 0;i < canvas.width * canvas.height;i++) {
		canvas.image[i] = 0;
	}
}

int getPixel(Canvas canvas, int x, int y) {
	return canvas.image[y * canvas.width + x];
}

void setPixel(Canvas canvas, int x, int y, int value) {
	canvas.image[y * canvas.width + x] = value;
}

int closestPalleteCollor(int value) {
	return palleteLength * value / 255;
}

void dither(Canvas canvas) {
	for(int j = 0;j < canvas.height;j++) {
		for(int i = 0;i < canvas.width;i++) {
			int oldPixel = getPixel(canvas, i, j);
			int newPixel = closestPalleteCollor(oldPixel);
			setPixel(canvas, i, j, newPixel);
			int error = oldPixel - newPixel;
			setPixel(canvas, i + 1, j    , getPixel(canvas, i + 1, j    ) + error * 7 / 16);
			setPixel(canvas, i - 1, j + 1, getPixel(canvas, i - 1, j + 1) + error * 3 / 16);
			setPixel(canvas, i    , j + 1, getPixel(canvas, i    , j + 1) + error * 5 / 16);
			setPixel(canvas, i + 1, j + 1, getPixel(canvas, i + 1, j + 1) + error * 1 / 16);
		}
	}
}

void applyToAllPixels(Canvas canvas, int (*f)(int, int, int)) {
	for(int i = 0;i < canvas.width;i++) {
		for(int j = 0;j < canvas.height;j++) {
			setPixel(canvas, i, j, f(getPixel(canvas, i, j), i, j));
		}
	}
}

void copyScreen(Canvas source, Canvas target) {
	int width = source.width < target.width ? source.width : target.width;
	int height = source.height < target.height ? source.height : target.height;
	for(int i = 0;i < width * height;i++) {
		target.image[i] = source.image[i];
	}
}

void renderScreen(Canvas canvas) {
	for(int j = 0;j < canvas.height;j++) {
		for(int i = 0;i < canvas.width;i++) {
			int value = getPixel(canvas, i, j);
			int index = closestPalleteCollor(value);
			putchar(pallete[index]);
		}
		putchar('\n');
	}
}