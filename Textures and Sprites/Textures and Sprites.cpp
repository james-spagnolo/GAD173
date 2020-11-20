#include <SFML/Graphics.hpp>
int main()
{
	sf::RenderWindow window(sf::VideoMode(640, 480), "Hello SFML World!");
	sf::CircleShape circle(100.0f);
	sf::Texture texture;
	sf::Sprite blueBrick;
	sf::Sprite paddle;

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

		/*
		//Game Logic
		if (!texture.loadFromFile("Assets/Textures/Breakout.png", sf::IntRect(0, 0, 32, 32)))
		{
			//Error
		}
		*/

		if (!texture.loadFromFile("Assets/Textures/Breakout_Pieces.png"))
		{
			//Error
		}


		blueBrick.setTexture(texture);
		blueBrick.setTextureRect(sf::IntRect(8, 8, 32, 16));
		blueBrick.setPosition(window.getSize().x / 2, window.getSize().y / 2);

		paddle.setTexture(texture);
		paddle.setTextureRect(sf::IntRect(48, 92, 64, 16));
		paddle.setPosition(window.getSize().x / 2, window.getSize().y / 2 + 20);

		//Rendering
		window.clear();
		window.draw(circle);
		window.draw(blueBrick);
		window.draw(paddle);
		window.display();
	}
	return EXIT_SUCCESS;
}