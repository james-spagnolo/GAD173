#include<iostream>

using namespace std;

// pass by reference
void my_newfunction(int* x) {

	*x = 99;
	cout << "Value of x from my_newfunction: " << *x << endl;
}

int main() {

	int x = 10;
	my_newfunction(&x);
	cout << "Value of x from main function: " << x << endl;
}