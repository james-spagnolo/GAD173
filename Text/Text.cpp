#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

int main()
{
	// start the clock
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");

	float radius = 50;
	sf::CircleShape circle(radius);

	circle.setPosition(0, 300);
	circle.setFillColor(sf::Color::Green);
	float xSpeed = 350;

	// text
	sf::Font font;
	if (!font.loadFromFile("Assets/Fonts/arial.ttf"))
	{
		std::cout << "cannot load font" << std::endl;
	}

	sf::Text text;

	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString("0");
	text.setPosition(0.95 * window.getSize().x, 0);

	int numCollisions = 0;

	while (window.isOpen())
	{
		// compute deltaTime, duration of frame
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		//left border collision
		if (circle.getPosition().x < 0)
		{
			// reset position
			circle.setPosition(0, 300);
			// reverse speed
			xSpeed = -xSpeed;
			++numCollisions;
			text.setString(to_string(numCollisions));
		}

		// right border collision
		if (circle.getPosition().x > window.getSize().x - 2 * radius)
		{
			// reset position
			circle.setPosition(window.getSize().x - 2 * radius, 300);
			// reverse speed
			xSpeed = -xSpeed;
			++numCollisions;
			text.setString(to_string(numCollisions));
		}

		circle.move(xSpeed * deltaTime, 0);

		window.clear();
		window.draw(text);
		window.draw(circle);
		window.display();
	}

	return 0;
}