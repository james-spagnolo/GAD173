#include "App.h"
#include "Brick.h"

//using namespace std;

//Constructor
App::App(const char* title, int screenWidth, int screenHeight, int screenBpp)
{
	window.create(sf::VideoMode(screenWidth, screenHeight, screenBpp), title);
	window.setFramerateLimit(0);
	view = window.getDefaultView();
}

//Destructor
App::~App()
{
	//Release Memory
	//myfile.close();

	savedLevel.close();

	// release the memory for the dynamically allocated array
	for (int i = 0; i < ROWS; ++i) {

		//delete[] bricks[i];
		//delete[] collidable[i];

		//delete[] b[i];

		delete[] bricks[i];
	}

	//delete[] bricks;
	//delete[] collidable;

	//delete[] b;

	delete[] bricks;
}


bool App::Init()
{
	//Initialize App data members
	

	//myfile.open("Brick Positions.txt");

	fstream savedLevel("SavedBricks.txt", ios::out);

	//savedLevel.open("Saved Bricks.txt");

	//savedLevel.open("Saved Level Brick Positions.txt");

	SetupGraphics(); //Initializes Sprites & Textures

	SetupAudio(); //Initializes Sounds & Audio

	SetupPaddle(); //Initializes Paddle

	SetupBall(); //Initializes Ball

	SetupUI(); //Initializes User Interface

	SetupGameLogic(); //Initializes GameLogic members

	SetupArrayOfBricks(); //Initializes 2D array of Bricks

	SetupEnemy();
	

	return true;
}




void App::Update()
{
	
	CheckGameState(); //Checks GameState every frame

	if (gameState == States::game) {

		BallAndPaddle(); //Handles Ball & Paddle Movement / Collision

		BallAndWall(); //Handles Ball & Wall collisions

		BallAndBricks(); //Handles Ball & Brick Collisions
	}
	

	CheckBricks();

	UpgradeAndPaddle();

	EnemyAI();
		
	RefreshUI(); //Refresh UI every frame

	//RefreshBricks();

	SaveLevel();

	

}



void App::Draw()
{
	window.clear();
	window.setView(view);

	/// DRAW ///
	
	DrawGameState(); //Draw current game state

	window.display();
	
}



void App::HandleEvents()
{
	if (event.type == sf::Event::Closed)
		window.close();

	// Other Keyboard, Mouse Events

	ButtonEvent();

	if (gameState == States::game) {

		UpgradeSpawner();

		if (collectedUpgrade) {

			upgradeClock.restart();
			collectedUpgrade = false;
		}
	}

	// Mouse events with level editor
	if (gameState == States::levelEditor)
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			sf::Vector2i localPosition = sf::Mouse::getPosition(window);

			for (int row = 0; row < ROWS; ++row)
			{
				for (int col = 0; col < COLS; ++col)
				{
					if (bricks[row][col].sprite.getGlobalBounds().contains(sf::Vector2f(localPosition)))
					{
						bool click = false;

						if (!click) {

							bricks[row][col].SetCollidable(!bricks[row][col].GetCollidable());
							click = true;
						}
						
					}
				}
			}

		}
	}
}




void App::Run()
{
	while (window.isOpen())
	{
		deltaTime = clock.restart().asSeconds();
		
		while (window.pollEvent(event))
		{
			HandleEvents();
		}
		Update();
		Draw();
	}
}







// Initializes game

void App::SetupPaddle() {

	//Paddle Variables
	

	pad = 25; //Padding used to smoothen Collision Detection

	paddle.setPosition(0.5f*window.getSize().x - 0.5f*paddle.GetSize().x, window.getSize().y - (1.2f * paddle.GetSize().y));

	//paddle.setSize(paddle.GetSize());

	//Sets paddle values to default values (makes sure upgrades don't carry over when player "retries"
	paddle.setSize(paddle.GetDefaultSize());
	paddle.SetSpeed(paddle.GetDefaultSpeed());


	paddle.sprite.setPosition(paddle.getPosition());

	/*
		//Setup Paddle Sprites
	for (int i = 0; i < paddleIndex; i++) {


		paddleSprite[i].setTexture(breakoutParts);
		paddleSprite[i].setScale(sf::Vector2f(paddleScale, paddleScale));
		paddleSprite[i].setPosition(window.getSize().x / 2, window.getSize().y * 0.8);


		paddleSprite[i].setTextureRect(sf::IntRect(8 + i * 68, 151, paddleWidth, paddleHeight));
	}
	*/
	

	currentPaddle = rand() % (paddleIndex + 1);

	paddle.SetSprite(currentPaddle);

	
}



