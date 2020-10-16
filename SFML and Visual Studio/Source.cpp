#include <SFML/Graphics.hpp>
int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Hello SFML World!");
	sf::CircleShape circle(100.0f);

	//Main Game Loop
	while (window.isOpen())
	{
		//User Input
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

		}

		//Game Logic

		//Rendering
		window.clear();
		window.draw(circle);
		window.display();
	}
	return EXIT_SUCCESS;
}