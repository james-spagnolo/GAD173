#pragma once

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <iostream>
#include <fstream>

#include "Brick.h"
#include "Item.h"
#include "Enemy.h"
#include "Ball.h"
#include "Paddle.h"

using namespace std;

const int ROWS = 3;
const int COLS = 8;
const int buttonIndex = 4;
const int brickIndex = 6;
const int paddleIndex = 3;
const int ballIndex = 6;
const int upgradeIndex = 3;


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
	
	Paddle paddle;

	sf::Sprite paddleSprite[paddleIndex];
	float paddleWidth;
	float paddleHeight;
	float scaledPaddleWidth;
	float scaledPaddleHeight;
	float speed;
	float paddleScale;



	//Ball

	Ball ball;

	sf::Sprite ballSprite[ballIndex];
	float radius;
	float ballScale;
	float scaledRadius;
	float ballSpeed;
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
	//sf::Sprite** bricks;
	//bool** collidable;


	Brick** bricks;

	Brick brick;


	//Items
	Item upgrade;

	enum class Upgrades { LargerPaddle, PaddleSpeed, LargerBall, FireBall };

	Upgrades upgradeState;

	sf::Clock spawnClock;
	sf::Time spawnTime;

	sf::Clock upgradeClock;
	sf::Time upgradeTime;

	sf::Text upgradeText;

	float upgradeLimit;

	int upgradeScore;

	bool upgradeSpawned;
	bool collectedUpgrade;
	bool hasUpgrade;
	bool showUpgradeText;


	Enemy enemy;
	Item enemyDrop;

	sf::Clock firingClock;
	sf::Time firingTime;

	sf::Clock animationClock;

	bool enemyMovingRight;
	bool enemyFiring;

	int currentEnemy = 0;

	
	//Menu Screen
	
	float buttonHeight;
	float buttonWidth;
	float buttonX;
	float buttonY;

	sf::RectangleShape button[buttonIndex];
	sf::Text buttonText[buttonIndex];
	
	


	//Game Logic and UI
	enum class States { game, gameOver, winScreen, menu, levelEditor };

	States gameState;


	sf::Font font;
	sf::Text scoreText;
	sf::Text livesText;
	sf::Text endText;
	sf::Text mainText;
	
	sf::Color scoreColour;
	sf::Color livesColour;

	
	bool playingSoundEffect;
	bool savingLevel;
	float textGap;
	int state;
	int playerScore;
	int playerLives;
	int brickPoints;
	int bricksLeft;
	int levelBricks;

	
	//Music and Audio
	sf::SoundBuffer brickBuffer;
	sf::Sound brickSound;

	sf::SoundBuffer gameOverBuffer;
	sf::Sound gameOverSound;

	sf::SoundBuffer winScreenBuffer;
	sf::Sound winScreenSound;

	sf::SoundBuffer missedBallBuffer;
	sf::Sound missedBallSound;

	sf::SoundBuffer paddleHitBuffer;
	sf::Sound paddleHitSound;

	sf::Music gameMusic;


	ofstream myfile;

	ofstream savedLevel;

	ifstream loadLevel;

	ifstream levelOne;
	ifstream levelTwo;


	//Textures and Sprites
	sf::Texture breakoutParts;
	sf::Sprite brickSprite[brickIndex];
	

	int currentPaddle;

	


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

	void SetupEnemy();



	//APP UPDATE

	void CheckGameState(); //Checks Game state every frame

	void BallAndPaddle(); //Handles Ball & Paddle Movement

	void BallAndWall(); //Handles Ball & Wall collisions

	void BallAndBricks(); //Handles Ball & Brick collisions

	void CheckBricks();

	void EnemyAI();

	void EnemyDropAndPaddle();

	void UpgradeSpawner(); //Checks when to spawn an upgrade

	void UpgradeAndPaddle(); //Handles Upgrade & Paddle collision
	
	void RefreshUI(); //Refreshes UI every frame

	void RefreshBricks();


	
	//APP EVENTS

	void ButtonEvent(); //Handles Buttons

	void SpawnUpgrade(); //Spawns Upgrade

	void Upgrade(); //Upgrades player

	void SaveLevel(); //Handles saving the level to disk in level editor

	void LoadLevel(); //Loads saved level

	void PlayLevel(); //Plays saved Level



	//APP DRAW

	void DrawGameState(); //Draws the current Game state

	void DrawButtons(); //Draws Buttons
	



public:
	App(const char* title, int screenWidth, int screenHeight, int screenBpp);
	~App();

	bool Init();
	void HandleEvents();
	void Draw();
	void Update();
	void Run();
};