void App::SetupBall() {

	
	ballSpeed = rand() % 100 + 400;

	xSpeed = ballSpeed; //Ball Speed (randomised for variation)
	ySpeed = -ballSpeed; //Ball Speed (randomised for variation)


	xStart = paddle.sprite.getPosition().x + 0.5f*paddle.GetSize().x - ball.GetRadius();
	yStart = paddle.sprite.getPosition().y + (2*ball.GetRadius());

	ball.setPosition(xStart, yStart);

	ball.sprite.setPosition(ball.getPosition());

	ball.setRadius(ball.GetDefaultRadius());
	ball.SetPower(ball.GetDefaultPower());

	

	currentBall = rand() % (ballIndex + 1);

	ball.SetSprite(currentBall);

	
}



void App::SetupUI() {

	//Font Setup
	font.loadFromFile("Assets/Fonts/RetroGaming.ttf");


	textGap = 50;

	//Score Text Setup
	scoreColour = sf::Color::White;

	scoreText.setPosition(textGap, 0);
	scoreText.setString("Score: " + to_string(playerScore));
	scoreText.setFont(font);
	scoreText.setFillColor(scoreColour);
	scoreText.setCharacterSize(24);



	//Live Text Setup
	livesColour = sf::Color::Green;

	livesText.setPosition(scoreText.getPosition().x + 4 * textGap, 0);
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFont(font);
	livesText.setFillColor(livesColour);
	livesText.setCharacterSize(24);



	//End Screen Text Setup
	endText.setPosition(window.getSize().x / 4, window.getSize().y / 8);
	endText.setString("");
	endText.setFont(font);
	endText.setFillColor(sf::Color::Red);
	endText.setCharacterSize(48);


	//Main Menu Text Setup
	mainText.setPosition(window.getSize().x / 3, window.getSize().y / 4);
	mainText.setString("Breakout");
	mainText.setFont(font);
	mainText.setFillColor(sf::Color::White);
	mainText.setCharacterSize(48);

	//Upgrade Text Setup
	upgradeText.setFont(font);
	upgradeText.setPosition(window.getSize().x / 2, paddle.getPosition().y - 2*upgradeText.getGlobalBounds().height);
	upgradeText.setString("");
	upgradeText.setFillColor(sf::Color::Blue);
	upgradeText.setCharacterSize(48);

	//Button Setup

	buttonWidth = 200;
	buttonHeight = 50;
	buttonX = (window.getSize().x / 2) - (buttonWidth / 2);


	//Button Setup
	for (int i = 0; i < (buttonIndex - 1); i++) {

		button[i].setPosition(sf::Vector2f(buttonX, mainText.getPosition().y + (1.5 * (buttonHeight + (i * buttonHeight)))));
		button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button[i].setFillColor(sf::Color::White);

		buttonText[i].setPosition(sf::Vector2f(button[i].getPosition().x + (buttonWidth / 4), button[i].getPosition().y + (buttonHeight / 4)));
		buttonText[i].setFont(font);
		buttonText[i].setFillColor(sf::Color::Black);
		buttonText[i].setCharacterSize(24);
	}


	//Level Editor Button Setup
	if (gameState == States::levelEditor) {

		for (int i = 0; i < buttonIndex; i++) {

			button[i].setPosition(sf::Vector2f(buttonWidth * i, window.getSize().y - buttonHeight));
			button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
			button[i].setFillColor(sf::Color::White);


			buttonText[i].setPosition(sf::Vector2f(button[i].getPosition().x + (buttonWidth / 4), button[i].getPosition().y + (buttonHeight / 4)));
			buttonText[i].setFont(font);
			buttonText[i].setFillColor(sf::Color::Black);
			buttonText[i].setCharacterSize(24);

		}
	}
	
}



void App::SetupAudio() {

	//Setup Sound files
	if (!brickBuffer.loadFromFile("Assets/Audio/BrickBreak.wav")) {
		cout << "Couldn't load BrickBreak.wav";
	}

	if (!missedBallBuffer.loadFromFile("Assets/Audio/MissedBall.wav")) {
		cout << "Couldn't load MissedBall.wav";
	}

	if (!gameOverBuffer.loadFromFile("Assets/Audio/GameOver.ogg")) {
		cout << "Couldn't load GameOver.ogg";
	}

	if (!winScreenBuffer.loadFromFile("Assets/Audio/WinSound.ogg")) {
		cout << "Couldn't load WinSound.ogg";
	}

	if (!paddleHitBuffer.loadFromFile("Assets/Audio/PaddleHit.wav")) {
		cout << "Couldn't load PaddleHit.wav";
	}

	if (!gameMusic.openFromFile("Assets/Audio/GameMusic.wav")) {
		cout << "Couldn't load GameMusic.wav";
	}


	brickSound.setBuffer(brickBuffer);
	missedBallSound.setBuffer(missedBallBuffer);
	gameOverSound.setBuffer(gameOverBuffer);
	winScreenSound.setBuffer(winScreenBuffer);
	paddleHitSound.setBuffer(paddleHitBuffer);


	//Play Game Music on Initialization
	//gameMusic.play();

}



