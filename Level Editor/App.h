#pragma once

#include "SFML/Graphics.hpp"

class App
{
private:
	sf::Event event;
	sf::View view;
	sf::RenderWindow window;
	sf::Clock clock;
	

	//Paddle
	sf::RectangleShape paddle;

	float paddleWidth;
	float paddleHeight;
	float speed;

	// Ball
	sf::CircleShape ball;
	float radius;
	float xSpeed;
	float ySpeed;

	float pad;

	//Brick
	sf::RectangleShape brick;

	float brickHeight;
	float brickWidth;
	
	bool collidable;


	//Measures duration of last frame
	float deltaTime;

public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);
	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
};