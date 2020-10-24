#pragma once

#include "SFML/Graphics.hpp"

const int ROWS = 3;
const int COLS = 5;

class App
{
private:
	sf::Event event;
	sf::View view;
	sf::RenderWindow window;
	sf::Clock clock;

	//Measures duration of last frame
	float deltaTime;

	//Paddle
	sf::RectangleShape paddle;
	float paddleWidth;
	float paddleHeight;
	float speed;

	//Ball
	sf::CircleShape ball;
	float radius;
	float xSpeed;
	float ySpeed;
	float xStart;
	float yStart;
	float pad;

	//Bricks
	sf::RectangleShape Bricks[ROWS][COLS];
	bool collidable[ROWS][COLS];
	float brickHeight;
	float brickWidth;
	float gap;
	float edgeGap;

	//Game Logic
	sf::Font font;
	sf::Text scoreText;
	sf::Text livesText;
	sf::Color scoreColour;
	int playerScore;
	int playerLives;
	int brickPoints;
	int bricksLeft;



public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);
	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
};