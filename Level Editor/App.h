#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <fstream>

using namespace std;

const int ROWS = 3;
const int COLS = 5;
const int buttonIndex = 2;
const int brickIndex = 6;
const int paddleIndex = 3;
const int ballIndex = 6;


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
	sf::Sprite paddleSprite[paddleIndex];
	float paddleWidth;
	float paddleHeight;
	float scaledPaddleWidth;
	float scaledPaddleHeight;
	float speed;
	float paddleScale;



	//Ball
	sf::Sprite ballSprite[ballIndex];
	float radius;
	float ballScale;
	float scaledRadius;
	float xSpeed;
	float ySpeed;
	float xStart;
	float yStart;
	float pad;
	int sign;
	int currentBall;



	//Bricks
	//sf::RectangleShape Bricks[ROWS][COLS];
	//bool collidable[ROWS][COLS];
	float brickHeight;
	float brickWidth;
	
	double brickScale;
	float scaledBrickHeight;
	float scaledBrickWidth;

	float gap;
	float edgeGap;

	//Pointer
	sf::Sprite** bricks;
	bool** collidable;


	//Menu Screen
	
	float buttonHeight;
	float buttonWidth;
	float buttonX;
	float buttonY;

	sf::RectangleShape button[buttonIndex];
	sf::Text buttonText[buttonIndex];
	
	


	//Game Logic and UI
	sf::Font font;
	sf::Text scoreText;
	sf::Text livesText;
	sf::Text endText;
	sf::Text mainText;
	sf::Color scoreColour;
	sf::Color livesColour;

	bool endState;
	bool playingSoundEffect;
	float textGap;
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


	ofstream myfile;


	//Textures and Sprites
	sf::Texture breakoutParts;
	sf::Sprite brickSprite[brickIndex];
	

	int currentPaddle;

	enum class States { game, gameOver, winScreen, menu, levelEditor};

	States gameState;


	//Custom Methods

	void RestartGame(); //Restarts Game

	//APP INIT
	void SetupPaddle(); //Initializes Paddle

	void SetupBall(); //Initializes Ball

	void SetupUI(); //Initializes UI

	void SetupAudio(); //Initializes Sounds & Music

	void SetupGraphics(); //Initializes Textures & Sprites

	void SetupGameLogic(); //Initializes Game Logic members

	void SetupArrayOfBricks(); //Initializes Bricks


	//APP UPDATE

	void CheckGameState(); //Checks Game state every frame

	void BallAndPaddle(); //Handles Ball & Paddle Movement

	void BallAndWall(); //Handles Ball & Wall collisions

	void BallAndBricks(); //Handles Ball & Brick collisions
	
	void RefreshUI(); //Refreshes UI every frame

	
	//APP EVENTS

	void ButtonEvent(); //Handles Buttons

	//APP DRAW

	void DrawGameState(); //Draws the current Game state
	



public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);
	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
};