void App::SetupGraphics() {

	//Setup Texture files
	if (!breakoutParts.loadFromFile("Assets/Textures/Breakout_Pieces.png"))
	{
		//Error
	}

	

}



void App::SetupGameLogic() {

	//Game Logic Variables
	playerScore = 0; //Tracks Players Score - Starts at 0 and increases

	playerLives = 6; //Tracks Players Lives - Starts at 5 and drops to 0

	brickPoints = 10; //Tracks how many points Bricks are worth

	bricksLeft = 0; //Will track bricks remaining

	//state = 3; //Tracks the Game State - 0 = Play State

	gameState = States::menu; //Start at menu state

	savingLevel = false;

	playingSoundEffect = false; //Tracks if a sound effect is being played

	upgradeSpawned = false;
	collectedUpgrade = false;
	hasUpgrade = false;

	upgradeLimit = 10.0f;
	upgradeScore = 25;
}



void App::SetupArrayOfBricks() {

	//Brick Variables
	
	//Sets up gaps between bricks
	gap = brick.GetSize().x / 2;
	edgeGap = (window.getSize().x - COLS * brick.GetSize().x - (COLS - 1) * gap) / 2;
	
	bricksLeft = 0;

	//bricks = new sf::Sprite * [ROWS];
	//collidable = new bool* [ROWS];

	bricks = new Brick * [ROWS];

	for (int row = 0; row < ROWS; ++row) {

		bricks[row] = new Brick[COLS];
	}

	//Setup the Array of Bricks
	for (int row = 0; row < ROWS; ++row)
	{

		//For every Row and Collumn (or for every brick)
		for (int col = 0; col < COLS; ++col)
		{
			


			//Sets up array of bricks
			bricks[row][col].setPosition(edgeGap + col * (bricks[row][col].GetSize().x + gap), 4*gap + row * (bricks[row][col].GetSize().y + gap));

			bricks[row][col].setSize(bricks[row][col].GetSize());


			//Sets the position of each brick
			bricks[row][col].sprite.setPosition(bricks[row][col].getPosition());


			int randomHealth = rand() % (bricks[row][col].GetMaxHealth()) + 1;

			bricks[row][col].SetHealth(randomHealth);

			bricks[row][col].SetCollidable(true);

			//cout << brick[row][col].GetHealth() << endl;

			RefreshBricks();

			//if (brick[row][col].GetCollidable()) { bricksLeft += 1; }
			

			//window.draw(bricks[row][col].sprite);

			

			//myfile << brick[row][col].sprite.getPosition().x << "\t" << brick[row][col].sprite.getPosition().y << "\n";

			//brick[row][col].SetCollidable(true);


			

			//cout << brick[row][col].health << "\n";

			//Picks a random sprite for the brick
			//int randomBrick = rand() % brickIndex;

			
			//cout << bricks[ROWS - 1][COLS - 1].GetCollidable();
			

			//brick[row][col].SetSprite(brick[row][col].health);

			//cout << brick[row][col].collidable << "\t";

			//collidable[row][col] = true; //Set the brick to be destroyable

			//collidable[row][col] = true; //Set the brick to be destroyable

			
		}

	}
}



void App::SetupEnemy() {

	enemy.setPosition(0.5f * window.getSize().x - 0.5f * enemy.GetSize().x, livesText.getGlobalBounds().height);
	enemy.sprite.setPosition(enemy.getPosition());

	enemy.SetSpriteSheet(0);

	enemyMovingRight = true;
	enemyFiring = false;

	enemyDrop.SetCollidable(false);

	enemyDrop.SetSprite(5);

	enemyDrop.sprite.setScale(2, 2);

	enemyDrop.setPosition(enemy.sprite.getPosition());
	enemyDrop.sprite.setPosition(enemyDrop.getPosition());

}





//Runs in Update

