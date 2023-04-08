#include <stdio.h>
#include <string.h>

int main(int argc, char const* argv[]) {
	char string1[-1];
	char string2[-1];

	strcpy(string1, "this is ");
	strcpy(string2, "string concatenation");

	strncat(string1, string2, strlen(string2));

	printf("%s", string1);

	return 0;
}



// #include <string.h>

// int main() {
// 	char string1[] = "this is ";
// 	char string2[] = "string concatenation";
// 	strncat(string1, string2, strlen(string2));
// }