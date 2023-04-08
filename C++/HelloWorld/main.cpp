// compile with g++ -std=c++11 -o helloWorld main.cpp functions.cpp classes.cpp
//               or greater ↑

#include <iostream>
#include <vector>
#include <string>

#include "functions.hpp"
#include "classes.hpp"



double const pi = 3.141592;



int main() {
	std::cout << "Hello World!\n\n";

	int num;
	std::string str;

	std::cout << "Enter a number: ";
	std::cin >> num;
	std::cout << std::cin.peek() << " \"" << (char) std::cin.peek() << "\"\n";
	fflush(stdin); // try removing this and inputting "3b4", for example; num will be 3 and str will be "b4". fflush prevents that by clearing keyboard buffer. you can alternatively treat strings like that as invalid input, in that case you can check if there's something in the keyboard buffer, using the condition (std::cin.peek() == EOF /* for failure mode */ || std::cin.peek() != '\n')
	if(std::cin.fail()) {
		std::cout << "Invalid input! >:(\n";
		return 1; // non-zero return = warning the caller that something wrong happened
		// the following applies if there was no return 1; (e.g. if the program kept executing even after invalid input)
		std::cin.clear(); // try removing this and inputting "a", for example; std::cin will enter failure mode and ignore any std::cin >> call. std::cin.clear() returns back to normal mode.
	} else {
		std::cout << "You entered the number " << num << ".\n";
	}
	std::cout << "Enter a string: ";
	std::cin >> str; // this is only going to read up to a whitespace character, use getline(std::cin, str) if you want to read up to a '\n'
	std::cout << "You entered the string " << str << ".\n";

	std::cout << "\n";


	std::vector<int> myVector;
	std::vector<int> myVectorWith4ints(4); // creating a vector with 4 ints (storing the default value of the type on each element) | identical to myVectorWith4Ints.resize(4)
	std::vector<int> myVectorWithFour42s(4, 42); // creating a vector with 4 ints (storing 42 on each element) | identical to myVectorWithFour42s.resize(4, 42)
	std::vector<int> myInitializedVector = {1, 2, 3};

	std::vector<std::vector<int>> myTwoDimensionalVector = {
		{1, 2, 3},
		{4, 5, 6, 7},
		{8, 9}
	};

	std::vector<std::vector<int>> myTwoDimensionalVectorWithFixedLength(5);
	for(int i = 0;i < 5;i++) {
		myTwoDimensionalVectorWithFixedLength[i] = std::vector<int>(3);
	}

	// myVector = {}
	myVector.push_back(42); // myVector = {42}
	myVector.push_back(666); // myVector = {42, 666}
	myVector.pop_back(); // myVector = {42}
	myVector.size(); // 1
	myVector.push_back(666); // myVector = {42, 666}
	for(int element : myVector) {
		// cannot directly change the value of [element] inside here, you have to know the index
		std::cout << element << "\n";
	}
	std::cout << "\n";



	sum(3, 4); // 7
	sum(5.0, 3.0); // 8.0
	sum("abcd", "dcba"); // "abcddcba"

	coolSum(3, 4); // 7
	coolSum(5.0, 3.0); // 8.0
	// coolSum("abcd", "dcba"); // actually this can't be done because strings are treated as const char* by default, and they can't be concatenated using +. what c++ done with sum("abcd", "dbca") is it automatically converted const char* to std::string, and std::string can be concatenated using +. it would work if you made variables with the type std::string



	MyClass myInstance;
	std::cout << myInstance.getCounter() << "\n";
	myInstance.incrementCounterBy(10);
	std::cout << myInstance.getCounter() << "\n";

	std::cout << "\n";

	MyClass &stillMyInstance = myInstance;
	std::cout << stillMyInstance.getCounter() << "\n";
	stillMyInstance.setCounterTo(42);
	std::cout << stillMyInstance.getCounter() << "\n";
	std::cout << myInstance.getCounter() << "\n";

	std::cout << "\n";

	std::cout << "start of scope!\n";
	{
		MyClass myOtherInstance(34);
		std::cout << myOtherInstance.getCounter() << "\n";
		myOtherInstance.incrementCounterBy(10);
		std::cout << myOtherInstance.getCounter() << "\n";
	}
	std::cout << "end of scope!\n";

	std::cout << "\n";



	int myVariable = 3;
	int &referenceToMyVariable = myVariable;
	std::cout << myVariable << " " << referenceToMyVariable << "\n";
	myVariable = 42;
	std::cout << myVariable << " " << referenceToMyVariable << "\n";
	referenceToMyVariable = 31;
	std::cout << myVariable << " " << referenceToMyVariable << "\n";

	std::cout << "\n";

	myVariable = 3;
	std::cout << myVariable << "\n";
	duplicate(myVariable);
	std::cout << myVariable << "\n";
	duplicate(myVariable);
	std::cout << myVariable << "\n";

	sum(3.f, 4.f); // 7f; check out functions.hpp to see why this is here

	std::cout << "\n";


	std::cout << &myVariable << "\n";
	std::cout << &referenceToMyVariable << "\n";

	int* myPointer = &myVariable;
	std::cout << myPointer << "\n";
	std::cout << &myPointer << "\n";
	std::cout << *myPointer << "\n";

	std::cout << "\n";

	// NULL is an int, while nullptr is std::nullptr_t, which makes the probability of having a bug related to nullptr smaller
	myPointer = nullptr;
	std::cout << myPointer << "\n";
	std::cout << &myPointer << "\n";
	// std::cout << *myPointer << "\n"; // zsh: segmentation fault
	myPointer = NULL;
	std::cout << myPointer << "\n";
	std::cout << &myPointer << "\n";
	// std::cout << *myPointer << "\n"; // zsh: segmentation fault
}