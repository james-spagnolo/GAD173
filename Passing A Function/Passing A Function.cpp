#include <iostream>

using namespace std;

// function prototypes
void func(void (*f)(int));

void print(int x);

float newFunction(float (*f)(float));

float floatFunction(float f);

float g = 5.0f;


int main()
{
	func(print);
	newFunction(floatFunction);
}


// function definition
void print(int x) {
	cout << x << endl;
}

// function definition
void func(void (*f)(int)) {
	for (int ctr = 0; ctr < 5; ctr++) {
		(*f)(ctr);
	}
}

/*
float floatFunction(float f) {

	f = 2 * f;

	return f;
}

float newFunction(float (*f)(float)) {

	for (int i = 0; i < 5; i++) {

		return (*f)(i);
	}
}

*/
