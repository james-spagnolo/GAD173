// my first program in C++
#include <iostream>
#include <time.h>

using namespace std;

int main()
{
	srand(time(NULL));

	// declare an int variable
	int i;

	// initialise the int variable
	i = rand();

	cout << "i = " << i << endl;

	// output the addess of the variable i
	cout << "address of i = " << &i << endl;

	cin.get();

	// declare a pointer to an integer
	int* p;

	// intialise the integer pointer variable
	p = &i;

	// output the value of the integer pointer p
	cout << "p = " << p << endl;

	// output the contents of the address pointed to by p
	cout << "*p = " << *p << endl;

	cin.get();

	/******************Pointers and Arrays***********************/
	cout << "\n\n******************Pointers and Arrays*****************" << endl << endl;
	const int ROW = 12;
	const int COL = 7;

	cout << "\n\n**********************Static Arrays**********************" << endl << endl;
	// declare an array of integers
	int myIntegers[COL];

	// populate the integer array
	cout << "statically allocated array:\n\n";
	for (int e : myIntegers) {
		e = rand();
		cout << e << "\t";
	}

	// this is statically allocated memory, 
	// and cannot be freed during program execution

	cin.get();

	// now let's use pointers for dynamic memory allocation, 
	// which can be freed during program execution

	cout << "\n\n*********************1D dynamic arrays********************" << endl << endl;

	// declare a pointer to an integer
	int* q;

	// dynamically allocate memory for an array of integers
	q = new int[COL];

	// populate this dynamically allocated array
	cout << "\n\ndynamically allocated array:\n\n";
	for (int i = 0; i < COL; ++i) {
		q[i] = rand();
		cout << "q[" << i << "] = " << q[i] << "\t";
	}

	// release the memory for the dynamically allocated array
	delete q;

	cin.get();

	cout << "\n\n**************2D dynamic arrays************************" << endl << endl;

	// now let's use pointers for dynamic memory allocation, 
	// which can be freed during program execution

	int** r;

	bool** col;

	// dynamically allocate memory for an array of pointers to integers
	r = new int* [ROW];

	// dynamically allocate memory for arrays of integers
	for (int i = 0; i < ROW; ++i) {
		r[i] = new int[COL];
	}

	// initialise and output the 2D array
	for (int i = 0; i < ROW; ++i) {
		for (int j = 0; j < COL; ++j) {
			r[i][j] = rand();
			cout << r[i][j] << "\t";
		}
		cout << endl;
	}

	// release the memory for the dynamically allocated array
	for (int i = 0; i < ROW; ++i) {
		delete[] r[i];
	}

	delete[] r;

	cin.get();
}