#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;


int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Hello SFML World!");
	sf::RectangleShape button(sf::Vector2f(100.0f, 50.0f));
	button.setPosition(300, 200);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// handle LMB
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);
				if (button.getGlobalBounds().contains(sf::Vector2f(localPosition))) {
					cout << "Hello World!" << endl;

					//TASK: generate random colour of button mouse click

					int r = rand() % 256;
					int g = rand() % 256;
					int b = rand() % 256;

					sf::Color randomColour = sf::Color::Color(r, g, b, 255);

					button.setFillColor(randomColour);
				}
			}
		}
		window.clear();
		window.draw(button);
		window.display();
	}
	return EXIT_SUCCESS;
}