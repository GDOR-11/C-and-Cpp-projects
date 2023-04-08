class MyClass {
	// private by default
	int counter = 3;

public:
	MyClass();
	MyClass(int num);
	~MyClass();

	void incrementCounterBy(int num);
	void setCounterTo(int num);
	int getCounter();
};