#include<iostream>

using namespace std;

// pass by value
void my_function(int x) {

	x = 50;
	cout << "Value of x from my_function: " << x << endl;
}

int main() {

	int x = 10;
	my_function(x);
	cout << "Value of x from main function: " << x << endl;
}