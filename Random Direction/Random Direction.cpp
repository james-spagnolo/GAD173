#include <iostream>
#include <time.h>

using namespace std;

int main() {

	srand(time(NULL));

	int sign;

	int xSpeed;
	int ySpeed;

	while (getchar())
	{
		//Generate Random Sign: -1 or +1
		sign = 2 * (rand() % 2) - 1;

		//cout << "sign = " << sign << endl;

		//Generate a Random Direction
		xSpeed = sign*(rand() % 100 + 500);
		ySpeed = -(rand() % 100 + 500);

		cout << "xSpeed = " << xSpeed << ", ySpeed = " << ySpeed << endl;
	}
}