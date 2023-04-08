#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum SkinColor {
	white,
	black
};
typedef enum {
	blue,
	green,
	brown
} EyeColor;

struct Person {
	char name[50];
	int age;
	float height;
	float weight;
	enum SkinColor skinColor;
	EyeColor eyeColor;
};

typedef struct {
	char *value;
} String;
void setstr(String string, char value[]) {
	string.value = (char *) malloc(strlen(value) * sizeof(char));
	// memcpy(string.value, value, strlen(value) * sizeof(char));
}

int main(int argc, const char *argv[]) {
	String string;
	// string.value = malloc(0);
	setstr(string, "string :)");
	printf("%s", string.value);
	// printf("%lu %lu %c", sizeof(*string.value), sizeof("string :)"), *string.value);
	return 0;
}