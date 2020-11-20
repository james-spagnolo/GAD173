// basic file operations
#include <iostream>
#include <fstream>

using namespace std;

int main() {

	int x, y;
	x = 99;
	y = 101;

	// declare
	ofstream myfile;

	// open
	myfile.open("example.txt");

	// write to file
	myfile << x << "\t" << y;

	// close
	myfile.close();

	return 0;
}

// TASK: Write the position of a single brick to file, in the crimson zombie framework