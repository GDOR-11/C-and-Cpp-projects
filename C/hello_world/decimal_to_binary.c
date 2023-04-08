#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int* decimalToBinary(unsigned int decimal) {
	int arrlen = 8 * sizeof(unsigned int) + 1;
	int* binary = malloc(sizeof(int) * arrlen);
	binary[0] = arrlen;

	if(decimal == 0) {
		return binary;
	}

	int closestPowerOf2 = floor(log(round(decimal)) / log(2));
	binary[closestPowerOf2 + 1] = 1;

	int* binary2 = decimalToBinary(decimal - pow(2, closestPowerOf2));

	for(int i = 1;i < arrlen;i++) {
		binary[i] |= binary2[i];
	}
	return binary;
}
void printintarray(int* array) {
	for(int i = 1;i < array[0];i++) {
		printf("%d", array[i]);
	}
}
int* copyintarray(int* array) {
	int* copy = malloc(sizeof(int) * array[0]);
	for(int i = 1;i < array[0];i++) {
		copy[i] = array[i];
	}
	return copy;
}
void reverseintarray(int* array) {
	int* copy = copyintarray(array);
	for(int i = 1;i < array[0];i++) {
		array[i] = copy[array[0] - i];
	}
}

int main() {
	while(1) {
		// fflush is needed here because
		// if the user inputted something that is not a number in the previous iteration,
		// scanf wont remove it from the keyboard buffer
		fflush(stdin);

		// scanf is not able to read numbers with more then 9 digits
		// so i created this mess to detect when the number is too big
		unsigned int decimal;
		char debug;

		int status1 = scanf("%9u", &decimal);
		int status2 = scanf("%c", &debug);
		if(status1 == 0 || debug != '\n' || decimal == -1) {
			printf("please enter a number in the range [0..999999999]\n");
			continue;
		}

		int* binary = decimalToBinary(decimal);

		reverseintarray(binary);
		printintarray(binary);

		putchar('\n');
	}
}