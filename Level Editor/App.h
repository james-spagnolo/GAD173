#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

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
	int sign;

	//Bricks
	sf::RectangleShape Bricks[ROWS][COLS];
	bool collidable[ROWS][COLS];
	float brickHeight;
	float brickWidth;
	float gap;
	float edgeGap;

	//Game Logic and UI
	sf::Font font;
	sf::Text scoreText;
	sf::Text livesText;
	sf::Text endText;
	sf::Color scoreColour;
	sf::Color livesColour;

	bool endState;
	bool playingSoundEffect;
	int state;
	int playerScore;
	int playerLives;
	int brickPoints;
	int bricksLeft;

	
	//Music and Audio
	sf::SoundBuffer brickBuffer;
	sf::Sound brickSound;

	sf::SoundBuffer gameOverBuffer;
	sf::Sound gameOverSound;

	sf::SoundBuffer winScreenBuffer;
	sf::Sound winScreenSound;

	sf::SoundBuffer missedBallBuffer;
	sf::Sound missedBallSound;

	sf::Music gameMusic;



public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);
	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
};