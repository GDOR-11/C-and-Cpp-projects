#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <string.h>

int min(a, b) {
	if(a > b) {
		a = b;
	}
	return a;
}
int max(a, b) {
	if(a < b) {
		a = b;
	}
	return a;
}


int main(int argc, char const* argv[]) {
	char min_char = CHAR_MIN;
	char max_char = CHAR_MAX;
	unsigned char max_unsigned_char = UCHAR_MAX;


	short min_short = SHRT_MIN;
	short max_short = SHRT_MAX;
	unsigned short max_unsigned_short = USHRT_MAX;


	int min_int = INT_MIN;
	int max_int = INT_MAX;
	unsigned int max_unsigned_int = UINT_MAX;


	long min_long = LONG_MIN;
	long max_long = LONG_MAX;
	unsigned long max_unsigned_long = ULONG_MAX;


	long long min_long_long = LLONG_MIN;
	long long max_long_long = LLONG_MAX;
	unsigned long long max_unsigned_long_long = ULLONG_MAX;



	float min_float = -FLT_MAX;
	float max_float = FLT_MAX;

	double min_double = -DBL_MAX;
	double max_double = DBL_MAX;

	long double min_long_double = -LDBL_MAX;
	long double max_long_double = LDBL_MAX;

	// ALERT: these values may not be the same for everyone, because some compilers have different limits
	printf(
		"char: [%hhi..%hhi]\n"
		"unsigned char: [0..%hhu]\n\n"

		"short: [%hi..%hi]\n"
		"unsigned short: [0..%hu]\n\n"

		"int: [%i..%i]\n"
		"unsigned int: [0..%u]\n\n"

		"long: [%li..%li]\n"
		"unsigned long: [0..%lu]\n\n"

		"long long: [%lli..%lli]\n"
		"unsigned long long: [0..%llu]\n\n"

		"float: [%f,%f]\n"

		"double: [%lf,%lf]\n"

		"long double: [%Lf,%Lf]"
		,
		min_char, max_char, max_unsigned_char,
		min_short, max_short, max_unsigned_short,
		min_int, max_int, max_unsigned_int,
		min_long, max_long, max_unsigned_long,
		min_long_long, max_long_long, max_unsigned_long_long,
		min_float, max_float, min_double, max_double, min_long_double, max_long_double
	);



	//        ↓  number here is not mandatory
	int array[5] = {1, 2, 3, 4, 5};
	array[0] = 10;
	array[4] = 6;

	//                        ↓  number here is not mandatory
	int multiDimensionalArray[3][6] = {
	//                           ↑  but here it is
		{11, 12, 13, 14, 15, 16},
		{21, 22, 23, 24, 25, 26},
		{31, 32, 33, 34, 35, 36}
	};


	char* readOnlyString = "you cant modify this string, but you can read it!";
	int readOnlyStringLength = strlen(readOnlyString); // strlen(str)

	char modifiableString[] = "you can read and modify this string!";
	int modifiableStringLength = strlen(modifiableString); // strlen(str)

	int stringComparison = strncmp(readOnlyString, modifiableString, max(readOnlyStringLength, modifiableStringLength)); // strncmp(str1, str2, numchars)
	char stringConcatenation[] = "";
	strncat(stringConcatenation, readOnlyString, strlen(readOnlyString));
	strncat(stringConcatenation, modifiableString, strlen(modifiableString));



	printf("\n");
	return 0;
}