#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if(argc != 2) {
		fprintf(stderr, "Incorrect sintax. The correct sintax is \"line FILE_PATH\"");
		exit(1);
	}

	FILE* file = fopen(argv[1], "r");
	if(file == NULL) {
		fprintf(stderr, "Could not open %s.\n", argv[1]);
		exit(2);
	}

	for(int line = 1;1;line++) {
		short ch = fgetc(file);
		if(ch == EOF) {
			break;
		}
		printf("%d: ", line);

		while(ch != '\n') {
			putchar(ch);
			ch = fgetc(file);
		}
		putchar('\n');
	}
	exit(0);
}