void App::CheckGameState() {

	//Check the Game State
	switch (gameState)
	{
	case States::game:
		// State 0 = Play State
		endText.setString("");

		
		break;

	case States::gameOver:
		// State 1 = Game Over
		endText.setString("Game Over \nBricks Left: " + to_string(bricksLeft));
		endText.setFillColor(sf::Color::Red);

		gameMusic.stop(); //Music stops playing

		if (!playingSoundEffect)
		{
			gameOverSound.play(); //Play Game Over sound effect
			playingSoundEffect = true; //Makes sure sound effect plays only once
		}
		break;

	case States::winScreen:
		// State 2 = Win Screen
		endText.setString("You Win! \nScore: " + to_string(playerScore));
		scoreColour = sf::Color::Green;
		endText.setFillColor(scoreColour);
		
		gameMusic.stop(); //Music stops playing

		if (!playingSoundEffect)
		{
			winScreenSound.play(); //Play Win Screen sound effect
			playingSoundEffect = true; //Makes sure sound effect plays only once
		}

		break;

	case States::menu:
		// State 3 = Menu State
		

	case States::levelEditor:
		// State 4 = Level Editor

	default:
		break;
	}




	//Check if the Player has run out of lives
	if (playerLives <= 0)
	{
		//Launch Game Over State
		gameState = States::gameOver;
	}
}



void App::BallAndPaddle() {

		// Left Paddle Movement - (Left Key Pressed)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{

			// Checks if the paddle's position is greater than the left bound of the screen
			if (paddle.sprite.getPosition().x >= 0)
			{

				// Moves the paddle towards the left by paddle speed
				// Multiplied by deltaTime to keep it frame rate dependant
				paddle.sprite.move(-paddle.GetSpeed() * deltaTime, 0);
			}
		}


		// Right Paddle Movement - (Right Key Pressed)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{

			// Checks that the paddles right bound won't intersect the right bound of the screen
			if (paddle.sprite.getPosition().x <= window.getSize().x - paddle.GetSize().x)
			{

				// Moves the paddle towards the right by paddle speed
				// Multiplied by deltaTime to keep it frame rate dependant
				paddle.sprite.move(paddle.GetSpeed() * deltaTime, 0);
			}

		}


		// Move the ball based on speed and frame rate
		ball.sprite.move(xSpeed * deltaTime, ySpeed * deltaTime);


	// Check if the Ball collided with the Paddle
	if (ball.sprite.getGlobalBounds().intersects(paddle.sprite.getGlobalBounds())) {


		brickSound.play(); // Play Paddle Sound


		// Ball hits Left or right border
		if (
			ball.sprite.getPosition().x > paddle.sprite.getPosition().x + paddle.GetSize().x - pad ||
			ball.sprite.getPosition().x + 2 * ball.GetRadius() < paddle.sprite.getPosition().x + pad
			)
		{
			// Reset position

			// Reverse x speed
			xSpeed = -xSpeed;
		}


		// Top or bottom border
		if (
			ball.sprite.getPosition().y + 2 * ball.GetRadius() < paddle.sprite.getPosition().y + pad ||
			ball.sprite.getPosition().y > paddle.sprite.getPosition().y + ball.GetRadius() - pad
			)
		{
			// Reset position

			//If the middle of the ball hits left side of the paddle
			if (ball.sprite.getPosition().x + ball.GetRadius() < paddle.sprite.getPosition().x + (paddle.GetSize().x / 2)) {

				xSpeed = -ballSpeed;
			}

			//If the middle of the ball hits the right side of the paddle
			else if (ball.sprite.getPosition().x + ball.GetRadius() > paddle.sprite.getPosition().x + (paddle.GetSize().x / 2)) {

				xSpeed = ballSpeed;
			}

			// Reverse y speed
			ySpeed = -ySpeed;
		}

	}
}



void App::BallAndWall() {

	// Left border collision detection
	// Detects when the ball's left bound meets the screens left bound
	if (ball.sprite.getPosition().x <= 0)
	{
		// Reset Position of Ball
		ball.sprite.setPosition(0, ball.sprite.getPosition().y);

		// Reverse horizontal velocity of the ball
		xSpeed = -xSpeed;
	}


	// Right border collision detection
	// Detects when the ball's right bound meets the screens right bound
	if (ball.sprite.getPosition().x >= window.getSize().x - 2 * ball.getRadius())
	{
		// Reset Position of Ball
		ball.sprite.setPosition((window.getSize().x - 2 * ball.getRadius()), ball.sprite.getPosition().y);

		// Reverse horizontal velocity of the ball
		xSpeed = -xSpeed;
	}


	// Top border collision detection
	// Detects when the ball's upper bound meets the screens upper bound
	if (ball.sprite.getPosition().y <= 0)
	{

		// Reset Position of Ball
		ball.sprite.setPosition(ball.sprite.getPosition().x, 0);

		// Reverse the vertical velocity of the ball
		ySpeed = -ySpeed;
	}


	// Bottom border collision detection
	// Detects when the ball's lower bound meets the screens lower bound
	if (ball.sprite.getPosition().y >= window.getSize().y - 2 * ball.getRadius()) {

		// Checks the game hasn't ended
		if (playerLives > 0)
		{
			playerLives -= 1; //Player loses a life
		}

		missedBallSound.play(); //Play Missed Ball sound effect

		currentBall = rand() % ballIndex;

		ball.SetSprite(currentBall);

		// Reset the ball's position to be on top of the the middle of the paddle
		ball.sprite.setPosition(paddle.sprite.getPosition().x + (paddle.getSize().x / 2) - ball.getRadius(), paddle.sprite.getPosition().y - (2 * ball.getRadius() + pad));

		// Send the ball towards the bricks
		ySpeed = -ySpeed;
	}
}



