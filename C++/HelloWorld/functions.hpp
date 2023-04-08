int sum(int a, int b);
double sum(double a, double b);
std::string sum(std::string a, std::string b);

// templates can't be separated between hpp and cpp files
template <typename T>
T coolSum(T a, T b) {
	return a + b;
}

void duplicate(int &num);
float sum(const float &a, const float &b); // including the & saves performance, as it prevents the compiler from creating a copy of the object; const doesn't seem to matter for performance, only for readability