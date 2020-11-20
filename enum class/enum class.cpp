#include <iostream>

using namespace std;

enum class Colors { red, orange, yellow, green = 0, blue, indigo, violet };

int main() {

	Colors myColor;

	myColor = Colors::red;

	if (myColor == Colors::red) {
		myColor = Colors::orange;
	}

	cout << "myColor is " << int(myColor) << endl;

	cout << "red is " << int(Colors::red) << endl;

	cout << "green is " << int(Colors::red) << endl;

	return EXIT_SUCCESS;
}