void App::BallAndBricks()   {

	//For every row and collumn (for every brick)
	for (int row = 0; row < ROWS; ++row)
	{

		for (int col = 0; col < COLS; ++col)
		{

			// Check IF the ball has hit the brick AND it hasn't already been hit
			if (ball.sprite.getGlobalBounds().intersects(bricks[row][col].sprite.getGlobalBounds()) && bricks[row][col].GetCollidable())
			{

				// Destroy the brick - (It will no longer be collidable)
				//collidable[row][col] = false;
				int startHealth = bricks[row][col].GetHealth();

				bricks[row][col].ChangeHealth(-ball.GetPower());

				int endHealth = bricks[row][col].GetHealth();

				playerScore += (startHealth - endHealth) * brickPoints;

				RefreshBricks();

				//brick[row][col].SetSprite(brick[row][col].health);

				if (bricks[row][col].GetCollidable() == false) {

					bricksLeft -= 1; //Bricks remaining reduced by 1

					cout << "Bricks Left: " << bricksLeft << endl;
				}

				//brick[row][col].SetCollidable(false);


				brickSound.play(); // Play Brick Break Sound


				//playerScore += brickPoints; //Score increased by brick point worth


				// Check if all Bricks have been hit
				if (bricksLeft <= 0)
				{
					//Launch the Win State
					gameState = States::winScreen;
				}

				// Check IF the ball has hit the bricks Left OR Right border
				if (
					ball.sprite.getPosition().x > bricks[row][col].sprite.getPosition().x + bricks[row][col].getSize().x - pad ||
					ball.sprite.getPosition().x + 2*ball.getRadius() < bricks[row][col].sprite.getPosition().x + pad
					)
				{
					// Reset position

					// Reverse x (horizontal) velocity of ball
					xSpeed = -xSpeed;
				}

				// Check IF the ball has hit the bricks Top OR Bottom border
				if (
					ball.sprite.getPosition().y + 2*ball.getRadius() < bricks[row][col].sprite.getPosition().y + pad ||
					ball.sprite.getPosition().y > bricks[row][col].sprite.getPosition().y + bricks[row][col].getSize().y - pad
					)
				{
					// Reset position

					// Reverse y (vertical) velocity of ball
					ySpeed = -ySpeed;
				}

			}
		}
	}

}



void App::CheckBricks() {

	bricksLeft = 0;

	for (int row = 0; row < ROWS; row++) {

		for (int col = 0; col < COLS; col++) {

			if (bricks[row][col].GetCollidable()) {

				bricksLeft++;
			}
		}
	}
}



void App::UpgradeAndPaddle() {

	

	if (gameState == States::game) {

		//Moves the upgrade down if it's collidable
		if (upgrade.GetCollidable()) {

			upgrade.sprite.move(0, upgrade.GetSpeed() * deltaTime);
		}

		//Checks if the upgrade and paddle collide
		if (upgrade.sprite.getGlobalBounds().intersects(paddle.sprite.getGlobalBounds())) {

			if (upgrade.GetCollidable()) {

				upgrade.SetCollidable(false);

				collectedUpgrade = true;

				hasUpgrade = true;

				playerScore += upgradeScore;
				
			}
		}


		if (hasUpgrade) {

			Upgrade();
			upgradeClock.restart();
			hasUpgrade = false;

		}

		upgradeTime = upgradeClock.getElapsedTime();

		if (upgradeTime < upgrade.textTimer) {

			showUpgradeText = true;
		}
		else {

			showUpgradeText = false;
		}
		
	}
}



void App::EnemyDropAndPaddle() {

	if (enemyDrop.sprite.getGlobalBounds().intersects(paddle.sprite.getGlobalBounds())) {

				playerLives -= 1;

				playerScore -= upgradeScore;

				paddleHitSound.play();

				enemyDrop.SetCollidable(false);
			
	}
}



