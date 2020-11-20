#include<iostream>

using namespace std;


// pass by value
int* my_function(int x) {

	x = 50;
	cout << "Value of x from my_function: " << x << endl;

	return &x;
}


int main() {

	int x = 10;

	int* p;

	cout << "address of x in main: " << &x << endl;

	p = my_function(x);

	cout << "address of x in my_function(): " << p << endl;

	cout << "p = " << p << endl;
}