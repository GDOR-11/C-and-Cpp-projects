#include <string>
#include "functions.hpp"

int sum(int a, int b) {
	return a + b;
}
double sum(double a, double b) {
	return a + b;
}
std::string sum(std::string a, std::string b) {
	return a + b;
}

void duplicate(int &num) {
	num *= 2;
}
float sum(const float &a, const float &b) {
	return a + b;
}