void App::EnemyAI() {

	if (gameState == States::game) {

		

		firingTime = firingClock.getElapsedTime();

		if (enemy.GetCollidable()) {


			if (firingTime >= enemy.GetFireRate()) {

				enemyFiring = true;

				//cout << firingTime.asSeconds();
			}


			if (enemyFiring) {

				enemyDrop.SetCollidable(true);

				enemyDrop.sprite.setPosition(enemy.sprite.getPosition());

				//cout << enemyDrop.sprite.getPosition().x << "\t" << enemyDrop.sprite.getPosition().y << endl;

				firingClock.restart();

				enemyFiring = false;
			}

			else {

				//Enemy is moving

				if ((enemy.sprite.getPosition().x < (window.getSize().x - enemy.GetSize().x)) && enemyMovingRight) {

					//Move towards the right
					enemy.sprite.move(enemy.GetSpeed() * deltaTime, 0);

					if (enemy.sprite.getPosition().x > (window.getSize().x - 1.05f * enemy.GetSize().x)) {

						enemyMovingRight = false;
					}
				}

				if ((enemy.sprite.getPosition().x > 0) && enemyMovingRight == false) {

					//Move towards the left
					enemy.sprite.move(-enemy.GetSpeed() * deltaTime, 0);

					if (enemy.sprite.getPosition().x < 0.05f * enemy.GetSize().x) {

						enemyMovingRight = true;
					}
				}
			}

			if (animationClock.getElapsedTime() >= enemy.GetSpriteDuration()) {

				if (enemy.GetSpriteCycle()) {

					enemy.rect.left += enemy.spriteSize;

					if (enemy.rect.left >= enemy.GetSpriteIndex() * enemy.spriteSize) {

						enemy.SetSpriteCycle(false);
					}
				}
				else {

					enemy.rect.left -= enemy.spriteSize;

					if (enemy.rect.left <= 0) {

						enemy.SetSpriteCycle(true);
					}
				}

				enemy.SetSpriteSheet(currentEnemy);
				animationClock.restart();
			}
			
		}



		if (enemyDrop.GetCollidable()) {

			EnemyDropAndPaddle();

			enemyDrop.sprite.move(0, enemyDrop.GetSpeed() * deltaTime);
		}
	}
}



void App::RefreshUI() {

	//Check how many lives the player has
	switch (playerLives)
	{
	case 5:
		//If 5 Lives, Lives Colour = Green
		livesColour = sf::Color::Green;
		break;
	case 4:
		//If 4 Lives, Lives Colour = Lime
		livesColour = sf::Color::Color(128, 255, 0);
		break;
	case 3:
		//If 3 Lives, Lives Colour = Yellow
		livesColour = sf::Color::Yellow;
		break;
	case 2:
		//If 2 Lives, Lives Colour = Orange
		livesColour = sf::Color::Color(255, 128, 0);
		break;
	case 1:
		//If 1 Live, Lives Colour = Red
		livesColour = sf::Color::Red;
		break;
	case 0:
		//If 0 Lives, Lives Colour = Red
		livesColour = sf::Color::Red;
		break;
	default:
		break;
	}


	// Keeps the Score and Lives text updated
	scoreText.setString("Score: " + to_string(playerScore));
	livesText.setString("Lives: " + to_string(playerLives));
	livesText.setFillColor(livesColour);
	scoreText.setFillColor(scoreColour);


	//Update Buttons every frame
	for (int i = 0; i < buttonIndex; i++)
	{

		if (gameState == States::levelEditor) {

			button[i].setPosition(sf::Vector2f(i*(buttonWidth + 5), window.getSize().y - 2*buttonHeight));
		}

		else {

			button[i].setPosition(sf::Vector2f(buttonX, mainText.getPosition().y + (1.5 * (buttonHeight + (i * buttonHeight)))));
		}

		button[i].setSize(sf::Vector2f(buttonWidth, buttonHeight));
		button[i].setFillColor(sf::Color::White);

		buttonText[i].setPosition(sf::Vector2f(button[i].getPosition().x + (buttonWidth / 4), button[i].getPosition().y + (buttonHeight / 4)));
		buttonText[i].setFont(font);
		buttonText[i].setFillColor(sf::Color::Black);
		buttonText[i].setCharacterSize(24);

	}


	//Sets button text based on game state
	switch (gameState) {

	case States::game:
		//Play State
		break;

	case States::gameOver:
		//GameOver State
		buttonText[0].setString("Retry");
		buttonText[1].setString("Menu");
		break;

	case States::winScreen:
		//Win State
		buttonText[0].setString("Retry");
		buttonText[1].setString("Menu");
		break;

	case States::menu:
		//Menu State
		buttonText[0].setString("Play");
		buttonText[1].setString("Level Editor");
		break;

	case States::levelEditor:
		//Level Editor
		buttonText[0].setString("Back");
		buttonText[1].setString("Save");
		buttonText[2].setString("Load");
		buttonText[3].setString("Play");
		break;

	default:
		break;
	}

}



