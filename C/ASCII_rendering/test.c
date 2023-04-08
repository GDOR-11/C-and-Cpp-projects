#include <complex.h>
#include <math.h>
#include <stdlib.h>
#include "renderer.h"

Canvas canvas;
Canvas canvas2;

int cursorI = 0;
int cursorJ = 0;
double screenStartX = -3.0;
double screenStartY = -2.0;
double screenEndX = 1.0;
double screenEndY = 2.0;

const int maxIterations = 10000;

int hideCursor = 0;
int zoomed = 1;


double cabs_sq(double complex z) {
	double re = creal(z);
	double im = cimag(z);
	return re * re + im * im;
}
int mandelbrotPixel(int prevColor, int i, int j) {
	if(zoomed == 0) {
		return getPixel(canvas2, i, j);
	}
	double x = (screenEndX - screenStartX) * i / canvas.width + screenStartX;
	double y = (screenEndY - screenStartY) * j / canvas.height + screenStartY;

	double complex c = x + y * I;
	double complex z = c;
	double complex dz = 1.0;

	double m2;
	int iter = 0;
	for(;iter < maxIterations;iter++) {
		dz = 2 * z * dz;
		z = z * z + c;
		m2 = cabs_sq(z);
		if(m2 > 1000) {
			break;
		}
	}

	int color;
	if(iter == maxIterations) {
		color = 0;
	} else {
		double distance = sqrt(m2 / cabs_sq(dz)) * 0.5 * log(m2);
		color = 255 * sqrt(sqrt(sqrt(16 * distance / (screenEndX - screenStartX))));
		color = color < 255 ? color : 255;
	}


	return color;
}
int renderCursor(int prevColor, int i, int j) {
	if(!hideCursor && i >= cursorI && i <= cursorI + canvas.width / 2 && j >= cursorJ && j <= cursorJ + canvas.height / 2) {
		prevColor = 255 - prevColor;
	}
	return prevColor;
}

void moveCursor(int x, int y) {
	if(cursorI == 0 && x == 1) {
		cursorI = canvas.width / 2;
	}
	if(cursorI == canvas.width / 2 && x == -1) {
		cursorI = 0;
	}
	if(cursorJ == 0 && y == 1) {
		cursorJ = canvas.height / 2;
	}
	if(cursorJ == canvas.height / 2 && y == -1) {
		cursorJ = 0;
	}
}
void zoom() {
	double averageX = (screenStartX + screenEndX) / 2;
	double averageY = (screenStartY + screenEndY) / 2;

	if(cursorI == 0) {
		screenEndX = averageX;
	} else {
		screenStartX = averageX;
	}
	if(cursorJ == 0) {
		screenEndY = averageY;
	} else {
		screenStartY = averageY;
	}
}

int main(int argc, char** argv) {
	int width = 888;
	int height = 444;
	if(argc == 3) {
		width = atoi(argv[1]);
		height = atoi(argv[2]);
		if(width % 2 == 1 || height % 2 == 1) {
			printf("both dimensions have to be even");
			return 0;
		}
	}
	canvas = createCanvas(width, height);
	canvas2 = createCanvas(width, height);
	while(1) {
		applyToAllPixels(canvas, mandelbrotPixel);
		if(zoomed == 1) {
			copyScreen(canvas, canvas2);
		}
		if(hideCursor == 0) {
			applyToAllPixels(canvas, renderCursor);
		}
		renderScreen(canvas);
		int keyPressed = 0;
		while(!keyPressed) {
			fflush(stdin);
			char c = getchar();
			int xmove = 0, ymove = 0;
			zoomed = 0;
			switch(c) {
				case 'w':
					ymove = -1;
					break;
				case 'a':
					xmove = -1;
					break;
				case 's':
					ymove = 1;
					break;
				case 'd':
					xmove = 1;
					break;
				case 'h':
					hideCursor = !hideCursor;
					break;
				case ' ':
					zoom();
					zoomed = 1;
					break;
				default:
					continue;
			}
			moveCursor(xmove, ymove);
			keyPressed = 1;
		}
	}
}