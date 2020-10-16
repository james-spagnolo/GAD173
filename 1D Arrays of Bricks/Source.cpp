#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow Window(sf::VideoMode(640, 480), "SFML works!");

	const int N = 5;
	float Length = 100;
	float height = 30;

	float gap = 20;
	float edgeGap = (Window.getSize().x - N * Length - (N - 1) * gap) / 2;

	// check 2d+N*Length+(N-1)*separation = WindowSize
	// declare an array of bricks
	sf::RectangleShape Bricks[N];

	// initialise the bricks
	for (int i = 0; i < N; ++i)
	{
		Bricks[i].setSize(sf::Vector2f(Length, height));
		Bricks[i].setPosition(edgeGap + i * (Length + gap), 150);
	}


	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				Window.close();
		}

		Window.clear();

		// draw the bricks
		for (sf::RectangleShape x : Bricks)
		{
			Window.draw(x);
		}

		Window.display();
	}

	return 0;
}
// TASK: create three rows of bricks
// with vertical and horizontal separation