void App::RefreshBricks() {

	for (int row = 0; row < ROWS; row++) {

		for (int col = 0; col < COLS; col++) {

			switch (bricks[row][col].GetHealth())
			{
			case 0: break;
			case 1: bricks[row][col].SetSprite(2); break;
			case 2: bricks[row][col].SetSprite(3); break;
			case 3: bricks[row][col].SetSprite(0); break;
			case 4: bricks[row][col].SetSprite(1); break; //Green
			case 5: bricks[row][col].SetSprite(4); break; //Yellow
			case 6: bricks[row][col].SetSprite(6); break; //Brown
			case 7: bricks[row][col].SetSprite(5); break; //Silver
			default: break;
			}
		}
	}
	
}



// Restarts Game

void App::RestartGame() {

	SetupPaddle(); //Initializes Paddle

	SetupBall(); //Initializes Ball

	SetupUI(); //Initializes User Interface

	SetupGameLogic(); //Initializes GameLogic members

	SetupArrayOfBricks(); //Initializes 2D array of Bricks

	SetupEnemy();
}


//Runs in Event

void App::ButtonEvent() {

	for (int i = 0; i < buttonIndex; ++i) {

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

			sf::Vector2i localPosition = sf::Mouse::getPosition(window);

			if (button[i].getGlobalBounds().contains(sf::Vector2f(localPosition))) {

				switch (gameState) {

				case States::game:
					//Play State
					break;

				case States::gameOver:
					//GameOver State
					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameMusic.play();
						gameState = States::game; //Relaunch the game
					}

					//Button 2
					if (i == 1) {

						RestartGame(); //Resets the game
						gameState = States::menu; //Return to main menu
					}
					break;

				case States::winScreen:
					//Win State

					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameMusic.play();
						gameState = States::game; //Launch Game
					}

					//Button 2
					if (i == 1) {

						RestartGame(); //Resets the game
						gameState = States::menu; //Return to main menu
					}
					break;

				case States::menu:
					//Menu State

					//Button 1
					if (i == 0) {

						RestartGame(); //Resets the game
						gameMusic.play();
						gameState = States::game; //Launch Game
					}

					//Button 2
					if (i == 1) {

						RestartGame(); //Resets the game
						gameState = States::levelEditor; //Launch Level Editor
					}
					break;

				case States::levelEditor:
					//Level Editor

					//Back Button (Button 1)
					if (i == 0) {

						RestartGame(); //Resets the game
						gameState = States::menu; //Return to Main Menu
					}

					//Save Button (Button 2)
					if (i == 1) {

						savingLevel = true;
						SaveLevel();
						//savingLevel = true;
					}

					if (i == 2) {

						LoadLevel();
					}

					if (i == 3) {

						PlayLevel();
					}

				default:
					break;
				}


			}
		}
	}
}



void App::UpgradeSpawner() {

	

	if (!upgradeSpawned) {

		spawnClock.restart();
		upgradeSpawned = true;
	}
	

	spawnTime = spawnClock.getElapsedTime();

	if (spawnTime >= upgrade.spawnTimer) {

		SpawnUpgrade();
		upgradeSpawned = false;
	}
}



void App::SpawnUpgrade() {

	//Choose a random upgrade
	int randomUpgrade = rand() % (upgradeIndex + 1);

	switch (randomUpgrade) {

		case 0: upgradeState = Upgrades::LargerPaddle; break;
		case 1: upgradeState = Upgrades::PaddleSpeed; break;
		case 2: upgradeState = Upgrades::LargerBall; break;
		case 3: upgradeState = Upgrades::FireBall; break;
	}

	//Set the upgrades sprite
	switch (upgradeState) {

		case Upgrades::LargerPaddle: 
			upgrade.SetSprite(0);
			break;

		case Upgrades::PaddleSpeed:
			upgrade.SetSprite(1);
			break;

		case Upgrades::LargerBall:
			upgrade.SetSprite(2);
			break;

		case Upgrades::FireBall:
			upgrade.SetSprite(3);
			break;

		default: break;

	}

	//Set the upgrades location
	int spawnLocations = window.getSize().x - upgrade.GetSize().x;
	float RandomSpawnLocation = rand() % spawnLocations;

	upgrade.setPosition(RandomSpawnLocation, 0);
	upgrade.sprite.setPosition(upgrade.getPosition());

	//Set the upgrade to be collidable
	upgrade.SetCollidable(true);


}



