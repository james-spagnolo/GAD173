#include <iostream>
#include "Rectangle.h"

using namespace std;

int main()
{
	Rectangle rectangle;

	rectangle.set_values(2, 3);
	cout << "area = " << rectangle.area() << endl;

	return EXIT_SUCCESS;
}