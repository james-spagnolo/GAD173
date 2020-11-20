#include<iostream>

using namespace std;

const int N = 5;

const int ROWS = 5;
const int COLS = 5;

// pass by reference
void my_function(int* x, int P) {

	for (int i = 0; i < P; ++i) {

		*(x + i) = i;
	}
}

int main() {

	int x[N] = { 10, 20, 30, 40, 50 };

	int y[ROWS][COLS] = { {10, 20, 30, 40, 50}, {60, 70, 80, 90, 100} };

	cout << "before method call\n";

	for (int row = 0; row < ROWS; ++row) {

		for (int col = 0; col < COLS; ++col) {

			cout << *(*(y + col) + row) << endl;
		}
	}


	/*
	for (int i = 0; i < N; ++i) {

		cout << *(x + i) << endl;
	}
	*/


	my_function(x, N);

	cout << "after method call\n";

	for (int i = 0; i < N; ++i) {

		cout << *(x + i) << endl;
	}

	
}






/// TASK: declare, initialize and output a 2D array in main function
/// pass the 2D array into a custom function, modify it
/// output in custom function, and output in the main function