void App::Upgrade() {

	switch (upgradeState) {

	case Upgrades::LargerPaddle:

		//Increase size of paddle
		paddle.sprite.scale(paddle.GetSizeChange(), paddle.GetSizeChange());

		//Reset paddles position
		paddle.sprite.setPosition(paddle.sprite.getPosition().x, window.getSize().y - (1.5f * paddle.GetSize().y));

		upgradeText.setString("PADDLE SIZE +");

		break;
	case Upgrades::PaddleSpeed:

		paddle.SetSpeed(paddle.GetSpeed() * paddle.GetSpeedChange());

		upgradeText.setString("PADDLE SPEED +");

		cout << "Paddle Speed: " << paddle.GetSpeed() << endl;

		break;

	case Upgrades::LargerBall:

		ball.sprite.scale(ball.GetSizeChange(), ball.GetSizeChange());

		upgradeText.setString("BALL SIZE +");

		cout << "x: " << ball.GetRadius() << endl;

		break;

	case Upgrades::FireBall:

		ball.SetPower(ball.GetPower() + 1);

		upgradeText.setString("BALL POWER +");

		cout << "Ball Power: " << ball.GetPower() << endl;

		break;

	default: break;
	}

	upgradeText.setPosition(window.getSize().x / 2 - (upgradeText.getGlobalBounds().width / 2), paddle.getPosition().y - 2 * upgradeText.getGlobalBounds().height);
}



void App::SaveLevel() {

	
	if (savingLevel) {

		savedLevel.open("SavedBricks.txt", ios::out);



		for (int row = 0; row < ROWS; row++) {

			for (int col = 0; col < COLS; col++) {

				if (bricks[row][col].GetCollidable()) {

					savedLevel << "TT";
				}

				else {

					savedLevel << "FF";
				}

				if (!(row == ROWS - 1 && col == COLS - 1)) {

					savedLevel << "\t";
				}

				levelBricks += 1;

			}
		}

		savedLevel.close();

		savingLevel = false;

		}
		
}



void App::LoadLevel() {

	loadLevel.open("SavedBricks.txt", ios::in);

	//savedLevel.open("Saved Level Brick Positions.txt");
	string isCollidable;
	
	bricksLeft = 0;

	if (loadLevel.is_open()) {

			while (!loadLevel.eof()) {

					for (int row = 0; row < ROWS; row++) {

						for (int col = 0; col < COLS; col++) {

							

							loadLevel >> isCollidable;

							bricks[row][col].SetCollidable(false);

							


							if (isCollidable == "TT") {

								bricks[row][col].SetCollidable(true);

								bricksLeft += 1;
							}

							else if (isCollidable == "FF") {

								bricks[row][col].SetCollidable(false);
							}

						}

					}

				}
			}

	loadLevel.close();
}



void App::PlayLevel() {

	LoadLevel();

	cout << bricksLeft;

	gameMusic.play();

	gameState = States::game;


}



//Runs in Draw

void App::DrawGameState() {

	//Checks the Player is alive and there's bricks left

	switch (gameState)
	{
	case States::game:
		// State 0 = Play State
		//window.draw(paddle);

		window.draw(paddle.sprite);
		window.draw(ball.sprite);

		if (upgrade.GetCollidable()) {
			window.draw(upgrade.sprite);
		}

		if (enemy.GetCollidable()) {

			window.draw(enemy.sprite);

			if (enemyDrop.GetCollidable()) {

				window.draw(enemyDrop.sprite);
			}
		}

		if (showUpgradeText) {

			window.draw(upgradeText);
		}
		

		//Draw the Text
		window.draw(scoreText);
		window.draw(livesText);

		//Draw the Array of Bricks
		//For every row
		for (int row = 0; row < ROWS; ++row)
		{
			//For every collumn
			for (int col = 0; col < COLS; ++col)
			{
				//If it hasn't been hit already
				if (bricks[row][col].GetCollidable())
				{
					
					//Draw a brick
					window.draw(bricks[row][col].sprite);
				}
			}
		}
		break;

	case States::gameOver:
		// State 1 = GameOver State
		window.draw(endText);

		DrawButtons();
		break;

	case States::winScreen:
		// State 2 = Win State
		window.draw(endText);

		DrawButtons();

		break;

	case States::menu:
		// State 3 = Menu State
		window.draw(mainText);

		DrawButtons();
		break;

	case States::levelEditor:
		// State 4 = Level Editor

		// Draw Array of Bricks
		for (int row = 0; row < ROWS; ++row) {

			//For every collumn
			for (int col = 0; col < COLS; ++col) {

				//If it hasn't been hit already
				if (bricks[row][col].GetCollidable()) {

					//Draw a brick
					window.draw(bricks[row][col].sprite);
				}
			}
		}

		DrawButtons();


	default:
		break;

	}
}



void App::DrawButtons() {

	if (gameState == States::levelEditor) {

		for (int i = 0; i < buttonIndex; ++i) {

			window.draw(button[i]);
			window.draw(buttonText[i]);
		}
	}

	else {

		for (int i = 0; i < (buttonIndex - 2); i++) {
			window.draw(button[i]);
			window.draw(buttonText[i]);
		}
	}


}
	