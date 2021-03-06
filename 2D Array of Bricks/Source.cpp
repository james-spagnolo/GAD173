#include <SFML/Graphics.hpp>

int main()
{
	sf::RenderWindow Window(sf::VideoMode(640, 480), "2D Array of Bricks");

	const int ROWS = 3;
	const int COLS = 5;

	bool collidable[ROWS][COLS];

	float Length = 100;
	float height = 30;

	float gap = 20;
	float edgeGap = (Window.getSize().x - COLS * Length - (COLS - 1) * gap) / 2;

	// check 2d+N*Length+(N-1)*separation = WindowSize
	// declare an array of bricks
	sf::RectangleShape Bricks[ROWS][COLS];

	// initialise the bricks
	for (int row = 0; row < ROWS; ++row)
	{
		for (int col = 0; col < COLS; ++col)
		{
			Bricks[row][col].setSize(sf::Vector2f(Length, height));
			Bricks[row][col].setPosition(edgeGap + col * (Length + gap), gap + row * (height + gap));
			collidable[row][col] = true;
			Window.draw(Bricks[row][col]);
		}
		
	}


	while (Window.isOpen())
	{
		sf::Event event;
		while (Window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				Window.close();
			}
				

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i localPosition = sf::Mouse::getPosition(Window);

				for (int row = 0; row < ROWS; ++row) {
					
					for (int col = 0; col < COLS; ++col) {

						if (Bricks[row][col].getGlobalBounds().contains(sf::Vector2f(localPosition))) {
							
							collidable[row][col] = !collidable[row][col];
						}
					}
				}
				
			}
		}

		Window.clear();

		// draw the bricks
		for (int row = 0; row < ROWS; ++row)
		{
			for (int col = 0; col < COLS; ++col)
			{
				if (collidable[row][col])
				{
					Window.draw(Bricks[row][col]);
				}
				
			}
		}

		Window.display();
	}

	return 0;
}
// TASK: create three rows of bricks
// with vertical and horizontal separation