#include <iostream>
#include "classes.hpp"

void MyClass::incrementCounterBy(int num) {
	counter += num;
}
void MyClass::setCounterTo(int num) {
	counter = num;
}
int MyClass::getCounter() {
	return counter;
}

MyClass::MyClass() {
	std::cout << "counstructor 1 called!\n";
}
MyClass::MyClass(int num) {
	std::cout << "counstructor 2 called!\n";
	setCounterTo(num);
}
MyClass::~MyClass() {
	// this is where you free all the memory allocated in the class, otherwise that memory wil keep "being used"
	std::cout << "destructor called! counter = " << counter << "\n";
}